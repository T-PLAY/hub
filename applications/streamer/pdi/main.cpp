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

CPDIdev g_pdiDev;
CPDImdat g_pdiMDat;
CPDIser g_pdiSer;
unsigned long g_dwFrameSize;
BOOL g_bCnxReady;
unsigned long g_dwStationMap;
HWND g_hwnd = NULL;

#define BUFFER_SIZE 0x1FA400 // 30 seconds of xyzaer+fc 8 sensors at 240 hz
//BYTE	g_pMotionBuf[0x0800];  // 2K worth of data.  == 73 frames of XYZAER
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
    //Connect To Tracker
    else if (!(Connect())) {
    }

    //Configure Tracker
    else if (!(SetupDevice())) {
    }

    else {
        //            DisplaySingle();
        //            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        if (!StartCont()) {
        } else {

            while (true) { // each server connect
                try {
                    OutputStream posStream("Polhemus Patriot (probe)", Stream::Format::DOF6, { 1 });

                    while (true) { // each acquisition
                        // Block program until frames arrive

                        unsigned char* pBuf = nullptr;
                        unsigned char* pLastBuf = nullptr;
                        unsigned long size;

                        //                        const auto start = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
                        const auto start = std::chrono::high_resolution_clock::now();
                        //                    if (!(g_pdiDev.ReadSinglePnoBuf(pBuf, size))) {
                        if (!(g_pdiDev.LastPnoPtr(pBuf, size))) {
                            AddResultMsg("LastPnoPtr");
                            exit(1);
                        } else if ((pBuf == 0) || (size == 0)) {
                            std::cout << "pBuf = 0, size = " << size << std::endl;

                        } else if (pBuf != pLastBuf) {
                            //                            const auto& end = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
                            const auto end = std::chrono::high_resolution_clock::now();
                            assert(pBuf != nullptr);
                            assert(pBuf != 0);
                            //                            assert(size == 8 + 4 + 12 + 16);
                            assert(size == 8 + 12 + 16);
                            assert(size == posStream.getAcquisitionSize() + 8); // header 8 bytes, frame count 4 bytes

                            unsigned char* data = &pBuf[8]; // size of header = 8 bytes
                            const auto timestampStart = std::chrono::duration_cast<std::chrono::microseconds>((end - std::chrono::microseconds(18'500)).time_since_epoch()).count(); // Polhemus technical spec latency = 18.5ms
                            const auto timestampEnd = std::chrono::duration_cast<std::chrono::microseconds>(end.time_since_epoch()).count();

                            //                            std::cout << "chrono timestamp = " << timestampStart << std::endl;
                            //                            uint32_t timestampPdi = ((uint32_t*)data)[0]; // milliseconds
                            //                            std::cout << "pdi timestamp = " << timestampPdi << std::endl;

                            // Try to get a frame of a depth image
                            posStream << Stream::Acquisition { timestampStart, timestampEnd, &data[4] };

                            pLastBuf = pBuf;
                            //                            float* translation = (float*)&data[4];
                            //                            float* quaternion = (float*)&data[4 + 12];
                            //                            std::cout << "x:" << translation[0] << ", y:" << translation[1] << ", z:" << translation[2] << ", az:" << quaternion[0] << ", el:" << quaternion[1] << ", ro:" << quaternion[2] << ", q4:" << quaternion[3] << std::endl;
                        }
                        else {
                            std::cout << "no new frame" << std::endl;
                        }

                        const auto maxFps = 80;
//                        const auto now = std::chrono::high_resolution_clock::now();
//                        std::cout << "sleep for " << (start + std::chrono::microseconds(1'000'000 / maxFps) - now).count() << std::endl;
                        //                        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                        const auto end = start + std::chrono::nanoseconds(1'000'000'000 / maxFps);
                        std::this_thread::sleep_until(end);

//                        const auto end2 = std::chrono::high_resolution_clock::now();
//                        const auto fps = (1'000'000'000) / std::chrono::duration_cast<std::chrono::nanoseconds>(end2 - start).count();
//                        std::cout << "fps : " << fps << std::endl;

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

    //Close Tracker Connection
    Disconnect();

    return 0;
}

//typedef enum
//{
// PDI_ODATA_SPACE = 0        // space
// , PDI_MODATA_SPACE = 0
// , PDI_MODATA_CRLF          // <cr><lf>
// , PDI_MODATA_POS           // x, y, z Cartesion pos coords
// , PDI_MODATA_POS_EP        // x, y, z Cartesion pos coords, extended precision
// , PDI_MODATA_ORI           // az, el, ro Euler ori angles
// , PDI_MODATA_ORI_EP        // az, el, ro Euler ori angles, extended precision
// , PDI_MODATA_DIRCOS        // Direction Cosine Matrix
// , PDI_MODATA_QTRN          // Orientation Quaternion
// , PDI_MODATA_TIMESTAMP     // Timestamp (32-bit)
// , PDI_MODATA_FRAMECOUNT    // Frame Count (32-bit)
// , PDI_MODATA_STYLUS        // Stylus Flag (32-bit)
// , PDI_MODATA_DISTLEV       // Distortion Level (32-bit)
// , PDI_MODATA_EXTSYNC       // External Sync (32-bit) (1 = Detected)
// , PDI_MODATA_DIGIO = 14	   // Digital IO bitfield

//} ePDIMotionData;

bool Initialize()
{

    g_pdiDev.Trace(true, 7);
    g_pdiDev.Log(true);

    ::SetConsoleTitle("PDIconsole");

    g_pdiMDat.Empty();
    //    g_pdiMDat.Append(PDI_MODATA_FRAMECOUNT);
    //    g_pdiMDat.Append(PDI_MODATA_TIMESTAMP);
    g_pdiMDat.Append(PDI_MODATA_POS);
    //    g_pdiMDat.Append(PDI_MODATA_ORI);
    g_pdiMDat.Append(PDI_MODATA_QTRN);
    g_dwFrameSize = 8 + 4 + 12 + 16;

    g_bCnxReady = false;
    g_dwStationMap = 0;

    return true;
}

bool Connect()
{
    //TCHAR tc;

    //basic_string<TCHAR> tmsg;
    //tmsg = "test" + basic_string<TCHAR>(g_pdiDev.GetLastResultStr() ) + "\r\n";
    std::string msg;
    if (!(g_pdiDev.CnxReady())) {
        g_pdiDev.SetSerialIF(&g_pdiSer);

        //BOOL bRet = g_pdiDev.ConnectUSB(false);
        //ePiCommType eType = g_pdiDev.CnxType();

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

    //msg.Format("%s result: %s\r\n", szCmd, m_pdiDev.GetLastResultStr() );
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

    //g_pdiDev.StartPipeExport();
    //AddResultMsg("StartPipeExport");

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
        //        g_dwOverflowCount = 0;
        bRet = TRUE;
        //Sleep(1000);  // don't need to sleep here if event-driven.
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

//void DisplayFrame(unsigned char* pBuf, unsigned long dwSize)
//{
//    char szFrame[200];
//    unsigned long i = 0;

//    while (i < dwSize) {
//        BYTE ucSensor = pBuf[i + 2];
//        SHORT shSize = pBuf[i + 6];

//        // skip rest of header
//        i += 8;

//        PDWORD pFC = (PDWORD)(&pBuf[i]);
//        PFLOAT pPno = (PFLOAT)(&pBuf[i + 4]);

//        _sntprintf(szFrame, _countof(szFrame), "%2d   %d  %+011.6f %+011.6f %+011.6f   %+011.6f %+011.6f %+011.6ff\r",
//            ucSensor, *pFC, pPno[0], pPno[1], pPno[2], pPno[3], pPno[4], pPno[5]);
//        std::string sF = std::string(szFrame);

//        std::cout << sF << std::endl;

//        i += shSize;
//    }
//}

//void DisplaySingle()
//{

//    unsigned char* pBuf;
//    unsigned long dwSize;

//    std::cout << std::endl;

//    if (!(g_pdiDev.ReadSinglePnoBuf(pBuf, dwSize))) {
//        AddResultMsg("ReadSinglePno");
//        exit(1);
//    } else if ((pBuf == 0) || (dwSize == 0)) {
//    } else {
//        DisplayFrame(pBuf, dwSize);
//    }
//}

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
