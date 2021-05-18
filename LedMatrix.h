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
#include "Arduino.h"
#include <Adafruit_NeoPixel.h>
#include "common.h"
#include "LedRow.h"


//----------------------------------------------------------------------------
// define


//----------------------------------------------------------------------------
// class
class LedMatrix
{
    public:
        LedMatrix();
        ~LedMatrix();
        void SetPixel(uint16_t row, uint16_t col, uint32_t color);
        void SetPixelArray(uint16_t xs, uint16_t xe, uint8_t ys, uint8_t ye, uint32_t color);
        void Clear(void);
        void Show(void);
        void SetBrightness(uint8_t brightness);
    
    private:  
        Adafruit_NeoPixel* m_led_strip;  
        Adafruit_NeoPixel* m_led_strip_2;
        Adafruit_NeoPixel* m_led_strip_3;
        Adafruit_NeoPixel* m_led_strip_4;
        LedRow* m_led_row[LedRow::LED_ROW_NOF];
};


#endif  // _LED_MATRIX_H
