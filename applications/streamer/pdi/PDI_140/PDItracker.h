/////////////////////////////////////////////////////////////////////
// Polhemus  www.polhemus.com
// © 2003-14 Polhemus, All Rights Reserved
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
//
//
//
//
//
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
#ifndef _PDITRACKER_H_
#define _PDITRACKER_H_

/////////////////////////////////////////////////////////////////////
// Liberty Binary Mode Header Format
/////////////////////////////////////////////////////////////////////
typedef struct _BIN_HDR_STRUCT {
    short preamble;
    unsigned char station;
    unsigned char cmd;
    unsigned char err;
    unsigned char reserved;
    short length;

} * LPBINHDR, BINHDR;

#define LIBERTY_PREAMBLE 0x594C
#define PATRIOT_PREAMBLE 0x4150
#define LATUS_PREAMBLE 0x554C
#define MINUTEMAN_PREAMBLE 0x4D4D
#define PALATUS_PREAMBLE 0x4C50
#define HOST_PREAMBLE 0x2A2A
#define FT_PREAMBLE 0x5446

/////////////////////////////////////////////////////////////////////
// Liberty WhoAmI Binary Mode Header Format
/////////////////////////////////////////////////////////////////////
typedef struct _BINWHOAMI_STRUCT {
    unsigned char ucMaxSensors;
    unsigned char ucTrackerType;
    unsigned char ucReserved[6];

} * LPBINWHO, BINWHO;

/////////////////////////////////////////////////////////////////////
// Tracker Types
/////////////////////////////////////////////////////////////////////
typedef enum {
    PI_TRK_LIBERTY,
    PI_TRK_PATRIOT,
    PI_TRK_LATUS,
    PI_TRK_MINUTEMAN,
    PI_TRK_PALATUS,
    PI_TRK_SCOUT,
    PI_TRK_G4,
    PI_TRK_FASTRAK,
    PI_TRK_FT3

    ,
    PI_TRK_UNKNOWN,
    PI_TRK_MAX = PI_TRK_UNKNOWN
} ePiTrackerType;

#define IS_FT( eT ) ( ( eT == PI_TRK_FASTRAK ) || ( eT == PI_TRK_FT3 ) )

#define LIBERTY_MAX_SENSORS 16
#define PATRIOT_MAX_SENSORS 2
#define LATUS_MAX_MARKERS 12
#define PALATUS_MAX_MARKERS 4
#define MM_MAX_SENSORS 2
#define FT_MAX_SENSORS 4

#define LATUS_MAX_RECEPTORS LIBERTY_MAX_SENSORS
#define PALATUS_MAX_RECEPTORS PATRIOT_MAX_SENSORS

/////////////////////////////////////////////////////////////////////
// Liberty Error Codes
/////////////////////////////////////////////////////////////////////
typedef enum {
    PI_DEVERR_NO_ERROR = 0 //	0   ' '

    // Command errors
    ,
    PI_DEVERR_INVALID_COMMAND = 1 //	1	// 0x01
    ,
    PI_DEVERR_STATION_OUT_OF_RANGE //	2	// 0x02
    ,
    PI_DEVERR_INVALID_PARAMETER //	3	// 0x03
    ,
    PI_DEVERR_TOO_FEW_PARAMETERS //	4	// 0x04
    ,
    PI_DEVERR_TOO_MANY_PARAMETERS //	5	// 0x05
    ,
    PI_DEVERR_PARAMETER_BELOW_LIMIT //	6	// 0x06
    ,
    PI_DEVERR_PARAMETER_ABOVE_LIMIT //	7	// 0x07
    ,
    PI_DEVERR_SP_COM_FAILURE //	8	// 0x08
    ,
    PI_DEVERR_SP1_INIT_FAILURE //	9	// 0x09
    ,
    PI_DEVERR_SP2_INIT_FAILURE //	10	// 0x0a
    ,
    PI_DEVERR_SP3_INIT_FAILURE //	11	// 0x0b
    ,
    PI_DEVERR_SP4_INIT_FAILURE //	12	// 0x0c
    ,
    PI_DEVERR_SP_NONE_DETECTED //	13	// 0x0d
    ,
    PI_DEVERR_SRC_INIT_FAILURE //	14	// 0x0e
    ,
    PI_DEVERR_MEM_ALLOC_ERROR //	15	// 0x0f
    ,
    PI_DEVERR_EXCESS_CMD_CHARS //	16	// 0x10
    ,
    PI_DEVERR_EXIT_UTH //	17	// 0x11
    ,
    PI_DEVERR_SOURCE_READ_ERROR //	18	// 0x12
    ,
    ,
    ,
    ,
    ,
    ,

    ,
    ,
    ,
    ,
    ,
    ,
    ,
    ,
    ,
    ,
    ,
    ,

    // Realtime errors
    ,
    ,
    ,
    ,
    ,
    ,
    ,

    ,

    ,
    ,
    ,
    ,
    ,
    ,
    ,

    ,

    ,
    ,
    PI_DEVERR_SRC_SIG_NOT_LOCKED //	71	// 0x47
    ,
    PI_DEVERR_SNS_CAL_DATA //	72	// 0x48

    ,
    PI_DEVERR_MAX
} ePiDevError;

typedef enum {
    PI_G4_ERROR_NONE = PI_DEVERR_NO_ERROR

    ,
    PI_G4_ERROR_NO_FRAME_DATA_AVAIL = -100 // -100
    ,
    ,
    ,
    ,
    ,
    ,
    ,
    ,
    ,
    ,
    ,
    ,
    ,
    ,
    ,
    ,
    ,
    ,
    ,
    ,
    ,

    ,
    PI_G4_ERROR_MAX
} ePiG4Error;

typedef enum {
    PI_INFO_NONE,
    PI_INFO_G4HUBCOUNT_CHANGED = 1

    ,
    PI_INFO_MAX
} ePiHostInfo;

/////////////////////////////////////////////////////////////////////
// END $Workfile: PDItracker.h $
/////////////////////////////////////////////////////////////////////
#endif // _PDITRACKER_H_
