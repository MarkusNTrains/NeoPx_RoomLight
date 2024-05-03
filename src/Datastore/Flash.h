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
#ifndef _FLASH_H
#define _FLASH_H


//----------------------------------------------------------------------------
// include
#include "../common.h"



//----------------------------------------------------------------------------
// class
class Flash
{
    public:
        const static uint8_t CELL_ERASED_VALUE = 0xFF;
        const static uint32_t PAGE_SIZE = FLASH_PAGE_SIZE;
    #if defined(__SAMD51__)
        #define NOF_PAGES (NVMCTRL->PARAM.bit.NVMP)
        #define MAX_SIZE (PAGE_SIZE * NOF_PAGES),
        #define ROW_SIZE (MAX_SIZE / 64)
    #else
        const static uint32_t ROW_SIZE = (PAGE_SIZE * 4);
    #endif

        Flash();
        ~Flash();

        static void WritePage(const volatile uint32_t flash_addr, const void *data, uint32_t size);
        static void Erase(const volatile uint32_t flash_addr, uint32_t size);
        static void Read(const volatile uint32_t flash_addr, void *data, uint32_t size);
        static void Erase(const volatile uint32_t flash_addr);

    private:
};



#endif // _FLASH_H
#endif