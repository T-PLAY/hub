/////////////////////////////////////////////////////////////////////
// Polhemus  www.polhemus.com
// © 2004-14 Polhemus, All Rights Reserved
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
//
//  Filename:           $Workfile: PDIbiterrFT.h $
//
//  Project Name:       Polhemus Tracker Command Interface API
//
//  Original Author:    S. Gagnon
//
//  Description:        Interface for the CFTBITErr class
//
//  VSS $Header: /PiDevTools11/Inc/PDIbiterrFT.h 2     1/09/14 1:05p Suzanne $
//
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
#ifndef _PDIBITERRFT_H_
#define _PDIBITERRFT_H_

class CFTBITErr;

/////////////////////////////////////////////////////////////////////
// CLASS CPDIbiterrFT
/////////////////////////////////////////////////////////////////////

class PDI_API CPDIbiterrFT
{
  public:
    CPDIbiterrFT();
    virtual ~CPDIbiterrFT();

    VOID Parse( LPTSTR szBuf,
                DWORD dwSize ) const; // Parses BIT results into provided string buffer
    BOOL IsClear( VOID );

  private:
    CPDIbiterrFT( const CPDIbiterrFT& );

    CFTBITErr* m_pFTB;

    friend class CPDIfastrak;
};

#endif