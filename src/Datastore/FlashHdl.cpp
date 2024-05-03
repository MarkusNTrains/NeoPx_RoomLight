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


#ifndef __AVR__

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
    this->m_active_block_idx = 0xFFFF;
    this->m_write_cnt = 0;
    this->m_last_row_addr = 0;

  #if (IS_DEBUG_MODE == ON)
    /*Serial.print(F("ROW_SIZE: "));
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
    Serial.println((uint32_t)&(FLASH_WritableArea[FLASH_RESERVED_SIZE - 1]), HEX);*/
  #endif

    // init flash
    uint32_t start_addr = FLASH_StartAddress;
    uint32_t page_addr = FLASH_StartAddress;
    uint32_t row_addr = FLASH_StartAddress;
    this->m_last_row_addr = FLASH_StartAddress;

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

        this->m_last_row_addr = row_addr;
        this->m_block_a[idx] = new FlashBlock(start_addr, page_addr, row_addr);

        start_addr += sizeof(FlashBlockData);
        page_addr = start_addr - (start_addr % Flash::PAGE_SIZE);
        row_addr = start_addr - (start_addr % Flash::ROW_SIZE);
    }

    // find active block
    if (FindNewestBlock() == false)
    {
      #if (IS_DEBUG_MODE == ON)
        Serial.println(F("No valid block found!"));
      #endif
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

    if (this->m_active_block_idx >= NOF_BLOCKS)
    {
        err = NoValidBlock;
    }
    else if (offset >= FlashBlockData::BLOCK_DATA_SIZE)
    {
        err = OffsetOutOfBoundaries;
    }
    else if ((size > FlashBlockData::BLOCK_DATA_SIZE) | ((size + offset) > FlashBlockData::BLOCK_DATA_SIZE))
    {
        err = DataToLarge;
    }
    else
    {
        Flash::Read(this->m_block_a[this->m_active_block_idx]->GetStartAddress() + FlashBlockData::BLOCK_HEADER_SIZE + offset, data, size);
    }

    return err;
}


//*****************************************************************************
// description:
//   WriteToNextBlock
// parameter:
//   data: data to write
//   size: size of data in byte --> must be smaller than BLOCK_DATA_SIZE
//*****************************************************************************
FlashHdl::Error FlashHdl::WriteToNextBlock(uint8_t* data, uint32_t size)
{
    Error err = Successfull;
    uint32_t idx = 0;

  #if (IS_DEBUG_MODE == ON)
    Serial.println(F(" "));
    Serial.println(F("*****  START WRITE  *******"));

    /*Serial.println(F("Data to write"));
    for (int cnt = 0; cnt < size; cnt++)
    {
        Serial.print(" ");
        Serial.print(data[cnt], HEX);
    }
    Serial.println(" ");*/
  #endif

    if ((size > FlashBlockData::BLOCK_DATA_SIZE) | (size > FlashBlockData::BLOCK_DATA_SIZE))
    {
        err = DataToLarge;
    }
    else
    {
        //--------------------------------------------------------------
        // check if something did change
        uint8_t rd_block_data[sizeof(FlashBlockData)];
        if (this->m_active_block_idx < NOF_BLOCKS)
        {
            FlashBlockData* rd_block_data_p = (FlashBlockData*)rd_block_data;
            Flash::Read(this->m_block_a[this->m_active_block_idx]->GetStartAddress(), rd_block_data, sizeof(FlashBlockData));
            if (memcmp(rd_block_data_p->data, data, size) == 0)
            {
              #if (IS_DEBUG_MODE == ON)
                Serial.println(F("No change in data --> Write not needed"));
              #endif
                return DataDidNotChangeNoWriteDone;
            }
        }

        //--------------------------------------------------------------
        // increment active block idx
        this->m_active_block_idx++;
        if (this->m_active_block_idx >= NOF_BLOCKS)
        {
            this->m_active_block_idx = 0;
        }

        //--------------------------------------------------------------
        // increment write cnt and check if a row needs to be reseted
        this->m_write_cnt++;
        if (this->m_write_cnt > 0xFFFD)
        {
            // write cnt will overflow, therfore reset it and clear the whole writable flash area
            this->m_write_cnt = 0;
            this->EraseWholeWritableFlash();
        }
        else
        {
            //check if row needs to be erased
            Flash::Read(this->m_block_a[this->m_active_block_idx]->GetStartAddress(), rd_block_data, sizeof(FlashBlockData));

            bool is_erase_needed = false;
            uint32_t row_addr = this->m_block_a[this->m_active_block_idx]->GetRowAddress();
            uint32_t data_addr = this->m_block_a[this->m_active_block_idx]->GetStartAddress();
            for (idx = 0; idx < (size + FlashBlockData::BLOCK_HEADER_SIZE); idx++)
            {
                if (rd_block_data[idx] != Flash::CELL_ERASED_VALUE)
                {
                    is_erase_needed = true;
                }

                // check if row will change with next data byte
                if ((row_addr + Flash::ROW_SIZE) <= (data_addr + idx+ 1))
                {
                    if (is_erase_needed == true)
                    {
                        is_erase_needed = false;
                        Flash::Erase(row_addr);
                    }
                    row_addr += Flash::ROW_SIZE;
                }
            }

            if (is_erase_needed == true)
            {
                Flash::Erase(row_addr);
            }
        }

        //--------------------------------------------------------------
        // write block
        uint32_t wr_idx = 0;
        uint32_t page_addr = this->m_block_a[this->m_active_block_idx]->GetPageAddress();
        uint32_t start_idx = this->m_block_a[this->m_active_block_idx]->GetStartAddress() - page_addr;
        uint32_t end_idx = start_idx;
        uint8_t rd_page_data[Flash::PAGE_SIZE];
        uint8_t wr_page_data[Flash::PAGE_SIZE];
        uint8_t wr_data[FlashBlockData::BLOCK_HEADER_SIZE + FlashBlockData::BLOCK_DATA_SIZE];
        FlashBlockData* wr_data_p = (FlashBlockData*)wr_data;
        wr_data_p->valid_pattern = FlashBlockData::VALID_PATTERN;
        wr_data_p->block_cnt = this->m_write_cnt;
        memcpy(wr_data_p->data, data, size);

        while (wr_idx < (size + FlashBlockData::BLOCK_HEADER_SIZE))
        {
            //--- prepare write data ---
            Flash::Read(page_addr, &wr_page_data, Flash::PAGE_SIZE);

            end_idx = start_idx + ((size + FlashBlockData::BLOCK_HEADER_SIZE) - wr_idx);
            if (end_idx > Flash::PAGE_SIZE)
            {
                end_idx = Flash::PAGE_SIZE;
            }

            for (idx = start_idx; idx < end_idx; idx++)
            {
                wr_page_data[idx] = wr_data[wr_idx];
                wr_idx++;
            }

            //--- write page ---
            Flash::WritePage(page_addr, wr_page_data, Flash::PAGE_SIZE);

            //--- verify page ---
            Flash::Read(page_addr, rd_page_data, Flash::PAGE_SIZE);
            if (memcmp(wr_page_data, rd_page_data, Flash::PAGE_SIZE) != 0)
            {
                err = WriteVerifyFailed;
              #if (IS_DEBUG_MODE == ON)
                Serial.println(F("Verify failed"));
              #endif
            }
            else
            {
              #if (IS_DEBUG_MODE == ON)
                Serial.println(F("Verify successfull"));
              #endif
            }

            start_idx = 0;
            page_addr += Flash::PAGE_SIZE;
        }
    }

    return err;
}


