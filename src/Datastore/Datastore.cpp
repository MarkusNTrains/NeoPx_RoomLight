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



//-----------------------------------------------------------------------------
// const
#define EEPROM_BlockHeaderSize 2


//*****************************************************************************
// description:
//   constructor
//*****************************************************************************
Datastore::Datastore()
{
  #if (IS_DEBUG_MODE == ON)
    Serial.println(F("Datastore Init"));
  #endif

    //--- init member parameter -------------------------------------------
    this->m_eeprom_last_update_timestamp_ms = millis();
    this->m_last_parameter_changed_timestamp_ms = millis();
    this->m_eeprom_pageSize = 0;
    this->m_eeprom_nofPages = 0;
    this->m_eeprom_active_page = 0;
    this->m_is_eeprom_update_needed = false;
    this->m_parameter_p = new Parameter();
    bool valid_page_found = false;


#if (DATASTORE_SaveDataOnEEPROM == ON)
  #ifdef __AVR__
    //--- calculate page size and nof pages -------------------------------
    this->m_eeprom_pageSize = Parameter::BUFFER_Size + EEPROM_BlockHeaderSize;
    this->m_eeprom_nofPages = EEPROM.length() / this->m_eeprom_pageSize;


    //--- find valid page in EEPROM ---------------------------------------
    uint16_t valid_pattern = 0;
    uint16_t page_start_addr = 0;

    for (uint16_t page = 0; page < this->m_eeprom_nofPages; page++)
    {
        page_start_addr = page * this->m_eeprom_pageSize;
        valid_pattern = (uint16_t)EEPROM_ReadParameter(Parameter::Id::ParameterSet_Validity, page_start_addr);
        if (valid_pattern == Parameter::PARAMETERSET_Valid)
        {
            // valid page found
            this->m_eeprom_active_page = page;
            valid_page_found = true; 
    #if (IS_DEBUG_MODE == ON)
            Serial.print(F("Valid Page: "));
            Serial.println(this->m_eeprom_active_page);
    #endif

            // so read data from EEPROM
            for (uint8_t id = 0; id < Parameter::Id::Nof; id++)
            {
                this->m_parameter_p->SetValue((Parameter::Id)id, this->EEPROM_ReadParameter((Parameter::Id)id, page_start_addr));
            }

            break;  // skip for loop
        }
    }
  #else
    //--- find read stored parameters ---
    FlashHdl::Error err = this->m_flashHdl.ReadBlock(this->m_parameter_p->GetBufferPtr(), Parameter::BUFFER_Size, 0);
    switch (err)
    {
        case FlashHdl::Error::Successfull:
            valid_page_found = true;
            break;

        case FlashHdl::Error::NoValidBlock:

        case FlashHdl::Error::DataToLarge:
        case FlashHdl::Error::OffsetOutOfBoundaries:
        case FlashHdl::Error::WriteVerifyFailed:
        case FlashHdl::Error::DataDidNotChangeNoWriteDone:
        default:
          #if (IS_DEBUG_MODE == ON)
            Serial.print(F("Read Failure: "));
            Serial.println(err);
          #endif
            break;
    }
  #endif
#endif
    
    //--- check if factory reset is needed
    if (valid_page_found == false)
    {
        this->FactoryReset();
    }

  #if (IS_DEBUG_MODE == ON)
    Serial.println(F("Datastore Ready"));
  #endif
    
}


//*****************************************************************************
// description:
//   destructor
//*****************************************************************************
Datastore::~Datastore()
{
    delete this->m_parameter_p;
}


