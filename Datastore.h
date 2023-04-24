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
enum class ParameterId
{
    LightScene = 0,
    Brightness,
    Color,
    UserSetting_Xs,
    Nof
};


//----------------------------------------------------------------------------
// class
class Datastore
{
	public:
        enum ParameterId
        {
            LightScene = 0,
            Brightness,
            Color,
            UserSetting_Xs,
            UserSetting_Xe,
            UserSetting_Ys,
            UserSetting_Ye,
            Nof
        };

        Datastore();
        ~Datastore();
		
        void FactoryReset();
        uint32_t GetParameter(ParameterId id);
        void SetParameter(ParameterId id, uint8_t value);
        void SetParameter(ParameterId id, uint16_t value);
        void SetParameter(ParameterId id, uint32_t value);

    private:
        const uint16_t EEPROM_ValidPatternAddr_MSB = 0;
        const uint16_t EEPROM_ValidPatternAddr_LSB = 1;
        const uint16_t EEPROM_ParameterStartAddr = 2;

        const uint16_t EEPROM_ValidPattern = 0x55AA;

        Parameter* m_parameter_list[ParameterId::Nof];

        void WriteToEEPROM(ParameterId id);
};

#endif  // _DATASTORE_H
