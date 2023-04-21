/*******************************************************************************
Project   RoomLight

  This is an OpenSource Project.
  You can use, share or improve this project. If you improve this source code
  please share with the comunity or at least with the author of the original 
  source code
  
  Created 18. May 2021 by MarkusNTrains
================================================================================
$HeadURL:  $
$Id:  $
*******************************************************************************/


#ifndef _COMMON_H
#define _COMMON_H


//-----------------------------------------------------------------------------
// includes
#include "Arduino.h"


//-----------------------------------------------------------------------------
// defines
//--- version -------------------------------------------------------------
#define SW_VERSION_Major 1
#define SW_VERSION_Minor 3


//--- hardware setting ----------------------------------------------------
// enhance the following setting to use it for another room light
// and enhance the following files:
//  - LED_MATRIX_LUT in file LedRow.cpp
//  - LedMatrix Constructor in file LedMatrix.cpp
//  - ip, myDns, gatway, subnet in file WebServer.cpp
#define ROOM_LIGHT_MarkusNTrains    0
#define ROOM_LIGHT_Altenglienicke   1
#define ROOM_LIGHT_TestBoard        2
#define ROOM_LIGHT ROOM_LIGHT_MarkusNTrains

#if (ROOM_LIGHT == ROOM_LIGHT_MarkusNTrains)
    // LED Matrix defines -----------------------------
    #define ROOM_LIGHT_NofRows  4
    #define ROOM_LIGHT_RowNofPx 285
    
    // LED Strips define ------------------------------
    #define ROOM_LIGHT_NofLedStrips 4

    #define ROOM_LIGHT_LedStrip1_Pin 26
    #define ROOM_LIGHT_LedStrip2_Pin 28 
    #define ROOM_LIGHT_LedStrip3_Pin 24
    #define ROOM_LIGHT_LedStrip4_Pin 22

    #define ROOM_LIGHT_LedStrip1_NofLed 300
    #define ROOM_LIGHT_LedStrip2_NofLed 285
    #define ROOM_LIGHT_LedStrip3_NofLed 285
    #define ROOM_LIGHT_LedStrip4_NofLed 285

#elif (ROOM_LIGHT == ROOM_LIGHT_Altenglienicke)
    // LED Matrix defines -----------------------------
    #define ROOM_LIGHT_NofRows  6
    #define ROOM_LIGHT_RowNofPx 240

    // LED Strips define ------------------------------
    #define ROOM_LIGHT_NofLedStrips 6

    #define ROOM_LIGHT_LedStrip1_Pin 22
    #define ROOM_LIGHT_LedStrip2_Pin 24
    #define ROOM_LIGHT_LedStrip3_Pin 26
    #define ROOM_LIGHT_LedStrip4_Pin 28
    #define ROOM_LIGHT_LedStrip4_Pin 30
    #define ROOM_LIGHT_LedStrip4_Pin 32

    #define ROOM_LIGHT_LedStrip1_NofLed 66
    #define ROOM_LIGHT_LedStrip2_NofLed 240
    #define ROOM_LIGHT_LedStrip3_NofLed 240
    #define ROOM_LIGHT_LedStrip4_NofLed 240
    #define ROOM_LIGHT_LedStrip5_NofLed 240
    #define ROOM_LIGHT_LedStrip6_NofLed 66

#else 
    // LED Matrix defines -----------------------------
    #define ROOM_LIGHT_NofRows  1
    #define ROOM_LIGHT_RowNofPx 14

    // LED Strips define ------------------------------
    #define ROOM_LIGHT_NofLedStrips 1

    #define ROOM_LIGHT_LedStrip1_Pin 22

    #define ROOM_LIGHT_LedStrip1_NofLed ROOM_LIGHT_RowNofPx
#endif


//--- debug setting -------------------------------------------------------
#define IS_DEBUG_MODE 1
#undef IS_DEBUG_MODE




#endif //_COMMON_H
