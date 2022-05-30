/////////////////////////////////////////////////////////////////////
// Polhemus  www.polhemus.com
// © 2003-14 Polhemus, All Rights Reserved
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
//
//  Filename:           $Workfile: PDIdefs.h $
//
//  Project Name:       Polhemus Developer Interface  
//
//  Description:        Definitions file
//
//  VSS $Header: /Viper/Apps/VPcmdMgr.root/VPcmdMgr/VPcmdMgr/legacy/PDIdefs.h 19    3/18/19 7:12p Suzanne $  
//
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
#if !defined(_PDIDEFS_H_)
#define _PDIDEFS_H_

/////////////////////////////////////////////////////////////////////

#if defined(PDI_EXPORTS)
#define PDI_API __declspec(dllexport)
#define PDI_TMPLT
#else
#define PDI_API __declspec(dllimport)
#define PDI_TMPLT extern
#endif

/////////////////////////////////////////////////////////////////////

#if defined(PSH_EXPORTS)
#if !defined(PSH_API)
#define PSH_API __declspec(dllexport)
#endif
#elif !defined(PSH_API)
#define PSH_API __declspec(dllimport)
#endif


/////////////////////////////////////////////////////////////////////
// Windows message ids
/////////////////////////////////////////////////////////////////////

#define WM_PI_DATA_STOPPED	    (WM_USER + 1110)
#define WM_PI_DATA_STARTED	    (WM_USER + 1111)
#define WM_PI_RAWDATA_READY		(WM_USER + 1112)
#define WM_PI_RAWDATA_ERROR		(WM_USER + 1113)
#define WM_PI_RAWDATA_WARNING	(WM_USER + 1114)
#define WM_PI_NATIVEDATA_READY	(WM_USER + 1115)
#define WM_PI_DATA_INFO			(WM_USER + 1116)

/////////////////////////////////////////////////////////////////////
// Connection types
/////////////////////////////////////////////////////////////////////
typedef enum
{
    PI_CNX_NONE = -2
    , PI_CNX_UNKNOWN = -1
    , PI_CNX_USB = 0
    , PI_CNX_SERIAL
	, PI_CNX_SOCKET

} ePiCommType;

/////////////////////////////////////////////////////////////////////
// Serial configuration parameters
/////////////////////////////////////////////////////////////////////
// Baudrate
typedef enum
{
	PI_BR_1200		= 0  // not used...
    , PI_BR_2400	        
    , PI_BR_4800        
    , PI_BR_9600        
    , PI_BR_19200       
    , PI_BR_38400       
    , PI_BR_57600       
    , PI_BR_115200    
	
	, PI_BR_MAX
} ePiBaudRate;

// Parity
typedef enum
{
    PI_PARITY_NONE  
    , PI_PARITY_ODD 
	, PI_PARITY_EVEN

	, PI_PARITY_MAX
} ePiParity;

/////////////////////////////////////////////////////////////////////
// Pipe Export definitions
/////////////////////////////////////////////////////////////////////

#define PDI_EXPORT_PIPE_NAME     _T("//./pipe/PDIPnOPipe")

// Socket Begin
/////////////////////////////////////////////////////////////////////
// TCP Export definitions
/////////////////////////////////////////////////////////////////////
#define PDI_EXPORT_TCP_ADDR     _T("0.0.0.0")
#define PDI_EXPORT_TCP_ADDR_INET (ULONG)0x00000000
#define PDI_EXPORT_TCP_PORT     7234
#define PDI_EXPORT_TCP_MAX_CLTS 10

/////////////////////////////////////////////////////////////////////
// UDP Export definitions
/////////////////////////////////////////////////////////////////////
#define PDI_EXPORT_UDP_ADDR     _T("127.0.0.1")
#define PDI_EXPORT_UDP_ADDR_INET  0x7f000001 
#define PDI_EXPORT_UDP_PORT     5123

/////////////////////////////////////////////////////////////////////
// Legacy PDI Socket Export definitions
/////////////////////////////////////////////////////////////////////
#define PDI_EXPORT_SOCK_ADDR  PDI_EXPORT_TCP_ADDR
#define PDI_EXPORT_SOCK_PORT  PDI_EXPORT_TCP_PORT

// Socket End

/////////////////////////////////////////////////////////////////////
// Frame Rate definitions
/////////////////////////////////////////////////////////////////////
typedef enum
{
	PI_FRATE_30				// FasTrak only
	, PI_FRATE_40			// FasTrak only
	, PI_FRATE_60 = 2
	, PI_FRATE_MIN = 3
    , PI_FRATE_120 = PI_FRATE_MIN
	, PI_FRATE_HALF = PI_FRATE_120
    , PI_FRATE_240 
	, PI_FRATE_NORMAL = PI_FRATE_240
	, PI_FRATE_MAX = PI_FRATE_240
} ePiFrameRate;

