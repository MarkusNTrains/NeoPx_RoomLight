/*******************************************************************************
Project   RoomLight

  This is an OpenSource Project.
  You can use, share or improve this project. If you improve this source code
  please share with the comunity or at least with the author of the original 
  source code
  
  Created 20. April 2023 by MarkusNTrains
================================================================================
$HeadURL:  $
$Id:  $
*******************************************************************************/


#ifndef _DATASTORE_H
#define _DATASTORE_H


//----------------------------------------------------------------------------
// include
#include "Parameter.h"
#include "common.h"



//----------------------------------------------------------------------------
// enum


//----------------------------------------------------------------------------
// class
class Datastore
{
	public:
        enum ParameterId
        {
            LightSceneID = 0,
            Brightness,
            Color,
            SceneDisco_Brightness,
            SceneMoBa_Brightness,
            SceneMoBa_Color,
            SceneLightOn_Brightness,
            SceneLightOn_Color,
            SceneOfficeTable_Brightness,
            SceneOfficeTable_Color,
            SceneRainbow_Brightness,
            UserSetting_Xs,
            UserSetting_Xe,
            UserSetting_Ys,
            UserSetting_Ye,
            Nof
        };

        Datastore();
        ~Datastore();
		
        void Task();
        void FactoryReset();
        uint32_t GetParameter(ParameterId id);
        void SetParameter(ParameterId id, uint8_t value);
        void SetParameter(ParameterId id, uint16_t value);
        void SetParameter(ParameterId id, uint32_t value);

    private:
        //--- EEPROM ---
        const uint32_t AFTER_PARAMETER_CHANGE_EEPROM_WriteLockTmoMs = 5000;
        const uint32_t EEPROM_WriteLockTmoMs = 60000;

        const uint16_t EEPROM_ValidPatternAddr_MSB = 0;
        const uint16_t EEPROM_ValidPatternAddr_LSB = 1;
        const uint16_t EEPROM_ParameterStartAddr = 2;

        const uint16_t EEPROM_PageValidPattern = 0x55AA;

        //--- Parameter ---
        const uint8_t BRIGHTNESS_Default = 100;
        const uint8_t BRIGHTNESS_Min = 0;
        const uint8_t BRIGHTNESS_Max = 255;
        const uint8_t BRIGHTNESS_Width = 1;

        const uint32_t COLOR_Default = 0xFF000000;
        const uint32_t COLOR_Min = 0;
        const uint32_t COLOR_Max = 0xFFFFFFFF;
        const uint8_t COLOR_Width = 4;

        //--- Memeber Variable ---
        Parameter* m_parameter_list[ParameterId::Nof];
        uint32_t m_last_parameter_changed_timestamp_ms;
        uint32_t m_eeprom_last_update_timestamp_ms;
        uint16_t m_eeprom_pageSize;
        uint16_t m_eeprom_nofPages;
        uint16_t m_eeprom_active_page;
        bool m_is_eeprom_update_needed;

        //--- Member Function ---
        void EEPROM_WriteToNextPage();
        void EEPROM_WritePage();
        uint32_t EEPROM_ReadParameter(ParameterId id, uint16_t page_start_addr);
        void EEPROM_WriteParameter(ParameterId id, uint16_t page_start_addr);
};

#endif  // _DATASTORE_H
