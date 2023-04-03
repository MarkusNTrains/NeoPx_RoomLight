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
#define SW_VERSION_Major 1
#define SW_VERSION_Minor 1

#define IS_DEBUG_MODE
#undef IS_DEBUG_MODE

// enhance the following setting to use it for another room light
// and enhance the following files:
//  - LED_MATRIX_LUT in file LedRow.cpp
//  - LedMatrix Constructor in file LedMatrix.cpp
//  - ip, myDns, gatway, subnet in file WebServer.cpp
#define ROOM_LIGHT_MARKUSNTRAINS 0
#define ROOM_LIGHT_ALTENGLIENICKE 1
#define ROOM_LIGHT ROOM_LIGHT_MARKUSNTRAINS

#if (ROOM_LIGHT == ROOM_LIGHT_MARKUSNTRAINS)
    #define ROOM_LIGHT_NofRows      4
    #define ROOM_LIGHT_RowNofPx     285
    #define ROOM_LIGHT_NofLedStrips 4
#else // ROOM_LIGHT == ROOM_LIGHT_ALTENGLIENICKE
    #define ROOM_LIGHT_NofRows      6
    #define ROOM_LIGHT_RowNofPx     240
    #define ROOM_LIGHT_NofLedStrips 6
#endif


#endif //_COMMON_H
