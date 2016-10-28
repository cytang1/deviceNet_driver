/*
------------------------------------------------------

 FILENAME : DNM_Sample2.cpp

 VERSION  : 1.1.1.0

 (C) 2010 Molex Incorporated

------------------------------------------------------

 Purpose: Sample code to receive notification callbacks 
           from DeviceNet MultiSlave.

------------------------------------------------------
 Revision History
------------------------------------------------------

 Revision     Date         By    Description
------------------------------------------------------
 1.0.0.0   July 23,2009    DK    Initial Release
 1.0.1.0    Aug 18,2009    DK    Minor update to DLL API
 1.0.2.0   Sept 02,2009    DK    Fix flag used in DNM_AddDevice
 1.1.0.0    Oct 20,2009    DK    Modify to reflect changes made in dll (v1.2.0.0).
 1.1.1.0    Jan 04,2010    DK    Modify to reflect changes made in dll (v1.2.1.0).

*/



/* OS Includes */
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>


/* MultiSlave Includes */
#include "dnm_funct_var.h"


/* Local Definitions   */
#define  DNM_SAMPLE2_INPUT_SIZE         2
#define  DNM_SAMPLE2_OUTPUT_SIZE        3

#define  DNM_SAMPLE2_DEVICE_MACID       7

#define  DNM_SAMPLE2_EVENT_STATUS       WM_USER+1
#define  DNM_SAMPLE2_EVENT_IO_1         WM_USER+2
#define  DNM_SAMPLE2_EVENT_IO_2         WM_USER+3
#define  DNM_SAMPLE2_EVENT_EXPLICIT_REQ WM_USER+4
#define  DNM_SAMPLE2_EVENT_EXPLICIT_RES WM_USER+5
#define  DNM_SAMPLE2_EVENT_G2_STATUS    WM_USER+6
#define  DNM_SAMPLE2_EVENT_END_THREAD   WM_USER+7


/* Global Data Variables */
DNM_MODULE_HEADER          g_dnm_Header;
DNM_ONLINE_CFG             g_dnm_Online;
DNM_DEVICE_CFG             g_dnm_Device;                 
DNM_STATUS                 g_dnm_Status;

DNM_EXPLICIT_REQUEST_CFG   g_dnm_ExpRequest;                 
DNM_EXPLICIT_RESPONSE_CFG  g_dnm_ExpResponse;

CARD_HANDLE                g_CardHandle;
BYTE                       g_by_Input[ DNM_SAMPLE2_INPUT_SIZE];
BYTE                       g_by_Output[DNM_SAMPLE2_OUTPUT_SIZE];
BYTE                       g_by_G2Status;

/* Global Library DLL Variables    */
HINSTANCE                  g_h_LibInst;

/* Global Thread Variables         */
HANDLE                     g_h_ThreadInst;
DWORD                      g_dw_ThreadId;


/* Local Function Headers */
void  DisplayFailure(CHAR * p_ch_FunctionName);
BOOL  GetProcAddresses(void);
void  ExitFunction(void);
DWORD HandleNotification(VOID *pData);



