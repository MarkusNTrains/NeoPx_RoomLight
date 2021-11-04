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
#include <Adafruit_NeoPixel.h>


//----------------------------------------------------------------------------
// define
#define LED_STRIP_NOF 4


//----------------------------------------------------------------------------
// class
class LedMatrix
{
    public:
        LedMatrix();
        ~LedMatrix();
        void SetPixel(uint16_t row, uint16_t col, uint32_t color);
        void SetPixelArray(uint16_t xs, uint16_t xe, uint16_t ys, uint16_t ye, uint32_t color);
        void Clear(void);
        void Show(void);
        void SetBrightness(uint8_t brightness);
    
    private:  
        Adafruit_NeoPixel* m_led_strip[LED_STRIP_NOF];  
        LedRow* m_led_row[LedRow::LED_ROW_NOF];
};


#endif  // _LED_MATRIX_H
