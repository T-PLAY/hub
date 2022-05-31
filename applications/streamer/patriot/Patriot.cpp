/*
 * Polhemus Patriot tracker support  Copyright (C) 2010 Nikolaj Sheller
 *
 * This library is covered by the LGPL, read LICENSE for details.
 */

#include "Patriot.h"
#include <cassert>
#include <thread>

namespace Polhemus {
Patriot::Patriot() {
  /*
   * Populate map containing description of error codes
   * See TAP Cmd IF Spec. 2.4 PATRIOT Error Code Summary
   */
  errorDescriptions[0x00] = "No Error";
  errorDescriptions[0x01] = "Invalid Command";
  errorDescriptions[0x02] = "Invalid Station";
  errorDescriptions[0x03] = "Invalid Parameter";
  errorDescriptions[0x04] = "Too Few Parameters";
  errorDescriptions[0x05] = "Too Many Parameters";
  errorDescriptions[0x06] = "Parameter Below Limit";
  errorDescriptions[0x07] = "Parameter Above Limit";
  errorDescriptions[0x08] = "Communication Failure with Sensor Processor Board";
  errorDescriptions[0x09] = "Error Initiating Sensor Processor 1";
  errorDescriptions[0x0a] = "Error Initiating Sensor Processor 2";
  errorDescriptions[0x0b] = "Error Initiating Sensor Processor 3";
  errorDescriptions[0x0c] = "Error Initiating Sensor Processor 4";
  errorDescriptions[0x0d] = "No Sensor Processors Detected";
  errorDescriptions[0x0e] = "Error Initiating Source Processor";
  errorDescriptions[0x0f] = "Memory Allocation Error";
  errorDescriptions[0x10] = "Excessive Command Characters Entered";
  errorDescriptions[0x11] = "You must exit UTH mode to send this command";
  errorDescriptions[0x12] = "Error reading source prom. Using Defaults";
  errorDescriptions[0x13] = "This is a read only command";
  errorDescriptions[0x14] = "Non-fatal text message";
  errorDescriptions[0x15] = "Error loading map (N/A for PATRIOT)";
  errorDescriptions[' '] = "No Error (ASCII mode only)";
  errorDescriptions['a'] = "Source Fail X";
  errorDescriptions['b'] = "Source Fail Y";
  errorDescriptions['c'] = "Source Fail XY";
  errorDescriptions['d'] = "Source Fail Z";
  errorDescriptions['e'] = "Source Fail XZ";
  errorDescriptions['f'] = "Source Fail YZ";
  errorDescriptions['g'] = "Source Fail XYZ";
  errorDescriptions['u'] = "Position outside of mapped area (N/A for PATRIOT)";
  errorDescriptions['A'] = "Source Fail X + BIT Errors";
  errorDescriptions['B'] = "Source Fail Y + BIT Errors";
  errorDescriptions['C'] = "Source Fail X + BIT Errors";
  errorDescriptions['D'] = "Source Fail Z + BIT Errors";
  errorDescriptions['E'] = "Source Fail XZ + BIT Errors";
  errorDescriptions['F'] = "Source Fail YZ + BIT Errors";
  errorDescriptions['G'] = "Source Fail XYZ + BIT Errors";
  errorDescriptions['I'] = "BIT Error";

  connected = false;
  sensors.push_back(new Sensor(this, '1'));
  sensors.push_back(new Sensor(this, '2'));
}

Patriot::~Patriot() {
  clearPendingData("Shutdown");

  if (handle != 0) {
    libusb_close(handle);
    handle = 0;
  }

  libusb_exit(NULL);

  delete trackerVerInf;

  delete sensors[0];
  delete sensors[1];
}

bool Patriot::connect() {
  if (libusb_init(NULL) < 0) {
    assert(libusb_init(NULL) >= 0);
    return false;
  }

  handle = libusb_open_device_with_vid_pid(0, USB_PARMS[PATRIOT].vendorID,
                                           USB_PARMS[PATRIOT].productID);

  if (handle == 0) {
    assert(handle != 0);
    return false;
  }

  clearPendingData("Initialization");
  putTrackerIntoBinaryMode();
  // bool isbinary = getIsInBinaryOutputMode();

  unsigned int stationStateBitmap = getStationActivationState();
  updateSensorActiveState(stationStateBitmap);

  connected = true;
  return connected;
}

void Patriot::updateSensorActiveState(signed int activeStationBitmap) {
  for (unsigned int bitmapPosition = 0; bitmapPosition < PATRIOT_SENSOR_COUNT;
       bitmapPosition++)
    sensors[bitmapPosition]->setActive(activeStationBitmap &
                                       1 << bitmapPosition);
}

/**
 * Set Patriot output format using ‘^U’ – Active Station State
 * See TAP Cmd IF Spec.
 */
signed int Patriot::getStationActivationState() {
  ActiveStationStateBinaryResponse activeStationStateBinaryResponse;
  string binaryModeRequest = "\x15"
                             "0\r";

  send(binaryModeRequest);
  Patriot::receive(activeStationStateBinaryResponse);

  // TODO Endianess conversion on
  return activeStationStateBinaryResponse.activeStationBitmap;
}

bool Patriot::checkResponseData(BinaryResponseHeader &binaryResponsebuffer) {
  if (binaryResponsebuffer.errorIndicator != ' ' &&
      binaryResponsebuffer.errorIndicator != 0x00) {
    cout << "Response error: "
         << errorDescriptions[binaryResponsebuffer.errorIndicator] << "\n";
    return false;
  }

  if (binaryResponsebuffer.responseSize < 0) {
    cout << "Bad size of response\n\"" << (char *)&binaryResponsebuffer
         << "\"\n";
    return false;
  }

  if (binaryResponsebuffer.frameTag1 != 'P' ||
      binaryResponsebuffer.frameTag2 != 'A')
    return false;
  else
    return true;
}

bool Patriot::receive(SingleDataBinaryResponse2_4_1 &binaryResponse) {
  return receive((void *)&binaryResponse,
                 sizeof(SingleDataBinaryResponse2_4_1));
}

bool Patriot::receive(WhoAmIBinaryResponse &binaryResponse) {
  return receive((void *)&binaryResponse, sizeof(WhoAmIBinaryResponse));
}

bool Patriot::receive(SingleDataBinaryResponse &binaryResponse) {
  return receive((void *)&binaryResponse, sizeof(SingleDataBinaryResponse));
}

bool Patriot::receive(OutputFormatBinaryResponse &binaryResponse) {
  return receive((void *)&binaryResponse, sizeof(OutputFormatBinaryResponse));
}

bool Patriot::receive(UnitsBinaryResponse &binaryResponse) {
  return receive((void *)&binaryResponse, sizeof(UnitsBinaryResponse));
}

bool Patriot::receive(ActiveStationStateBinaryResponse &binaryResponse) {
  return receive((void *)&binaryResponse,
                 sizeof(ActiveStationStateBinaryResponse));
}

bool Patriot::receive(void *binaryResponsebuffer, int bufferSize) {
  int bytesRead = 0;

  int ret = libusb_bulk_transfer(handle, USB_PARMS[PATRIOT].bulkReadEndPoint,
                                 (unsigned char *)binaryResponsebuffer,
                                 READ_BUFFER_SIZE, &bytesRead, USB_TIMEOUT);

  if (bytesRead <= 0 || ret != LIBUSB_SUCCESS)
    return false;

  // TODO How to invoke overriding methods in C++? Use templates?
  return checkResponseData(*(BinaryResponseHeader *)binaryResponsebuffer);
}

/**
 * Clear any pending data from tracking device
 */
void Patriot::clearPendingData(string command) {
  unsigned char buffer[READ_BUFFER_SIZE];
  int readBytes = 0;

  libusb_bulk_transfer(handle, USB_PARMS[PATRIOT].bulkReadEndPoint, buffer,
                       READ_BUFFER_SIZE, &readBytes, USB_TIMEOUT);

  if (readBytes != 0 && readBytes != -1) {
    cout << "Command run: \"" << command << "\"\n";
    cout << "Cleared data:\n\"" << buffer << "\"\n";
  }
}

void Patriot::send(string data) const {
  int writtenBytes = 0;

  unsigned char *udata = (unsigned char *)data.c_str();

  libusb_bulk_transfer(handle, USB_PARMS[PATRIOT].bulkWriteEndPoint, udata,
                       data.length(), &writtenBytes, USB_TIMEOUT);

  if (writtenBytes == 0)
    throw new exception();
}

BinaryPosition *Patriot::getMeasurement() {
  SingleDataBinaryResponse2_4_1 singleDataBinaryResponse;

  if (!isConnected())
    throw exception();

  if (!getIsInBinaryOutputMode())
    putTrackerIntoBinaryMode();

  requestBinaryPositionData();
  receive(singleDataBinaryResponse);
  if (!checkResponseData(singleDataBinaryResponse))
    throw exception();

  return new BinaryPosition(singleDataBinaryResponse);
}

bool Patriot::isConnected() const { return connected; }

const vector<Sensor *> &Patriot::getSensors() const { return sensors; }

void Patriot::setAlignmentReferenceOrigin(char sensorID, Position &origin) {
  std::ostringstream alignmentCommand;
  alignmentCommand << 'A' << sensorID << ',';
  alignmentCommand << origin.getXpos() << ',' << origin.getYpos() << ','
                   << origin.getZpos();
  alignmentCommand << ", 100.0,   0.0, 0.0";
  alignmentCommand << ",  0.0, 100.0, 0.0\r";
  // alignmentCommand << ",,,,,,\r";

  send(alignmentCommand.str());

  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  cout << "setAlignmentReferenceOrigin sent:\n\"" << alignmentCommand.str()
       << "\"\n";
}

void Patriot::resetAlignmentFrame(char sensorID) {
  ostringstream resetAlignmentFrame;
  resetAlignmentFrame << '\x12' << sensorID << '\r';

  send(resetAlignmentFrame.str());
}

/**
 * Request version info from Patriot using '^V' - WhoAmI
 * See TAP Cmd IF Spec.
 */
void Patriot::requestVersionInfo() {
  string versionInforRequest = "\x16\r";
  send(versionInforRequest);
}

/**
 * Request data record from Patriot using ‘P’ – Single Data Record Output
 * See TAP Cmd IF Spec.
 */
void Patriot::requestBinaryPositionData() {
  string versionInforRequest = "P";
  send(versionInforRequest);
}

TrackerVersionInfo const *Patriot::getVersionInfo() {
  if (trackerVerInf == 0) {
    requestVersionInfo();
    WhoAmIBinaryResponse whoAmIBinaryResponse;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    //			usleep(100000);
    receive(&whoAmIBinaryResponse, sizeof(WhoAmIBinaryResponse));
    trackerVerInf = new TrackerVersionInfo(whoAmIBinaryResponse);
  }
  return trackerVerInf;
}

/**
 * Returns true is tracker in binary output format.
 * ‘F’ – Output Format. See TAP Cmd IF Spec.
 */
bool Patriot::getIsInBinaryOutputMode() {
  OutputFormatBinaryResponse outPutFormatBinaryResponse;
  string binaryModeRequest = "F\r";

  send(binaryModeRequest);
  receive(outPutFormatBinaryResponse);

  // TODO Endianess conversion
  return (outPutFormatBinaryResponse.asciiOrBinary == BINARY);
}

/**
 * Set Patriot output format using ‘F’ – Output Format
 * See TAP Cmd IF Spec.
 */
void Patriot::setTrackerBinaryMode(bool setBinaryMode) {
  if (setBinaryMode) {
    string binaryModeRequest = "F1\r";
    send(binaryModeRequest);
  } else {
    string asciiModeRequest = "F0\r";
    send(asciiModeRequest);
  }

  OutputFormatBinaryResponse outPutFormatBinaryResponse;
  Patriot::receive(outPutFormatBinaryResponse);
}

/**
 * Set Patriot ASCII output format using ‘F’ – Output Format
 * See TAP Cmd IF Spec.
 */
void Patriot::putTrackerIntoASCIIMode() { setTrackerBinaryMode(false); }

/**
 * Set Patriot Binary output format using ‘F’ – Output Format
 * See TAP Cmd IF Spec.
 */
void Patriot::putTrackerIntoBinaryMode() { setTrackerBinaryMode(true); }

void Patriot::putTrackerInMetricUnits() {
  string setUnitsRequest = "U1\r";
  send(setUnitsRequest);
}

void Patriot::putTrackerInEnglishUnits() {
  string setUnitsRequest = "U0\r";
  send(setUnitsRequest);
}

bool Patriot::isTrackerInMetricUnits() {
  UnitsBinaryResponse unitsBinaryResponse;
  string setUnitsRequest = "U\r";

  send(setUnitsRequest);
  Patriot::receive(unitsBinaryResponse);

  if (unitsBinaryResponse.inchesOrCentimeters == CENTIMETERS)
    return true;
  if (unitsBinaryResponse.inchesOrCentimeters == INCHES)
    return false;
  throw new exception();
}

/**
 * Enable hemisphere tracking. Indicate which hemisphere sensor is in.
 */
void Patriot::enableHemisphereTracking(const Sensor &sensor,
                                       const Position &origin) {
  ostringstream alignmentCommand;
  alignmentCommand << 'H' << sensor.getStationNumber() << ',';
  alignmentCommand << origin.getXpos() << ',' << origin.getYpos() << ','
                   << origin.getZpos() << '\r';

  send(alignmentCommand.str());
}
} // namespace Polhemus
