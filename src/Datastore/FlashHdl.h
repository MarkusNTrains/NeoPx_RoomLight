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

#ifndef _FLASH_HDL_H
#define _FLASH_HDL_H


//----------------------------------------------------------------------------
// include
#include "../common.h"
#include "Flash.h"
#include "Parameter.h"


//----------------------------------------------------------------------------
// class
class FlashBlock
{
    public:
        FlashBlock(uint32_t start_addr, uint32_t page_addr, uint32_t row_addr)
        {
            this->start_address = start_addr;
            this->page_address = page_addr;
            this->row_address = row_addr;
        }
        ~FlashBlock() {}

        uint32_t GetStartAddress() { return start_address; }
        uint32_t GetPageAddress() { return page_address; }
        uint32_t GetRowAddress() { return row_address; }

    private:
        uint32_t start_address;
        uint32_t page_address;
        uint32_t row_address;
};


class FlashBlockData
{
    public:
        const static uint32_t BLOCK_HEADER_SIZE = 4;
        const static uint32_t BLOCK_DATA_SIZE = (((Parameter::BUFFER_Size + BLOCK_HEADER_SIZE + 3) / 4) * 4) - BLOCK_HEADER_SIZE;  // must be a multiple of 4
        const static uint16_t VALID_PATTERN = 0x55AA;

        uint16_t valid_pattern;  // when change enhance BLOCK_HEADER_SIZE
        uint16_t block_cnt;      // when change enhance BLOCK_HEADER_SIZE
        uint8_t data[BLOCK_DATA_SIZE];
};


class FlashHdl
{
    public:
        enum Error
        {
            Successfull = 0,
            DataToLarge,
            OffsetOutOfBoundaries,
            WriteVerifyFailed,
            NoValidBlock,
            Nof,
        };

        const static uint32_t NOF_BLOCKS = 10;
        const static uint32_t FLASH_STORE_SIZE = (NOF_BLOCKS * sizeof(FlashBlockData));
        const static uint32_t FLASH_RESERVED_SIZE = (FLASH_STORE_SIZE + (2 * Flash::ROW_SIZE));


        FlashHdl();
        ~FlashHdl();

        Error ReadBlock(uint8_t* data, uint32_t size, uint32_t offset);
        Error WriteToNextBlock(uint8_t* data, uint32_t size);

    private:
        uint16_t m_active_block_idx;
        uint16_t m_write_cnt;
        uint32_t m_last_row_addr;
        FlashBlock* m_block_a[NOF_BLOCKS];

        bool FindNewestBlock();
        void EraseWholeWritableFlash();
        void EraseRow(uint32_t row_addr);
};


const static uint8_t FLASH_WritableArea[FlashHdl::FLASH_RESERVED_SIZE] = { 0xFF };
const static uint32_t FLASH_StartAddress = ((((((uint32_t)&FLASH_WritableArea) - 1) / Flash::ROW_SIZE) * Flash::ROW_SIZE) + Flash::ROW_SIZE);


#endif // _FLASH_HDL_H
