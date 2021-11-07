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

#ifndef _LED_SCENE_H
#define _LED_SCENE_H

//----------------------------------------------------------------------------
// include
#include "common.h"
#include "LedMatrix.h"
#include "LedArea.h"

//----------------------------------------------------------------------------
// define

//----------------------------------------------------------------------------
// typedef
typedef enum
{
    OFFICE_TABLE_WW = 0,
    LIGHT_ON_WW,
    RAINBOW,
    SUNSET,
    SUNRISE,
    POWER_OFF,
    MOVING_DOT,
    SBH,
    USER_SETTING,
    IDLE,
} light_scene_t;

//----------------------------------------------------------------------------
// class
class LedScene
{
    public:
        LedScene();
        ~LedScene();
        void Tasks(void);
        void LedScene::ChangeLightScene(light_scene_t scene);
        void ChangeLightScene(light_scene_t scene, uint8_t brightness);
        light_scene_t GetLightScene(void);
        LedArea *GetLedArea(void);
        void SetLedArea(uint16_t xs, uint16_t xe, uint16_t ys, uint16_t ye, uint32_t color);
        uint8_t GetBrightness(void);
        void SetBrightness(uint8_t brightness);
        uint32_t GetColor(void);
        void SetColor(uint32_t color);

    private:
        const uint32_t TMO_TILL_NEXT_UPDATE_MS = 20;
        const uint32_t COLOR_BLUE = 0x000000FF;
        const uint32_t COLOR_GREEN = 0x0000FF00;
        const uint32_t COLOR_RED = 0x00FF0000;
        const uint32_t COLOR_WHITE = 0xFF000000;

        LedMatrix *m_led_matrix;
        LedArea *m_led_area;
        light_scene_t m_light_scene;
        light_scene_t m_state;
        uint8_t m_current_brightness;
        uint8_t m_desired_brightness;
        uint32_t m_color;
        uint32_t m_update_time_ms;
        uint16_t m_px;

        void ShowOfficeTableWW_Enter(uint16_t brightness);
        void ShowOfficeTableWW_Task(void);
        void LightOnWW_Enter(uint16_t brightness);
        void LightOnWW_Task(void);
        void Sunrise_Task(void);
        void PowerOff_Task(void);
        void MovingDot_Task(void);
        void UserSetting_Task(void);
        void WhiteOverRainbow_Task(int whiteSpeed, int whiteLength);
        void UpdateBrightness(void);
};

#endif // _LED_SCENE_H
