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

  Created 27.04.2023 14:40:21 by MarkusNTrains
================================================================================
$HeadURL: file:///C:/_my/Arduino/NeoPx_RoomLight/Parameter.h $
$Id: 27.04.2023 14:40:21 automatic gen. $
*******************************************************************************/

/*
Defines the Parameter of RoomLight device and their properties.
 Usage of this file allows to have configuration parameter specific information
 only within this file. See RoomLight_Parameter.ods for more Information.
*/

#ifndef _PARAMETER_H
#define _PARAMETER_H


//----------------------------------------------------------------------------
// include
#include <stdint.h>


//----------------------------------------------------------------------------
// class
class Parameter
{
    public:
        enum Id
        {
            ParameterSet_Validity = 0,
            LightSceneID,
            SceneDisco_Brightness ,
            SceneMoBa_Brightness ,
            SceneMoBa_Color ,
            SceneLightOn_Brightness ,
            SceneLightOn_Color ,
            SceneOfficeTable_Brightness ,
            SceneOfficeTable_Color ,
            SceneRainbow_Brightness ,
            SceneUserSetting_Brightness ,
            SceneUserSetting_Color ,
            SceneUserSetting_Xs ,
            SceneUserSetting_Xe ,
            SceneUserSetting_Ys ,
            SceneUserSetting_Ye ,
            Nof,
            Unknown
        };

        //--- Parameter ---
        const static uint16_t PARAMETERSET_Valid = 0x55AA;
        const static uint16_t PARAMETERSET_Invalid = 0xFFFF;

        const static uint8_t BRIGHTNESS_Default = 100;
        const static uint8_t BRIGHTNESS_Min = 0;
        const static uint8_t BRIGHTNESS_Max = 255;

        const static uint32_t COLOR_Default = 0xFF000000;
        const static uint32_t COLOR_Min = 0;
        const static uint32_t COLOR_Max = 0xFFFFFFFF;

        const static uint16_t BUFFER_Size = 31;

        Parameter();
        ~Parameter();

        void Reset(Id id);
        void ResetAll();
        uint32_t GetValue(Id id);
        bool SetValue(Id id, uint32_t value);

        static uint16_t GetAddr(Id id);
        static uint8_t GetWidth(Id id);

    private:
        uint8_t m_buffer[BUFFER_Size];
};


//----------------------------------------------------------------------------
// typedef struct
typedef struct
{
    uint32_t default_value;
    uint32_t min;
    uint32_t max;
    uint16_t addr;
    uint8_t width;
} parameter_t;



//----------------------------------------------------------------------------
// extern
extern const parameter_t PARAM_TABLE[Parameter::Id::Nof];


#endif  // _PARAMETER_H

