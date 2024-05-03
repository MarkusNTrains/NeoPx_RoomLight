/*******************************************************************************
Project   RoomLight

  This is an OpenSource Project.
  You can use, share or improve this project. If you improve this source code
  please share with the comunity or at least with the author of the original 
  source code
  
  Created 25. April 2024 by MarkusNTrains
================================================================================
$HeadURL:  $
$Id:  $
*******************************************************************************/

#ifdef __AVR__
#ifndef _EEPROM_HDL_H
#define _EEPROM_HDL_H


//----------------------------------------------------------------------------
// include
#include "../common.h"
#include <EEPROM.h>
#include "StoreHdl.h"


//----------------------------------------------------------------------------
// class
class EepromHdl : StoreHdl
{
    public:
        EepromHdl();
        ~EepromHdl();

        Error ReadBlock(uint8_t* data, uint32_t size, uint32_t offset);
        Error WriteToNextBlock(uint8_t* data, uint32_t size);

    private:
        const static uint8_t CELL_ERASED_VALUE = 0xFF;
        const static uint32_t EEPROM_StartAddress = 0;

        uint16_t m_nof_blocks;
        uint16_t m_active_block_idx;
        uint16_t m_write_cnt;
        StoreBlockInfo* m_block_a;

        bool FindNewestBlock();
        void EraseWholeEeprom();
};



#endif // _EEPROM_HDL_H
#endif // __AVR__