//------------------------------------------------------
//
// FILENAME : DNM_FUNCT.H
//
// VERSION  : 1.2.1.0
//
// AUTHOR   : d.k.
//
// (c) 2010 Molex Incorporated
//------------------------------------------------------

//------------------------------------------------------
// Revision History
//------------------------------------------------------

// REV           DATE        BY   Description
//------------------------------------------------------
// 1.1.0.0     Jul 15,2009   DK   Initial Release.
// 1.1.1.0     Aug 18,2009   DK   Change TCHAR to CHAR.
// 1.2.0.0     Oct 19,2009   DK   Add flags to SetIdentity, ReadDeviceIoVirtual and GetDeviceVirtual.
// 1.2.1.0     Jan 04,2010   DK   Change 'Virtual' text to 'Monitored'.
//


#ifndef __DNMSLAVE_FUNCTIONS_H__
#define __DNMSLAVE_FUNCTIONS_H__

#include "dnm_defn.h"
#include "dnm_strct.h"



//------------------------------------------------------
// DEVICENET MULTISLAVE FUNCTION PROTOTYPES
//------------------------------------------------------

#ifdef DNMSLAVE_API32_EXPORTS
#define DNMSLAVE_API32_API __declspec(dllexport)
#else
#define DNMSLAVE_API32_API __declspec(dllimport)
#endif


