/*******************************************************************************
Project   RoomLight

  This is an OpenSource Project.
  You can use, share or improve this project. If you improve this source code
  please share with the comunity or at least with the author of the original 
  source code
  
  Created 8. September 2022 by MarkusNTrains
================================================================================
$HeadURL:  $
$Id:  $
*******************************************************************************/

#ifndef _LIGHT_SCENE_SUN_H
#define _LIGHT_SCENE_SUN_H


//----------------------------------------------------------------------------
// include
#include "common.h"
#include "LightSceneHdl.h"
#include "LightHdl.h"



//----------------------------------------------------------------------------
// enum
enum LightningState
{
    LIGHTSCENESUN_STATE_Sunrise = 0,
    LIGHTSCENESUN_STATE_Sunset,
    LIGHTSCENESUN_STATE_Fading,
    LIGHTSCENESUN_STATE_FadingToNight,
    LIGHTSCENESUN_STATE_Unknown,
};


//----------------------------------------------------------------------------
// extern
class LightSceneHdl;


//----------------------------------------------------------------------------
// class
class LightScene_Sun
{
    public:
        const static uint8_t NIGHT_BRIGHTNESS = 4;

        LightScene_Sun(LightSceneHdl* parent, LightHdl* light_hdl);
        ~LightScene_Sun();
        void Day_Enter(void);
        void Day_Task(void);
        void Night_Enter(void);
        void Night_Task(void);
        void Sunrise_Enter(void);
        void Sunrise_Exit(void);
        void Sunrise_Task(void);
        void Sunset_Enter(void);
        void Sunset_Exit(void);
        void Sunset_Task(void);
        void CalculateAndShow_Sunlight(void);


    private:
        const static uint32_t TASK_Sun_TmoMs = 80; 
        const static uint32_t TASK_DayNight_TmoMs = 1000;

        const static uint32_t PIXEL_DISTANCE_MM = 16;  // distance between neo pixels in mm
        const static uint32_t SUN_MAX_HEIGHT = 10000;

        const static uint8_t DAY_BRIGHTNESS_WHITE = 150;
        const static uint8_t DAY_BRIGHTNESS_RGB = (DAY_BRIGHTNESS_WHITE * 5) / 3;
        const static uint32_t DAY_COLOR = LightHdl::COLOR_WHITE;
        const static uint8_t SUNRISE_BIRGHTNESS = 15;
        const static uint32_t NIGHT_COLOR = LightHdl::COLOR_BLUE;
        const static uint8_t RED_MAX = (255 * DAY_BRIGHTNESS_RGB) / 255;
        const static uint8_t GREEN_MAX = (120 * DAY_BRIGHTNESS_RGB) / 255;
        const static uint8_t BLUE_MAX = (32 * DAY_BRIGHTNESS_RGB ) / 255;

        LightSceneHdl* m_scene_hdl_p;
        LightHdl* m_light_hdl_p;

        LightningState m_state;
        uint32_t m_sun_height;
        uint32_t m_sun_pos;
        uint32_t m_day_color;
        uint8_t m_twilight_brightness;
        uint32_t m_task_timestamp_ms;
};

#endif // _LIGHT_SCENE_SUN_H
