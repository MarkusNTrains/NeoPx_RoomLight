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
    //--- init member parameter -------------------------------------------
    this->m_eeprom_last_update_timestamp_ms = millis();
    this->m_last_parameter_changed_timestamp_ms = millis();


    //--- set parameter definition ----------------------------------------
    uint32_t light_scene = (uint32_t)LightScene::UserSetting;
#if (ROOM_LIGHT == ROOM_LIGHT_MarkusNTrains)
    light_scene = (uint32_t)LightScene::OfficeTable;
#endif
    this->m_parameter_list[ParameterId::Brightness] = new Parameter(100, 0, 100, 1);
    this->m_parameter_list[ParameterId::Color] = new Parameter(0xFF000000, 0, 0xFFFFFFFF, 4);
    this->m_parameter_list[ParameterId::LightScene] = new Parameter(light_scene, 0, (uint32_t)LightScene::Nof, 1);
    this->m_parameter_list[ParameterId::UserSetting_Xs] = new Parameter(0, 0, 0xFFFF, 2);
    this->m_parameter_list[ParameterId::UserSetting_Xe] = new Parameter(ROOM_LIGHT_RowNofPx - 1, 0, 0xFFFF, 2);
    this->m_parameter_list[ParameterId::UserSetting_Ys] = new Parameter(0, 0, 0xFFFF, 2);
    this->m_parameter_list[ParameterId::UserSetting_Ye] = new Parameter((ROOM_LIGHT_NofRows - 1), 0, 0xFFFF, 2);


    //--- init parameter address and EEPROM page size ---------------------
    uint16_t addr = EEPROM_ParameterStartAddr;
    for (uint8_t idx = 0; idx < ParameterId::Nof; idx++)
    {
        this->m_parameter_list[idx]->SetAddr(addr);
        addr += this->m_parameter_list[idx]->GetWidth();
        this->m_eeprom_pageSize += this->m_parameter_list[idx]->GetWidth();
    }


    //--- calculate page size and nof pages -------------------------------
    this->m_eeprom_pageSize = addr;
    this->m_eeprom_nofPages = EEPROM.length() / this->m_eeprom_pageSize;


    //--- find valid page in EEPROM ---------------------------------------
    uint16_t valid_pattern = 0;
    bool valid_page_found = false;
    uint16_t page_start_addr = 0;

    for (uint16_t page = 0; page < this->m_eeprom_nofPages; page++)
    {
        page_start_addr = page * this->m_eeprom_pageSize;
        valid_pattern = ((uint16_t)EEPROM.read(page_start_addr + EEPROM_ValidPatternAddr_MSB)) << 8;
        valid_pattern |= (uint16_t)EEPROM.read(page_start_addr + EEPROM_ValidPatternAddr_LSB);
        if (valid_pattern == EEPROM_PageValidPattern)
        {
            // valid page found
            this->m_eeprom_active_page = page;
            valid_page_found = true; 
#if (IS_DEBUG_MODE == ON)
            Serial.print("Valid Page: ");
            Serial.println(this->m_eeprom_active_page);
#endif

            // so read data from EEPROM
            for (uint8_t id = 0; id < ParameterId::Nof; id++)
            {
                this->m_parameter_list[id]->SetValue(this->EEPROM_ReadParameter(id, page_start_addr));
            }

            break;  // skip for loop
        }
    }
    
    if (valid_page_found == false)
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
//   Task
//*****************************************************************************
void Datastore::Task()
{
    if (this->m_is_eeprom_update_needed == true)
    {
        if (millis() - this->m_eeprom_last_update_timestamp_ms > EEPROM_WriteLockTmoMs)
        {
            if (millis() - this->m_last_parameter_changed_timestamp_ms > AFTER_PARAMETER_CHANGE_EEPROM_WriteLockTmoMs)
            {
                this->m_is_eeprom_update_needed = false;

                uint16_t page_start_addr = this->m_eeprom_active_page * this->m_eeprom_pageSize;
                bool is_eeprom_write_needed = false;

                for (uint8_t id = 0; id < ParameterId::Nof; id++)
                {
                    if (this->m_parameter_list[id]->GetValue() != this->EEPROM_ReadParameter(id, page_start_addr))
                    {
    #if (IS_DEBUG_MODE == ON)
                        Serial.print("Parameter changed: ");
                        Serial.println(id);
    #endif
                        is_eeprom_write_needed = true;
                    }
                }

                if (is_eeprom_write_needed == true)
                {
                    this->m_eeprom_last_update_timestamp_ms = millis();
                    this->EEPROM_WriteToNextPage();
    #if (IS_DEBUG_MODE == ON)
                    Serial.print("Write to next page: ");
                    Serial.println(this->m_eeprom_active_page);
    #endif
                }
                else
                {
    #if (IS_DEBUG_MODE == ON)
                    Serial.println("EEPROM write not needed");
    #endif
                }
            }
        }
    }
}