#ifdef __cplusplus
    extern "C" {
#endif

DNMSLAVE_API32_API BOOL DNM_LoadDriver( CHAR * p_ch_DriverName );
DNMSLAVE_API32_API BOOL DNM_Driver(     CHAR * p_ch_Buffer, WORD * p_w_Version, DWORD dw_Size );
DNMSLAVE_API32_API BOOL DNM_FreeDriver( void );

DNMSLAVE_API32_API BOOL DNM_EnumDrivers(             DWORD dw_Index, CHAR * p_ch_Name, DWORD * p_dw_Length );
DNMSLAVE_API32_API BOOL DNM_GetModuleHeader( CARD_HANDLE CardHandle, DNM_MODULE_HEADER * p_ModuleHeader );
DNMSLAVE_API32_API BOOL DNM_Version(             CHAR * p_ch_Buffer, WORD * p_w_Version, DWORD dw_Size );
     
DNMSLAVE_API32_API BOOL DNM_OpenCard(  CARD_HANDLE * p_CardHandle, CHAR * p_ch_CardName, void * p_v_Module, DWORD dw_Flags );
DNMSLAVE_API32_API BOOL DNM_CloseCard( CARD_HANDLE CardHandle );
DNMSLAVE_API32_API BOOL DNM_Online(    CARD_HANDLE CardHandle, DNM_ONLINE_CFG * p_OnlineCfg );
DNMSLAVE_API32_API BOOL DNM_Offline(   CARD_HANDLE CardHandle );   

DNMSLAVE_API32_API BOOL DNM_AddDevice(     CARD_HANDLE CardHandle, DNM_DEVICE_CFG * p_DeviceCfg, DWORD dw_AdditionalFlags );
DNMSLAVE_API32_API BOOL DNM_DeleteDevice(  CARD_HANDLE CardHandle, WORD w_DeviceId );
DNMSLAVE_API32_API BOOL DNM_EnableDevice(  CARD_HANDLE CardHandle, WORD w_DeviceId );
DNMSLAVE_API32_API BOOL DNM_DisableDevice( CARD_HANDLE CardHandle, WORD w_DeviceId );

DNMSLAVE_API32_API BOOL DNM_GetBusStatus(   CARD_HANDLE CardHandle, WORD * p_w_BusStatus );
DNMSLAVE_API32_API BOOL DNM_GetCANCounters( CARD_HANDLE CardHandle, DNM_CAN_COUNTERS * p_CANCounters );
DNMSLAVE_API32_API BOOL DNM_GetCardStatus(  CARD_HANDLE CardHandle, BOOL * p_b_CardOk );

DNMSLAVE_API32_API BOOL DNM_GetDevice(          CARD_HANDLE CardHandle, WORD w_DeviceId, DNM_DEVICE_CFG * p_DeviceCfg );
DNMSLAVE_API32_API BOOL DNM_GetDeviceEvent(     CARD_HANDLE CardHandle, WORD w_DeviceId, BYTE by_EventId, BYTE * p_by_DeviceEvent );
DNMSLAVE_API32_API BOOL DNM_GetDeviceStatus(    CARD_HANDLE CardHandle, WORD w_DeviceId, DNM_STATUS * p_DeviceStatus );
DNMSLAVE_API32_API BOOL DNM_GetDeviceG2Status(  CARD_HANDLE CardHandle, WORD w_DeviceId, BYTE * p_by_G2Status );
DNMSLAVE_API32_API BOOL DNM_GetDeviceMonitored( CARD_HANDLE CardHandle, WORD w_DeviceId, DNM_MONITORED_DATA_AGE * p_DeviceDataAge );
DNMSLAVE_API32_API BOOL DNM_ClearDeviceEvent(   CARD_HANDLE CardHandle, WORD w_DeviceId, BYTE by_EventId, BYTE by_EventMask );

DNMSLAVE_API32_API BOOL DNM_GetDevicePath(        CARD_HANDLE CardHandle, WORD w_DeviceId, BYTE by_PathId, DNM_PATH_CFG * p_PathCfg );
DNMSLAVE_API32_API BOOL DNM_InitializePathBuffer( CARD_HANDLE CardHandle, WORD w_Offset, DNM_PATH_CFG * p_PathCfg );

DNMSLAVE_API32_API BOOL DNM_IoActive( CARD_HANDLE CardHandle, WORD w_DeviceId, WORD w_Timeout );
DNMSLAVE_API32_API BOOL DNM_IoIdle(   CARD_HANDLE CardHandle, WORD w_DeviceId );

DNMSLAVE_API32_API BOOL DNM_ReadDeviceIo(          CARD_HANDLE CardHandle, WORD w_DeviceId, BYTE by_IoArea, void * p_v_Buffer, WORD w_Size );
DNMSLAVE_API32_API BOOL DNM_ReadDeviceIoMonitored( CARD_HANDLE CardHandle, WORD w_DeviceId, BYTE by_IoArea, void * p_v_Buffer, WORD w_Size, BYTE * p_by_DataAge );
DNMSLAVE_API32_API BOOL DNM_WriteDeviceIo(         CARD_HANDLE CardHandle, WORD w_DeviceId, BYTE by_IoArea, void * p_v_Buffer, WORD w_Size );

DNMSLAVE_API32_API BOOL DNM_ReceiveDeviceExplicit( CARD_HANDLE CardHandle, WORD w_DeviceId, DNM_EXPLICIT_REQUEST_CFG  * p_ExplicitRequest );
DNMSLAVE_API32_API BOOL DNM_SendDeviceExplicit(    CARD_HANDLE CardHandle, WORD w_DeviceId, DNM_EXPLICIT_RESPONSE_CFG * p_ExplicitResponse );
DNMSLAVE_API32_API BOOL DNM_LocalDeviceExplicit(   CARD_HANDLE CardHandle, WORD w_DeviceId, DNM_EXPLICIT_REQUEST_CFG  * p_ExplicitRequest, DNM_EXPLICIT_RESPONSE_CFG * p_ExplicitResponse );

DNMSLAVE_API32_API BOOL DNM_RegisterBusStatusEvent(   CARD_HANDLE CardHandle, DWORD dw_ThreadId, DWORD dw_MsgId, LPARAM lParam );
DNMSLAVE_API32_API BOOL DNM_RegisterDeviceEvent(      CARD_HANDLE CardHandle, WORD w_DeviceId, BYTE by_EventId, DWORD dw_ThreadId, DWORD dw_MsgId, LPARAM lParam );
DNMSLAVE_API32_API BOOL DNM_UnRegisterBusStatusEvent( CARD_HANDLE CardHandle );
DNMSLAVE_API32_API BOOL DNM_UnRegisterDeviceEvent(    CARD_HANDLE CardHandle, WORD w_DeviceId, BYTE by_EventId );

DNMSLAVE_API32_API BOOL DNM_SetAccessTimeout(             CARD_HANDLE CardHandle, DWORD dw_Timeout );
DNMSLAVE_API32_API BOOL DNM_SetEventNotificationInterval( CARD_HANDLE CardHandle, DWORD dw_Interval );

DNMSLAVE_API32_API BOOL DNM_GetIdentity( DWORD CardHandle, DNM_IDENTITY_DATA * p_IdentityCfg );
DNMSLAVE_API32_API BOOL DNM_SetIdentity( DWORD CardHandle, DNM_IDENTITY_DATA * p_IdentityCfg, DWORD dw_Flags );


#ifdef __cplusplus
    }
#endif


#endif  // __DNMSLAVE_FUNCTIONS_H__
