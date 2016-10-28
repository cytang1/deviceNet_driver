//------------------------------------------------------
//
// FILENAME : DNM_STRCT.H
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
// 1.2.0.0     Oct 19,2009   DK   Added VirtualTimeout variables to DNM_DEVICE_CFG and
//                                 added DNM_VIRTUAL_DATA_AGE structure.
// 1.2.1.0     Jan 04,2010   DK   Change 'Virtual' text to 'Monitored'.
//


#ifndef __DNMSLAVE_STRUCTURES_H__
#define __DNMSLAVE_STRUCTURES_H__


//------------------------------------------------------
// DEVICENET MULTISLAVE STRUCTURES
//------------------------------------------------------

#define DNM_MAX_PATH_DATA_LENGTH               100
//--------------------------
// Path Config
//--------------------------
typedef struct
{
    WORD w_MaxPathSize;
    WORD w_PathLength;
    BYTE by_Path[DNM_MAX_PATH_DATA_LENGTH];
} DNM_PATH_CFG;  


#define DNM_MAX_EXPLICIT_DATA_LENGTH           256
#define DNM_MAX_LOCAL_EXPLICIT_DATA_LENGTH      96
//--------------------------
// Explicit Request Config
//--------------------------
typedef struct
{
    WORD w_ConnectionId;
    WORD w_Size;
    WORD w_Service;
    WORD w_Class;
    WORD w_Instance;
    BYTE by_Data[DNM_MAX_EXPLICIT_DATA_LENGTH];
} DNM_EXPLICIT_REQUEST_CFG;  

//--------------------------
// Explicit Response Config
//--------------------------
typedef struct
{
    WORD w_ConnectionId;
    WORD w_Size;
    WORD w_Service;
    BYTE by_Data[DNM_MAX_EXPLICIT_DATA_LENGTH];
} DNM_EXPLICIT_RESPONSE_CFG;  


//--------------------------
// CAN Counters
//--------------------------
typedef struct
{
    WORD w_CanTx;
    WORD w_CanAck;
    WORD w_CanRx;
    WORD w_CanErr;
    WORD w_CanLost;
    WORD w_CanOverrun;
} DNM_CAN_COUNTERS;  

//--------------------------
// Module header
//--------------------------
typedef struct
{
    WORD w_ModuleType;
    WORD w_WinSize;
    WORD w_CardId;
    WORD w_KernelId;
    WORD w_KernelRev;
    WORD w_ModuleId;
    WORD w_ModuleRev;
    BYTE by_NetSerial[4];
    BYTE by_CardType[16];
    BYTE by_CardSerial[8];
    WORD w_IrqControl;
    WORD w_IrqStatus;
    WORD w_MainCode;
    WORD w_CanStatus;
    WORD w_CanTx;
    WORD w_CanAck;
    WORD w_CanRx;
    WORD w_CanErr;
    WORD w_CanLost;
    WORD w_CanOverrun;
    WORD w_AddCode;
    BYTE by_ModuleString[60];
    WORD w_MajorTickInterval;
    WORD w_MinorTickCount;
} DNM_MODULE_HEADER;

//--------------------------
// Online config data
//--------------------------
typedef struct
{
    WORD w_Reserved;
    WORD w_BaudRate;
} DNM_ONLINE_CFG;


//--------------------------
// Device config data
//--------------------------
typedef struct
{
    WORD w_MacId;
    WORD w_Reserved1;
    WORD w_Reserved2;
    WORD w_SlaveState;
    WORD w_ConnectionFlags;
    WORD w_ExplicitRequestSize;
    WORD w_ExplicitRequestOffset;
    WORD w_ExplicitResponseSize;
    WORD w_ExplicitResponseOffset;
    WORD w_MonitoredStrobeMasterMacId;
    WORD w_Output1Size;
    WORD w_Output1Offset;
    WORD w_Output1PathOffset;
    WORD w_Input1AutoGenerate;
    WORD w_Input1AutoGenTime;
    WORD w_Input1Size;
    WORD w_Input1Offset;
    WORD w_Input1PathOffset;
    WORD w_MonitoredTimeout1;
    WORD w_Output2Size;
    WORD w_Output2Offset;
    WORD w_Output2PathOffset;
    WORD w_Input2AutoGenerate;
    WORD w_Input2AutoGenTime;
    WORD w_Input2Size;
    WORD w_Input2Offset;
    WORD w_Input2PathOffset;
    WORD w_MonitoredTimeout2;
} DNM_DEVICE_CFG;


//--------------------------
// status structure
//--------------------------
typedef struct
{
    BYTE by_StatusCode;
    BYTE by_StatusFlags;
} DNM_STATUS;


//--------------------------
// Monitored Data Age structure
//--------------------------
// - Each counter increments every 10 ms until a new data packet
//    clears it to 0.
typedef struct
{
    BYTE by_Output1AgeCount;
    BYTE by_Input1AgeCount;
    BYTE by_Output2AgeCount;
    BYTE by_Input2AgeCount;
} DNM_MONITORED_DATA_AGE;


//--------------------------
// DNM Identity structure
//--------------------------
typedef struct
{
    WORD  w_MacId;
    WORD  w_VendorId;
    WORD  w_DeviceType;
    WORD  w_ProductCode;
    BYTE  by_MajorRev;
    BYTE  by_MinorRev;
    BYTE  by_NetworkSerial[4];
    BYTE  by_ProductName[33];
    BYTE  by_Reserved;
} DNM_IDENTITY_DATA;


typedef DWORD CARD_HANDLE;



#endif  // __DNMSLAVE_STRUCTURES_H__
