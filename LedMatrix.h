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


#ifndef _LED_MATRIX_H
#define _LED_MATRIX_H


//----------------------------------------------------------------------------
// include
#include "common.h"
#include "LedRow.h"



//----------------------------------------------------------------------------
// class
class LedMatrix
{
    public:
        const static uint8_t LED_STRIP_NOF = ROOM_LIGHT_NofLedStrips;

        LedMatrix();
        ~LedMatrix();
        uint32_t GetPixelColor(uint16_t row, uint16_t column);
        void SetPixel(uint16_t row, uint16_t column, uint32_t color);
        void SetPixelArray(uint16_t xs, uint16_t xe, uint16_t ys, uint16_t ye, uint32_t color);
        void Clear(void);
        void Show(void);
        void SetBrightness(uint8_t brightness);
        void SetColor(uint32_t color);
    
    private:  
        Adafruit_NeoPixel* m_led_strip[LED_STRIP_NOF];  
        LedRow* m_led_row[LedRow::LED_ROW_NOF];
};


#endif  // _LED_MATRIX_H
