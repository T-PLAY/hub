// License: Apache 2.0. See LICENSE file in root directory.
// Copyright(c) 2019 Intel Corporation. All Rights Reserved.

#include <chrono>
#include <iostream> // for cout

#include <cassert>
#include <stream.h>

#include <tchar.h>

#include <PDI.h>

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

#include <constants.h>


CPDIdev g_pdiDev;
CPDImdat g_pdiMDat;
CPDIser g_pdiSer;
// unsigned long g_dwFrameSize;
BOOL g_bCnxReady;
unsigned long g_dwStationMap;
HWND g_hwnd = NULL;

#define BUFFER_SIZE 0x1FA400 // 30 seconds of xyzaer+fc 8 sensors at 240 hz
// BYTE	g_pMotionBuf[0x0800];  // 2K worth of data.  == 73 frames of XYZAER
BYTE g_pMotionBuf[BUFFER_SIZE];

bool Initialize();
bool Connect();
bool SetupDevice();
void Disconnect();
void DisplaySingle();
void AddResultMsg(const char* szCmd);
bool StartCont();
bool StopCont();

static void sigHandler(int sig)
{
    printf("Signal %d received, exiting\n", sig);
    StopCont();
    Disconnect();
    exit(0);
};

int main(int argc, char* argv[])
{
    signal(SIGINT, sigHandler);
#ifdef SIGBREAK
    signal(SIGBREAK, sigHandler); // handles Ctrl-Break on Win32
#endif
    signal(SIGABRT, sigHandler);
    signal(SIGTERM, sigHandler);

    if (!Initialize()) {
    }
    // Connect To Tracker
    else if (!(Connect())) {
    }

    // Configure Tracker
    else if (!(SetupDevice())) {
    }

    else {
        if (!StartCont()) {
        } else {

            while (true) { // each server connect
                try {
                    std::vector<std::unique_ptr<OutputStream>> outputStreams;
                    outputStreams.push_back(std::make_unique<OutputStream>("Polhemus Patriot (confidence)", Stream::Format::DOF6, std::vector<int>({ 1 }), ClientSocket("192.168.137.1", 4042)));
                    //outputStreams.push_back(std::make_unique<OutputStream>("Polhemus Patriot (confidence)", Stream::Format::DOF6, std::vector<int>({ 1 })));
                    outputStreams.push_back(std::make_unique<OutputStream>(g_probePoseSensorName, Stream::Format::DOF6, std::vector<int>({ 1 }), ClientSocket("192.168.137.1", 4042)));
                    //outputStreams.push_back(std::make_unique<OutputStream>(g_probePoseSensorName, Stream::Format::DOF6, std::vector<int>({ 1 })));
                    constexpr int packetSize = 8 + 12 + 16;
                    const size_t acquisitionSize = outputStreams[0]->getAcquisitionSize();
                    assert(packetSize == 8 + acquisitionSize); // header 8 bytes, frame count 4 bytes

                    while (true) { // each acquisition
                        // Block program until frames arrive

                        unsigned char* pBuf = nullptr;
                        unsigned char* pLastBuf = nullptr;
                        unsigned long size;

                        const auto start = std::chrono::high_resolution_clock::now();
                        if (!(g_pdiDev.LastPnoPtr(pBuf, size))) {
                            AddResultMsg("LastPnoPtr");
                            exit(1);
                        } else if ((pBuf == nullptr) || (size == 0)) {
                            std::cout << "pBuf = 0, size = " << size << std::endl;

                        } else if (pBuf != pLastBuf) {
                            assert(size % packetSize == 0);
                            assert(pBuf != nullptr);
                            assert(pBuf != 0);

                            size_t i = 0;
                            while (i < size) {

                                int ucSensor = (int)pBuf[i + 2];

                                const auto end = std::chrono::high_resolution_clock::now();

                                unsigned char* data = &pBuf[i + 8]; // size of header = 8 bytes
                                const auto timestampStart = std::chrono::duration_cast<std::chrono::microseconds>((end - std::chrono::microseconds(18'500)).time_since_epoch()).count(); // Polhemus technical spec latency = 18.5ms
                                const auto timestampEnd = std::chrono::duration_cast<std::chrono::microseconds>(end.time_since_epoch()).count();

                                // float* translation = (float*)data;
                                // float* quaternion = (float*)&data[12];
                                // std::string str = std::string("sensor:") + std::to_string(ucSensor) + std::string(", x:") + std::to_string(translation[0]) + ", y:" + std::to_string(translation[1]) + ", z:" + std::to_string(translation[2]) + "\naz:" + std::to_string(quaternion[0]) + ", el:" + std::to_string(quaternion[1]) + ", ro:" + std::to_string(quaternion[2]) + ", q4:" + std::to_string(quaternion[3]);
                                // std::cout << str << std::endl;

                                // Try to get a frame of a depth image
                                *outputStreams[ucSensor - 1] << Stream::Acquisition { timestampStart, timestampEnd, data, acquisitionSize };

                                i += packetSize;
                            }
                            pLastBuf = pBuf;

                        } else {
                            std::cout << "no new frame" << std::endl;
                        }

                        const auto maxFps = 60;
                        const auto end = start + std::chrono::nanoseconds(1'000'000'000 / maxFps);
                        while (std::chrono::high_resolution_clock::now() < end)
                            ;

                    } // while (true) // each acquisition

                } catch (const Socket::exception& e) {
                    std::cerr << "[pdi] catch socket exception : " << e.what() << std::endl;
                } catch (const Stream::exception& e) {
                    std::cerr << "[pdi] catch stream exception : " << e.what() << std::endl;
                } catch (const std::exception& e) {
                    std::cerr << "[pdi] catch main exception : " << e.what() << std::endl;
                    return EXIT_FAILURE;
                }
            } // while (true) // each server connect

            StopCont();
        }
    } // (!Initialize())

    // Close Tracker Connection
    Disconnect();

    return 0;
}

// typedef enum
//{

//} ePDIMotionData;

bool Initialize()
{

    g_pdiDev.Trace(true, 7);
    g_pdiDev.Log(true);

    ::SetConsoleTitle("PDIconsole");

    g_pdiMDat.Empty();
    g_pdiMDat.Append(PDI_MODATA_POS);
    g_pdiMDat.Append(PDI_MODATA_QTRN);

    g_bCnxReady = false;
    g_dwStationMap = 0;

    return true;
}

bool Connect()
{
    // TCHAR tc;

    // basic_string<TCHAR> tmsg;
    // tmsg = "test" + basic_string<TCHAR>(g_pdiDev.GetLastResultStr() ) + "\r\n";
    std::string msg;
    if (!(g_pdiDev.CnxReady())) {
        g_pdiDev.SetSerialIF(&g_pdiSer);

        // BOOL bRet = g_pdiDev.ConnectUSB(false);
        // ePiCommType eType = g_pdiDev.CnxType();

        ePiCommType eType = g_pdiDev.DiscoverCnx(false);
        switch (eType) {
        case PI_CNX_USB:
            msg = "USB Connection: " + std::string(g_pdiDev.GetLastResultStr()) + "\r\n";
            break;
        case PI_CNX_SERIAL:
            msg = "Serial Connection: " + std::string(g_pdiDev.GetLastResultStr()) + "\r\n";
            break;
        default:
            msg = "DiscoverCnx result: " + std::string(g_pdiDev.GetLastResultStr()) + "\r\n";
            break;
        }

        g_bCnxReady = g_pdiDev.CnxReady();
        std::cout << msg;

    } else {
        g_bCnxReady = true;
        std::cout << "Already connected\r\n";
    }

    return g_bCnxReady;
}

void AddResultMsg(const char* szCmd)
{
    std::string msg;

    // msg.Format("%s result: %s\r\n", szCmd, m_pdiDev.GetLastResultStr() );
    msg = std::string(szCmd) + " \r\nresult: " + std::string(g_pdiDev.GetLastResultStr()) + "\r\n";
    std::cout << msg;
}

void UpdateStationMap()
{
    g_pdiDev.GetStationMap(g_dwStationMap);
    AddResultMsg("GetStationMap");

    // char szMsg[100];
    // _sntprintf(szMsg, _countof(szMsg), "ActiveStationMap: %#x\r\n", g_dwStationMap);
    std::cout << "ActiveStationMap: " << g_dwStationMap << std::endl;
}

bool SetupDevice()
{
    g_pdiDev.SetPnoBuffer(g_pMotionBuf, BUFFER_SIZE);
    AddResultMsg("SetPnoBuffer");

    // g_pdiDev.StartPipeExport();
    // AddResultMsg("StartPipeExport");
    g_pdiDev.SetMetric(true); // centimeters
    AddResultMsg("SetMetric");



    // set hemisphere tracking
    std::cout << _T("Set SHemiTrack :");
    bool bResult = g_pdiDev.SetSHemiTrack(-1);
    if (bResult != TRUE) {
        std::cout << _T("Failed. Continuing anyway.") << std::endl;
    } else
        std::cout << _T("Success.") << std::endl;

    std::string msg;

    g_pdiDev.SetSDataList(-1, g_pdiMDat);
    AddResultMsg("SetSDataList");

    CPDIbiterr cBE;
    g_pdiDev.GetBITErrs(cBE);
    AddResultMsg("GetBITErrs");

    char sz[100];
    cBE.Parse(sz, 100);
    msg = "BIT Errors: " + std::string(sz) + "\r\n";
    std::cout << msg;

    if (!(cBE.IsClear())) {
        g_pdiDev.ClearBITErrs();
        AddResultMsg("ClearBITErrs");
    }

    UpdateStationMap();

    return true;
}

bool StartCont()
{
    BOOL bRet = false;

    if (!(g_pdiDev.StartContPno(g_hwnd))) {
    } else {
        bRet = TRUE;
        // Sleep(1000);  // don't need to sleep here if event-driven.
    }
    AddResultMsg(_T("\nStartContPno"));

    return bRet;
}

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
bool StopCont()
{
    bool bRet = false;

    if (!(g_pdiDev.StopContPno())) {
    } else {
        bRet = TRUE;
        Sleep(1000);
    }
    AddResultMsg(_T("StopContPno"));

    return bRet;
}

void Disconnect()
{
    std::string msg;
    if (!(g_pdiDev.CnxReady())) {
        msg = "Already disconnected\r\n";
    } else {
        g_pdiDev.Disconnect();
        msg = "Disconnect result: " + std::string(g_pdiDev.GetLastResultStr()) + "\r\n";
    }
    std::cout << msg;
}

// void DisplayFrame(unsigned char* pBuf, unsigned long dwSize)
//{

//}

// void DisplaySingle()
//{

//}

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
