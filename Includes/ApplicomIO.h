/*
+------------------------------------------------------------------------------+
| applicom ========                                                            |
| ====international                                                            |
|                                                                              |
|  Copyright (c) 2000 Applicom International. All rights reserved.             |
+------------------------------------------------------------------------------+
  Description .......:  Include Declarations file for C language applications
                             
+------------------------------------------------------------------------------+
  File Name..........: APPLICOMIO.H
  Version............: 2.1
  Date...............: 12/10/2000
+-- [WARNING DO NOT MODIFY THIS FILE]------------------------------------------+
*/

#ifndef __APPLICOMIO_HEADER_FILE__
  #define __APPLICOMIO_HEADER_FILE__

/* Definition of type use in function prototype (like WIN32 SDK)*/

#ifndef BOOL
  #define BOOL int
#endif

#ifndef BYTE
  #define BYTE unsigned char
#endif

#ifndef WORD 
  #define WORD unsigned short
#endif

#ifndef DWORD 
  #define DWORD unsigned long
#endif

/* Translate a card number to a chan number */

#define CARD2CHAN(x) ((WORD)((x-1)*4))

/* Function prototype */ 

BOOL WINAPI AuInitBus_io   (DWORD* pdwStatus);
BOOL WINAPI AuExitBus_io   (DWORD* pdwStatus);


BOOL WINAPI AuWriteReadMsg_io (WORD    wChan,
                               WORD    wNes,
                               DWORD   dwMsgParam,
                               WORD    wNbTx,
                               BYTE*   lpbyBufTx,
                               WORD*   pwNbRx,
                               BYTE*   lpbyBufRx,
                               DWORD*  pdwStatus);
 
#endif