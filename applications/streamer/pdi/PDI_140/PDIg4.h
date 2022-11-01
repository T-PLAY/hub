/////////////////////////////////////////////////////////////////////
// Polhemus  www.polhemus.com
// © 2009-14 Polhemus, All Rights Reserved
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
//
//  Filename:           $Workfile: PDIg4.h $
//
//  Project Name:       Polhemus Developer Interface
//
//  Description:        G4 Device Object Class
//
//  VSS $Header: /PiDevTools12/Inc/PDIg4.h 22    8/01/19 3:46p Suzanne $
//
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
#ifndef _PDIG4_H_
#define _PDIG4_H_

/////////////////////////////////////////////////////////////////////

#include "PDIdev.h"
/////////////////////////////////////////////////////////////////////

class CG4CmdIF;
class CPDImdat;
class CPiError;

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
// CLASS CPDIg4
/////////////////////////////////////////////////////////////////////
class PDI_API CPDIg4 : public CPDIdev
{
  public:
    CPDIg4();
    CPDIg4( LPCTSTR szSrcCalPath );
    CPDIg4( ePDIposUnits cmdPosUnits, ePDIoriUnits cmdOriUnits );
    CPDIg4( LPCTSTR szSrcCalPath, ePDIposUnits eCmdPosUnits, ePDIoriUnits eCmdOriUnits );
    ~CPDIg4();

    /////////////////////////////////////////////////////////////////////////////////
    // Connection Methods
    /////////////////////////////////////////////////////////////////////////////////
    ePiCommType DiscoverCnx( BOOL bInitCfg        = TRUE,
                             LPCTSTR szSrcCalPath = 0 ); // Legacy connection method

    BOOL ConnectG4( LPCTSTR szSrcCalPath = 0 ); // Attempts to connect to G4 host device

    BOOL Disconnect( VOID ); // Ends current connection

    BOOL CnxReady( VOID ); // Returns TRUE if Connection established

    BOOL ResetTracker( VOID ); // Initializes G4 and RESTORES CONNECTION (Unlike CPDIdev version)

    /////////////////////////////////////////////////////////////////////////////////
    // G4 Configuration Methods
    /////////////////////////////////////////////////////////////////////////////////

    /////////////////////////////////////////////////////////////////////////////////
    // Interface Configuration
    VOID SetSrcCalFilePath(
        LPCTSTR szSrcCalPath ); // Source Calibration file needed for G4 initialization
    VOID SetCmdPosUnits(
        ePDIposUnits e ); // Position units to be used in all subsequent tracker commands
    VOID
    SetCmdOriUnits( ePDIoriUnits e ); // Orientation units to be used in all subsequent commands
    BOOL
    SetPNODataList( const CPDImdat& ); // Set/Get a CPDImdat configuration for legacy P&O output
    VOID GetPNODataList( CPDImdat& );

    /////////////////////////////////////////////////////////////////////////////////
    // Tracker Configuration

    // Native G4 P&O Settings
    // If specified, the units used for subsequent tracker commands are also set
    BOOL SetPNOPosUnits( const ePDIposUnits e, BOOL bCmdUnitsAlso = TRUE );
    BOOL SetPNOOriUnits( const ePDIoriUnits e, BOOL bCmdUnitsAlso = TRUE );
    BOOL GetPNOPosUnits( ePDIposUnits& e, BOOL bCmdUnitsAlso = TRUE );
    BOOL GetPNOOriUnits( ePDIoriUnits& e, BOOL bCmdUnitsAlso = TRUE );
    BOOL ResetPNOPosUnits();
    BOOL ResetPNOOriUnits();

    // Frame Of Reference							// Set
    BOOL SetFrameOfRef( const PDI7vec& ); // Translation & Rotation defined together (Rotation units
                                          // as defined by SetCmdOriUnits()
    BOOL SetFrameOfRefROT( const PDIori& );   // Rotation only, in Euler angles
    BOOL SetFrameOfRefROT( const PDIqtrn& );  // Rotation only, as a quaternion
    BOOL SetFrameOfRefTRANS( const PDIpos& ); // Tranlation only
                                              // Get
    BOOL GetFrameOfRef( PDI7vec& );     // Transation & Rotation read together (Rotation units as
                                        // defined by SetCmdOriUnits()
    BOOL GetFrameOfRefROT( PDIori& );   // Rotation only, in Euler angles
    BOOL GetFrameOfRefROT( PDIqtrn& );  // Rotation only, as a quaternion
    BOOL GetFrameOfRefTRANS( PDIpos& ); // Tranlation only
                                        // Reset
    BOOL ResetFrameOfRef();             // Translation & Rotation reset
    BOOL ResetFrameOfRefROT();          // Rotation only
    BOOL ResetFrameOfRefTRANS();        // Translation only

