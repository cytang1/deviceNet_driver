//------------------------------------------------------
//
// FILENAME : DNM_DEFN.H
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
// 1.2.0.0     Oct 19,2009   DK   Minor reformatting.
//                                Add DNM_SETIDENTITY_IGNORE_DEVICE_ID flag.
//                                Add DNM_EVENT_VIRTUAL_IO flags.
// 1.2.1.0     Jan 04,2010   DK   Change 'Virtual' text to 'Monitored'.
//


#ifndef __DNMSLAVE_DEFINITIONS_H__
#define __DNMSLAVE_DEFINITIONS_H__


//------------------------------------------------------
// DEVICENET MULTISLAVE DEFINITIONS
//------------------------------------------------------

//--------------------------
// DeviceNet Slave Device MAC ID
//--------------------------
#define DNM_DEVICE_MIN_MAC_ID                          0
#define DNM_DEVICE_MAX_MAC_ID                         63
#define DNM_DEVICE_NUM_MAC_ID                         64
#define DNM_DEVICE_APPLY_TO_ALL                   0x1000

//--------------------------
// DeviceNet BaudRates
//--------------------------
#define DNM_BAUD_RATE_125K                             0
#define DNM_BAUD_RATE_250K                             1
#define DNM_BAUD_RATE_500K                             2

//--------------------------
// CAN Bus Status Bits
//--------------------------
#define DNM_CAN_BUS_STATUS_ONLINE                 0x0001
#define DNM_CAN_BUS_STATUS_BUS_WARNING	          0x0002
#define DNM_CAN_BUS_STATUS_BUS_OFF                0x0004
#define DNM_CAN_BUS_STATUS_ACTIVITY               0x0008
#define DNM_CAN_BUS_STATUS_TX_ACK_ERROR           0x0010
#define DNM_CAN_BUS_STATUS_TX_OVERRUN_ERROR       0x0020
#define DNM_CAN_BUS_STATUS_RX_OVERRUN_ERROR       0x0040
#define DNM_CAN_BUS_STATUS_MESSAGE_LOST           0x0080
#define DNM_CAN_BUS_STATUS_GENERAL_ERROR          0x0100
#define DNM_CAN_BUS_STATUS_BUS_POWER              0x0200
#define DNM_CAN_BUS_STATUS_RESERVED_1             0x0400
#define DNM_CAN_BUS_STATUS_RESERVED_2             0x0800
#define DNM_CAN_BUS_STATUS_ONLINE_125             0x1000
#define DNM_CAN_BUS_STATUS_ONLINE_250             0x2000
#define DNM_CAN_BUS_STATUS_ONLINE_500             0x4000
#define DNM_CAN_BUS_STATUS_SLAVE_ACTIVE           0x8000


//--------------------------
// Connection Flags
//--------------------------
#define DNM_CONNECTION_FLAG_EXPLICIT              0x0001
#define DNM_CONNECTION_FLAG_POLL                  0x0002
#define DNM_CONNECTION_FLAG_STROBE                0x0004
#define DNM_CONNECTION_FLAG_COS                   0x0010
#define DNM_CONNECTION_FLAG_CYCLIC                0x0020
#define DNM_CONNECTION_FLAG_ACK_SUPRESS           0x0040

#define DNM_CONNECTION_FLAG_QUICK_C_VALUE         0x1000
#define DNM_CONNECTION_FLAG_QUICK_C_SUPPORT       0x2000


//--------------------------
// Slave State (see w_SlaveState in DNM_DEVICE_CFG)
//--------------------------
#define DNM_SLAVE_STATE_ENABLED                   0x0000
#define DNM_SLAVE_STATE_DISABLED                  0x0001
#define DNM_SLAVE_STATE_MONITORED                 0x0002


//--------------------------
// Event ID's
//--------------------------
#define DNM_EVENT_STATUS                               0
#define DNM_EVENT_IO_1                                 1
#define DNM_EVENT_IO_2                                 2
#define DNM_EVENT_EXPLICIT_REQ                         3
#define DNM_EVENT_EXPLICIT_RES                         4
#define DNM_EVENT_G2_STATUS                            5
#define DNM_EVENT_MONITORED_IO_1                       6
#define DNM_EVENT_MONITORED_IO_2                       7

//--------------------------
// DNM_EVENT_STATUS
//  - returned as wParam in registered callback ThreadMessage
//  - returned by DNM_GetDeviceStatus (as StatusCode) and DNM_GetDeviceEvent
//--------------------------
#define DNM_EVENT_STATUS_IDLE                       0x00
#define DNM_EVENT_STATUS_ACTIVE                     0x01
#define DNM_EVENT_STATUS_ACTIVE_TX_IDLE             0x02
#define DNM_EVENT_STATUS_IDLE_DUPMAC_ERROR          0x03


