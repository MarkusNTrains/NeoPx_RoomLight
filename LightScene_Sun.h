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
// define


//----------------------------------------------------------------------------
// enum


//----------------------------------------------------------------------------
// extern
class LightSceneHdl;


//----------------------------------------------------------------------------
// class
class LightScene_Sun
{
    public:
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

    private:
        const uint8_t DAY_BRIGHTNESS = 150;
        const uint8_t NIGHT_BRIGHTNESS = 4;

        const uint32_t PIXEL_DISTANCE_MM = 16;  // distance between neo pixels in mm
        const uint32_t SUN_MAX_HEIGHT = 10000;


        LightSceneHdl* m_scene_hdl_p;
        LightHdl* m_light_hdl_p;

        uint32_t m_sun_height;
        uint32_t m_sun_pos;        
};

#endif // _LIGHT_SCENE_SUN_H
