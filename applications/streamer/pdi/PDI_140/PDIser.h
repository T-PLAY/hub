/////////////////////////////////////////////////////////////////////
// Polhemus  www.polhemus.com
// © 2003-14 Polhemus, All Rights Reserved
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
//
//  Filename:           $Workfile: PDIser.h $
//
//  Project Name:       Polhemus Developer Interface
//
//  Description:        Serial Interface Description Class
//
//  VSS $Header: /PiDevTools12/Inc/PDIser.h 5     12/22/16 2:59p Suzanne $
//
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
#ifndef _PDISER_H_
#define _PDISER_H_

#include "PDIdefs.h"
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
class CPiSerialInfo;
/////////////////////////////////////////////////////////////////////
// CLASS CPDIser
/////////////////////////////////////////////////////////////////////
class PDI_API CPDIser
{
  public:
    CPDIser( ePiBaudRate br = PI_BR_115200, ePiParity par = PI_PARITY_NONE, INT nPort = 1 );
    virtual ~CPDIser( VOID );

    CPDIser& operator=( const CPDIser& );

    ePiBaudRate GetBaudRate( VOID ); // Get current baud setting
    ePiParity GetParity( VOID );     // Get current parity setting
    INT GetPort( VOID );             // Get current port number

    VOID SetBaudRate( ePiBaudRate ); // Set baudrate
    VOID SetParity( ePiParity );     // Set parity
    VOID SetPDISerialPort( INT );
    __declspec( deprecated( "** CPDIser::SetPort is deprecated and may be removed in a future "
                            "version of PDI. Use CPDIser::SetPDISerialPort() instead **" ) ) VOID
        SetPort( INT ); // Set port number

  private:
    CPDIser( const CPDIser& );

    CPiSerialInfo* m_pSI;

    friend class CPDIdev;
};

/////////////////////////////////////////////////////////////////////
// END $Workfile: PDIser.h $
/////////////////////////////////////////////////////////////////////
#endif // _PDISER_H_
