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


//-----------------------------------------------------------------------------
// includes
#include "Datastore.h"
#include "LightSceneHdl.h"
#include <EEPROM.h>



//*****************************************************************************
// description:
//   constructor
//*****************************************************************************
Datastore::Datastore()
{
    //--- init parameter list ---------------------------------------------
    uint32_t light_scene = (uint32_t)LightScene::UserSetting;
#if (ROOM_LIGHT == ROOM_LIGHT_MarkusNTrains)
    light_scene = (uint32_t)LightScene::OfficeTable;
#endif


    //--- set parameter definition ----------------------------------------
    this->m_parameter_list[ParameterId::Brightness] = new Parameter(100, 0, 100, 1);
    this->m_parameter_list[ParameterId::Color] = new Parameter(0xFF000000, 0, 0xFFFFFFFF, 4);
    this->m_parameter_list[ParameterId::LightScene] = new Parameter(light_scene, 0, (uint32_t)LightScene::Nof, 1);
    this->m_parameter_list[ParameterId::UserSetting_Xs] = new Parameter(0, 0, 0xFFFF, 2);
    this->m_parameter_list[ParameterId::UserSetting_Xe] = new Parameter(0, 0, 0xFFFF, 2);
    this->m_parameter_list[ParameterId::UserSetting_Ys] = new Parameter(0, 0, 0xFFFF, 2);
    this->m_parameter_list[ParameterId::UserSetting_Ye] = new Parameter(0, 0, 0xFFFF, 2);


    //--- init parameter address ------------------------------------------
    uint16_t addr = EEPROM_ParameterStartAddr;
    for (uint8_t idx = 0; idx < ParameterId::Nof; idx++)
    {
        this->m_parameter_list[idx]->SetAddr(addr);
        addr += this->m_parameter_list[idx]->GetWidth();
    }


    //--- check if EEPROM is valid ----------------------------------------
    uint32_t value = 0;
    uint16_t valid_pattern = ((uint16_t)EEPROM.read(EEPROM_ValidPatternAddr_MSB)) << 8;
    valid_pattern |= EEPROM.read(EEPROM_ValidPatternAddr_LSB);
    if (valid_pattern == EEPROM_ValidPattern)
    {
        // EEPROM is valid, so read data from EEPROM
        for (uint8_t idx = 0; idx < ParameterId::Nof; idx++)
        {
            value = 0;
            addr = this->m_parameter_list[idx]->GetAddr();
            switch (this->m_parameter_list[idx]->GetWidth())
            {
                case 4:
                    value |= (((uint32_t)EEPROM.read(addr++)) << 24);
                case 3:
                    value |= (((uint32_t)EEPROM.read(addr++)) << 16);
                case 2:
                    value |= (((uint32_t)EEPROM.read(addr++)) << 8);
                case 1:
                    value |= (((uint32_t)EEPROM.read(addr++)));
                    this->m_parameter_list[idx]->SetValue(value);
                    break;

                default:
                    break;
            }
        }
    }
    else
    {
        this->FactoryReset();
    }
}


//*****************************************************************************
// description:
//   destructor
//*****************************************************************************
Datastore::~Datastore()
{
}


//*****************************************************************************
// description:
//   FactoryReset
//*****************************************************************************
void Datastore::FactoryReset()
{
        Serial.println("factory Reset");

    // clear valid pattern
    EEPROM.write(EEPROM_ValidPatternAddr_MSB, 0xFF);
    EEPROM.write(EEPROM_ValidPatternAddr_LSB, 0xFF);

    // reset all parameter
    for (uint8_t id = 0; id < ParameterId::Nof; id++)
    {
        this->m_parameter_list[id]->Reset();
        this->WriteToEEPROM(id);
    }

    // set valid pattern
    EEPROM.write(EEPROM_ValidPatternAddr_MSB, (uint8_t)(EEPROM_ValidPattern >> 8));
    EEPROM.write(EEPROM_ValidPatternAddr_LSB, (uint8_t)EEPROM_ValidPattern);
}


//*****************************************************************************
// description:
//   Get Parameter
//*****************************************************************************
uint32_t Datastore::GetParameter(ParameterId id)
{
    return this->m_parameter_list[id]->GetValue();
}


//*****************************************************************************
// description:
//   Set Parameter
//*****************************************************************************
void Datastore::SetParameter(ParameterId id, uint8_t value)
{
    this->m_parameter_list[id]->SetValue((uint32_t)value);
    this->WriteToEEPROM(id);
}


//*****************************************************************************
// description:
//   Set Parameter
//*****************************************************************************
void Datastore::SetParameter(ParameterId id, uint16_t value)
{
    this->m_parameter_list[id]->SetValue((uint32_t)value);
    this->WriteToEEPROM(id);
}


//*****************************************************************************
// description:
//   Set Parameter
//*****************************************************************************
void Datastore::SetParameter(ParameterId id, uint32_t value)
{
    this->m_parameter_list[id]->SetValue(value);
    this->WriteToEEPROM(id);
}


//*****************************************************************************
// description:
//   Write Value to EEPROM
//*****************************************************************************
void Datastore::WriteToEEPROM(ParameterId id)
{
    uint32_t value = this->m_parameter_list[id]->GetValue();
    uint16_t addr = this->m_parameter_list[id]->GetAddr();

    switch (this->m_parameter_list[id]->GetWidth())
    {
        case 4:
            EEPROM.update(addr++, (uint8_t)(value >> 24));
        case 3:
            EEPROM.update(addr++, (uint8_t)(value >> 16));
        case 2:
            EEPROM.update(addr++, (uint8_t)(value >> 8));
        case 1:
            EEPROM.update(addr++, (uint8_t)(value));
            break;

        default:
            break;
    }
}

