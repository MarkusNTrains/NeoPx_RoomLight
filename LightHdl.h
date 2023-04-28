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
#include "Datastore.h"
#include "LedMatrix.h"
#include "LedArea.h"


//----------------------------------------------------------------------------
// class
class LightHdl
{
    public:
        const static uint32_t COLOR_BLUE = 0x000000FF;
        const static uint32_t COLOR_GREEN = 0x0000FF00;
        const static uint32_t COLOR_RED = 0x00FF0000;
        const static uint32_t COLOR_WHITE = 0xFF000000;

        LightHdl(Datastore* datastore_p);
        ~LightHdl();

        uint32_t GetLedColor(uint16_t row, uint16_t column);
        void SetLed(uint16_t row, uint16_t column, uint32_t color);
        LedArea* GetLedArea(void);
        void LightHdl::SetLedArea(LedArea* area);
        void SetLedArea(uint16_t xs, uint16_t xe, uint16_t ys, uint16_t ye);
        void SetLedArea(uint16_t xs, uint16_t xe, uint16_t ys, uint16_t ye, uint32_t color);
        void UpdateLedArea(void);
        uint8_t GetBrightness(void);
        void SetBrightness_Fade(uint8_t brightness);
        void SetBrightness_Instantly(uint8_t brightness);
        bool UpdateBrightness();
        uint32_t GetColor(void);
        void SetColor(uint32_t color);
        void Clear(void);
        void Show(void);

    protected:
        LedMatrix* m_led_matrix;

    private:
        LedArea* m_led_area;
        uint8_t m_current_brightness;
        uint8_t m_desired_brightness;
        uint32_t m_color;
        uint32_t m_update_time_ms;
        Datastore* m_datastore_p;

        uint8_t UpdateValueTo(uint8_t current_value, uint8_t desired_value, uint8_t factor);
};

#endif // _LIGHT_HDL_H
