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

#ifndef _STORE_HDL_H
#define _STORE_HDL_H


//----------------------------------------------------------------------------
// include
#include "../common.h"
#include "Parameter.h"


//----------------------------------------------------------------------------
// class
class StoreHdl
{
    public:
        enum Error
        {
            Successfull = 0,
            DataToLarge,
            OffsetOutOfBoundaries,
            WriteVerifyFailed,
            NoValidBlock,
            DataDidNotChangeNoWriteDone,
            Nof,
        };


        virtual Error ReadBlock(uint8_t* data, uint32_t size, uint32_t offset) = 0;
        virtual Error WriteToNextBlock(uint8_t* data, uint32_t size) = 0;

    private:
        virtual bool FindNewestBlock() = 0;
};


class StoreBlockData
{
    public:
        const static uint32_t BLOCK_HEADER_SIZE = 4;
        const static uint32_t BLOCK_DATA_SIZE = Parameter::BUFFER_Size;
        const static uint16_t VALID_PATTERN = 0x55AA;

        uint16_t valid_pattern;  // when change enhance BLOCK_HEADER_SIZE
        uint16_t block_cnt;      // when change enhance BLOCK_HEADER_SIZE
        uint8_t data[BLOCK_DATA_SIZE];
};

class StoreBlockInfo
{
    public:
        StoreBlockInfo() {}
        StoreBlockInfo(uint32_t start_addr, uint32_t page_addr, uint32_t row_addr)
        {
            this->start_address = start_addr;
            this->page_address = page_addr;
            this->row_address = row_addr;
        }
        ~StoreBlockInfo() {}

        uint32_t GetStartAddress() { return start_address; }
        void SetStartAddress(uint32_t addr) { this->start_address = addr; }
        uint32_t GetPageAddress() { return page_address; }
        uint32_t GetRowAddress() { return row_address; }

    private:
        uint32_t start_address;
        uint32_t page_address;
        uint32_t row_address;
};



#endif // _STORE_HDL_H
