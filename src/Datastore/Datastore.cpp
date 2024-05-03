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
    this->m_save_timestamp_ms = millis();
    this->m_parameter_changed_timestamp_ms = millis();
    this->m_did_parameter_change = false;
    this->m_parameter_p = new Parameter();
    bool valid_page_found = false;


#if (DATASTORE_SaveDataPermanent == ON)
    //--- find stored parameters ---
  #ifdef __AVR__
    StoreHdl::Error err = this->m_eepromHdl.ReadBlock(this->m_parameter_p->GetBufferPtr(), Parameter::BUFFER_Size, 0);
  #else
    StoreHdl::Error err = this->m_flashHdl.ReadBlock(this->m_parameter_p->GetBufferPtr(), Parameter::BUFFER_Size, 0);
  #endif
    switch (err)
    {
        case StoreHdl::Error::Successfull:
            valid_page_found = true;
            break;

        case StoreHdl::Error::NoValidBlock:

        case StoreHdl::Error::DataToLarge:
        case StoreHdl::Error::OffsetOutOfBoundaries:
        case StoreHdl::Error::WriteVerifyFailed:
        case StoreHdl::Error::DataDidNotChangeNoWriteDone:
        default:
          #if (IS_DEBUG_MODE == ON)
            Serial.print(F("Read Failure: "));
            Serial.println(err);
          #endif
            break;
    }
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
#if (DATASTORE_SaveDataPermanent == ON)
    if (this->m_did_parameter_change == true)
    {
        if (millis() - this->m_save_timestamp_ms > STORE_WriteLockAfterStoreTmoMs)
        {
            if (millis() - this->m_parameter_changed_timestamp_ms > STORE_WriteLockAfterParameterChangeTmoMs)
            {
                this->m_did_parameter_change = false;
                this->m_save_timestamp_ms = millis();
            #ifdef __AVR__
                this->m_eepromHdl.WriteToNextBlock(this->m_parameter_p->GetBufferPtr(), Parameter::BUFFER_Size);
            #else
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

    // reset all parameter
    this->m_parameter_p->ResetAll();

#if (DATASTORE_SaveDataPermanent == ON)
  #ifdef __AVR__
    this->m_eepromHdl.WriteToNextBlock(this->m_parameter_p->GetBufferPtr(), Parameter::BUFFER_Size);  
  #else
    this->m_flashHdl.WriteToNextBlock(this->m_parameter_p->GetBufferPtr(), Parameter::BUFFER_Size);  
  #endif
#endif

#if (IS_DEBUG_MODE == ON)
    Serial.println(F("Do Factory Reset... Done"));
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
        this->m_did_parameter_change = true;
        this->m_parameter_changed_timestamp_ms = millis();
    }
}

