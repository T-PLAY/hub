/////////////////////////////////////////////////////////////////////
// Polhemus  www.polhemus.com
// © 2011-14 Polhemus, All Rights Reserved
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
//
//  Filename:           $Workfile: PDIfastrak.h $
//
//  Project Name:       Polhemus Developer Interface
//
//  Description:        FasTrak Device Object Class
//
//  VSS $Header: /PiDevTools14/Inc/PDIfastrak.h 28    4/24/19 4:50p Suzanne $
//
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
#ifndef _PDIFASTRAK_H_
#define _PDIFASTRAK_H_

/////////////////////////////////////////////////////////////////////

#include "PDIbiterrFT.h"
#include "PDIdev.h"
/////////////////////////////////////////////////////////////////////

class CFTCmdIF;
class CPDIser;
class CPDImdat;
class CPiError;

/////////////////////////////////////////////////////////////////////
typedef struct _FT_BIN_HDR_STRUCT {
    CHAR rectype; // '0' if P&O,  '2' if other cmd response
    CHAR station;
    CHAR err;

} * LPFT_BINHDR, FT_BINHDR;

typedef struct _FT_ASC_HDR_STRUCT {
    CHAR rectype;
    CHAR station;
    CHAR cmd;

} * LPFT_ASCHDR, FT_ASCHDR;

/////////////////////////////////////////////////////////////////////
// CLASS CPDIfastrak
/////////////////////////////////////////////////////////////////////

class PDI_API CPDIfastrak : public CPDIdev
{
  public:
    CPDIfastrak();
    ~CPDIfastrak();

    ////////////////////////////////////
    // Connection Methods
    ////////////////////////////////////
    BOOL CnxReady( VOID ) // Returns TRUE if Connection established
    {
        return CPDIdev::CnxReady();
    }
    ePiCommType CnxType( VOID ) // Returns current connection type.
    {
        return CPDIdev::CnxType();
    }
    virtual ePiCommType
    DiscoverCnx( BOOL bInitCfg = TRUE ); // Search for USB, then Serial Connection to device
                                         //{ return CPDIdev::DiscoverCnx( bInitCfg ); }
    BOOL
    ConnectSer( BOOL bInitCfg = FALSE ) // Attempts to connect via current serial port parameters
    {
        return CPDIdev::ConnectSer( bInitCfg );
    }
    BOOL ConnectUSB( BOOL bInitCfg = FALSE ) // Attempts to connect via USB device
    {
        return CPDIdev::ConnectUSB( bInitCfg );
    }
    BOOL Disconnect( VOID ) // Ends current connection
    {
        return CPDIdev::Disconnect();
    }

    // Serial Interface Parameters
    BOOL SetSerialIF( CPDIser* pS ) // Sets current serial port parameters locally: if no
                                    // connection, does not configure device
    {
        return CPDIdev::SetSerialIF( pS );
    }
    BOOL GetSerialIF( CPDIser* pS ) // Returns current serial port parameters locally: if no
                                    // connection, does not configure device
    {
        return CPDIdev::GetSerialIF( pS );
    }

    ////////////////////////////////////
    // Configuration Methods
    ////////////////////////////////////
    BOOL GetAttFilter( CPDIfilter& f ) // Gets Attitude Filter ( 'Y' Command )
    {
        return CPDIdev::GetAttFilter( f );
    }
    BOOL GetBinary( BOOL& b ) // Gets binary output mode( 'F' Command )
    {
        return CPDIdev::GetBinary( b );
    }
    BOOL GetFrameRate( INT_PTR& nRate ); // Gets P&O Frame Rate, depends on Active Station Count
    BOOL GetMetric( BOOL& b )            // Gets device to metric output ( 'U' Command )
    {
        return CPDIdev::GetMetric( b );
    }
    BOOL GetPosFilter( CPDIfilter& f ) // Gets Position Filter ( 'X' Command )
    {
        return CPDIdev::GetPosFilter( f );
    }
    BOOL GetStationMap( DWORD& dwMap ) // Gets Active Station Map ( 'l' Command )
    {
        return CPDIdev::GetStationMap( dwMap );
    }
    BOOL GetSyncMode( ePDIFTSyncMode& ); // Gets Synchronization Mode ('y' Command )

