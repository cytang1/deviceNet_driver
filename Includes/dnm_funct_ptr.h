//------------------------------------------------------
//
// FILENAME : DNM_FUNCT_PTR.H
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


#ifndef __DNMSLAVE_FUNCT_POINTERS__
#define __DNMSLAVE_FUNCT_POINTERS__

#include "dnm_defn.h"
#include "dnm_strct.h"



//------------------------------------------------------
// DEVICENET MULTISLAVE FUNCTION PROTOTYPES
//------------------------------------------------------

#define DNMSLAVE_API32_API __declspec(dllimport)


typedef DNMSLAVE_API32_API BOOL (*dnm_LoadDriver_T)     ( CHAR * p_ch_DriverName );
typedef DNMSLAVE_API32_API BOOL (*dnm_Driver_T)         ( CHAR * p_ch_Buffer, WORD * p_w_Version, DWORD dw_Size );
typedef DNMSLAVE_API32_API BOOL (*dnm_FreeDriver_T)     ( void );

typedef DNMSLAVE_API32_API BOOL (*dnm_EnumDrivers_T)    ( DWORD dw_Index, CHAR * p_ch_Name, DWORD * p_dw_Length );
typedef DNMSLAVE_API32_API BOOL (*dnm_GetModuleHeader_T)( CARD_HANDLE CardHandle, DNM_MODULE_HEADER * p_ModuleHeader );
typedef DNMSLAVE_API32_API BOOL (*dnm_Version_T)        ( CHAR * p_ch_Buffer, WORD * p_w_Version, DWORD dw_Size );
     
typedef DNMSLAVE_API32_API BOOL (*dnm_OpenCard_T)       ( CARD_HANDLE * p_CardHandle, CHAR * p_ch_CardName, void * p_v_Module, DWORD dw_Flags );
typedef DNMSLAVE_API32_API BOOL (*dnm_CloseCard_T)      ( CARD_HANDLE CardHandle );
typedef DNMSLAVE_API32_API BOOL (*dnm_Online_T)         ( CARD_HANDLE CardHandle, DNM_ONLINE_CFG * p_OnlineCfg );
typedef DNMSLAVE_API32_API BOOL (*dnm_Offline_T)        ( CARD_HANDLE CardHandle );   

typedef DNMSLAVE_API32_API BOOL (*dnm_AddDevice_T)      ( CARD_HANDLE CardHandle, DNM_DEVICE_CFG * p_DeviceCfg, DWORD dw_AdditionalFlags );
typedef DNMSLAVE_API32_API BOOL (*dnm_DeleteDevice_T)   ( CARD_HANDLE CardHandle, WORD w_DeviceId );
typedef DNMSLAVE_API32_API BOOL (*dnm_EnableDevice_T)   ( CARD_HANDLE CardHandle, WORD w_DeviceId );
typedef DNMSLAVE_API32_API BOOL (*dnm_DisableDevice_T)  ( CARD_HANDLE CardHandle, WORD w_DeviceId );

typedef DNMSLAVE_API32_API BOOL (*dnm_GetBusStatus_T)   ( CARD_HANDLE CardHandle, WORD * p_w_BusStatus );
typedef DNMSLAVE_API32_API BOOL (*dnm_GetCANCounters_T) ( CARD_HANDLE CardHandle, DNM_CAN_COUNTERS * p_CANCounters );
typedef DNMSLAVE_API32_API BOOL (*dnm_GetCardStatus_T)  ( CARD_HANDLE CardHandle, BOOL * p_b_CardOk );

typedef DNMSLAVE_API32_API BOOL (*dnm_GetDevice_T)          ( CARD_HANDLE CardHandle, WORD w_DeviceId, DNM_DEVICE_CFG * p_DeviceCfg );
typedef DNMSLAVE_API32_API BOOL (*dnm_GetDeviceEvent_T)     ( CARD_HANDLE CardHandle, WORD w_DeviceId, BYTE by_EventId, BYTE * p_by_DeviceEvent );
typedef DNMSLAVE_API32_API BOOL (*dnm_GetDeviceStatus_T)    ( CARD_HANDLE CardHandle, WORD w_DeviceId, DNM_STATUS * p_DeviceStatus );
typedef DNMSLAVE_API32_API BOOL (*dnm_GetDeviceG2Status_T)  ( CARD_HANDLE CardHandle, WORD w_DeviceId, BYTE * p_by_G2Status );
typedef DNMSLAVE_API32_API BOOL (*dnm_GetDeviceMonitored_T) ( CARD_HANDLE CardHandle, WORD w_DeviceId, DNM_MONITORED_DATA_AGE * p_DeviceDataAge );
typedef DNMSLAVE_API32_API BOOL (*dnm_ClearDeviceEvent_T)   ( CARD_HANDLE CardHandle, WORD w_DeviceId, BYTE by_EventId, BYTE by_EventMask );