int main(int argc, char* argv[])
{
   BOOL   b_Result;
   BOOL   b_DeviceFound;
   BOOL   b_DeviceOpen;
   BOOL   b_DeviceOnline;
   BOOL   b_MenuQuit;
   BOOL   b_ShowUserExit;
   
   WORD   w_Version;
   DWORD  dw_Size, dw_Index;
   DWORD  dw_AdditionalFlags;
   CHAR   ch_Buffer[100];
   char   ch_Menu;

   printf("\nStart DNM_Sample2: v1.1.1.0\n\n");
   
   memset( &g_by_Input,  0, DNM_SAMPLE2_INPUT_SIZE  );
   memset( &g_by_Output, 0, DNM_SAMPLE2_OUTPUT_SIZE );

   /* Inform user of exit by default */ 
   b_ShowUserExit = TRUE;

   /* Load DN MultiSlave DLL as a Library */
   g_h_LibInst = NULL;
   g_h_LibInst = LoadLibrary("dnmslave32.dll");
   if (!g_h_LibInst)
   {  /* Function Failure */
      printf("  FAILURE: LoadLibrary: dnmslave32.dll Failed\n\n");
      ExitFunction();
      return 0;
   }


   /* Now Get Function (or procedure) addresses */
   if(!GetProcAddresses())
   {  /* Function Failure */
      printf("  FAILURE: GetProcAddresses Failed\n\n");
      FreeLibrary(g_h_LibInst);
      ExitFunction();
      return 0;
   }

   /* Load DeviceNet Communications Driver DLL with DNM_LoadDriver */
   b_Result = DNM_LoadDriver( "SSDN32.DLL" );
   if (!b_Result)
   {  /* Function Failure */
      DisplayFailure("DNM_LoadDriver");
      FreeLibrary(g_h_LibInst);
      ExitFunction();
      return 0;
   }


   /* Get MultiSlave DLL version info with DNM_Version */
   b_Result = DNM_Version(ch_Buffer, &w_Version, 100);
   if (!b_Result)
   {  /* Function Failure */
      DisplayFailure("DNM_Version");
      DNM_FreeDriver();
      FreeLibrary(g_h_LibInst);
      ExitFunction();
      return 0;
   }
   else
   {
      printf("        DLL Version : 0x%.4x\n'%s'\n\n", w_Version, ch_Buffer);
   }


   /* Look for DeviceNet Devices with DNM_EnumDrivers */
   dw_Index      = 0;
   b_DeviceFound = FALSE;
   b_MenuQuit    = FALSE;
   do
   {
      dw_Size  = 100;      
      b_Result = DNM_EnumDrivers(dw_Index, ch_Buffer, &dw_Size);
      if (!b_Result)
      {  /* Function Failure */
         DisplayFailure("DNM_EnumDrivers");
         b_MenuQuit = TRUE;
      }
      else
      {
         /* DNM_EnumDrivers returned valid response */
         if (dw_Size == 0)
         {
            /* ...but no more DN Devices adapters were found */
            printf(" No More DN Devices found, EXIT NOW\n");
            b_MenuQuit = TRUE;
         }
         else
         {
            /* ...and it found another device */ 

            /* Ask user if they want to use it */
            printf("Found DeviceNet Device '%s'  Use it?  Y/N \n", ch_Buffer);

            /* Wait for user response */
            do{
               Sleep(100);
            }while(!kbhit());

            ch_Menu = getch();

            /* Check user response */
            switch(ch_Menu)
            {
               case 'Y':
               case 'y':
                  /* Use this DN Device */
                  b_MenuQuit    = TRUE;
                  b_DeviceFound = TRUE;
                  printf("DN Device Selected.  Now attempt to open the device...\n\n");
                  break;

               default:
                  /* Get the next DN Device */
                  dw_Index++;
                  break;
            }
         }
      }
   }while(!b_MenuQuit);


   /* Open the DN Device and print off the FW header */
   b_DeviceOpen = FALSE;
   if (b_DeviceFound)
   {
      /* Open the device with DNM_OpenCard     */
      b_Result = DNM_OpenCard(&g_CardHandle, ch_Buffer, NULL, DNM_OPENFLAG_APPIRQ);
      if (!b_Result)
      {  /* Function Failure */
         DisplayFailure("DNM_OpenCard");
      }
      else
      {
         b_DeviceOpen = TRUE;
         /* Read Module Header with DNM_GetModuleHeader*/
         b_Result = DNM_GetModuleHeader(g_CardHandle, &g_dnm_Header);
         if (!b_Result)
         {  /* Function Failure */
            DisplayFailure("DNM_GetModuleHeader");
         }
         else
         {
            /* Read Module Header */
            printf("Information in Device Header:\n");
            printf("        Module Type = 0x%.4x\n", g_dnm_Header.w_ModuleType);
            printf("          Kernel ID = 0x%.4x\n", g_dnm_Header.w_KernelId);
            printf("         Kernel Rev = 0x%.4x\n", g_dnm_Header.w_KernelRev);
            printf("          Module ID = 0x%.4x\n", g_dnm_Header.w_ModuleId);
            printf("         Module Rev = 0x%.4x\n", g_dnm_Header.w_ModuleRev);
            printf("          Card Type = '%.16s'\n", g_dnm_Header.by_CardType);
            printf("        Card Serial = '%.8s'\n", g_dnm_Header.by_CardSerial);
            printf("         CAN Status = 0x%.4x\n", g_dnm_Header.w_CanStatus);
            printf("             CAN TX = 0x%.4x\n", g_dnm_Header.w_CanTx);
            printf("             CAN RX = 0x%.4x\n", g_dnm_Header.w_CanRx);
            printf("      Module String = '%s'\n",   g_dnm_Header.by_ModuleString);
            printf("\n\n");
         }
      }
   }
   

   /* Initialize a single slave */
   b_DeviceOnline = FALSE;
   if (b_DeviceOpen)
   {
      /* Add Slave MacID 1 */
      memset( &g_dnm_Device, 0, sizeof(DNM_DEVICE_CFG) );
      g_dnm_Device.w_MacId           = DNM_SAMPLE2_DEVICE_MACID;
      g_dnm_Device.w_SlaveState      = DNM_SLAVE_STATE_DISABLED;
      g_dnm_Device.w_ConnectionFlags = DNM_CONNECTION_FLAG_EXPLICIT | DNM_CONNECTION_FLAG_POLL;

      g_dnm_Device.w_ExplicitRequestSize    = 100;
      g_dnm_Device.w_ExplicitRequestOffset  = DNM_MEMORY_AREA_OFFSET_START + 0x1000;
      g_dnm_Device.w_ExplicitResponseSize   = 100;
      g_dnm_Device.w_ExplicitResponseOffset = DNM_MEMORY_AREA_OFFSET_START + 0x1800;

      g_dnm_Device.w_Input1Size      = DNM_SAMPLE2_INPUT_SIZE;
      g_dnm_Device.w_Input1Offset    = DNM_MEMORY_AREA_OFFSET_START;
      g_dnm_Device.w_Output1Size     = DNM_SAMPLE2_OUTPUT_SIZE;
      g_dnm_Device.w_Output1Offset   = DNM_MEMORY_AREA_OFFSET_START + DNM_SAMPLE2_INPUT_SIZE;

      dw_AdditionalFlags             = DNM_ADDDEVICE_IO_1_DATA_CHANGED;

      b_Result = DNM_AddDevice(g_CardHandle, &g_dnm_Device, dw_AdditionalFlags);
      if (!b_Result)
      {  /* Function Failure */
         DisplayFailure("DNM_AddDevice");
      }

      /* Go ONLINE */
      g_dnm_Online.w_BaudRate = DNM_BAUD_RATE_500K;
      g_dnm_Online.w_Reserved = 0;

      b_Result = DNM_Online(g_CardHandle, &g_dnm_Online);
      if (!b_Result)
      {  /* Function Failure */
         DisplayFailure("DNM_Online");
      }
      else
      {
         b_DeviceOnline = TRUE;
      }
   }

   if (b_DeviceOnline)
   {
      /* Create Thread responsible for notification Callback */
      g_h_ThreadInst = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)HandleNotification, 0, 0, &g_dw_ThreadId );
      if (!g_h_ThreadInst)
      {
         printf("  FAILURE: CreateThread\n");
      }
      else
      {
         /* Register for EVENTS */
         b_Result = DNM_RegisterDeviceEvent(g_CardHandle, DNM_SAMPLE2_DEVICE_MACID, DNM_EVENT_STATUS, g_dw_ThreadId, DNM_SAMPLE2_EVENT_STATUS, 0 );
         if (!b_Result)
         {  /* Function Failure */
            DisplayFailure("DNM_RegisterDeviceEvent");
         }
         b_Result = DNM_RegisterDeviceEvent(g_CardHandle, DNM_SAMPLE2_DEVICE_MACID, DNM_EVENT_IO_1, g_dw_ThreadId, DNM_SAMPLE2_EVENT_IO_1, 0 );
         if (!b_Result)
         {  /* Function Failure */
            DisplayFailure("DNM_RegisterDeviceEvent");
         }
         b_Result = DNM_RegisterDeviceEvent(g_CardHandle, DNM_SAMPLE2_DEVICE_MACID, DNM_EVENT_EXPLICIT_REQ, g_dw_ThreadId, DNM_SAMPLE2_EVENT_EXPLICIT_REQ, 0 );
         if (!b_Result)
         {  /* Function Failure */
            DisplayFailure("DNM_RegisterDeviceEvent");
         }
         b_Result = DNM_RegisterDeviceEvent(g_CardHandle, DNM_SAMPLE2_DEVICE_MACID, DNM_EVENT_EXPLICIT_RES, g_dw_ThreadId, DNM_SAMPLE2_EVENT_EXPLICIT_RES, 0 );
         if (!b_Result)
         {  /* Function Failure */
            DisplayFailure("DNM_RegisterDeviceEvent");
         }
         b_Result = DNM_RegisterDeviceEvent(g_CardHandle, DNM_SAMPLE2_DEVICE_MACID, DNM_EVENT_G2_STATUS, g_dw_ThreadId, DNM_SAMPLE2_EVENT_G2_STATUS, 0 );
         if (!b_Result)
         {  /* Function Failure */
            DisplayFailure("DNM_RegisterDeviceEvent");
         }

         /* Let the thread run */
         Sleep(10);
      }

      /* Allow the Slave to run */
      b_Result = DNM_EnableDevice(g_CardHandle, DNM_SAMPLE2_DEVICE_MACID);
      if (!b_Result)
      {  /* Function Failure */
         DisplayFailure("DNM_EnableDevice");
      }
   }

   /* Poll Data on the found device */
   if (b_DeviceOnline)
   {
      b_MenuQuit = FALSE;
      do
      {
         /* Poll one of the selections below */
         printf("Menu:\n");
         printf("1)  Get Output Data\n");
         printf("2)  Get  Input Data\n");
         printf("3)  Get Device Status\n");
         printf("q)  Quit\n");
         printf("\n\n");

         /* Wait for user response */
         do{
            Sleep(100);
         }while(!kbhit());

         ch_Menu = getch();

         /* Check user response */
         switch(ch_Menu)
         {
            case '1':
               /* Get Output Data with DNM_ReadDeviceIo */
               b_Result = DNM_ReadDeviceIo(g_CardHandle, DNM_SAMPLE2_DEVICE_MACID, DNM_IO_ACCESS_OUTPUT_1, &g_by_Output, DNM_SAMPLE2_OUTPUT_SIZE);
               if (!b_Result)
               {  /* Function Failure */
                  DisplayFailure("DNM_ReadDeviceIo");
                  b_MenuQuit = TRUE;
               }
               else
               {
                  printf("\n             Output Data : ");
                  for (dw_Index = 0; dw_Index < DNM_SAMPLE2_OUTPUT_SIZE; dw_Index++)
                  {
                     printf(" 0x%.2x,", g_by_Output[dw_Index] );
                  }
                  printf("\n\n\n");
               }
               break;


            case '2':
               /* Get Input Data with DNM_ReadDeviceIo */
               b_Result = DNM_ReadDeviceIo(g_CardHandle, DNM_SAMPLE2_DEVICE_MACID, DNM_IO_ACCESS_INPUT_1, &g_by_Input, DNM_SAMPLE2_INPUT_SIZE);
               if (!b_Result)
               {  /* Function Failure */
                  DisplayFailure("DNM_ReadDeviceIo");
                  b_MenuQuit = TRUE;
               }
               else
               {
                  printf("\n              Input Data : ");
                  for (dw_Index = 0; dw_Index < DNM_SAMPLE2_INPUT_SIZE; dw_Index++)
                  {
                     printf(" 0x%.2x,", g_by_Input[dw_Index] );
                  }
                  printf("\n\n\n");
               }
               break;


            case '3':
               /* Get Device Status with DNM_GetDeviceStatus and DNM_GetDeviceG2Status */
               b_Result = DNM_GetDeviceStatus(g_CardHandle, DNM_SAMPLE2_DEVICE_MACID, &g_dnm_Status);
               if (!b_Result)
               {  /* Function Failure */
                  DisplayFailure("DNM_GetDeviceStatus");
                  b_MenuQuit = TRUE;
               }
               else
               {
                  b_Result = DNM_GetDeviceG2Status(g_CardHandle, DNM_SAMPLE2_DEVICE_MACID, &g_by_G2Status);
                  if (!b_Result)
                  {  /* Function Failure */
                     DisplayFailure("DNM_GetDeviceG2Status");
                     b_MenuQuit = TRUE;
                  }
                  else
                  {
                     printf("    Device Status:\n");
                     printf("     Status_Code     = 0x%.2x\n", g_dnm_Status.by_StatusCode);
                     printf("       G2_Status     = 0x%.2x\n", g_by_G2Status);
                     printf("\n\n");
                  }
               }
               break;

            case 'Q':
            case 'q':
               b_ShowUserExit = FALSE;
               b_MenuQuit     = TRUE;
               break;
         }

      }while(!b_MenuQuit);
   }
  
   /* Unregister all events and close event threads */
   if (b_DeviceOnline)
   {
      b_Result = DNM_UnRegisterDeviceEvent(g_CardHandle, DNM_SAMPLE2_DEVICE_MACID, DNM_EVENT_STATUS);
      if (!b_Result)
      {  /* Function Failure */
         DisplayFailure("DNM_UnRegisterDeviceEvent");
      }
      b_Result = DNM_UnRegisterDeviceEvent(g_CardHandle, DNM_SAMPLE2_DEVICE_MACID, DNM_EVENT_IO_1);
      if (!b_Result)
      {  /* Function Failure */
         DisplayFailure("DNM_UnRegisterDeviceEvent");
      }
      b_Result = DNM_UnRegisterDeviceEvent(g_CardHandle, DNM_SAMPLE2_DEVICE_MACID, DNM_EVENT_EXPLICIT_REQ);
      if (!b_Result)
      {  /* Function Failure */
         DisplayFailure("DNM_UnRegisterDeviceEvent");
      }
      b_Result = DNM_UnRegisterDeviceEvent(g_CardHandle, DNM_SAMPLE2_DEVICE_MACID, DNM_EVENT_EXPLICIT_RES);
      if (!b_Result)
      {  /* Function Failure */
         DisplayFailure("DNM_UnRegisterDeviceEvent");
      }
      b_Result = DNM_UnRegisterDeviceEvent(g_CardHandle, DNM_SAMPLE2_DEVICE_MACID, DNM_EVENT_G2_STATUS);
      if (!b_Result)
      {  /* Function Failure */
         DisplayFailure("DNM_UnRegisterDeviceEvent");
      }

      /* Done with notification, destroy the thread */
      PostThreadMessage( g_dw_ThreadId,DNM_SAMPLE2_EVENT_END_THREAD,0,0);

      /* Let the thread run */
      Sleep(10);
   }

   /* Go OFFLINE */
   if (b_DeviceOnline)
   {
      b_Result = DNM_Offline(g_CardHandle);
      if (!b_Result)
      {  /* Function Failure */
         DisplayFailure("DNM_Offline");
      }
      
      b_DeviceOnline = FALSE;
   }

   /* Close the found DN Device using DNM_CloseCard */
   if (b_DeviceOpen)
   {
      b_Result = DNM_CloseCard(g_CardHandle);
      if (!b_Result)
      {  /* Function Failure */
         DisplayFailure("DNM_CloseCard");
      }
      
      b_DeviceOpen = FALSE;
   }

   /* Unload DeviceNet Communications Driver DLL with DNM_FreeDriver */
   b_Result = DNM_FreeDriver();
   if (!b_Result)
   {  /* Function Failure */
      DisplayFailure("DNM_FreeDriver");
   }

   /* Free DN MultiSlave DLL */
   FreeLibrary(g_h_LibInst);

   if (b_ShowUserExit)
   {
      ExitFunction();
   }
   return 0;
}



