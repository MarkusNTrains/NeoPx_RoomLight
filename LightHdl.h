/*******************************************************************************
Project   RoomLight

  This is an OpenSource Project.
  You can use, share or improve this project. If you improve this source code
  please share with the comunity or at least with the author of the original 
  source code
  
  Created 9. September 2022 by MarkusNTrains
================================================================================
$HeadURL:  $
$Id:  $
*******************************************************************************/

#ifndef _LIGHT_HDL_H
#define _LIGHT_HDL_H


//----------------------------------------------------------------------------
// include
#include "common.h"
#include "LedMatrix.h"
#include "LedArea.h"


//----------------------------------------------------------------------------
// define


//----------------------------------------------------------------------------
// enum


//----------------------------------------------------------------------------
// class
class LightHdl
{
    public:
        LightHdl();
        ~LightHdl();
        LedArea* GetLedArea(void);
        void SetLedArea(uint16_t xs, uint16_t xe, uint16_t ys, uint16_t ye);
        void SetLedArea(uint16_t xs, uint16_t xe, uint16_t ys, uint16_t ye, uint32_t color);
        void UpdateLedArea(void);
        uint8_t GetBrightness(void);
        void SetBrightness_Fade(uint8_t brightness);
        void SetBrightness_Instantly(uint8_t brightness);
        void UpdateBrightness(void);
        uint32_t GetColor(void);
        void SetColor(uint32_t color);
        void Clear(void);
        void Show(void);

    protected:
        LedMatrix* m_led_matrix;

    private:
        const uint32_t COLOR_BLUE = 0x000000FF;
        const uint32_t COLOR_GREEN = 0x0000FF00;
        const uint32_t COLOR_RED = 0x00FF0000;
        const uint32_t COLOR_WHITE = 0xFF000000;

        LedArea* m_led_area;
        uint8_t m_current_brightness;
        uint8_t m_desired_brightness;
        uint32_t m_color;
        uint32_t m_update_time_ms;

        static uint8_t UpdateValueTo(uint8_t current_value, uint8_t desired_value, uint8_t factor);
};

#endif // _LIGHT_HDL_H