//*****************************************************************************
// description:
//   FactoryReset
//*****************************************************************************
void Datastore::FactoryReset()
{
#if (IS_DEBUG_MODE == ON)
    Serial.println("factory Reset");
#endif

    // reset all parameter
    for (uint8_t id = 0; id < ParameterId::Nof; id++)
    {
        this->m_parameter_list[id]->Reset();
    }

    this->m_eeprom_active_page = 0;
    EEPROM_WritePage();
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
    this->SetParameter(id, (uint32_t)value);
}


//*****************************************************************************
// description:
//   Set Parameter
//*****************************************************************************
void Datastore::SetParameter(ParameterId id, uint16_t value)
{
    this->SetParameter(id, (uint32_t)value);
}


//*****************************************************************************
// description:
//   Set Parameter
//*****************************************************************************
void Datastore::SetParameter(ParameterId id, uint32_t value)
{
    if (this->m_parameter_list[id]->SetValue(value) == true)
    {
        this->m_is_eeprom_update_needed = true;
        this->m_last_parameter_changed_timestamp_ms = millis();
    }
}


//*****************************************************************************
// description:
//   Write all parameter to next EEPROM page
//*****************************************************************************
void Datastore::EEPROM_WriteToNextPage()
{
    uint16_t last_page_start_addr = this->m_eeprom_active_page * this->m_eeprom_pageSize;

    // go to next page
    this->m_eeprom_active_page++;
    if (this->m_eeprom_active_page >= this->m_eeprom_nofPages)
    {
        this->m_eeprom_active_page = 0;
    }

    this->EEPROM_WritePage();

    // clear valid pattern of last page
    EEPROM.write(last_page_start_addr + EEPROM_ValidPatternAddr_MSB, 0xFF);
    EEPROM.write(last_page_start_addr + EEPROM_ValidPatternAddr_LSB, 0xFF);

}


//*****************************************************************************
// description:
//   Write page to EEPROM
//*****************************************************************************
void Datastore::EEPROM_WritePage()
{
    uint16_t page_start_addr = this->m_eeprom_active_page * this->m_eeprom_pageSize;

    // write all parameter
    for (uint8_t id = 0; id < ParameterId::Nof; id++)
    {
        EEPROM_WriteParameter(id, page_start_addr);
    }

    // set valid pattern
    EEPROM.write(page_start_addr + EEPROM_ValidPatternAddr_MSB, (uint8_t)(EEPROM_PageValidPattern >> 8));
    EEPROM.write(page_start_addr + EEPROM_ValidPatternAddr_LSB, (uint8_t)EEPROM_PageValidPattern);
}


//*****************************************************************************
// description:
//   Read Parameter from EEPROM
//*****************************************************************************
uint32_t Datastore::EEPROM_ReadParameter(ParameterId id, uint16_t page_start_addr)
{
    uint32_t value = 0;
    uint16_t addr = page_start_addr + this->m_parameter_list[id]->GetAddr();
    
    switch (this->m_parameter_list[id]->GetWidth())
    {
        case 4:
            value |= (((uint32_t)EEPROM.read(addr++)) << 24);
        case 3:
            value |= (((uint32_t)EEPROM.read(addr++)) << 16);
        case 2:
            value |= (((uint32_t)EEPROM.read(addr++)) << 8);
        case 1:
            value |= (((uint32_t)EEPROM.read(addr++)));
            break;

        default:
            break;
    }

    return value;
}


//*****************************************************************************
// description:
//   Write Parameter to EEPROM
//*****************************************************************************
void Datastore::EEPROM_WriteParameter(ParameterId id, uint16_t page_start_addr)
{
    uint32_t value = this->m_parameter_list[id]->GetValue();
    uint16_t addr = page_start_addr + this->m_parameter_list[id]->GetAddr();

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


