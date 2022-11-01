/////////////////////////////////////////////////////////////////////
// Polhemus  www.polhemus.com
// © 2003-14 Polhemus, All Rights Reserved
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
//
//  Filename:           $Workfile: PDIdev.h $
//
//  Project Name:       Polhemus Developer Interface
//
//  Description:        Tracker Device Object Class
//
//  VSS $Header: /PiDevTools14/Inc/PDIdev.h 17    5/31/17 6:16p Suzanne $
//
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
#ifndef _PDIDEV_H_
#define _PDIDEV_H_

/////////////////////////////////////////////////////////////////////
class CPDImdat;
class CPDIser;
class CPiCmdIF;
class CPiError;

class CPDIfilter;

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
// CLASS CPDIdev
/////////////////////////////////////////////////////////////////////
class PDI_API CPDIdev
{
  public:
    CPDIdev( VOID );
    virtual ~CPDIdev( VOID );

    ////////////////////////////////////
    // Connection Methods
    ////////////////////////////////////
    virtual BOOL CnxReady( VOID ); // Returns TRUE if Connection established
    ePiCommType CnxType( VOID );   // Returns current connection type.
    virtual ePiCommType
    DiscoverCnx( BOOL bInitCfg = TRUE ); // Search for USB, then Serial Connection to device
    BOOL
    ConnectSer( BOOL bInitCfg = FALSE ); // Attempts to connect via current serial port parameters
    BOOL ConnectUSB( BOOL bInitCfg = FALSE ); // Attempts to connect via USB device
    virtual BOOL Disconnect( VOID );          // Ends current connection

    // Serial Interface Parameters
    BOOL SetSerialIF( CPDIser* ); // Sets current serial port parameters locally: if no connection,
                                  // does not configure device
    BOOL GetSerialIF( CPDIser* ); // Returns current serial port parameters locally: if no
                                  // connection, does not configure device

    ////////////////////////////////////
    // Configuration Methods
    ////////////////////////////////////
    virtual BOOL GetAttFilter( CPDIfilter& );   // Gets Attitude Filter ( 'Y' Command )
    BOOL GetBinary( BOOL& );                    // Gets binary output mode( 'F' Command )
    BOOL GetBufEnabled( BOOL& );                // Gets Tracker buffering mode ( '@B' Command )
    BOOL GetEcho( BOOL& );                      // Gets echo mode ( '^E' Command )
    virtual BOOL GetFrameRate( ePiFrameRate& ); // Gets P&O Frame Rate ( 'R' Command )
    virtual BOOL GetMetric( BOOL& );            // Gets device to metric output ( 'U' Command )
    virtual BOOL GetPosFilter( CPDIfilter& );   // Gets Position Filter ( 'X' Command )
    virtual BOOL GetStationMap( DWORD& dwMap ); // Gets Active Station Map ( '^U' Command )
    virtual BOOL GetSourceFrame( PDIori& );     // Gets Source Frame Rotation ( 'G' Command )
    BOOL GetSyncEnabled( BOOL& );               // Gets Sync Enabled ( 'Z' Command )

    virtual BOOL SetAttFilter( const CPDIfilter& ); // Sets Attitude Filter ( 'Y' Command )
    BOOL SetBinary( BOOL );                         // Sets device to binary output ( 'F' Command )
    BOOL SetBufEnabled( BOOL );                     // Sets Tracker Buffering Mode ( '@B' Command )
    BOOL SetEcho( BOOL );                           // Sets echo mode ( '^E' Command )
    BOOL SetFrameRate( const ePiFrameRate );        // Sets P&O Frame Rate ( 'R' Command )
    virtual BOOL SetMetric( BOOL );                 // Sets device to metric output ( 'U' Command )
    virtual BOOL SetPosFilter( const CPDIfilter& ); // Sets Position Filter ( 'X' Command )
    BOOL SetStationMap( const DWORD& dwMap );       // Sets Active Station Map ( '^U' Command )
    virtual BOOL SetSourceFrame( const PDIori& );   // Sets Source Frame Rotation ( 'G' Command )
    BOOL SetSyncEnabled( BOOL );                    // Sets Sync Enabled ( 'Z' Command )

    // Per-station configuration Notes:
    //	: nStation arguments are 1-based.
    //	: if (nStation == -1), all stations are configured

    virtual BOOL ClearSBoresight( INT nStation ); // UN-Boresight station ('^B' command )
    BOOL EnableStation( INT nStation,
                        BOOL bEnable ); // Enables or disables station ( '^U' Command )

    virtual BOOL
    GetSDataList( INT nStation,
                  CPDImdat& ); // Gets motion data output list per station ( 'O' Command )
    BOOL GetSAlignment( INT nStation,
                        PDI3vec& O,
                        PDI3vec& X,
                        PDI3vec& Y ); // Gets Alignment Reference Frame per station ( 'A' Command )
    virtual BOOL GetSBoresight( INT nStation,
                                PDIori& ); // Get boresight per station ( 'B' Command )
    BOOL GetSHemisphere( INT nStation,
                         PDI3vec& ); // Get Hemisphere Of Operation per station ( 'H' Command )
    BOOL GetSHemiTrack( INT nStation,
                        BOOL& ); // Query Hemisphere Tracking per station ('H' Command )
    BOOL GetSStylusMode( INT nStation,
                         ePiStylusMode& ); // Get Stylus Mode per station ( 'L' Command )

