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

#ifndef _SIMPLE_LED_STRIP_H
#define _SIMPLE_LED_STRIP_H


//----------------------------------------------------------------------------
// include
#include "../common.h"
#include "../Datastore/Datastore.h"
#include "../LightSource.h"



//----------------------------------------------------------------------------
// extern


//----------------------------------------------------------------------------
// class
class SimpleLedStripHdl : public LightSource
{
    public:
        enum SceneID
        {
            Off = 0,
            Red,
            Green,
            Blue,
            WarmWhite,
            White,
            ColdWhite,
            Nof,
            Unknown
        };

        SimpleLedStripHdl(Datastore* datastore_p, 
            Parameter::Id lightscene_param_id, Parameter::Id brightness_param_id, Parameter::Id color_param_id, Parameter::Id white_param_id,
            uint8_t red_pin, uint8_t green_pin, uint8_t blue_pin, uint8_t warmwhite_pin, uint8_t coldwhite_pin);
        ~SimpleLedStripHdl();

        void Task();
        uint8_t GetActiveScene();
        void ChangeScene(uint8_t scene);
        uint8_t GetBrightness();
        void SetBrightness(uint8_t brightness);
        uint32_t GetColor();
        void SetColor(uint32_t color);
        uint16_t GetWhite();
        void SetWhite(uint16_t white);

    protected:

    private:
        const static uint32_t TASK_TmoMs = 2000;

        SceneID m_scene_id;

        Parameter::Id m_scene_param_id;
        Parameter::Id m_brightness_param_id;
        Parameter::Id m_color_param_id;
        Parameter::Id m_white_param_id;

        uint8_t m_red_pin;
        uint8_t m_green_pin;
        uint8_t m_blue_pin;
        uint8_t m_warmwhite_pin;
        uint8_t m_coldwhite_pin;

        uint32_t m_task_timestamp_ms;

        void Update();
        void SetPWM(uint8_t pin, uint8_t pwm);
};

#endif // _SIMPLE_LED_STRIP_H
