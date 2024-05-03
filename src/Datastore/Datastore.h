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
#include "../common.h"

#ifdef __AVR__
  #include "EepromHdl.h"
#else
  #include "FlashHdl.h"
#endif




//----------------------------------------------------------------------------
// class
class Datastore
{
	public:
        Datastore();
        ~Datastore();
		
        void Task();
        void FactoryReset();
        uint32_t GetParameter(Parameter::Id id);
        void SetParameter(Parameter::Id id, uint8_t value);
        void SetParameter(Parameter::Id id, uint16_t value);
        void SetParameter(Parameter::Id id, uint32_t value);

    private:
        //--- Store Permanent ---
        const static uint32_t STORE_WriteLockAfterParameterChangeTmoMs = 10000ul;
        const static uint32_t STORE_WriteLockAfterStoreTmoMs = 10000ul;

        //--- Memeber Variable ---
    #ifdef __AVR__
        EepromHdl m_eepromHdl;
    #else
        FlashHdl m_flashHdl;
    #endif
        Parameter* m_parameter_p;
        uint32_t m_parameter_changed_timestamp_ms;
        uint32_t m_save_timestamp_ms;
        bool m_did_parameter_change;

        //--- Member Function ---
        void EEPROM_WriteToNextPage();
        void EEPROM_WritePage();
        uint32_t EEPROM_ReadParameter(Parameter::Id id, uint16_t page_start_addr);
        void EEPROM_WriteParameter(Parameter::Id id, uint16_t page_start_addr);
        void EEPROM_WriteParameter(Parameter::Id id, uint16_t page_start_addr, uint32_t value);
};

#endif  // _DATASTORE_H