    BOOL ResetSAlignment( INT nStation ); // Reset Alignment per station ( '^R' Command )

    virtual BOOL
    SetSDataList( INT nStation,
                  const CPDImdat& ); // Sets motion data output list per station ( 'O' Command )
    BOOL
    SetSAlignment( INT nStation,
                   const PDI3vec& O,
                   const PDI3vec& X,
                   const PDI3vec& Y ); // Sets Alignment Reference Frame per station ( 'A' Command )
    virtual BOOL SetSBoresight( INT nStation,
                                const PDIori&,
                                BOOL bReset = FALSE ); // Set boresight per station ( 'B' Command )
    BOOL
    SetSHemisphere( INT nStation,
                    const PDI3vec& );   // Set Hemisphere Of Operation per station ( 'H' Command )
    BOOL SetSHemiTrack( INT nStation ); // Enable Hemisphere Tracking per station ('H' Command )
    BOOL SetSStylusMode( INT nStation,
                         ePiStylusMode ); // Set Stylus Mode per station ( 'L' Command )

    ////////////////////////////////////
    // Configuration management
    ////////////////////////////////////
    BOOL GetSavedLabels( PDIlabels& ); // Gets currently saved configuration labels ( '^X' )
    BOOL GetCfgLabel( LPCSTR& );       // Gets current configuration label. ( '^X' )
    BOOL GetStartupSlot(
        ePiConfigSlot& eSlot ); // Gets current startup configuration slot number ('^W' )

    BOOL SaveCfgToSlot( const ePiConfigSlot& );  // Saves current configuration to slot ( '^K' )
    BOOL SetCfgLabel( const LPCSTR );            // Assigns label to current configuration ( '^X' )
    BOOL SetStartupSlot( const ePiConfigSlot& ); // Sets startup configuration slot number ('^W' )

    // The following are better suited for Unicode builds
    BOOL GetCfgLabel( LPCTSTR szTBuf,
                      DWORD nBufSize ); // Gets current configuration label. ( '^X' )
    BOOL SetCfgLabel( const LPCWSTR );  // Assigns label to current configuration ( '^X' )

    ////////////////////////////////////
    //	Map management
    ////////////////////////////////////
    BOOL GetCompState( INT& nMap ); // Gets current compensation state ( 'D' )
    BOOL GetSavedMaps( PDIlabels&,
                       INT& nCount ); // Gets currently saved map labels and count( '^D' )
    BOOL SetCompState(
        INT nMap ); // Enables compensation, using the given map number, 0 disables ( 'D' )

    ////////////////////////////////////
    // Runtime Methods
    ////////////////////////////////////
    // Host P&O Buffer methods
    BOOL ClearPnoBuffer( VOID );                   // Clears output P&O output buffer
    BOOL LastPnoPtr( PBYTE& pBuf, DWORD& dwSize ); // Returns pointer to last P&O frame collected
    BOOL ResetPnoPtr( VOID ); // Resets P&O output pointer to beginning of buffer
    BOOL SetPnoBuffer( PBYTE pBuf, DWORD dwSize ); // Sets/Clears output P&O output buffer
    BOOL ResetHostFrameCount( VOID );              // Resets Host P&O frame counter
    BOOL LastHostFrameCount(
        DWORD& dwFrameCount ); // Returns index of last P&O frame collected by the host

    // P&O Collection methods
    BOOL ReadSinglePno( HWND ); // Reads single P&O frame. ('P' command): posts data msg to HWND
    BOOL ReadSinglePnoBuf(
        PBYTE& pBuf,
        DWORD& dwSize );          // Reads single P&O frame. ('P' command): returns pointer to frame
    BOOL ResetFrameCount( VOID ); // Reset motion framecount ( 'Q' Command )
    BOOL ResetTimeStamp( VOID );  // Reset motion frame timestamp ( 'T' Command )
    BOOL StartContPno( HWND );    // Starts continuous P&O collection: posts data msg to HWND
    BOOL StopContPno( VOID );     // Stops continuous P&O collection
    BOOL StartStylusPno( HWND );  // Starts stylus-driven P&O collection: posts data msgs to HWND
    BOOL StopStylusPno( VOID );   // Stops stylus-driven P&O collection

    virtual BOOL ResetTracker( VOID ); // Initializes Tracker, disconnects ( '^Y' Command )
    BOOL TxtCmd( LPCSTR szCmd,
                 DWORD& dwRspSize,
                 LPCSTR szRsp = 0 ); // Sends generic text command to device
    BOOL
    WhoAmI( LPCSTR& ); // Issues tracker WhoAmI command ('^V' command ); puts result in argument
    BOOL WhoAmISensor(
        INT nSensor,
        LPCSTR& ); // Issues sensor WhoAmI command ( '^V' command ); puts result in argument

