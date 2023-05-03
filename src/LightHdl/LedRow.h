/*******************************************************************************
Project   RoomLight

  This is an OpenSource Project.
  You can use, share or improve this project. If you improve this source code
  please share with the comunity or at least with the author of the original 
  source code
  
  Created 24. August 2020 by MarkusNTrains
================================================================================
$HeadURL:  $
$Id:  $
*******************************************************************************/


#ifndef _LED_ROW_H
#define _LED_ROW_H


//----------------------------------------------------------------------------
// include
#include "../common.h"
#include <Adafruit_NeoPixel.h>


//----------------------------------------------------------------------------
// define


//----------------------------------------------------------------------------
// class
class LedRow
{
    public:
        const static uint16_t LED_ROW_NOF = ROOM_LIGHT_NofRows;
        const static uint16_t LED_ROW_LENGTH = ROOM_LIGHT_RowNofPx;
    
	    LedRow(Adafruit_NeoPixel* led_strip_p, uint8_t row_idx);
	    ~LedRow();
        
        void Show();
        uint32_t GetPixelColor(uint16_t idx);
        void SetPixel(uint16_t idx, uint32_t color);
        void SetPixel(uint16_t start_idx, uint16_t width, uint16_t space, uint16_t nof_repeat, uint32_t color);
		
    private:	
        Adafruit_NeoPixel* m_led_strip_p;
        uint8_t m_row_idx;

        uint16_t GetLedIdxOfLut(uint16_t idx);
};

#endif  // _LED_ROW_H