    /////////////////////////////////////////////////////////////////////////////////
    // Per-Hub configuration
    //
    //	Note: Setting nHub argument to -1 sets/resets all hubs.
    //
    /////////////////////////////////////////////////////////////////////////////////
    // Filters: All filters
    BOOL SetHFilters( const INT nHub, const CPDIfilter& attFilter, const CPDIfilter& posFilter );
    BOOL GetHFilters( const INT nHub, CPDIfilter& attFilter, CPDIfilter& posFilter );
    BOOL ResetHFilters( const INT nHub );

    // Filters: Attitude & Position Filters handled separately
    BOOL SetHAttFilter( const INT nHub, const CPDIfilter& );
    BOOL SetHPosFilter( const INT nHub, const CPDIfilter& );
    BOOL GetHAttFilter( const INT nHub, CPDIfilter& );
    BOOL GetHPosFilter( const INT nHub, CPDIfilter& );
    BOOL ResetHAttFilter( const INT nHub );
    BOOL ResetHPosFilter( const INT nHub );

    // Stylus Mode
    BOOL SetHStylusMode( const INT nHub, const CPDIStylusCfgG4& cfg );
    BOOL GetHStylusMode( const INT nHub, CPDIStylusCfgG4& cfg );
    BOOL ResetHStylusMode( const INT nHub );

    /////////////////////////////////////////////////////////////////////////////////
    // Per-Sensor configuration
    //
    //	Note: Setting nHub argument to -1 sets/resets all sensors on all hubs.
    //		  When nHub is -1, nSens argument is ignored.
    //		  When nSens is -1, all sensors on nHub are set/reset
    //		  When bMap is TRUE, nSens is a bitmap of sensors on nHub that are set/reset
    /////////////////////////////////////////////////////////////////////////////////
    // Increment
    BOOL SetSIncrement( const INT nHub,
                        const INT nSens,
                        const FLOAT fPosIncr,
                        const FLOAT fOriIncr,
                        BOOL bMap = FALSE );
    BOOL GetSIncrement( const INT nHub, const INT nSens, FLOAT& fPosIncr, FLOAT& fOriIncr );
    BOOL ResetSIncrement( const INT nHub, const INT nSens, BOOL bMap = FALSE );

    // Boresight
    BOOL SetSBoresight( const INT nHub, const INT nSens, const PDI4vec&, BOOL bMap = FALSE );
    BOOL SetSBoresight( const INT nHub, const INT nSens, const PDIori&, BOOL bMap = FALSE );
    BOOL GetSBoresight( const INT nHub, const INT nSens, PDIori& );
    BOOL GetSBoresight( const INT nHub, const INT nSens, PDIqtrn& );
    BOOL ResetSBoresight( const INT nHub, const INT nSens, BOOL bMap = FALSE );

    // Tip Offset
    BOOL SetSTipOffset( const INT nHub, const INT nSens, const PDIpos&, BOOL bMap = FALSE );
    BOOL GetSTipOffset( const INT nHub, const INT nSens, PDIpos& );
    BOOL ResetSTipOffset( const INT nHub, const INT nSens, BOOL bMap = FALSE );

    ////////////////////////////////////
    // Legacy PDIdev configuration commands supported
    ////////////////////////////////////
    BOOL GetFrameRate( ePiFrameRate& ); // Gets P&O Frame Rate, read-only
    BOOL SetMetric( BOOL bMet );        // Sets G4 position units to CM if TRUE, INCHES if FALSE
    BOOL GetMetric( BOOL& bMet );       // Returns TRUE if position units set to CM

    BOOL SetAttFilter( const CPDIfilter& f ); // Sets Attitude Filter on all hubs (-1)
    BOOL GetAttFilter( CPDIfilter& f );       // Gets Attitude Filter from hub 0
    BOOL SetPosFilter( const CPDIfilter& f ); // Sets Position Filter on all hubs (-1)
    BOOL GetPosFilter( CPDIfilter& f );       // Gets Position Filter from hub 0

    BOOL
    SetSourceFrame( const PDIori& ); // Sets system Frame of Reference Rotation in Euler Degrees
    BOOL GetSourceFrame( PDIori& );  // Sets system Frame of Reference Rotation in Euler Degrees