    // The following are better suited for Unicode builds
    BOOL TxtCmd( LPCWSTR szWCmd,
                 DWORD& dwRspWSize,
                 LPCWSTR szWRsp = 0 ); // Sends generic text command to device
    BOOL WhoAmI( LPCTSTR szTBuf, DWORD nBufSize );
    BOOL WhoAmISensor( INT nSensor, LPCTSTR szTBuf, DWORD nBufSize );

    BOOL GetBITErrs( CPDIbiterr& ); // Read tracker BIT error data
    BOOL ClearBITErrs( VOID );      // Clears tracker BIT errors

    ////////////////////////////////////
    // Other Methods
    ////////////////////////////////////
    ePiErrCode GetLastResult( VOID ); // Returns numeric result of last operation
    ePiErrCode GetLastResult(
        ePiDevError& d ); // Returns numeric result of last operation, plus any device error code
    LPCTSTR GetLastResultStr( VOID ); // Returns const string result of last operation
    LPCTSTR ResultStr( ePiErrCode,
                       ePiDevError );   // Returns const string definition of enum result code
    INT MaxSensors( VOID );             // Returns max number of sensors supported by device.
    ePiTrackerType TrackerType( VOID ); // Returns tracker type.

    BOOL GetDetectedStationMap( DWORD& dwMap ); // Gets Detected Station Map
    INT StationCount( VOID );                   // Returns number of detected, enabled stations

    ////////////////////////////////////
    // Troubleshooting Methods
    ////////////////////////////////////
    PVOID Trace( BOOL, INT n = 0 ); // Enables trace output to debug window (DEBUG build only)
    VOID
    Log( BOOL,
         INT nWriteFlushCount = 0 ); // Enables tracker I/O logging to file PICMDIF.LOG
                                     // nWriteFlushCount=zero, only flush when logfile is closed.
    VOID LogASCII(
        BOOL ); // Debug Trace I/O output and Log file output in ASCII, rather than hex
                // Default condition is FALSE.  Tracker I/O is traced and logged as a hex stream

    ////////////////////////////////////
    // Pipe Export Methods
    ////////////////////////////////////
    BOOL StartPipeExport(
        LPCTSTR szPipeName = PDI_EXPORT_PIPE_NAME ); // Starts raw P&O export on named pipe
    BOOL CancelPipeExport( VOID );                   // Cancels raw P&O export on named pipe

    ////////////////////////////////////
    // TCP Export Methods
    ////////////////////////////////////
    BOOL StartTCPExport( LPCTSTR szInterfaceAddress = PDI_EXPORT_TCP_ADDR,
                         SHORT nSocketPort = PDI_EXPORT_TCP_PORT ); // Starts raw P&O on addr:port
    BOOL CancelTCPExport( VOID ); // Cancels raw P&O export on addr:port

    ////////////////////////////////////
    // UDP Export Methods
    ////////////////////////////////////
    BOOL StartUDPExport( ULONG in_addr,
                         SHORT nPort = PDI_EXPORT_UDP_PORT ); // Starts raw P&O on UDP port using
                                                              // host-byte order inet address
    BOOL StartUDPExport( LPCTSTR szInterfaceAddress = PDI_EXPORT_UDP_ADDR,
                         SHORT nSocketPort = PDI_EXPORT_UDP_PORT ); // Starts raw P&O on addr:port
    BOOL CancelUDPExport( VOID ); // Cancels raw P&O export on addr:port

    ////////////////////////////////////
    // Legacy Methods
    ////////////////////////////////////
    // Please avoid use of the following methods as they may be phased out in the future.
    BOOL EnableSensor( INT nSensor, BOOL bEnable ) // Enables or disables sensor ( '^U' Command )
    {
        return EnableStation( nSensor, bEnable );
    }
    BOOL RestartTracker( VOID ) // Restarts Tracker, disconnects ( '^Y' Command )
    {
        return ResetTracker();
    }
    BOOL StartSocketExport( LPCTSTR szInterfaceAddress = PDI_EXPORT_TCP_ADDR,
                            SHORT nSocketPort = PDI_EXPORT_TCP_PORT ) // Starts raw P&O on addr:port
    {
        return StartTCPExport( szInterfaceAddress, nSocketPort );
    }
    BOOL CancelSocketExport( VOID ) { return CancelTCPExport(); }

  private:
    CPDIdev( const CPDIdev& );

    CPiCmdIF* m_pDev;
    CPiError* m_pResult;

    friend class CPDIlatus;
    friend class CPDImm;
    friend class CPDIg4;
    friend class CPDIfastrak;
};

/////////////////////////////////////////////////////////////////////
// END $Workfile: PDIdev.h $
/////////////////////////////////////////////////////////////////////
#endif // _PDIDEV_H_
