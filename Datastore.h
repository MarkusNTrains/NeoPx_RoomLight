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
        //--- EEPROM ---
        const static uint32_t EEPROM_WriteLockAfterParameterChangeTmoMs = 5000;
        const static uint32_t EEPROM_WriteLockAfterEepromWriteTmoMs = 60000;

        //--- Memeber Variable ---
        Parameter* m_parameter_p;
        uint32_t m_last_parameter_changed_timestamp_ms;
        uint32_t m_eeprom_last_update_timestamp_ms;
        uint16_t m_eeprom_pageSize;
        uint16_t m_eeprom_nofPages;
        uint16_t m_eeprom_active_page;
        bool m_is_eeprom_update_needed;

        //--- Member Function ---
        void EEPROM_WriteToNextPage();
        void EEPROM_WritePage();
        uint32_t EEPROM_ReadParameter(Parameter::Id id, uint16_t page_start_addr);
        void EEPROM_WriteParameter(Parameter::Id id, uint16_t page_start_addr);
        void EEPROM_WriteParameter(Parameter::Id id, uint16_t page_start_addr, uint32_t value);
};

#endif  // _DATASTORE_H