    BOOL SetPresetFilters( const ePDIFilter& eF ); // Sets Position AND Attitude Filters to OFF,
                                                   // LOW, MED, HEAVY. Do not use CUSTOM!
    BOOL SetAttFilter( const CPDIfilter& f )       // Sets Attitude Filter ( 'Y' Command )
    {
        return CPDIdev::SetAttFilter( f );
    }
    BOOL SetBinary( BOOL b ) // Sets device to binary output ( 'F' Command )
    {
        return CPDIdev::SetBinary( b );
    }
    BOOL SetMetric( BOOL b ) // Sets device to metric output ( 'U' Command )
    {
        return CPDIdev::SetMetric( b );
    }
    BOOL SetPosFilter( const CPDIfilter& f ) // Sets Position Filter ( 'X' Command )
    {
        return CPDIdev::SetPosFilter( f );
    }
    BOOL SetStationMap( const DWORD& dwMap ) // Sets Active Station Map ( 'l' Command )
    {
        return CPDIdev::SetStationMap( dwMap );
    }
    BOOL SetSyncMode( const ePDIFTSyncMode& ); // Gets Synchronization Mode ('y' Command )

    // Per-station configuration Notes:
    //	: nStation arguments are 1-based.
    //	: if (nStation == -1), all stations are configured

    virtual BOOL ClearSBoresight( INT nStation ) // UN-Boresight station ('b' command )
    {
        return CPDIdev::ClearSBoresight( nStation );
    }
    BOOL EnableStation( INT nStation, BOOL bEnable ) // Enables or disables station ( 'l' Command )
    {
        return CPDIdev::EnableStation( nStation, bEnable );
    }

    BOOL GetSDataList( INT nStation,
                       CPDImdat& mdat ) // Gets motion data output list per station ( 'O' Command )
    {
        return CPDIdev::GetSDataList( nStation, mdat );
    }

    BOOL GetSAlignment( INT nStation,
                        PDI3vec& O,
                        PDI3vec& X,
                        PDI3vec& Y ) // Gets Alignment Reference Frame per station ( 'A' Command )
    {
        return CPDIdev::GetSAlignment( nStation, O, X, Y );
    }
    virtual BOOL GetSBoresight( INT nStation,
                                PDIori& ori ) // Get boresight angles per station ( 'G' Command )
    {
        return CPDIdev::GetSBoresight( nStation, ori );
    }

    BOOL GetSHemisphere( INT nStation,
                         PDI3vec& vec ) // Get Hemisphere Of Operation per station ( 'H' Command )
    {
        return CPDIdev::GetSHemisphere( nStation, vec );
    }
    BOOL GetSHemiTrack( INT nStation,
                        BOOL& bHTrack ) // Query Hemisphere Tracking per station ('H' Command )
    {
        return CPDIdev::GetSHemiTrack( nStation, bHTrack );
    }
    BOOL GetSStylusMode(
        INT nStation,
        ePiStylusMode& eMode ) // n/a FT-I&II Get Stylus Mode per station ( 'e' Command )
    {
        return CPDIdev::GetSStylusMode( nStation, eMode );
    }

    BOOL ResetSAlignment( INT nStation ) // Reset Alignment per station ( '^R' Command )
    {
        return CPDIdev::ResetSAlignment( nStation );
    }

    BOOL GetSIncrement( INT nStation, FLOAT& fIncr ); // Get Station Increment ('I' Command )
    BOOL GetSSourceFrame(
        INT nStation,
        PDIori& ); // Gets Source Frame Rotation "Transmitter Mounting Frame" ( 'r' Command )

    BOOL GetSTipOffset( INT nStation, PDIpos& ); // Get Station Tip Offset ('N' Command )

