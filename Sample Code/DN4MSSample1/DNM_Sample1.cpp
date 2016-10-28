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
#include "dnm_nist.h"

//Ethernet IP includes
#include "applicomIO.h"

//Boost Library include for shared memory
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>



/* Local Definitions   */
#define  DNM_SAMPLE1_INPUT_SIZE     4
#define  DNM_SAMPLE1_OUTPUT_SIZE    4
#define  DNM_SAMPLE1_DEVICE_MACID	10



/* Global Data Variables */
DNM_MODULE_HEADER          g_dnm_Header;
DNM_ONLINE_CFG             g_dnm_Online;
DNM_DEVICE_CFG             g_dnm_Device;                 
DNM_STATUS                 g_dnm_Status;
DNM_IDENTITY_DATA		   g_dnm_Identity;

CARD_HANDLE                g_CardHandle;
BYTE                       g_by_Input[ DNM_SAMPLE1_INPUT_SIZE];
BYTE                       g_by_Output[DNM_SAMPLE1_OUTPUT_SIZE];
BYTE                       g_by_G2Status;
BYTE					   g_device_macid[DNM_DEVICE_NUM_MAC_ID];

float					   g_xmeas_buffer[TE_SIM_NUM_XMEAS];
unsigned long			   g_tmp_buf[TE_SIM_NUM_XMEAS];
float					   g_xmv_buffer[TE_SIM_NUM_XMV];
unsigned long			   g_tmp_output_buf[TE_SIM_NUM_XMV];


/* Local Function Headers */
void DisplayFailure(CHAR * p_ch_FunctionName);
void ExitFunction(void);

double			double1 = 123456789.987654321;
float			float1 = 123456789.987654321;