//--------------------------
// DNM_EVENT_IO_1 and DNM_EVENT_IO_2
//  - returned as wParam in registered callback ThreadMessage
//  - returned by DNM_GetDeviceEvent
//--------------------------
#define DNM_EVENT_IO_OUTPUT_DATA_UPDATE_BIT         0x01
#define DNM_EVENT_IO_OUTPUT_RX_IDLE_BIT             0x02
#define DNM_EVENT_IO_OUTPUT_DATA_CHANGE_BIT         0x04


//--------------------------
// DNM_EVENT_EXPLICIT_REQ
//  - returned as wParam in registered callback ThreadMessage
//  - returned by DNM_GetDeviceEvent
//--------------------------
#define DNM_EVENT_IO_EXPLICIT_REQ_RX_BIT            0x01


//--------------------------
// DNM_EVENT_EXPLICIT_RES
//  - returned as wParam in registered callback ThreadMessage
//  - returned by DNM_GetDeviceEvent
//--------------------------
#define DNM_EVENT_IO_EXPLICIT_RES_SENT_BIT          0x01


//--------------------------
// DNM_EVENT_G2_STATUS
//  - returned as wParam in registered callback ThreadMessage
//  - returned by DNM_GetDeviceG2Status and DNM_GetDeviceEvent
//--------------------------
#define DNM_EVENT_G2_STATUS_EXPLICIT_BIT            0x01
#define DNM_EVENT_G2_STATUS_POLL_BIT                0x02
#define DNM_EVENT_G2_STATUS_STROBE_BIT              0x04
#define DNM_EVENT_G2_STATUS_COS_BIT                 0x10
#define DNM_EVENT_G2_STATUS_CYCLIC_BIT              0x20
#define DNM_EVENT_G2_STATUS_ACK_SUPRESS_BIT         0x40


//--------------------------
// DNM_EVENT_MONITORED_IO_1 and DNM_EVENT_MONITORED_IO_2
//  - returned as wParam in registered callback ThreadMessage
//  - returned by DNM_GetDeviceEvent
//--------------------------
#define DNM_EVENT_IO_INPUT_DATA_UPDATE_BIT          0x01
#define DNM_EVENT_IO_INPUT_RX_IDLE_BIT              0x02
#define DNM_EVENT_IO_INPUT_DATA_CHANGE_BIT          0x04


//--------------------------
// I/O data areas
//--------------------------
#define DNM_IO_ACCESS_INPUT_1                          0
#define DNM_IO_ACCESS_OUTPUT_1                         1
#define DNM_IO_ACCESS_INPUT_2                          2
#define DNM_IO_ACCESS_OUTPUT_2                         3

//--------------------------
// I/O path identifiers
//--------------------------
#define DNM_PATH_ID_INPUT_1                            0
#define DNM_PATH_ID_OUTPUT_1                           1
#define DNM_PATH_ID_INPUT_2                            2
#define DNM_PATH_ID_OUTPUT_2                           3



//--------------------------
// Allowed Shared Memory Area
//  - for I/O data
//  - connection paths
//  - explicit buffers 
//--------------------------
#define DNM_MEMORY_AREA_OFFSET_START              0x1000
#define DNM_MEMORY_AREA_OFFSET_END                0x3FFF



//--------------------------
// DNM_OpenCard dw_Flags bit flags
//--------------------------
#define DNM_OPENFLAG_APPIRQ                   0x00000001
#define DNM_OPENFLAG_OVERLAP                  0x00000002


//--------------------------
// DNM_SetIdentity dw_Flags bit flags
//--------------------------
#define DNM_SETIDENTITY_IGNORE_DEVICE_ID      0x00000001


//--------------------------
// DNM_AddDevice dw_AdditionalFlags bit flags
//-------------------------
#define DNM_ADDDEVICE_IO_1_DATA_CHANGED       0x00000001
#define DNM_ADDDEVICE_IO_2_DATA_CHANGED       0x00000002


//--------------------------
// DNM_DEVICE_CFG w_InputxAutoGenerate values
//-------------------------
#define DNM_INPUT_AUTO_GENERATE_OFF               0x0000
#define DNM_INPUT_AUTO_GENERATE_LOOPBACK          0x0001
#define DNM_INPUT_AUTO_GENERATE_INCREMENT         0x0002


//--------------------------
// StatusFlags byte
//  - returned by DNM_GetDeviceStatus
//--------------------------
#define DNM_GDS_STATUS_EXPLICIT_LOCK_BIT            0x01
#define DNM_GDS_STATUS_INPUT_1_LOCK_BIT             0x02
#define DNM_GDS_STATUS_INPUT_2_LOCK_BIT             0x04
#define DNM_GDS_STATUS_OUTPUT_1_LOCK_BIT            0x08
#define DNM_GDS_STATUS_OUTPUT_2_LOCK_BIT            0x10
#define DNM_GDS_STATUS_OUTPUT_1_RX_IDLE_BIT         0x20
#define DNM_GDS_STATUS_OUTPUT_2_RX_IDLE_BIT         0x40


#endif  // __DNMSLAVE_DEFINITIONS_H__
