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


//-----------------------------------------------------------------------------
// includes
#include "FlashHdl.h"



//----------------------------------------------------------------------------
// Instance



//*****************************************************************************
// description:
//   constructor
//*****************************************************************************
FlashHdl::FlashHdl()
{
    this->m_active_block_idx = 0;
    this->m_active_block_write_cnt = 0;

  #if (IS_DEBUG_MODE == ON)
    Serial.print(F("ROW_SIZE: "));
    Serial.println(Flash::ROW_SIZE);

    Serial.print(F("BLOCK_DATA_SIZE: "));
    Serial.println(FlashBlockData::BLOCK_DATA_SIZE);

    Serial.print(F("FlashBlockData size: "));
    Serial.println(sizeof(FlashBlockData));

    Serial.print(F("FLASH_STORE_SIZE: "));
    Serial.println(FLASH_STORE_SIZE);
    Serial.print(F("Flash Start Address: "));
    Serial.println((uint32_t)&(FLASH_WritableArea[0]), HEX);
    Serial.print(F("Flash End Address: "));
    Serial.println((uint32_t)&(FLASH_WritableArea[FLASH_RESERVED_SIZE - 1]), HEX);
  #endif

    // init flash
    uint32_t start_addr = FLASH_StartAddress;
    uint32_t page_addr = FLASH_StartAddress;
    uint32_t row_addr = FLASH_StartAddress;
    uint32_t last_row_addr = FLASH_StartAddress;

    #if (IS_DEBUG_MODE == ON)
    Serial.println(F("start | page | row"));
    #endif
    for (uint32_t idx = 0; idx < NOF_BLOCKS; idx++)
    {
      #if (IS_DEBUG_MODE == ON)
        Serial.print(start_addr, HEX);
        Serial.print(" ");
        Serial.print(page_addr, HEX);
        Serial.print(" ");
        Serial.println(row_addr, HEX);
      #endif
        this->m_block_a[idx] = new FlashBlock(start_addr, page_addr, row_addr);

        start_addr += sizeof(FlashBlockData);
        page_addr = start_addr - (start_addr % Flash::PAGE_SIZE);
        row_addr = start_addr - (start_addr % Flash::ROW_SIZE);
        last_row_addr = row_addr;
    }

    // find active block
    if (FindNewestBlock() == false)
    {
        // erase all blocks if they are not erased already
        //uint32_t nof_rows = ((((uint32_t)&(FLASH_WritableArea[FLASH_RESERVED_SIZE - 1])) - ((uint32_t)&(FLASH_WritableArea[0])))) / Flash::ROW_SIZE;
        //Serial.print(F("Nof Rows: "));
        //Serial.println(nof_rows);
        row_addr = FLASH_StartAddress;
        while (row_addr < last_row_addr)
        {
            Serial.print(F("Erase Row at address: "));
            Serial.println(row_addr, HEX);

            row_addr += Flash::ROW_SIZE;
        }
    }
}


//*****************************************************************************
// description:
//   destructor
//*****************************************************************************
FlashHdl::~FlashHdl()
{
}


//*****************************************************************************
// description:
//   Read active block
//*****************************************************************************
FlashHdl::Error FlashHdl::ReadBlock(uint8_t* data, uint32_t size, uint32_t offset)
{
    Error err = Successfull;

    if (offset >= FlashBlockData::BLOCK_DATA_SIZE)
    {
        err = OffsetOutOfBoundaries;
    }
    else if ((size > FlashBlockData::BLOCK_DATA_SIZE) | ((size + offset) > FlashBlockData::BLOCK_DATA_SIZE))
    {
        err = DataToLarge;
    }
    else
    {
        Flash::Read(this->m_block_a[this->m_active_block_idx]->GetStartAddress() + offset, data, size);
    }

    return err;
}


//*****************************************************************************
// description:
//   WriteToNextBlock
// parameter:
//   data: data to write
//   size: size of data in byte --> must be smaller than BLOCK_DATA_SIZE
//   offset: address offset where to write data
//*****************************************************************************
FlashHdl::Error FlashHdl::WriteToNextBlock(uint8_t* data, uint32_t size, uint32_t offset)
{
    Error err = Successfull;

    /*if (offset >= FlashBlockData::BLOCK_DATA_SIZE)
    {
        err = OffsetOutOfBoundaries;
    }
    else if ((size > FlashBlockData::BLOCK_DATA_SIZE) | ((size + offset) > FlashBlockData::BLOCK_DATA_SIZE))
    {
        err = DataToLarge;
    }
    else
    {

    }

    return err;*/
}


//*****************************************************************************
// description:
//   FindNewestBlock
// return:
//   true if a valid block is found
//*****************************************************************************
bool FlashHdl::FindNewestBlock()
{
    uint8_t idx = 0;
    uint16_t block_cnt = 0;
    FlashBlockData block;
    bool is_valid_block_found = false;

    for (idx = 0; idx < NOF_BLOCKS; idx++)
    {
        Flash::Read(this->m_block_a[idx]->GetStartAddress(), (void*)&block, sizeof(FlashBlockData));

        if (block.valid_pattern == Parameter::PARAMETERSET_Valid)
        {
            is_valid_block_found = true;
            this->m_active_block_idx = idx;
            if (block.block_cnt > this->m_active_block_write_cnt)
            {
                this->m_active_block_write_cnt  = block.block_cnt;
            }
        }
    }

    return is_valid_block_found;
}




