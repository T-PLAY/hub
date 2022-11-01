/////////////////////////////////////////////////////////////////////
// Polhemus  www.polhemus.com
// © 2005-14 Polhemus, All Rights Reserved
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
//
//
//
//
//
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

#ifndef _PDILATUS_H_
#define _PDILATUS_H_

/////////////////////////////////////////////////////////////////////

#include "PDIdev.h"

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
// CLASS CPDIlatus
/////////////////////////////////////////////////////////////////////
class PDI_API CPDIlatus : public CPDIdev
{
  public:
    CPDIlatus( VOID );

    ////////////////////////////////////
    // Configuration Methods
    ////////////////////////////////////
    BOOL GetAutoLaunchMode( BOOL& bMode ); // Gets AutoLaunch Mode ('@A' command )
    BOOL GetAutoLaunchRange( INT& nLaunch,
                             INT& nUnLaunch ); // Gets AutoLaunch Range Criteria ('^G' command )

    BOOL SetAutoLaunchMode( const BOOL& bMode ); // Sets AutoLaunch Mode ('@A' command )
    BOOL
    SetAutoLaunchRange( const INT& nLaunch,
                        const INT& nUnLaunch ); // Sets AutoLaunch Range Criteria ('^G' command )

    // Per-marker configuration Notes:
    //	: nMarker arguments are 1-based.
    //	: if (nMarker == -1), all markers are configured

    BOOL GetMrkIncrement( INT nMarker,
                          FLOAT& flPosIncr,
                          FLOAT& flOriIncr ); // Gets Marker Increment ('^N' command )
    BOOL SetMrkIncrement( INT nMarker,
                          const FLOAT& flPosIncr,
                          const FLOAT& flOriIncr ); // Sets Marker Increment ('^N' command )

    ////////////////////////////////////
    // Close Range Mode (Patriot WIRELESS only
    ////////////////////////////////////

    BOOL GetRcpCloseRangeMode(
        INT nReceptor,
        BOOL& bCRMode ); // Gets Receptor Close Range Mode ('@R' command ) Pat-W ONLY
    BOOL SetRcpCloseRangeMode( INT nReceptor,
                               const BOOL& bCRMode ); // Sets Receptor Close Range Mode

    ////////////////////////////////////
    // Receptor Alignment
    ////////////////////////////////////
    BOOL GetRcpMap( DWORD& dwMap );      // Gets Detected Receptor Map ( '^B' Command )
    BOOL GetRcpAlignMap( DWORD& dwMap ); // Gets Aligned Receptor Map ( '^B' Command )
    BOOL GetRcpAlignment( INT nReceptor,
                          PDIpos& pos,
                          PDIori& ori ); // Get Receptor Alignment ('^A' command )
    BOOL SetRcpAlignment( INT nReceptor,
                          const PDIpos& pos,
                          const PDIori& ori ); // Set Receptor Alignment ('^A' command )
    BOOL StoreStartupRcpConfig( VOID );        // Store Startup Receptor Alignments ('^S1' command )
    BOOL EraseStartupRcpConfig( VOID );        // Erase Startup Receptor Alignments ('^S0' command )

    ////////////////////////////////////
    // Runtime Methods
    ////////////////////////////////////
    BOOL GetMrkBoresightState( INT nMarker, BOOL& ); // Get Marker Boresight State ( 'B' Command )
    BOOL SetMrkBoresight( INT nMarker, const PDIori& ); // Set Marker Boresight ( 'B' Command )
    BOOL ClearMrkBoresight( INT nMarker );              // UN-Boresight Marker ( 'B' Command )

    BOOL GetRefFrame( PDIpno& ); // Get Marker Reference Frame ('G' command )

    BOOL SetRefFrameAbs( const PDIpno& pno ); // Set Absolute Marker Reference Frame ('G2' command )
    BOOL ResetRefFrame( VOID );               // Reset Marker Reference Frame ('G3' command )
    BOOL SetRefFrameMrkOri(
        VOID ); // Set Marker Reference Frame Rotation to Marker Ori ('G1' command )
    BOOL SetRefFrameMrkPno(
        VOID ); // Set Marker Reference Frame Translation & Rotation to marker P&O ('G0' command )

    BOOL MarkerPhaseStep( INT nMarker ); // Phase-Step Marker ('^P' command )

    BOOL MarkerLaunch( INT nReceptor ); // Launch Marker ('L' command )
    BOOL MarkerLaunchExt( INT nReceptor,
                          INT nMarker,
                          ePiHemisphere eHem ); // Launch Marker - Extended Control (Pat-W only)
    BOOL MarkerUnlaunch( INT nMarker );         // Unlaunch Marker ('^L' command )

    BOOL GetActiveMarkerMap( DWORD& dwMap ) // Gets Active Station Map ( '^U' Command )
    {
        return GetStationMap( dwMap );
    }
    BOOL GetLaunchedMarkerMap( DWORD& dwMap ) // Gets launched marker map ('^U' Command )
    {
        return GetDetectedStationMap( dwMap );
    }

    BOOL WhoAmIReceptor(
        INT nReceptor,
        LPCSTR& szWhoAmI ) // Issues sensor WhoAmI command ( '^V' command ); puts result in argument
    {
        return WhoAmISensor( nReceptor, szWhoAmI );
    }
    BOOL WhoAmIMarker( INT nMarker, LPCSTR& szWhoAmI );

    // unicode versions
    BOOL WhoAmIReceptor( INT nReceptor, LPCTSTR szTBuf, DWORD nBufSize ) {
        return WhoAmISensor( nReceptor, szTBuf, nBufSize );
    }
    BOOL WhoAmIMarker( INT nMarker, LPCTSTR szTBuf, DWORD nBufSize );

    ////////////////////////////////////
    // Other Methods
    ////////////////////////////////////
    INT MaxReceptors( VOID ); // Returns max number of receptors supported by device.
    INT MaxMarkers( VOID );   // Returns max number of markers supported by device.

  private:
    CPDIlatus( const CPDIlatus& );
};

/////////////////////////////////////////////////////////////////////
// END $Workfile: PDIlatus.h $
/////////////////////////////////////////////////////////////////////
#endif // _PDILATUS_H_