/* Event Thread Function */
DWORD HandleNotification(VOID *pData)
{
   MSG   ThreadMsg;
   BOOL  b_EndTheThread;
   BOOL  b_Ret, b_Result;
   BYTE  by_DeviceEvent;
   DWORD dw_Index;
   BYTE  by_Output[DNM_SAMPLE2_OUTPUT_SIZE + DNM_SAMPLE2_INPUT_SIZE];

   b_EndTheThread = FALSE;
   printf("THREAD: HandleNotification Thread Starts\n");

   while(b_EndTheThread == FALSE)
   {
      b_Ret = GetMessage(&ThreadMsg,NULL,0,0);

      if( (b_Ret != 0) && (b_Ret != -1) )
      {
         switch(ThreadMsg.message)
         {
            case DNM_SAMPLE2_EVENT_STATUS:

               /* by_DeviceEvent should read the same as ThreadMsg.wParam, 
                   unless the status has already changed */
               b_Result = DNM_GetDeviceEvent( g_CardHandle, DNM_SAMPLE2_DEVICE_MACID, DNM_EVENT_STATUS, &by_DeviceEvent );
               if (!b_Result)
               {  /* Function Failure */
                  DisplayFailure("DNM_GetDeviceEvent");
                  b_EndTheThread = TRUE;
               }
               else
               {
                  printf("    THREAD: DNM_SAMPLE2_EVENT_STATUS\n");
                  printf("              wParam :  0x%.8x\n", ThreadMsg.wParam);
                  printf("         DeviceEvent :  0x%.2x\n\n", by_DeviceEvent);
               }
               break;

            case DNM_SAMPLE2_EVENT_IO_1:

               /* by_DeviceEvent should read the same as ThreadMsg.wParam, 
                   unless another thread already read the data, and cleared the event */
               b_Result = DNM_GetDeviceEvent( g_CardHandle, DNM_SAMPLE2_DEVICE_MACID, DNM_EVENT_IO_1, &by_DeviceEvent );
               if (!b_Result)
               {  /* Function Failure */
                  DisplayFailure("DNM_GetDeviceEvent");
                  b_EndTheThread = TRUE;
               }
               else
               {
                  b_Result = DNM_ReadDeviceIo(g_CardHandle, DNM_SAMPLE2_DEVICE_MACID, DNM_IO_ACCESS_OUTPUT_1, &by_Output, DNM_SAMPLE2_OUTPUT_SIZE);
                  if (!b_Result)
                  {  /* Function Failure */
                     DisplayFailure("DNM_ReadDeviceIo");
                     b_EndTheThread = TRUE;
                  }
                  else
                  {
                     /* Set Input Data to just read output data */
                     b_Result = DNM_WriteDeviceIo(g_CardHandle, DNM_SAMPLE2_DEVICE_MACID, DNM_IO_ACCESS_INPUT_1, &by_Output, DNM_SAMPLE2_INPUT_SIZE);
                     if (!b_Result)
                     {  /* Function Failure */
                        DisplayFailure("DNM_WriteDeviceIo");
                        b_EndTheThread = TRUE;
                     }

                     printf("    THREAD: DNM_SAMPLE2_EVENT_IO_1\n");
                     printf("              wParam :  0x%.8x\n", ThreadMsg.wParam);
                     printf("         DeviceEvent :  0x%.2x\n", by_DeviceEvent);
                     printf("     New Output Data : ");
                     for (dw_Index = 0; dw_Index < DNM_SAMPLE2_OUTPUT_SIZE; dw_Index++)
                     {
                        printf(" 0x%.2x,", by_Output[dw_Index] );
                     }
                     printf("\n\n");
                  }
               }
               break;

            case DNM_SAMPLE2_EVENT_EXPLICIT_REQ:

               /* by_DeviceEvent should read the same as ThreadMsg.wParam, 
                   unless the explicit request has already been responded to */
               b_Result = DNM_GetDeviceEvent( g_CardHandle, DNM_SAMPLE2_DEVICE_MACID, DNM_EVENT_EXPLICIT_REQ, &by_DeviceEvent );
               if (!b_Result)
               {  /* Function Failure */
                  DisplayFailure("DNM_GetDeviceEvent");
                  b_EndTheThread = TRUE;
               }
               else
               {
                  /* Get the request */
                  b_Result = DNM_ReceiveDeviceExplicit(g_CardHandle, DNM_SAMPLE2_DEVICE_MACID, &g_dnm_ExpRequest);
                  if (!b_Result)
                  {  /* Function Failure */
                     DisplayFailure("DNM_ReceiveDeviceExplicit");
                     b_EndTheThread = TRUE;
                  }
                  else
                  {
                     /* Return Generic Error response */
                     g_dnm_ExpResponse.w_ConnectionId = g_dnm_ExpRequest.w_ConnectionId;
                     g_dnm_ExpResponse.w_Service      = 0x94;  /* Error */
                     g_dnm_ExpResponse.w_Size         = 0x02;
                     g_dnm_ExpResponse.by_Data[0]     = 0x16;  /* Object does not exist */
                     g_dnm_ExpResponse.by_Data[1]     = 0xFF;

                     /* Send the response */
                     b_Result = DNM_SendDeviceExplicit(g_CardHandle, DNM_SAMPLE2_DEVICE_MACID, &g_dnm_ExpResponse);
                     if (!b_Result)
                     {  /* Function Failure */
                        DisplayFailure("DNM_SendDeviceExplicit");
                        b_EndTheThread = TRUE;
                     }

                     printf("    THREAD: DNM_SAMPLE2_EVENT_EXPLICIT_REQ\n");
                     printf("              wParam :  0x%.8x\n", ThreadMsg.wParam);
                     printf("         DeviceEvent :  0x%.2x\n", by_DeviceEvent);
                     printf("    THREAD: EXPLICIT REQUEST\n");
                     printf("             Service :  0x%.4x\n", g_dnm_ExpRequest.w_Service);
                     printf("               Class :  0x%.4x\n", g_dnm_ExpRequest.w_Class);
                     printf("            Instance :  0x%.4x\n", g_dnm_ExpRequest.w_Instance);
                     printf("                Size :  0x%.4x\n", g_dnm_ExpRequest.w_Size);
                     printf("    THREAD: EXPLICIT RESPONSE\n");
                     printf("             Service :  0x%.4x\n", g_dnm_ExpResponse.w_Service);
                     printf("                Size :  0x%.4x\n", g_dnm_ExpResponse.w_Size);
                     printf("               Data0 :  0x%.2x\n", g_dnm_ExpResponse.by_Data[0]);
                     printf("               Data1 :  0x%.2x\n\n", g_dnm_ExpResponse.by_Data[1]);  
                  }
               }
               break;

            case DNM_SAMPLE2_EVENT_EXPLICIT_RES:

               /* by_DeviceEvent should read the same as ThreadMsg.wParam, 
                   unless the status has already changed */
               b_Result = DNM_GetDeviceEvent( g_CardHandle, DNM_SAMPLE2_DEVICE_MACID, DNM_EVENT_EXPLICIT_RES, &by_DeviceEvent );
               if (!b_Result)
               {  /* Function Failure */
                  DisplayFailure("DNM_GetDeviceEvent");
                  b_EndTheThread = TRUE;
               }
               else
               {
                  printf("    THREAD: DNM_SAMPLE2_EVENT_EXPLICIT_RES\n");
                  printf("              wParam :  0x%.8x\n", ThreadMsg.wParam);
                  printf("         DeviceEvent :  0x%.2x\n\n", by_DeviceEvent);
               }
               break;

            case DNM_SAMPLE2_EVENT_G2_STATUS:

               /* by_DeviceEvent should read the same as ThreadMsg.wParam, 
                   unless the status has already changed */
               b_Result = DNM_GetDeviceEvent( g_CardHandle, DNM_SAMPLE2_DEVICE_MACID, DNM_EVENT_G2_STATUS, &by_DeviceEvent );
               if (!b_Result)
               {  /* Function Failure */
                  DisplayFailure("DNM_GetDeviceEvent");
                  b_EndTheThread = TRUE;
               }
               else
               {
                  printf("    THREAD: DNM_SAMPLE2_EVENT_G2_STATUS\n");
                  printf("              wParam :  0x%.8x\n", ThreadMsg.wParam);
                  printf("         DeviceEvent :  0x%.2x\n\n", by_DeviceEvent);
               }
               break;

            case DNM_SAMPLE2_EVENT_END_THREAD:
               printf("THREAD: DNM_SAMPLE2_EVENT_END_THREAD\n");
               b_EndTheThread = TRUE;
               break;

            default:
               printf("THREAD: Unknown Message : 0x%.8x\n", ThreadMsg.message);
               break;
         }
      }
      else
      {
         /* error or WM_QUIT, so exit */
         printf("THREAD: WM_QUIT or error\n");
         b_EndTheThread = TRUE;
      }
   }

   printf("THREAD: HandleNotification Thread Ends\n");
   return 0;
}