    BOOL SetSDataList( INT nStation,
                       const CPDImdat& d ); // Sets motion data output list on all hubs/all sensors
    BOOL GetSDataList( INT nStation,
                       CPDImdat& d ); // Gets motion data output list from hub 0/sensor 0

    BOOL SetSBoresight(
        INT nStation,
        const PDIori&,
        BOOL bReset = FALSE ); // Set boresight on all hubs/nStation in Euler Degrees, no RESET
    BOOL GetSBoresight( INT nStation,
                        PDIori& );        // Get boresight from hub0, nStation in Euler Degrees
    BOOL ClearSBoresight( INT nStation ); // UN-Boresight all hubs, nStation

    BOOL GetStationMap( DWORD& dwMap ); // Gets first DWORD of System Station Map
    BOOL GetDetectedStationMap( DWORD& dwMap );

    INT MaxSensors();
    INT MaxHubSensors();

    /////////////////////////////////////////////////////////////////////////////////
    // Runtime Methods
    /////////////////////////////////////////////////////////////////////////////////
    // Host P&O Buffer methods
    // Use base-class CPDIdev methods
    // BOOL	ClearPnoBuffer	( VOID );							// Clears output P&O output buffer
    // BOOL	LastPnoPtr		( PBYTE & pBuf, DWORD & dwSize );	// Returns pointer to last P&O frame
    // collected
    // BOOL	ResetPnoPtr		( VOID );							// Resets P&O output pointer to beginning of
    // buffer
    // BOOL	SetPnoBuffer	( PBYTE pBuf, DWORD dwSize );		// Sets/Clears output P&O output
    // buffer BOOL	ResetHostFrameCount ( VOID );						// Resets Host P&O frame
    // counter BOOL	LastHostFrameCount  ( DWORD & dwFrameCount );		// Returns index of last P&O
    // frame collected by the host

    // P&O Collection methods - Legacy data format, configurable with SetSDataList()
    BOOL ReadSinglePno( HWND hwnd ); // Reads single P&O frame. : posts data msg to HWND
    BOOL ReadSinglePnoBuf( PBYTE& pBuf,
                           DWORD& dwSize ); // Reads single P&O frame. : returns pointer to frame
    BOOL StartContPno( HWND hwnd ); // Starts continuous P&O collection: posts data msg to HWND
    BOOL StopContPno( VOID );       // Stops continuous P&O collection

    // P&O Collection methods - Native G4 hierarchical format		// Data is organized in blocks of
    // type G4_HUBDATA
    BOOL ReadSinglePnoG4( HWND hwnd );
    BOOL ReadSinglePnoBufG4( PBYTE& pBuf, DWORD& dwSize );
    BOOL StartContPnoG4( HWND hwnd );
    BOOL StopContPnoG4( VOID ) { return StopContPno(); }

    // Other G4 Run-Time commands
    BOOL GetActiveHubs( INT& nHubCount,
                        INT HubIDs[G4_MAX_HUB_COUNT] ); // Read list of currently active hub ids
                                                        // managed by the system.
    BOOL IsHubActive( const INT& nHub ); // Returns TRUE if hub is currently Active.  Does not
                                         // refresh Active Hub List
    BOOL GetHubSensorMap( const INT& nHub,
                          DWORD& dwMap );        // Read current station map from specified hub
    BOOL GetSysSensorMap( UINT64 maparray[16] ); // Read entire system station map, return in
    BOOL GetActiveSensorCount( INT& nStaCount ); // Returns current active sensor count.
    BOOL GetSourceMap( INT& nSrcCount,
                       PDI7vec SrcMap[] ); // Read list of sources and their position & rotation

    BOOL WhoAmI( LPCTSTR& ); // Legacy
    BOOL WhoAmI( LPCTSTR szTBuf, DWORD nBufSize );

    BOOL WhoAmIG4Sys( INT_PTR& nSystemID, INT_PTR& nMaxHubCount, LPCTSTR& );
    BOOL WhoAmIG4Hub( const INT nHub, LPCTSTR& );
    BOOL WhoAmIG4Sens( const INT nHub, const INT nSns, LPCTSTR& );