    BOOL
    SetSDataList( INT nStation,
                  const CPDImdat& mdat ) // Sets motion data output list per station ( 'O' Command )
    {
        return CPDIdev::SetSDataList( nStation, mdat );
    }

    BOOL
    SetSAlignment( INT nStation,
                   const PDI3vec& O,
                   const PDI3vec& X,
                   const PDI3vec& Y ) // Sets Alignment Reference Frame per station ( 'A' Command )
    {
        return CPDIdev::SetSAlignment( nStation, O, X, Y );
    }
    virtual BOOL
    SetSBoresight( INT nStation,
                   const PDIori& ori ) // Set boresight per station ( 'G' and 'B' Commands )
    {
        return CPDIdev::SetSBoresight( nStation, ori, FALSE );
    }

    BOOL BoresightSta( INT nStation );                   // Boresight to angles set by 'G' command
    BOOL SetSBAngles( INT nStation, const PDIori& ori ); // Set boresight ref angles 'G'

    BOOL
    SetSHemisphere( INT nStation,
                    const PDI3vec& vec ) // Set Hemisphere Of Operation per station ( 'H' Command )
    {
        return CPDIdev::SetSHemisphere( nStation, vec );
    }
    BOOL SetSHemiTrack( INT nStation ) // Enable Hemisphere Tracking per station ('H' Command )
    {
        return CPDIdev::SetSHemiTrack( nStation );
    }
    BOOL SetSStylusMode( INT nStation,
                         ePiStylusMode eMode ) // Set Stylus Mode per station ( 'e' Command )
    {
        return CPDIdev::SetSStylusMode( nStation, eMode );
    }

    BOOL SetSIncrement( INT nStation, const FLOAT& fIncr ); // Set Station Increment ('I' Command )
    BOOL SetSSourceFrame(
        INT nStation,
        const PDIori& ); // Sets Source Frame Rotation "Transmitter Mounting Frame" ( 'r' Command )

    BOOL SetSTipOffset( INT nStation, const PDIpos& ); // Set Station Tip Offset ('N' Command )

    ////////////////////////////////////
    // Configuration management
    ////////////////////////////////////
    BOOL GetCfgLabel( LPCSTR& ); // Gets current configuration label. ( '^X' )

    BOOL SetCfgLabel( const LPCSTR ); // Assigns label to current configuration ( '^X' )
    BOOL SaveStartupCfg();            // Saves current config as startup config ('^K')

    BOOL ResetCfg(); // Reset system to factory default settings ( 'W' )

    // The following are better suited for Unicode builds
    BOOL GetCfgLabel( LPCTSTR szTBuf, DWORD nBufSize ); // Gets current configuration label. ( 'X' )
    BOOL SetCfgLabel( const LPCWSTR ); // Assigns label to current configuration ( 'X' )

    /////////////////////////////////////////////////////////////////////////////////
    // Runtime Methods
    /////////////////////////////////////////////////////////////////////////////////
    // Host P&O Buffer methods
    BOOL ClearPnoBuffer( VOID ) // Clears output P&O output buffer
    {
        return CPDIdev::ClearPnoBuffer();
    }
    BOOL LastPnoPtr( PBYTE& pBuf, DWORD& dwSize ) // Returns pointer to last P&O frame collected
    {
        return CPDIdev::LastPnoPtr( pBuf, dwSize );
    }
    BOOL ResetPnoPtr( VOID ) // Resets P&O output pointer to beginning of buffer
    {
        return CPDIdev::ResetPnoPtr();
    }
    BOOL SetPnoBuffer( PBYTE pBuf, DWORD dwSize ) // Sets/Clears output P&O output buffer
    {
        return CPDIdev::SetPnoBuffer( pBuf, dwSize );
    }
    BOOL ResetHostFrameCount( VOID ) // Resets Host P&O frame counter
    {
        return CPDIdev::ResetHostFrameCount();
    }
    BOOL LastHostFrameCount(
        DWORD& dwFrameCount ) // Returns index of last P&O frame collected by the host
    {
        return CPDIdev::LastHostFrameCount( dwFrameCount );
    }