/*  Display Previous Error Code  */
void DisplayFailure(CHAR * p_ch_FunctionName)
{
   DWORD  dw_LastError;

   if (p_ch_FunctionName == NULL)
   {
      printf("  FAILURE: DisplayFailure: NULL pointer\n\n");
   }
   else
   {
      dw_LastError = GetLastError();

      /* Function Failure, could not get Error String */
      printf("  FAILURE: %s: 0x%.8x\n\n", p_ch_FunctionName, dw_LastError);
   }
}


/*  Inform user the program is about to exit.  Wait for user input to continue */
void ExitFunction(void)
{
   char   ch_Menu;

   /* Inform user the program is about to exit */
   printf("\n Program is about to exit.  Press any button.\n");

   /* Wait for user response */
   do{
      Sleep(100);
   }while(!kbhit());

   ch_Menu = getch();
}


/* Get Function (or procedure) addresses from DN MultiSlave DLL */
BOOL GetProcAddresses(void)
{
   if ((DNM_LoadDriver = (dnm_LoadDriver_T)GetProcAddress(g_h_LibInst, "DNM_LoadDriver")) == NULL)
      return FALSE;
   if ((DNM_Driver     = (dnm_Driver_T)GetProcAddress(g_h_LibInst, "DNM_Driver")) == NULL)
      return FALSE;
   if ((DNM_FreeDriver = (dnm_FreeDriver_T)GetProcAddress(g_h_LibInst, "DNM_FreeDriver")) == NULL)
      return FALSE;

   if ((DNM_EnumDrivers     = (dnm_EnumDrivers_T)GetProcAddress(g_h_LibInst, "DNM_EnumDrivers")) == NULL)
      return FALSE;
   if ((DNM_GetModuleHeader = (dnm_GetModuleHeader_T)GetProcAddress(g_h_LibInst, "DNM_GetModuleHeader")) == NULL)
      return FALSE;
   if ((DNM_Version         = (dnm_Version_T)GetProcAddress(g_h_LibInst, "DNM_Version")) == NULL)
      return FALSE;
               
   if ((DNM_OpenCard   = (dnm_OpenCard_T)GetProcAddress(g_h_LibInst, "DNM_OpenCard")) == NULL)
      return FALSE;
   if ((DNM_CloseCard  = (dnm_CloseCard_T)GetProcAddress(g_h_LibInst, "DNM_CloseCard")) == NULL)
      return FALSE;
   if ((DNM_Online     = (dnm_Online_T)GetProcAddress(g_h_LibInst, "DNM_Online")) == NULL)
      return FALSE;
   if ((DNM_Offline    = (dnm_Offline_T)GetProcAddress(g_h_LibInst, "DNM_Offline")) == NULL)
      return FALSE;

   if ((DNM_AddDevice       = (dnm_AddDevice_T)GetProcAddress(g_h_LibInst, "DNM_AddDevice")) == NULL)
      return FALSE;
   if ((DNM_DeleteDevice    = (dnm_DeleteDevice_T)GetProcAddress(g_h_LibInst, "DNM_DeleteDevice")) == NULL)
      return FALSE;
   if ((DNM_EnableDevice    = (dnm_EnableDevice_T)GetProcAddress(g_h_LibInst, "DNM_EnableDevice")) == NULL)
      return FALSE;
   if ((DNM_DisableDevice   = (dnm_DisableDevice_T)GetProcAddress(g_h_LibInst, "DNM_DisableDevice")) == NULL)
      return FALSE;

   if ((DNM_GetBusStatus    = (dnm_GetBusStatus_T)GetProcAddress(g_h_LibInst, "DNM_GetBusStatus")) == NULL)
      return FALSE;
   if ((DNM_GetCANCounters  = (dnm_GetCANCounters_T)GetProcAddress(g_h_LibInst, "DNM_GetCANCounters")) == NULL)
      return FALSE;
   if ((DNM_GetCardStatus   = (dnm_GetCardStatus_T)GetProcAddress(g_h_LibInst, "DNM_GetCardStatus")) == NULL)
      return FALSE;

   if ((DNM_GetDevice          = (dnm_GetDevice_T)GetProcAddress(g_h_LibInst, "DNM_GetDevice")) == NULL)
      return FALSE;
   if ((DNM_GetDeviceEvent     = (dnm_GetDeviceEvent_T)GetProcAddress(g_h_LibInst, "DNM_GetDeviceEvent")) == NULL)
      return FALSE;
   if ((DNM_GetDeviceStatus    = (dnm_GetDeviceStatus_T)GetProcAddress(g_h_LibInst, "DNM_GetDeviceStatus")) == NULL)
      return FALSE;
   if ((DNM_GetDeviceG2Status  = (dnm_GetDeviceG2Status_T)GetProcAddress(g_h_LibInst, "DNM_GetDeviceG2Status")) == NULL)
      return FALSE;
   if ((DNM_GetDeviceMonitored = (dnm_GetDeviceMonitored_T)GetProcAddress(g_h_LibInst, "DNM_GetDeviceMonitored")) == NULL)
      return FALSE;
   if ((DNM_ClearDeviceEvent   = (dnm_ClearDeviceEvent_T)GetProcAddress(g_h_LibInst, "DNM_ClearDeviceEvent")) == NULL)
      return FALSE;

   if ((DNM_GetDevicePath         = (dnm_GetDevicePath_T)GetProcAddress(g_h_LibInst, "DNM_GetDevicePath")) == NULL)
      return FALSE;
   if ((DNM_InitializePathBuffer  = (dnm_InitializePathBuffer_T)GetProcAddress(g_h_LibInst, "DNM_InitializePathBuffer")) == NULL)
      return FALSE;

   if ((DNM_IoActive   = (dnm_IoActive_T)GetProcAddress(g_h_LibInst, "DNM_IoActive")) == NULL)
      return FALSE;
   if ((DNM_IoIdle     = (dnm_IoIdle_T)GetProcAddress(g_h_LibInst, "DNM_IoIdle")) == NULL)
      return FALSE;

   if ((DNM_ReadDeviceIo          = (dnm_ReadDeviceIo_T)GetProcAddress(g_h_LibInst, "DNM_ReadDeviceIo")) == NULL)
      return FALSE;
   if ((DNM_ReadDeviceIoMonitored = (dnm_ReadDeviceIoMonitored_T)GetProcAddress(g_h_LibInst, "DNM_ReadDeviceIoMonitored")) == NULL)
      return FALSE;
   if ((DNM_WriteDeviceIo         = (dnm_WriteDeviceIo_T)GetProcAddress(g_h_LibInst, "DNM_WriteDeviceIo")) == NULL)
      return FALSE;

   if ((DNM_ReceiveDeviceExplicit = (dnm_ReceiveDeviceExplicit_T)GetProcAddress(g_h_LibInst, "DNM_ReceiveDeviceExplicit")) == NULL)
      return FALSE;
   if ((DNM_SendDeviceExplicit    = (dnm_SendDeviceExplicit_T)GetProcAddress(g_h_LibInst, "DNM_SendDeviceExplicit")) == NULL)
      return FALSE;
   if ((DNM_LocalDeviceExplicit   = (dnm_LocalDeviceExplicit_T)GetProcAddress(g_h_LibInst, "DNM_LocalDeviceExplicit")) == NULL)
      return FALSE;

   if ((DNM_RegisterBusStatusEvent   = (dnm_RegisterBusStatusEvent_T)GetProcAddress(g_h_LibInst, "DNM_RegisterBusStatusEvent")) == NULL)
      return FALSE;
   if ((DNM_RegisterDeviceEvent      = (dnm_RegisterDeviceEvent_T)GetProcAddress(g_h_LibInst, "DNM_RegisterDeviceEvent")) == NULL)
      return FALSE;
   if ((DNM_UnRegisterBusStatusEvent = (dnm_UnRegisterBusStatusEvent_T)GetProcAddress(g_h_LibInst, "DNM_UnRegisterBusStatusEvent")) == NULL)
      return FALSE;
   if ((DNM_UnRegisterDeviceEvent    = (dnm_UnRegisterDeviceEvent_T)GetProcAddress(g_h_LibInst, "DNM_UnRegisterDeviceEvent")) == NULL)
      return FALSE;

   if ((DNM_SetAccessTimeout             = (dnm_SetAccessTimeout_T)GetProcAddress(g_h_LibInst, "DNM_SetAccessTimeout")) == NULL)
      return FALSE;
   if ((DNM_SetEventNotificationInterval = (dnm_SetEventNotificationInterval_T)GetProcAddress(g_h_LibInst, "DNM_SetEventNotificationInterval")) == NULL)
      return FALSE;

   if ((DNM_SetIdentity     = (dnm_SetIdentity_T)GetProcAddress(g_h_LibInst, "DNM_SetIdentity")) == NULL)
      return FALSE;
   if ((DNM_GetIdentity     = (dnm_GetIdentity_T)GetProcAddress(g_h_LibInst, "DNM_GetIdentity")) == NULL)
      return FALSE;

   return TRUE;
}

