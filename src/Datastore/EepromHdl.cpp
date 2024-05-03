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

//-----------------------------------------------------------------------------
// includes
#include "EepromHdl.h"



//----------------------------------------------------------------------------
// Instance



//*****************************************************************************
// description:
//   constructor
//*****************************************************************************
EepromHdl::EepromHdl()
{
    // init EEPROM
    this->m_nof_blocks = EEPROM.length() / sizeof(StoreBlockData);
    this->m_active_block_idx = 0xFFFF;
    this->m_write_cnt = 0;
    this->m_block_a = new StoreBlockInfo[this->m_nof_blocks];

  #if (IS_DEBUG_MODE == ON)
    Serial.print(F("Parameter::BUFFER_Size: "));
    Serial.println(Parameter::BUFFER_Size);

    Serial.print(F("m_nof_blocks: "));
    Serial.println(m_nof_blocks);

    Serial.print(F("StoreBlockData size: "));
    Serial.println(sizeof(StoreBlockData));
  #endif

    uint32_t start_addr = EEPROM_StartAddress;

  #if (IS_DEBUG_MODE == ON)
    Serial.println(F("id | start address"));
  #endif

    for (uint32_t idx = 0; idx < this->m_nof_blocks; idx++)
    {
      #if (IS_DEBUG_MODE == ON)
        Serial.print(idx);
        Serial.print(F(" "));
        Serial.println(start_addr, HEX);
      #endif

        this->m_block_a[idx].SetStartAddress(start_addr);

        start_addr += sizeof(StoreBlockData);
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
EepromHdl::~EepromHdl()
{
    delete this->m_block_a;
}


//*****************************************************************************
// description:
//   Read active block
//*****************************************************************************
EepromHdl::Error EepromHdl::ReadBlock(uint8_t* data, uint32_t size, uint32_t offset)
{
    Error err = Successfull;

    if (this->m_active_block_idx >= this->m_nof_blocks)
    {
        err = NoValidBlock;
    }
    else if (offset >= StoreBlockData::BLOCK_DATA_SIZE)
    {
        err = OffsetOutOfBoundaries;
    }
    else if ((size > StoreBlockData::BLOCK_DATA_SIZE) | ((size + offset) > StoreBlockData::BLOCK_DATA_SIZE))
    {
        err = DataToLarge;
    }
    else
    {
        uint32_t addr = this->m_block_a[this->m_active_block_idx].GetStartAddress() + StoreBlockData::BLOCK_HEADER_SIZE + offset;
        for (uint32_t idx = 0; idx < size; idx++)
        {
            data[idx] = EEPROM.read(addr++);
        }
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
EepromHdl::Error EepromHdl::WriteToNextBlock(uint8_t* data, uint32_t size)
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

    if ((size > StoreBlockData::BLOCK_DATA_SIZE) | (size > StoreBlockData::BLOCK_DATA_SIZE))
    {
        err = DataToLarge;
    }
    else
    {
        //--------------------------------------------------------------
        // check if something did change
        uint8_t rd_block_data[sizeof(StoreBlockData)];
        if (this->m_active_block_idx < this->m_nof_blocks)
        {
            StoreBlockData* rd_block_data_p = (StoreBlockData*)rd_block_data;
            EEPROM.get(this->m_block_a[this->m_active_block_idx].GetStartAddress(), rd_block_data);
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
        if (this->m_active_block_idx >= this->m_nof_blocks)
        {
            this->m_active_block_idx = 0;
        }

        //--------------------------------------------------------------
        // increment write cnt and check if a row needs to be reseted
        this->m_write_cnt++;
        if (this->m_write_cnt > 0xFFFD)
        {
            // write cnt will overflow, therfore reset it and clear the whole writable EEPROM area
            this->m_write_cnt = 0;
            this->EraseWholeEeprom();
        }

        //--------------------------------------------------------------
        // write block
        StoreBlockData wr_data;
        wr_data.valid_pattern = StoreBlockData::VALID_PATTERN;
        wr_data.block_cnt = this->m_write_cnt;
        memcpy(wr_data.data, data, size);

        //--- write data ---
        EEPROM.put(this->m_block_a[this->m_active_block_idx].GetStartAddress(), wr_data);

        //--- verify data ---
        StoreBlockData rd_data;
        EEPROM.get(this->m_block_a[this->m_active_block_idx].GetStartAddress(), rd_data);
        if (memcmp(&wr_data, &rd_data, sizeof(StoreBlockData)) != 0)
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
    }

    return err;
}


//*****************************************************************************
// description:
//   FindNewestBlock
// return:
//   true if a valid block is found
//*****************************************************************************
bool EepromHdl::FindNewestBlock()
{
    uint32_t idx = 0;
    uint16_t block_cnt = 0;
    StoreBlockData block;
    bool is_valid_block_found = false;

    for (idx = 0; idx < this->m_nof_blocks; idx++)
    {
        EEPROM.get(this->m_block_a[idx].GetStartAddress(), block);

        if (block.valid_pattern == StoreBlockData::VALID_PATTERN)
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
//   Erase whole EEPROM
//*****************************************************************************
void EepromHdl::EraseWholeEeprom()
{
    for (uint32_t idx = 0; idx < EEPROM.length(); idx++) 
    {
        EEPROM.write(idx, CELL_ERASED_VALUE);
    }
}



#endif //__AVR__