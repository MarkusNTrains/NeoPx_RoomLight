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


//----------------------------------------------------------------------------
// const
const uint8_t FLASH_BLOCK[1024] = {0xFF};


//----------------------------------------------------------------------------
// class
class FlashHdl
{
    public:
        FlashHdl();
        ~FlashHdl();

    private:
        //ParameterBlock* m_param_block_p;
        uint16_t m_active_block;
        uint16_t m_nof_blocks;

        void FindNewestBlock();
        void ReadBlock();
        void WriteToNextBlock();
};

#endif // _FLASH_HDL_H