    /////////////////////////////////////////////////////////////////////////////////
    // Other Methods
    /////////////////////////////////////////////////////////////////////////////////
    ePiErrCode GetLastResult( VOID ) // Returns numeric result of last operation
    {
        return CPDIdev::GetLastResult();
    }
    ePiErrCode GetLastResult(
        ePiDevError& d ) // Returns numeric result of last operation, plus any device error code
    {
        return CPDIdev::GetLastResult( d );
    }
    LPCTSTR GetLastResultStr( VOID ) // Returns const string result of last operation
    {
        return CPDIdev::GetLastResultStr();
    }
    LPCTSTR ResultStr( ePiErrCode eCode,
                       ePiDevError eDevErr ) // Returns const string definition of enum result code
    {
        return CPDIdev::ResultStr( eCode, eDevErr );
    }
    INT_PTR MaxHubs( VOID );
    INT_PTR MaxG4Sensors( VOID );

    /////////////////////////////////////////////////////////////////////////////////
    // Troubleshooting Methods
    /////////////////////////////////////////////////////////////////////////////////
    PVOID Trace( BOOL bT, INT n = 0 ) // Enables trace output to debug window (DEBUG build only)
    {
        return CPDIdev::Trace( bT, n );
    }
    VOID Log( BOOL bLog ) // Enables tracker I/O logging to file PICMDIF.LOG
    {
        CPDIdev::Log( bLog );
    }

    ////////////////////////////////////
    // Pipe Export Methods
    ////////////////////////////////////
    BOOL StartPipeExport(
        LPCTSTR szPipeName = PDI_EXPORT_PIPE_NAME ) // Starts raw P&O export on named pipe
    {
        return CPDIdev::StartPipeExport( szPipeName );
    }
    BOOL CancelPipeExport( VOID ) // Cancels raw P&O export on named pipe
    {
        return CPDIdev::CancelPipeExport();
    }

    ////////////////////////////////////
    // Socket Export Methods
    ////////////////////////////////////
    BOOL StartSocketExport( LPCTSTR szInterfaceAddress = PDI_EXPORT_TCP_ADDR,
                            SHORT nSocketPort          = PDI_EXPORT_TCP_PORT ) {
        return CPDIdev::StartSocketExport( szInterfaceAddress, nSocketPort );
    } // Starts raw P&O on addr:port
    BOOL CancelSocketExport( VOID ) {
        return CPDIdev::CancelSocketExport();
    } // Cancels raw P&O export on addr:port

  private:
    CPDIg4( const CPDIg4& );

    CG4CmdIF* m_pG4;
};

///////////////////
// Inline legacy configuration functions mapped to G4-specific functions

inline BOOL CPDIg4::SetMetric( BOOL bMet ) {
    // Sets G4 position units to CM if TRUE, INCHES if FALSE
    if ( bMet )
        return SetPNOPosUnits( E_PDI_POS_CM, TRUE );
    else
        return SetPNOPosUnits( E_PDI_POS_INCH, TRUE );
}

inline BOOL CPDIg4::SetAttFilter( const CPDIfilter& f ) {
    // Sets Attitude Filter on all hubs (-1)
    return SetHAttFilter( PDI_ALL_DEVICES, f );
}
inline BOOL CPDIg4::GetAttFilter( CPDIfilter& f ) {
    // Gets Attitude Filter from hub 0
    return GetHAttFilter( 0, f );
}
inline BOOL CPDIg4::SetPosFilter( const CPDIfilter& f ) {
    // Sets Position Filter on all hubs (-1)
    return SetHPosFilter( PDI_ALL_DEVICES, f );
}
inline BOOL CPDIg4::GetPosFilter( CPDIfilter& f ) {
    // Gets Position Filter from hub 0
    return GetHPosFilter( 0, f );
}
inline BOOL CPDIg4::SetSDataList( INT nStation, const CPDImdat& d ) {
    // Sets motion data output list on all hubs/all sensors
    return SetPNODataList( d ); // Returns F if G4-UNsupported items are included in CPDImdat
}
inline BOOL CPDIg4::GetSDataList( INT nStation,
                                  CPDImdat& d ) // Gets motion data output list from hub 0/sensor 0
{
    GetPNODataList( d );
    return TRUE;
}
inline BOOL CPDIg4::ClearSBoresight( INT nStation ) // UN-Boresight all hubs, nStation
{
    return ResetSBoresight( PDI_ALL_DEVICES, nStation );
}
inline BOOL CPDIg4::GetDetectedStationMap( DWORD& dwMap ) {
    return GetStationMap( dwMap );
}
inline INT CPDIg4::MaxSensors() {
    return G4_LEGACY_FRAMEWORK_MAX_SENSORS;
}

#endif // _PDIG4_H_