int main(int argc, char* argv[])
{
   BOOL   b_Result;
   BOOL   b_DeviceFound;
   BOOL   b_DeviceOpen;
   BOOL   b_DeviceOnline;
   BOOL   b_MenuQuit;
   BOOL   b_ShowUserExit;
   
   WORD   w_Version;
   DWORD  dw_Size, dw_Index, i;
   CHAR   ch_Buffer[100];
   char   ch_Menu;
   char   tmp_buf[50];

   printf("\nStart DNM_Sample1: v1.1.1.0\n\n");
   
   memset( &g_by_Input,  0, DNM_SAMPLE1_INPUT_SIZE  );
   memset( &g_by_Output, 0, DNM_SAMPLE1_OUTPUT_SIZE );



   //create a shared memory region -- For testing -- will mapped to TE Sim sheared mem later
   //using namespace boost::interprocess;
   //create_shm_tesim_xmeas();
   //map_shm_tesim_xmeas();

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
            /*do{
               Sleep(100);
            }while(!kbhit());

            ch_Menu = getch();*/

			ch_Menu = 'y';

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
	   //Add device device node, skipping macid 0, reserved for master
	   //Create XMEAS node and XMV node
	   //node 0:		Master
	   //node 1-41:		XMEAS 1 - 41
	   //node 42-53:	XMV 1- 12
	   for (i = 1; i < TE_SIM_NUM_XMEAS + TE_SIM_NUM_XMV + 1; i++)
	   {
		  
		   //Set the identify of each slave
		   memset(&g_dnm_Identity, 0, sizeof(DNM_IDENTITY_DATA));
		   g_dnm_Identity.w_MacId		= i;
		   b_Result = DNM_GetIdentity(g_CardHandle, &g_dnm_Identity);
		   if (!b_Result)
		   {  // Function Failure
			   DisplayFailure("DNM_GetIdentity Failure");
			   printf("DNM_GetIdentity Failure: MacId:%d\n", g_dnm_Device.w_MacId);
		   }
		   		   
		   sprintf(tmp_buf, "xmeas_%d", i);
		   memcpy(g_dnm_Identity.by_ProductName, tmp_buf, 33);
		   b_Result = DNM_SetIdentity(g_CardHandle, &g_dnm_Identity, 0);
		   if (!b_Result)
		   {  // Function Failure
			   DisplayFailure("DNM_SetIdentity Failure");
			   printf("DNM_SetIdentity Failure: MacId:%d\n", g_dnm_Device.w_MacId);
		   }



		   //Add slave device
		   memset(&g_dnm_Device, 0, sizeof(DNM_DEVICE_CFG));
		   g_dnm_Device.w_MacId				= i;
		   g_dnm_Device.w_SlaveState		= DNM_SLAVE_STATE_ENABLED;
		   g_dnm_Device.w_ConnectionFlags	= DNM_CONNECTION_FLAG_POLL;
		   g_dnm_Device.w_Input1Size		= DNM_SAMPLE1_INPUT_SIZE;
		   g_dnm_Device.w_Input1Offset		= DNM_MEMORY_AREA_OFFSET_START + (i*0x30);
		   g_dnm_Device.w_Output1Size		= DNM_SAMPLE1_OUTPUT_SIZE;
		   g_dnm_Device.w_Output1Offset		= g_dnm_Device.w_Input1Offset + DNM_SAMPLE1_INPUT_SIZE;

		   b_Result = DNM_AddDevice(g_CardHandle, &g_dnm_Device, 0);
		   if (!b_Result)
		   {  // Function Failure
			   DisplayFailure("DNM_AddDevice Failure");
			   printf("DNM_AddDevice Failure: MacId:%d\n", g_dnm_Device.w_MacId);
		   }
		   printf("DNM_AddDevice offset: MacId:%d, input offset:%x, output offset:%x\n", g_dnm_Device.w_MacId, g_dnm_Device.w_Input1Offset, g_dnm_Device.w_Output1Offset);


		   
	   }


     
      /* Go ONLINE */
      //g_dnm_Online.w_BaudRate = DNM_BAUD_RATE_500K;
	  g_dnm_Online.w_BaudRate = DNM_BAUD_RATE_125K;
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
    
      b_MenuQuit = FALSE;
      do
      {
         // Poll one of the selections below
         /*printf("Menu:\n");
         printf("1)  Get Output Data\n");
         printf("2)  Increment Input Data\n");
         printf("3)  Get Device Status\n");
         printf("q)  Quit\n");*/
		 printf("\n Pausing...\n");
         //printf("\n\n");

		 ch_Menu = '2';

         /* Wait for user response */
         do{
            Sleep(900);
         }while(!kbhit());

         //ch_Menu = getch();

		 ch_Menu = '2';

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

				
				//Map to shared memory, and fill g_xmeas_buffer with shared memory data
				map_shm_tesim_xmeas();

				for (i = 0; i < TE_SIM_NUM_XMEAS; i++) //MACID starts at 1, but buffer starts at 0
				{

					memcpy(&g_tmp_buf[i], &g_xmeas_buffer[i], 4);
					b_Result = DNM_WriteDeviceIo(g_CardHandle, (DNM_DEVICE_MACID_0 + i + 1), DNM_IO_ACCESS_INPUT_1, &g_xmeas_buffer[i], DNM_SAMPLE1_INPUT_SIZE);
					if (!b_Result)
					{  // Function Failure
						//DisplayFailure("DNM_WriteDeviceIo Failure 1");
						printf("\n  DNM_WriteDeviceIo Failure MscId:%d, data:%llx ", (DNM_DEVICE_MACID_0 + i + 1), g_xmeas_buffer[i]);
						b_MenuQuit = TRUE;
					}
					else
					{
						printf("\n  Input Data MscId:%d, data:%x", (DNM_DEVICE_MACID_0 + i + 1), g_tmp_buf[i]);
					}
				}

				for (i = 0; i < TE_SIM_NUM_XMV; i++) //MACID starts at 42, after the XMEAS node, but buffer starts at 0
				{
					//Receive data from DN node and write to shared memory
					//b_Result = DNM_ReadDeviceIo(g_CardHandle, (DNM_DEVICE_MACID_0 + TE_SIM_NUM_XMEAS + i + 1), DNM_IO_ACCESS_OUTPUT_1, &g_tmp_output_buf[i], DNM_SAMPLE1_OUTPUT_SIZE);
					b_Result = DNM_ReadDeviceIo(g_CardHandle, (DNM_DEVICE_MACID_0 + TE_SIM_NUM_XMEAS + i + 1), DNM_IO_ACCESS_OUTPUT_1, &g_xmv_buffer[i], DNM_SAMPLE1_OUTPUT_SIZE);
					//memcpy(&g_xmv_buffer[i], &g_tmp_output_buf[i], 4);
					if (!b_Result)
					{  // Function Failure
						//DisplayFailure("DNM_ReadDeviceIo Failure 1");
						//printf("\n  DNM_readDeviceIo Failure MscId:%d, data:%x ", (DNM_DEVICE_MACID_0 + TE_SIM_NUM_XMEAS + i + 1), g_tmp_output_buf[i]);
						printf("\n  DNM_readDeviceIo Failure MscId:%d, data:%f ", (DNM_DEVICE_MACID_0 + TE_SIM_NUM_XMEAS + i + 1), g_xmv_buffer[i]);
						b_MenuQuit = TRUE;
					}
					else
					{						
						printf("\n Output Data MacId:%d, 0x%x, float:%f", (DNM_DEVICE_MACID_0 + TE_SIM_NUM_XMEAS + i + 1), g_tmp_output_buf[i], g_xmv_buffer[i]);
					}
				}
				map_shm_tesim_xmv();

					
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


void create_shm_tesim_xmeas(void)
{
	DWORD					i;
	unsigned long long		*shmem_addr;
	//create a shared memory object
	using namespace boost::interprocess;
	
	//First to remove any old shared memory segment
	struct shm_remove
	{
		shm_remove() { shared_memory_object::remove("TESim_shm_xmeas"); }
		~shm_remove(){ shared_memory_object::remove("TESim_shm_xmeas"); }
	} remover;

	shared_memory_object tesim_shm(create_only, "TESim_shm_xmeas", read_write);

	//Set shared memory size
	tesim_shm.truncate(1000);

	//Map the whole shared memory in this process
	mapped_region region(tesim_shm, read_write);

	//Initialized shared memory to 0x37
	std::memset(region.get_address(), 0x37, region.get_size());

	shmem_addr = static_cast<unsigned long long*>(region.get_address());

	for (i = 0; i < 50; i++, shmem_addr++)
	{
		//*shmem_addr = 0xaabbccddeeff7700 + i;
		*shmem_addr = (i * 0x0010203004050607 + 1);
		printf("\n  Initialized shared memory:%d, data:%llx ", i, *shmem_addr);
	}
	

}

void map_shm_tesim_xmeas(void)
{
	double		*shmem_addr, *shmem_start_addr;
	DWORD		ii;
	double		buf1[TE_SIM_NUM_XMEAS];
		
	using namespace boost::interprocess;
	
			
	//open shared memory object
	shared_memory_object tesim_sim_shm(open_only, TE_SIM_XMEAS_SHMEM_NAME, read_write);
		
	//Map the whole shared memory in this process
	mapped_region region(tesim_sim_shm, read_write);
	
	//Copy the shared memory into the local xmeas array
	//Get shared memory pointer
	shmem_addr = static_cast<double*>(region.get_address());
	//Make a copy of the start pointer
	shmem_start_addr = shmem_addr;
	
	//Copy the entire shared memory into local array
	for (ii = 0; ii < TE_SIM_NUM_XMEAS; ii++, shmem_addr++)
	{		
		buf1[ii] = *shmem_addr;
		g_xmeas_buffer[ii] = (float)buf1[ii];
		
		printf("\nShared Mem to array index:%d, data-float:%f, double:%f ", ii,  g_xmeas_buffer[ii], buf1[ii]);
		
	}
}


void map_shm_tesim_xmv(void)
{
	double		*shmem_addr, *shmem_start_addr;
	DWORD		ii;
	double		buf1[TE_SIM_NUM_XMV];

	using namespace boost::interprocess;


	//open shared memory object
	shared_memory_object tesim_xmv_shm(open_only, TE_SIM_XMV_SHMEM_NAME, read_write);

	//Map the whole shared memory in this process
	mapped_region region(tesim_xmv_shm, read_write);

	//Copy the shared memory into the local xmeas array
	//Get shared memory pointer
	shmem_addr = static_cast<double*>(region.get_address());
	//Make a copy of the start pointer
	shmem_start_addr = shmem_addr;

	//Copy the entire shared memory into local array
	for (ii = 0; ii < TE_SIM_NUM_XMV; ii++, shmem_addr++)
	{		
		buf1[ii] = (float)g_xmv_buffer[ii];
		*shmem_addr = buf1[ii];
		
		printf("\nXMV Shared Mem i:%d, data-float:%f, double:%f__%f ", ii, g_xmv_buffer[ii], buf1[ii], *shmem_addr);

	}
}
