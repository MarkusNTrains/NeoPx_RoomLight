/*##############################################################################

  THIS IS GENERATED CODE (use RoomLight_Parameter.ods)
    ==> DO NOT EDIT MANUALLY !!!

##############################################################################*/


/*******************************************************************************
Project   RoomLight

  This is an OpenSource Project.
  You can use, share or improve this project. If you improve this source code
  please share with the comunity or at least with the author of the original
  source code

  Created 27.04.2023 11:42:28 by MarkusNTrains
================================================================================
$HeadURL: file:///C:/_my/Arduino/NeoPx_RoomLight/Parameter_RoomLight.cpp $
$Id: 27.04.2023 11:42:28 automatic gen. $
*******************************************************************************/

/*
Defines the Parameter of RoomLight device and their properties.
 Usage of this file allows to have configuration parameter specific information
 only within this file. See RoomLight_Parameter.ods for more Information.
*/

//------------------------------------------------------------------------------
// include
#include "Parameter.h"
#include "LightSceneHdl.h"



//------------------------------------------------------------------------------
// const parameter
const static parameter_t PARAM_TABLE[Parameter::Id::Nof] PROGMEM = {
    [Parameter::Id::ParameterSet_Validity] = {
        .default_value = (uint32_t)Parameter::PARAMETERSET_Invalid,
        .min = 0,
        .max = 0xFFFF,
        .addr = 0,
        .width = 2,
    },
    [Parameter::Id::LightSceneID] = {
        .default_value = (uint32_t)LightSceneID::OfficeTable,
        .min = 0,
        .max = ((uint32_t)LightSceneID::Nof - 1) ,
        .addr = 2,
        .width = 1,
    },
    [Parameter::Id::Brightness ] = {
        .default_value = Parameter::BRIGHTNESS_Default ,
        .min = Parameter::BRIGHTNESS_Min ,
        .max = Parameter::BRIGHTNESS_Max ,
        .addr = 3,
        .width = 1,
    },
    [Parameter::Id::Color] = {
        .default_value = Parameter::COLOR_Default,
        .min = Parameter::COLOR_Min,
        .max = Parameter::COLOR_Max,
        .addr = 4,
        .width = 4,
    },
    [Parameter::Id::SceneDisco_Brightness ] = {
        .default_value = Parameter::BRIGHTNESS_Default ,
        .min = Parameter::BRIGHTNESS_Min ,
        .max = Parameter::BRIGHTNESS_Max ,
        .addr = 8,
        .width = 1,
    },
    [Parameter::Id::SceneMoBa_Brightness ] = {
        .default_value = Parameter::BRIGHTNESS_Default ,
        .min = Parameter::BRIGHTNESS_Min ,
        .max = Parameter::BRIGHTNESS_Max ,
        .addr = 9,
        .width = 1,
    },
    [Parameter::Id::SceneMoBa_Color ] = {
        .default_value = Parameter::COLOR_Default,
        .min = Parameter::COLOR_Min,
        .max = Parameter::COLOR_Max,
        .addr = 10,
        .width = 4,
    },
    [Parameter::Id::SceneLightOn_Brightness ] = {
        .default_value = Parameter::BRIGHTNESS_Default ,
        .min = Parameter::BRIGHTNESS_Min ,
        .max = Parameter::BRIGHTNESS_Max ,
        .addr = 14,
        .width = 1,
    },
    [Parameter::Id::SceneLightOn_Color ] = {
        .default_value = Parameter::COLOR_Default,
        .min = Parameter::COLOR_Min,
        .max = Parameter::COLOR_Max,
        .addr = 15,
        .width = 4,
    },
    [Parameter::Id::SceneOfficeTable_Brightness ] = {
        .default_value = Parameter::BRIGHTNESS_Default ,
        .min = Parameter::BRIGHTNESS_Min ,
        .max = Parameter::BRIGHTNESS_Max ,
        .addr = 19,
        .width = 1,
    },
    [Parameter::Id::SceneOfficeTable_Color ] = {
        .default_value = Parameter::COLOR_Default,
        .min = Parameter::COLOR_Min,
        .max = Parameter::COLOR_Max,
        .addr = 20,
        .width = 4,
    },
    [Parameter::Id::SceneRainbow_Brightness ] = {
        .default_value = Parameter::BRIGHTNESS_Default ,
        .min = Parameter::BRIGHTNESS_Min ,
        .max = Parameter::BRIGHTNESS_Max ,
        .addr = 24,
        .width = 1,
    },
    [Parameter::Id::SceneUserSetting_Brightness ] = {
        .default_value = Parameter::BRIGHTNESS_Default ,
        .min = Parameter::BRIGHTNESS_Min ,
        .max = Parameter::BRIGHTNESS_Max ,
        .addr = 25,
        .width = 1,
    },
    [Parameter::Id::SceneUserSetting_Color ] = {
        .default_value = Parameter::COLOR_Default,
        .min = Parameter::COLOR_Min,
        .max = Parameter::COLOR_Max,
        .addr = 26,
        .width = 4,
    },
    [Parameter::Id::SceneUserSetting_Xs ] = {
        .default_value = 0,
        .min = 0,
        .max = (ROOM_LIGHT_RowNofPx - 1) ,
        .addr = 30,
        .width = 2,
    },
    [Parameter::Id::SceneUserSetting_Xe ] = {
        .default_value = (ROOM_LIGHT_RowNofPx - 1) ,
        .min = 0,
        .max = (ROOM_LIGHT_RowNofPx - 1) ,
        .addr = 32,
        .width = 2,
    },
    [Parameter::Id::SceneUserSetting_Ys ] = {
        .default_value = 0,
        .min = 0,
        .max = (ROOM_LIGHT_NofRows - 1) ,
        .addr = 34,
        .width = 1,
    },
    [Parameter::Id::SceneUserSetting_Ye ] = {
        .default_value = (ROOM_LIGHT_NofRows - 1) ,
        .min = 0,
        .max = (ROOM_LIGHT_NofRows - 1) ,
        .addr = 35,
        .width = 1,
    },
};


