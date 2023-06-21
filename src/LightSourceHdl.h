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

#ifndef _LIGHT_SOURCE_HDL_H
#define _LIGHT_SOURCE_HDL_H


//----------------------------------------------------------------------------
// include
#include "common.h"
#include "LightSource.h"
#include "Datastore/Datastore.h"
#include "RoomLight/RoomLightHdl.h"
#include "SimpleLedStrip/SimpleLedStripHdl.h"


//----------------------------------------------------------------------------
// define


//----------------------------------------------------------------------------
// enum


//----------------------------------------------------------------------------
// extern


//----------------------------------------------------------------------------
// class
class LightSourceHdl
{
    public:
        enum Source
        {
            RoomLight = 0,
            ShadowStation,
            Nof,
        };

        LightSourceHdl();
        ~LightSourceHdl();

        void Tasks();
        Source GetActiveSource();
        void ActivateSource(Source source);
        uint8_t GetActiveScene();
        uint8_t GetActiveAnimation();
        void ChangeScene(uint8_t scene_id);
        uint8_t GetBrightness();
        void SetBrightness(uint8_t brightness);
        uint32_t GetColor();
        void SetColor(uint32_t color);
        uint16_t GetWhite();
        void SetWhite(uint16_t white);
        void GetUserSettingArea(LedArea* area_p);
        void SetUserSettingArea(uint16_t xs, uint16_t xe, uint16_t ys, uint16_t ye);

    protected:

    private:
        Source m_active_source;
        Datastore* m_datastore_p;
        LightSource* m_active_light_source_p;
        RoomLightHdl* m_room_light_hdl_p;
        SimpleLedStripHdl* m_shadow_station_ligth_hdl_p;
};


#endif // _LIGHT_SOURCE_HDL_H