    // P&O Collection methods
    BOOL ReadSinglePno( HWND ); // Reads single P&O frame. ('P' command): posts data msg to HWND
    BOOL ReadSinglePnoBuf(
        PBYTE& pBuf,
        DWORD& dwSize ) // Reads single P&O frame. ('P' command): returns pointer to frame
    {
        return CPDIdev::ReadSinglePnoBuf( pBuf, dwSize );
    }
    BOOL StartContPno( HWND h ) // Starts continuous P&O collection: posts data msg to HWND
    {
        return CPDIdev::StartContPno( h );
    }
    BOOL StopContPno( VOID ) // Stops continuous P&O collection
    {
        return CPDIdev::StopContPno();
    }
    BOOL StartStylusPno( HWND h ) // Starts stylus-driven P&O collection: posts data msgs to HWND
    {
        return CPDIdev::StartStylusPno( h );
    }
    BOOL StopStylusPno( VOID ) // Stops stylus-driven P&O collection
    {
        return CPDIdev::StopStylusPno();
    }

    VOID EmulateLibertyPno(
        BOOL bEm ); // Enable/disable Liberty-style P&O frame headers and frame bundling

    BOOL ResetTracker( VOID ) // Initializes Tracker, disconnects ( '^Y' Command )
    {
        return CPDIdev::ResetTracker();
    }
    BOOL TxtCmd( LPCSTR szCmd,
                 DWORD& dwRspSize,
                 LPCSTR szRsp = 0 ) // Sends generic text command to device
    {
        return CPDIdev::TxtCmd( szCmd, dwRspSize, szRsp );
    }
    BOOL WhoAmI( LPCSTR& szBuf ) // Issues tracker System Status command ('S' command ); puts result
                                 // in argument
    {
        return CPDIdev::WhoAmI( szBuf );
    }
    // n/a Fastrak BOOL	WhoAmISensor	( INT nSensor,  LPCSTR & szBuf )	// Issues sensor WhoAmI
    // command ( '^V' command ); puts result in argument 						{ return
    // CPDIdev::WhoAmISensor( nSensor, szBuf ); }

    // The following are better suited for Unicode builds
    BOOL TxtCmdW( LPCWSTR szWCmd,
                  DWORD& dwRspWSize,
                  LPCWSTR szWRsp = 0 ) // Sends generic text command to device
    {
        return CPDIdev::TxtCmd( szWCmd, dwRspWSize, szWRsp );
    }
    BOOL WhoAmI( LPCTSTR szTBuf, DWORD nBufSize ) { return CPDIdev::WhoAmI( szTBuf, nBufSize ); }

    BOOL GetBITErr( CPDIbiterrFT& );   // Read tracker BIT error data
    BOOL ClearBITErr( CPDIbiterrFT& ); // Clears specified tracker BIT error

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

    INT MaxSensors( VOID ) // Returns max number of sensors supported by device.
    {
        return CPDIdev::MaxSensors();
    }
    ePiTrackerType TrackerType( VOID ) // Returns tracker type.
    {
        return CPDIdev::TrackerType();
    }

    BOOL GetDetectedStationMap( DWORD& dwMap ) // Gets Detected Station Map
    {
        return CPDIdev::GetDetectedStationMap( dwMap );
    }
    INT StationCount( VOID ) // Returns number of detected, enabled stations
    {
        return CPDIdev::StationCount();
    }

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

    ////////////////////////////////////
    // Legacy PDIdev configuration commands NOT supported
    ////////////////////////////////////

    BOOL SetSyncEnabled( BOOL );
    BOOL GetSyncEnabled( BOOL& );

  private:
    CPDIfastrak( const CPDIfastrak& );

    CFTCmdIF* m_pFT;
};

#endif //_PDIFASTRAK_H_