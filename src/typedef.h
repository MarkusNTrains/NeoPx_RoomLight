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


#ifndef _TYPEDEF_H
#define _TYPEDEF_H


//-----------------------------------------------------------------------------
// includes
#include "Arduino.h"


//----------------------------------------------------------------------------
// struct
typedef union 
{
    struct 
    {
        uint8_t blue;
        uint8_t green;
        uint8_t red;
        uint8_t res;
    };
    uint32_t color;
} rgb_color_t;

typedef union
{
    struct 
    {
        uint8_t blue;
        uint8_t green;
        uint8_t red;
        uint8_t white;
    };
    uint32_t color;
} wrgb_color_t;

typedef union
{
    struct
    {
        uint8_t warm;
        uint8_t cold;
    };
    uint16_t white;
} white_t;



#endif // _TYPEDEF_H