/*******************************************************************************
Project   RoomLight

  This is an OpenSource Project.
  You can use, share or improve this project. If you improve this source code
  please share with the comunity or at least with the author of the original 
  source code
  
  Created 25. April 2023 by MarkusNTrains
================================================================================
$HeadURL:  $
$Id:  $
*******************************************************************************/

#ifndef _LED_STRIP_H
#define _LED_STRIP_H


//----------------------------------------------------------------------------
// include
#include "../common.h"
#include "../Datastore/Datastore.h"



//----------------------------------------------------------------------------
// define
#define SIMPLELEDSTRIP_Red_Pin
#define SIMPLELEDSTRIP_Green_Pin
#define SIMPLELEDSTRIP_Blue_Pin
#define SIMPLELEDSTRIP_WarmWhite_Pin
#define SIMPLELEDSTRIP_ColdWhite_Pin


//----------------------------------------------------------------------------
// extern


//----------------------------------------------------------------------------
// class
class SimpleLedStrip
{
    public:
        enum Scene
        {
            Off = 0,
            Red,
            Gree,
            Blue,
            WarmWhite,
            ColdWhite,
            Nof,
            Unknown
        };

        SimpleLedStrip(Datastore* datastore_p, uint32_t task_tmo_m, 
            Parameter::Id lightscene_param_id, Parameter::Id brightness_param_id, Parameter::Id color_param_id, Parameter::Id white_param_id,
            uint8_t red_pin, uint8_t green_pin, uint8_t blue_pin, uint8_t warmwhite_pin, uint8_t coldwhite_pin);
        ~SimpleLedStrip();

        void Task();
        void SetScene(Scene scene);
        void SetBrightness(uint8_t brightness);
        void SetRGBColor(rgb_color_t color);
        void SetWhite(white_t white);

    protected:

    private:
        Datastore* m_datastore_p = nullptr;
        Parameter::Id m_scene_param_id;
        Parameter::Id m_brightness_param_id;
        Parameter::Id m_color_param_id;
        Parameter::Id m_white_param_id;

        uint8_t m_red_pin;
        uint8_t m_green_pin;
        uint8_t m_blue_pin;
        uint8_t m_warmwhite_pin;
        uint8_t m_coldwhite_pin;

        uint32_t m_task_tmo_ms;
        uint32_t m_task_timestamp_ms;

        void Update();
};

#endif // _LED_STRIP_H