typedef DNMSLAVE_API32_API BOOL (*dnm_GetDevicePath_T)        ( CARD_HANDLE CardHandle, WORD w_DeviceId, BYTE by_PathId, DNM_PATH_CFG * p_PathCfg );
typedef DNMSLAVE_API32_API BOOL (*dnm_InitializePathBuffer_T) ( CARD_HANDLE CardHandle, WORD w_Offset, DNM_PATH_CFG * p_PathCfg );

typedef DNMSLAVE_API32_API BOOL (*dnm_IoActive_T)       ( CARD_HANDLE CardHandle, WORD w_DeviceId, WORD w_Timeout );
typedef DNMSLAVE_API32_API BOOL (*dnm_IoIdle_T)         ( CARD_HANDLE CardHandle, WORD w_DeviceId );

typedef DNMSLAVE_API32_API BOOL (*dnm_ReadDeviceIo_T)          ( CARD_HANDLE CardHandle, WORD w_DeviceId, BYTE by_IoArea, void * p_v_Buffer, WORD w_Size );
typedef DNMSLAVE_API32_API BOOL (*dnm_ReadDeviceIoMonitored_T) ( CARD_HANDLE CardHandle, WORD w_DeviceId, BYTE by_IoArea, void * p_v_Buffer, WORD w_Size, BYTE * p_by_DataAge );
typedef DNMSLAVE_API32_API BOOL (*dnm_WriteDeviceIo_T)         ( CARD_HANDLE CardHandle, WORD w_DeviceId, BYTE by_IoArea, void * p_v_Buffer, WORD w_Size );

typedef DNMSLAVE_API32_API BOOL (*dnm_ReceiveDeviceExplicit_T) ( CARD_HANDLE CardHandle, WORD w_DeviceId, DNM_EXPLICIT_REQUEST_CFG  * p_ExplicitRequest );
typedef DNMSLAVE_API32_API BOOL (*dnm_SendDeviceExplicit_T)    ( CARD_HANDLE CardHandle, WORD w_DeviceId, DNM_EXPLICIT_RESPONSE_CFG * p_ExplicitResponse );
typedef DNMSLAVE_API32_API BOOL (*dnm_LocalDeviceExplicit_T)   ( CARD_HANDLE CardHandle, WORD w_DeviceId, DNM_EXPLICIT_REQUEST_CFG  * p_ExplicitRequest, DNM_EXPLICIT_RESPONSE_CFG * p_ExplicitResponse );

typedef DNMSLAVE_API32_API BOOL (*dnm_RegisterBusStatusEvent_T)   ( CARD_HANDLE CardHandle, DWORD dw_ThreadId, DWORD dw_MsgId, LPARAM lParam );
typedef DNMSLAVE_API32_API BOOL (*dnm_RegisterDeviceEvent_T)      ( CARD_HANDLE CardHandle, WORD w_DeviceId, BYTE by_EventId, DWORD dw_ThreadId, DWORD dw_MsgId, LPARAM lParam );
typedef DNMSLAVE_API32_API BOOL (*dnm_UnRegisterBusStatusEvent_T) ( CARD_HANDLE CardHandle );
typedef DNMSLAVE_API32_API BOOL (*dnm_UnRegisterDeviceEvent_T)    ( CARD_HANDLE CardHandle, WORD w_DeviceId, BYTE by_EventId );

typedef DNMSLAVE_API32_API BOOL (*dnm_SetAccessTimeout_T)             ( CARD_HANDLE CardHandle, DWORD dw_Timeout );
typedef DNMSLAVE_API32_API BOOL (*dnm_SetEventNotificationInterval_T) ( CARD_HANDLE CardHandle, DWORD dw_Interval );

typedef DNMSLAVE_API32_API BOOL (*dnm_GetIdentity_T)    ( DWORD CardHandle, DNM_IDENTITY_DATA * p_IdentityCfg );
typedef DNMSLAVE_API32_API BOOL (*dnm_SetIdentity_T)    ( DWORD CardHandle, DNM_IDENTITY_DATA * p_IdentityCfg, DWORD dw_Flags );


#endif  // __DNMSLAVE_FUNCT_POINTERS__