//*****************************************************************************
// description:
//   FindNewestBlock
// return:
//   true if a valid block is found
//*****************************************************************************
bool FlashHdl::FindNewestBlock()
{
    uint32_t idx = 0;
    uint16_t block_cnt = 0;
    FlashBlockData block;
    bool is_valid_block_found = false;

    for (idx = 0; idx < NOF_BLOCKS; idx++)
    {
        Flash::Read(this->m_block_a[idx]->GetStartAddress(), (void*)&block, sizeof(FlashBlockData));

        if (block.valid_pattern == FlashBlockData::VALID_PATTERN)
        {
            is_valid_block_found = true;
            if (block.block_cnt > this->m_write_cnt)
            {
              #if (IS_DEBUG_MODE == ON)
                Serial.print(F("Newest block is: "));
                Serial.println(block.block_cnt, HEX);
              #endif
                this->m_active_block_idx = idx;
                this->m_write_cnt = block.block_cnt;
            }
        }
    }

    return is_valid_block_found;
}


//*****************************************************************************
// description:
//   Erase whole writable flash (FLASH_WritableArea[])
//*****************************************************************************
void FlashHdl::EraseWholeWritableFlash()
{
    uint32_t row_addr = FLASH_StartAddress;
    while (row_addr <= this->m_last_row_addr)
    {
        this->EraseRow(row_addr);
        row_addr += Flash::ROW_SIZE;
    }
}


//*****************************************************************************
// description:
//   Erase a row
//*****************************************************************************
void FlashHdl::EraseRow(uint32_t row_addr)
{
    if ((row_addr < FLASH_StartAddress) || (row_addr > this->m_last_row_addr))
    {
  #if (IS_DEBUG_MODE == ON)
        Serial.print(F("Erase not allowed on address: "));
        Serial.println(row_addr, HEX);
  #endif
        return;
    }


    // check if row needs to be erased
    bool is_erase_needed;
    uint8_t data[Flash::ROW_SIZE];

    Flash::Read(row_addr, data, Flash::ROW_SIZE);
    for (uint32_t idx = 0; idx < Flash::ROW_SIZE; idx++)
    {
        if (data[idx] != Flash::CELL_ERASED_VALUE)
        {
            is_erase_needed = true;
            break;
        }
    }
    
    // if yes, erase block
    if (is_erase_needed == true)
    {
        Flash::Erase(row_addr);
    }
}


#endif //__AVR__