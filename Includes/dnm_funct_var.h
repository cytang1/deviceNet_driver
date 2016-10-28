//------------------------------------------------------
//
// FILENAME : DNM_FUNCT_VAR.H
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
// 1.2.0.0     Oct 19,2009   DK   Added ReadDeviceIoVirtual and GetDeviceVirtual.
// 1.2.1.0     Jan 04,2010   DK   Change 'Virtual' text to 'Monitored'.
//


#ifndef __DNMSLAVE_FUNCT_VARIABLES__
#define __DNMSLAVE_FUNCT_VARIABLES__

#include "dnm_funct_ptr.h"


//------------------------------------------------------
// DEVICENET MULTISLAVE FUNCTION VARIABLES
//------------------------------------------------------


dnm_LoadDriver_T                    DNM_LoadDriver;
dnm_Driver_T                        DNM_Driver;
dnm_FreeDriver_T                    DNM_FreeDriver;

dnm_EnumDrivers_T                   DNM_EnumDrivers;
dnm_GetModuleHeader_T               DNM_GetModuleHeader;
dnm_Version_T                       DNM_Version;
     
dnm_OpenCard_T                      DNM_OpenCard;
dnm_CloseCard_T                     DNM_CloseCard;
dnm_Online_T                        DNM_Online;
dnm_Offline_T                       DNM_Offline;   

dnm_AddDevice_T                     DNM_AddDevice;
dnm_DeleteDevice_T                  DNM_DeleteDevice;
dnm_EnableDevice_T                  DNM_EnableDevice;
dnm_DisableDevice_T                 DNM_DisableDevice;

dnm_GetBusStatus_T                  DNM_GetBusStatus;
dnm_GetCANCounters_T                DNM_GetCANCounters;
dnm_GetCardStatus_T                 DNM_GetCardStatus;

dnm_GetDevice_T                     DNM_GetDevice;
dnm_GetDeviceEvent_T                DNM_GetDeviceEvent;
dnm_GetDeviceStatus_T               DNM_GetDeviceStatus;
dnm_GetDeviceG2Status_T             DNM_GetDeviceG2Status;
dnm_GetDeviceMonitored_T            DNM_GetDeviceMonitored;
dnm_ClearDeviceEvent_T              DNM_ClearDeviceEvent;

dnm_GetDevicePath_T                 DNM_GetDevicePath;
dnm_InitializePathBuffer_T          DNM_InitializePathBuffer;

dnm_IoActive_T                      DNM_IoActive;
dnm_IoIdle_T                        DNM_IoIdle;

dnm_ReadDeviceIo_T                  DNM_ReadDeviceIo;
dnm_ReadDeviceIoMonitored_T         DNM_ReadDeviceIoMonitored;
dnm_WriteDeviceIo_T                 DNM_WriteDeviceIo;

dnm_ReceiveDeviceExplicit_T         DNM_ReceiveDeviceExplicit;
dnm_SendDeviceExplicit_T            DNM_SendDeviceExplicit;
dnm_LocalDeviceExplicit_T           DNM_LocalDeviceExplicit;

dnm_RegisterBusStatusEvent_T        DNM_RegisterBusStatusEvent;
dnm_RegisterDeviceEvent_T           DNM_RegisterDeviceEvent;
dnm_UnRegisterBusStatusEvent_T      DNM_UnRegisterBusStatusEvent;
dnm_UnRegisterDeviceEvent_T         DNM_UnRegisterDeviceEvent;

dnm_SetAccessTimeout_T              DNM_SetAccessTimeout;
dnm_SetEventNotificationInterval_T  DNM_SetEventNotificationInterval;

dnm_GetIdentity_T                   DNM_GetIdentity;
dnm_SetIdentity_T                   DNM_SetIdentity;



#endif  // __DNMSLAVE_FUNCT_VARIABLES__
