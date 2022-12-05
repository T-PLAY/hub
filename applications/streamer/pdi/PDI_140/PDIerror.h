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
#ifndef _PDIERROR_H_
#define _PDIERROR_H_

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

#define PI_ERROR_BAND_MASK 0xffff0000
#define PI_ERROR_LEVEL_MASK 0x0000f000
#define PI_ERROR_LEVEL_WARN1 0x00001000
#define PI_HOST_SYS_ERROR_BAND 0x20010000
#define PI_HOST_ERROR_BAND 0x20020000
#define PI_DEVICE_ERROR_BAND 0x20040000
#define PI_API_ERROR_BAND 0x20080000

#define IS_PI_SYS_ERROR( e ) ( ( e & PI_ERROR_BAND_MASK ) == PI_HOST_SYS_ERROR_BAND )
#define IS_PI_HOST_ERROR( e ) ( ( e & PI_ERROR_BAND_MASK ) == PI_HOST_ERROR_BAND )
#define IS_PI_DEVICE_ERROR( e ) ( ( e & PI_ERROR_BAND_MASK ) == PI_DEVICE_ERROR_BAND )
#define IS_PI_API_ERROR( e ) ( ( e & PI_ERROR_BAND_MASK ) == PI_API_ERROR_BAND )
#define ERR_LEVEL( e ) ( ( e & PI_ERROR_LEVEL_MASK ) >> 12 )

typedef enum {
    PI_NOERROR = 0 // equivalent results, success & noerror
    ,
    PI_HOST_SYS_ERROR = PI_HOST_SYS_ERROR_BAND,
    PI_HOST_SYS_CNX_USB_CREATE_ERROR,
    PI_HOST_SYS_CNX_SER_CREATE_ERROR,
    PI_HOST_SYS_CNX_CFG_ERROR,
    PI_HOST_SYS_WRITE_ERROR,
    PI_HOST_SYS_WAIT_WRITE_ERROR,
    PI_HOST_SYS_WAIT_READ_ERROR,
    PI_HOST_SYS_READ_ERROR,
    PI_HOST_SYS_CLOSE_ERROR,
    PI_HOST_SYS_PIPE_CREATE_ERROR,
    PI_HOST_SYS_PIPE_DISC_ERROR,
    PI_HOST_SYS_PIPE_WRITE_ERROR
    // Socket Begin
    ,
    PI_HOST_SYS_TCP_CREATE_ERROR,
    PI_HOST_SYS_TCP_DISC_ERROR,
    PI_HOST_SYS_TCP_WRITE_ERROR,
    PI_HOST_SYS_UDP_CREATE_ERROR,
    PI_HOST_SYS_UDP_DISC_ERROR,
    PI_HOST_SYS_UDP_WRITE_ERROR
    // Socket End
    ,
    PI_HOST_SYS_CREATE_THREAD_ERROR,
    PI_HOST_SYS_GET_THREAD_STATUS_ERROR,
    PI_HOST_SYS_TERM_THREAD_ERROR,
    PI_HOST_SYS_DEVIO_ERROR,
    PI_HOST_SYS_CREATE_HANDLE_ERROR

    ,
    PI_HOST_ERROR = PI_HOST_ERROR_BAND,
    PI_HOST_NOCNX_ERROR,
    PI_HOST_BUILD_CMD_ERROR,
    PI_HOST_PARSE_RSP_ERROR,
    PI_HOST_STATE_ERROR,
    PI_HOST_NO_RESP,
    PI_HOST_FRAME_ERROR,
    PI_HOST_ASYNCINPROG_ERROR,
    PI_HOST_OVERFLOW_ERROR,
    PI_HOST_FRAME_IGNORED,
    PI_HOST_SERNUM_NOT_FOUND,
    PI_HOST_CONT_CMD_SENT = PI_HOST_SERNUM_NOT_FOUND + 1 + PI_ERROR_LEVEL_WARN1,
    PI_HOST_INFO          = PI_HOST_ERROR_BAND + 0x1001

    ,
    PI_DEVICE_ERROR = PI_DEVICE_ERROR_BAND,
    PI_DEVICE_NOT_DETECTED,
    PI_DEVICE_ERROR_FLAG,
    PI_DEVICE_CMD_FAILED,
    PI_DEVICE_NOT_RECOG,
    PI_DEVICE_NOT_FOUND,
    PI_DEVICE_INVALID_RESPONSE,
    PI_DEVICE_EXCEPTION,
    PI_DEVICE_INFO = PI_DEVICE_ERROR_BAND + 0x1001

    ,
    PI_API_ERROR = PI_API_ERROR_BAND,
    PI_API_INVALID_ARGS,
    PI_API_CONFIG_ERROR,
    PI_API_NO_HWND_PROVIDED,
    PI_API_SCOUT_DEV,
    PI_API_FT_DEV,
    PI_API_G4_DEV,
    PI_API_LIBERTY_DEV,
    PI_API_PATRIOT_DEV,
    PI_API_UNSUPPORTED,
    PI_API_UNSUPPORTED_PARAM,
    PI_API_UNSUPPORTED_TRACKER

} ePiErrCode;

/////////////////////////////////////////////////////////////////////
// END $Workfile: PDIerror.h $
/////////////////////////////////////////////////////////////////////
#endif // _PDIERROR_H_
