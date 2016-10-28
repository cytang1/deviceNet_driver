/*
------------------------------------------------------

 FILENAME : DNM_Sample1.cpp

 VERSION  : 1.1.1.0

 (C) 2010 Molex Incorporated

------------------------------------------------------

 Purpose: Sample code to poll data from DeviceNet MultiSlave.

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
#include "dnm_funct.h"


/* Local Definitions   */
#define  DNM_SAMPLE1_INPUT_SIZE     5
#define  DNM_SAMPLE1_OUTPUT_SIZE    4

#define  DNM_SAMPLE1_DEVICE_MACID   1


/* Global Data Variables */
DNM_MODULE_HEADER          g_dnm_Header;
DNM_ONLINE_CFG             g_dnm_Online;
DNM_DEVICE_CFG             g_dnm_Device;                 
DNM_STATUS                 g_dnm_Status;

CARD_HANDLE                g_CardHandle;
BYTE                       g_by_Input[ DNM_SAMPLE1_INPUT_SIZE];
BYTE                       g_by_Output[DNM_SAMPLE1_OUTPUT_SIZE];
BYTE                       g_by_G2Status;


/* Local Function Headers */
void DisplayFailure(CHAR * p_ch_FunctionName);
void ExitFunction(void);



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
   CHAR   ch_Buffer[100];
   char   ch_Menu;

   printf("\nStart DNM_Sample1: v1.1.1.0\n\n");
   
   memset( &g_by_Input,  0, DNM_SAMPLE1_INPUT_SIZE  );
   memset( &g_by_Output, 0, DNM_SAMPLE1_OUTPUT_SIZE );

   /* Inform user of exit by default */ 
   b_ShowUserExit = TRUE;

   /* Load DeviceNet Communications Driver DLL with DNM_LoadDriver */
   b_Result = DNM_LoadDriver( "SSDN32.DLL" );
   if (!b_Result)
   {  /* Function Failure */
      DisplayFailure("DNM_LoadDriver");
      ExitFunction();
      return 0;
   }


   /* Get MultiSlave DLL version info with DNM_Version */
   b_Result = DNM_Version(ch_Buffer, &w_Version, 100);
   if (!b_Result)
   {  /* Function Failure */
      DisplayFailure("DNM_Version");
      DNM_FreeDriver();
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
      b_Result = DNM_OpenCard(&g_CardHandle, ch_Buffer, NULL, 0);
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

   /* Initialize MultiSlave */
   b_DeviceOnline = FALSE;
   if (b_DeviceOpen)
   {
      /* Add Slave MacID 1 */
      memset( &g_dnm_Device, 0, sizeof(DNM_DEVICE_CFG) );
      g_dnm_Device.w_MacId           = DNM_SAMPLE1_DEVICE_MACID;
      g_dnm_Device.w_SlaveState      = DNM_SLAVE_STATE_ENABLED;
      g_dnm_Device.w_ConnectionFlags = DNM_CONNECTION_FLAG_POLL;
      g_dnm_Device.w_Input1Size      = DNM_SAMPLE1_INPUT_SIZE;
      g_dnm_Device.w_Input1Offset    = DNM_MEMORY_AREA_OFFSET_START;
      g_dnm_Device.w_Output1Size     = DNM_SAMPLE1_OUTPUT_SIZE;
      g_dnm_Device.w_Output1Offset   = DNM_MEMORY_AREA_OFFSET_START + DNM_SAMPLE1_INPUT_SIZE;

      b_Result = DNM_AddDevice(g_CardHandle, &g_dnm_Device, 0);
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


   /* Poll Data on the found device */
   if (b_DeviceOnline)
   {
      b_MenuQuit = FALSE;
      do
      {
         /* Poll one of the selections below */
         printf("Menu:\n");
         printf("1)  Get Output Data\n");
         printf("2)  Increment Input Data\n");
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
               b_Result = DNM_ReadDeviceIo(g_CardHandle, DNM_SAMPLE1_DEVICE_MACID, DNM_IO_ACCESS_OUTPUT_1, &g_by_Output, DNM_SAMPLE1_OUTPUT_SIZE);
               if (!b_Result)
               {  /* Function Failure */
                  DisplayFailure("DNM_ReadDeviceIo");
                  b_MenuQuit = TRUE;
               }
               else
               {
                  printf("\n             Output Data : ");
                  for (dw_Index = 0; dw_Index < DNM_SAMPLE1_OUTPUT_SIZE; dw_Index++)
                  {
                     printf(" 0x%.2x,", g_by_Output[dw_Index] );
                  }
                  printf("\n\n\n");
               }
               break;


            case '2':
               /* Increment Input Data with DNM_WriteDeviceIo */
               for (dw_Index = 0; dw_Index < DNM_SAMPLE1_INPUT_SIZE; dw_Index++)
               {
                  g_by_Input[dw_Index] = (BYTE)(g_by_Input[dw_Index] + dw_Index + 1);
               }

               b_Result = DNM_WriteDeviceIo(g_CardHandle, DNM_SAMPLE1_DEVICE_MACID, DNM_IO_ACCESS_INPUT_1, &g_by_Input, DNM_SAMPLE1_INPUT_SIZE);
               if (!b_Result)
               {  /* Function Failure */
                  DisplayFailure("DNM_WriteDeviceIo");
                  b_MenuQuit = TRUE;
               }

               b_Result = DNM_ReadDeviceIo(g_CardHandle, DNM_SAMPLE1_DEVICE_MACID, DNM_IO_ACCESS_INPUT_1, &g_by_Input, DNM_SAMPLE1_INPUT_SIZE);
               if (!b_Result)
               {  /* Function Failure */
                  DisplayFailure("DNM_ReadDeviceIo");
                  b_MenuQuit = TRUE;
               }
               else
               {
                  printf("\n              Input Data : ");
                  for (dw_Index = 0; dw_Index < DNM_SAMPLE1_INPUT_SIZE; dw_Index++)
                  {
                     printf(" 0x%.2x,", g_by_Input[dw_Index] );
                  }
                  printf("\n\n\n");
               }
               break;


            case '3':
               /* Get Device Status with DNM_GetDeviceStatus and DNM_GetDeviceG2Status */
               b_Result = DNM_GetDeviceStatus(g_CardHandle, DNM_SAMPLE1_DEVICE_MACID, &g_dnm_Status);
               if (!b_Result)
               {  /* Function Failure */
                  DisplayFailure("DNM_GetDeviceStatus");
                  b_MenuQuit = TRUE;
               }
               else
               {
                  b_Result = DNM_GetDeviceG2Status(g_CardHandle, DNM_SAMPLE1_DEVICE_MACID, &g_by_G2Status);
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

   if (b_ShowUserExit)
   {
      ExitFunction();
   }
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


