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
#include "Flash.h"


//----------------------------------------------------------------------------
// Instance



//*****************************************************************************
// description:
//   constructor
//*****************************************************************************
Flash::Flash()
{
    
}


//*****************************************************************************
// description:
//   destructor
//*****************************************************************************
Flash::~Flash()
{
}


//*****************************************************************************
// description:
//   FindNewestBlock
//*****************************************************************************
static inline uint32_t Flash_ReadUnalignedUint32(const void *data)
{
    union {
        uint32_t u32;
        uint8_t u8[4];
    } res;

    const uint8_t *d = (const uint8_t *)data;
    res.u8[0] = d[0];
    res.u8[1] = d[1];
    res.u8[2] = d[2];
    res.u8[3] = d[3];

    return res.u32;
}


//*****************************************************************************
// description:
//   WritePage
//*****************************************************************************
void Flash::WritePage(const volatile uint32_t flash_addr, const void *data, uint32_t size)
{
// Überprüfen Sie, ob die angegebene Adresse innerhalb eines Seitenbereichs liegt
    if (flash_addr % PAGE_SIZE != 0) {
  #if (IS_DEBUG_MODE == ON)
        Serial.println(F("The Flash WRITE address is not at the beginning of a flash page"));
  #endif
        return;
    }

  #if (IS_DEBUG_MODE == ON)
    Serial.print(F("Write flash at address: "));
    Serial.println(flash_addr, HEX);
  #endif


    // Calculate data boundaries
    size = (size + 3) / 4;
    volatile uint32_t *dst_addr = (volatile uint32_t *)flash_addr;
    const uint8_t *src_addr = (uint8_t *)data;

    // Disable automatic page write
  #if defined(__SAMD51__)
    NVMCTRL->CTRLA.bit.WMODE = 0;
  #else
    NVMCTRL->CTRLB.bit.MANW = 1;
  #endif

    // Do writes in pages
    while (size) {
        // Execute "PBC" Page Buffer Clear
  #if defined(__SAMD51__)
        NVMCTRL->CTRLB.reg = NVMCTRL_CTRLB_CMDEX_KEY | NVMCTRL_CTRLB_CMD_PBC;
        while (NVMCTRL->INTFLAG.bit.DONE == 0) { }
  #else
        NVMCTRL->CTRLA.reg = NVMCTRL_CTRLA_CMDEX_KEY | NVMCTRL_CTRLA_CMD_PBC;
        while (NVMCTRL->INTFLAG.bit.READY == 0) { }
  #endif

        // Fill page buffer
        uint32_t i;
        for (i=0; i<(PAGE_SIZE/4) && size; i++) {
        *dst_addr = Flash_ReadUnalignedUint32(src_addr);
        src_addr += 4;
        dst_addr++;
        size--;
        }

        // Execute "WP" Write Page
  #if defined(__SAMD51__)
        NVMCTRL->CTRLB.reg = NVMCTRL_CTRLB_CMDEX_KEY | NVMCTRL_CTRLB_CMD_WP;
        while (NVMCTRL->INTFLAG.bit.DONE == 0) { }
  #else
        NVMCTRL->CTRLA.reg = NVMCTRL_CTRLA_CMDEX_KEY | NVMCTRL_CTRLA_CMD_WP;
        while (NVMCTRL->INTFLAG.bit.READY == 0) { }
  #endif
    }
}


//*****************************************************************************
// description:
//   Erase
//*****************************************************************************
void Flash::Erase(const volatile uint32_t flash_addr, uint32_t size)
{
    const uint8_t *ptr = (const uint8_t*)flash_addr;
    while (size > Flash::ROW_SIZE) 
    {
        Flash::Erase((uint32_t)ptr);
        ptr += Flash::ROW_SIZE;
        size -= Flash::ROW_SIZE;
    }
    
    Flash::Erase((uint32_t)ptr);
}


//*****************************************************************************
// description:
//   Erase
//*****************************************************************************
void Flash::Erase(const volatile uint32_t flash_addr)
{
    if (flash_addr % Flash::ROW_SIZE != 0) {
  #if (IS_DEBUG_MODE == ON)
        Serial.println(F("The Flash ERASE address is not at the beginning of a flash row"));
  #endif
        return;
    }

  #if (IS_DEBUG_MODE == ON)
    Serial.print(F("Erase flash at address: "));
    Serial.println(flash_addr, HEX);
  #endif

  #if defined(__SAMD51__)
    NVMCTRL->ADDR.reg = (flash_addr);
    NVMCTRL->CTRLB.reg = NVMCTRL_CTRLB_CMDEX_KEY | NVMCTRL_CTRLB_CMD_EB;
    while (!NVMCTRL->INTFLAG.bit.DONE) { }
  #else
    NVMCTRL->ADDR.reg = (flash_addr) / 2;
    NVMCTRL->CTRLA.reg = NVMCTRL_CTRLA_CMDEX_KEY | NVMCTRL_CTRLA_CMD_ER;
    while (!NVMCTRL->INTFLAG.bit.READY) { }
  #endif
}


//*****************************************************************************
// description:
//   Read
//*****************************************************************************
void Flash::Read(const volatile uint32_t flash_addr, void *data, uint32_t size)
{
    memcpy(data, (const void *)flash_addr, size);
}