/////////////////////////////////////////////////////////////////////
// P&O type definitions
/////////////////////////////////////////////////////////////////////
typedef FLOAT	PDI3vec[3];
typedef FLOAT	PDI4vec[4];
typedef FLOAT	PDI6vec[6];
typedef FLOAT	PDI7vec[7];
typedef FLOAT	PDI3x3[3][3];

typedef PDI3vec PDIpos;
typedef PDI3vec PDIori;
typedef PDI6vec PDIpno;

typedef FLOAT	PDIqtrn[4];
typedef PDI3x3	PDIdircos;


/////////////////////////////////////////////////////////////////////
// Stylus Mode definitions
/////////////////////////////////////////////////////////////////////
typedef enum
{
	PI_STYMODE_MARKER = 0
	, PI_STYMODE_TRACK
    , PI_STYMODE_MOUSE = PI_STYMODE_MARKER
    , PI_STYMODE_POINT = PI_STYMODE_TRACK
} ePiStylusMode;

/////////////////////////////////////////////////////////////////////
// Hemisphere definitions
/////////////////////////////////////////////////////////////////////
typedef enum
{
	PI_HEM_X_POS
	, PI_HEM_X_NEG
	, PI_HEM_Y_POS
	, PI_HEM_Y_NEG
	, PI_HEM_Z_POS
	, PI_HEM_Z_NEG

	, PI_HEM_MAX
} ePiHemisphere;

/////////////////////////////////////////////////////////////////////
// Filter Preset definitions
/////////////////////////////////////////////////////////////////////

typedef enum
{
	E_PDI_FILTER_NONE
	, E_PDI_FILTER_LIGHT
	, E_PDI_FILTER_MED
	, E_PDI_FILTER_HEAVY
	, E_PDI_FILTER_CUSTOM

	, E_PDI_FILTER_END

} ePDIFilter;

/////////////////////////////////////////////////////////////////////
// Configuration management definitions
/////////////////////////////////////////////////////////////////////

#define PI_CONFIG_LABEL_LEN  16
typedef enum
{
    PICFG_SLOT_DEFAULT=0
    , PICFG_SLOT_1 
    , PICFG_SLOT_2
    , PICFG_SLOT_3

    , PICFG_MAX_SLOT
	, PICFG_NO_SLOT = -1
} ePiConfigSlot;

typedef CHAR	PDIlabels[PICFG_MAX_SLOT][PI_CONFIG_LABEL_LEN];

#define PDI_DEFAULT_CONFIG_LABEL "Default\0"
#define PDI_VACANT_CONFIG_LABEL "***************\0"

/////////////////////////////////////////////////////////////////////
// MinuteMan definitions
/////////////////////////////////////////////////////////////////////

typedef enum
{
	E_PMM_FILTER_OFF
	, E_PMM_FILTER_LIGHT
	, E_PMM_FILTER_MED
	, E_PMM_FILTER_HEAVY

} ePiMMPnoFilter;

/////////////////////////////////////////////////////////////////////
// FasTrak definitions
/////////////////////////////////////////////////////////////////////

typedef enum
{
	E_PFT_SYNC_INTERNAL
	, E_PFT_SYNC_EXTERNAL
	, E_PFT_SYNC_VIDEO

} ePDIFTSyncMode;


/////////////////////////////////////////////////////////////////////
// Units
/////////////////////////////////////////////////////////////////////

typedef enum {
	E_PDI_ORI_EULER_DEGREE
	, E_PDI_ORI_EULER_RADIAN
	, E_PDI_ORI_QUATERNION

	, E_PDI_ORI_MAX

} ePDIoriUnits;

typedef enum {
	E_PDI_POS_INCH
	, E_PDI_POS_FOOT
	, E_PDI_POS_CM
	, E_PDI_POS_METER

	, E_PDI_POS_MAX
} ePDIposUnits;


/////////////////////////////////////////////////////////////////////
// Other definitions
/////////////////////////////////////////////////////////////////////

#define PDI_ALL_SENSORS			-1
#define PDI_ALL_STATIONS		-1
#define PDI_ALL_DEVICES			-1
#define PI_MAX_CMD_BUF_LEN      200


/////////////////////////////////////////////////////////////////////
// END $Workfile: PDIdefs.h $
/////////////////////////////////////////////////////////////////////
#endif // _PDIDEFS_H_