//*****************************************************************************
// description:
//   Task
//*****************************************************************************
void Datastore::Task()
{
#if (DATASTORE_SaveDataOnEEPROM == ON)
    if (this->m_is_eeprom_update_needed == true)
    {
        if (millis() - this->m_eeprom_last_update_timestamp_ms > EEPROM_WriteLockAfterEepromWriteTmoMs)
        {
            if (millis() - this->m_last_parameter_changed_timestamp_ms > EEPROM_WriteLockAfterParameterChangeTmoMs)
            {
                this->m_is_eeprom_update_needed = false;
            #ifdef __AVR__

                uint16_t page_start_addr = this->m_eeprom_active_page * this->m_eeprom_pageSize;
                bool is_eeprom_write_needed = false;

                for (uint8_t id = 0; id < Parameter::Id::Nof; id++)
                {
                    if (this->m_parameter_p->GetValue((Parameter::Id)id) != this->EEPROM_ReadParameter((Parameter::Id)id, page_start_addr))
                    {
    #if (IS_DEBUG_MODE == ON)
                        Serial.print(F("Parameter changed: "));
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
                    Serial.print(F("Write to next page: "));
                    Serial.println(this->m_eeprom_active_page);
        #endif
                }
                else
                {
        #if (IS_DEBUG_MODE == ON)
                    Serial.println(F("EEPROM write not needed"));
        #endif
                }
            #else
                this->m_eeprom_last_update_timestamp_ms = millis();
                this->m_flashHdl.WriteToNextBlock(this->m_parameter_p->GetBufferPtr(), Parameter::BUFFER_Size);
            #endif
            }
        }
    }
#endif
}


//*****************************************************************************
// description:
//   FactoryReset
//*****************************************************************************
void Datastore::FactoryReset()
{
#if (IS_DEBUG_MODE == ON)
    Serial.println(F("Do Factory Reset"));
#endif

#if (DATASTORE_SaveDataOnEEPROM == ON)    
  #ifdef __AVR__
    // clear EEPROM
    for (uint16_t addr = 0; addr < EEPROM.length(); addr++)
    {
        EEPROM.write(addr, 0xFF);
    }
  #endif
#endif

    // reset all parameter
    this->m_parameter_p->ResetAll();
    this->m_eeprom_active_page = 0;
#if (DATASTORE_SaveDataOnEEPROM == ON)
  #ifdef __AVR__
    this->EEPROM_WritePage();
  #else
    this->m_flashHdl.WriteToNextBlock(this->m_parameter_p->GetBufferPtr(), Parameter::BUFFER_Size);  
  #endif
#endif
}


//*****************************************************************************
// description:
//   Get Parameter
//*****************************************************************************
uint32_t Datastore::GetParameter(Parameter::Id id)
{
    return this->m_parameter_p->GetValue(id);
}


//*****************************************************************************
// description:
//   Set Parameter
//*****************************************************************************
void Datastore::SetParameter(Parameter::Id id, uint8_t value)
{
    this->SetParameter(id, (uint32_t)value);
}


//*****************************************************************************
// description:
//   Set Parameter
//*****************************************************************************
void Datastore::SetParameter(Parameter::Id id, uint16_t value)
{
    this->SetParameter(id, (uint32_t)value);
}


//*****************************************************************************
// description:
//   Set Parameter
//*****************************************************************************
void Datastore::SetParameter(Parameter::Id id, uint32_t value)
{
    if (this->m_parameter_p->SetValue(id, value) == true)
    {
        this->m_is_eeprom_update_needed = true;
        this->m_last_parameter_changed_timestamp_ms = millis();
    }
}


#ifdef __AVR__
#if (DATASTORE_SaveDataOnEEPROM == ON)
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
    EEPROM_WriteParameter(Parameter::Id::ParameterSet_Validity, last_page_start_addr, Parameter::PARAMETERSET_Invalid);
}


//*****************************************************************************
// description:
//   Write page to EEPROM
//*****************************************************************************
void Datastore::EEPROM_WritePage()
{
    uint16_t page_start_addr = this->m_eeprom_active_page * this->m_eeprom_pageSize;

    // write all parameter
    for (uint8_t id = 0; id < Parameter::Id::Nof; id++)
    {
        if (id != Parameter::Id::ParameterSet_Validity)
        {
            EEPROM_WriteParameter((Parameter::Id)id, page_start_addr);
        }
    }

    // set valid pattern
    EEPROM_WriteParameter(Parameter::Id::ParameterSet_Validity, page_start_addr);
}


//*****************************************************************************
// description:
//   Read Parameter from EEPROM
//*****************************************************************************
uint32_t Datastore::EEPROM_ReadParameter(Parameter::Id id, uint16_t page_start_addr)
{
    uint32_t value = 0;
    uint16_t addr = page_start_addr + this->m_parameter_p->GetAddr(id);
    
    switch (this->m_parameter_p->GetWidth(id))
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
void Datastore::EEPROM_WriteParameter(Parameter::Id id, uint16_t page_start_addr, uint32_t value)
{
    uint16_t addr = page_start_addr + this->m_parameter_p->GetAddr(id);

    switch (this->m_parameter_p->GetWidth(id))
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


//*****************************************************************************
// description:
//   Write Parameter to EEPROM
//*****************************************************************************
void Datastore::EEPROM_WriteParameter(Parameter::Id id, uint16_t page_start_addr)
{
    uint32_t value = this->m_parameter_p->GetValue(id);
    this->EEPROM_WriteParameter(id, page_start_addr, value);
}
#endif
#endif