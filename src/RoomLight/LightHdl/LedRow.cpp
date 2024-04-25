/*******************************************************************************
Project   RoomLight

  This is an OpenSource Project.
  You can use, share or improve this project. If you improve this source code
  please share with the comunity or at least with the author of the original 
  source code
  
  Created 24. August 2020 by MarkusNTrains
================================================================================
$HeadURL:  $
$Id:  $
*******************************************************************************/


//-----------------------------------------------------------------------------
// includes
#include "LedRow.h"




//*****************************************************************************
// description:
//   constructor
//*****************************************************************************
LedRow::LedRow(Adafruit_NeoPixel* led_strip_p, uint8_t row_idx)
{
    this->m_led_strip_p = led_strip_p;
    this->m_row_idx = row_idx;
}


//*****************************************************************************
// description:
//   destructor
//*****************************************************************************
LedRow::~LedRow()
{
}


//*****************************************************************************
// description:
//   Returns led idx from loock up table
//*****************************************************************************
uint16_t LedRow::GetLedIdxOfLut(uint16_t idx)
{
#ifdef __AVR__
    return pgm_read_word(&LED_MATRIX_LUT[this->m_row_idx][idx]);
#else
    return LED_MATRIX_LUT[this->m_row_idx][idx];
#endif
}


//*****************************************************************************
// description:
//   Send the updated pixel colors to the hardware.
//*****************************************************************************
void LedRow::Show()
{
    this->m_led_strip_p->show();    
}


//*****************************************************************************
// description:
//   Get Pixel Color
//*****************************************************************************
uint32_t LedRow::GetPixelColor(uint16_t idx)
{
    return this->m_led_strip_p->getPixelColor(this->GetLedIdxOfLut(idx));
}


//*****************************************************************************
// description:
//   Set Pixel
//*****************************************************************************
void LedRow::SetPixel(uint16_t idx, uint32_t color)
{
    uint16_t nof_px_to_fill = 1;
    uint16_t start_idx = this->GetLedIdxOfLut(idx);
    if ((idx + 1) < LedRow::LED_ROW_LENGTH)
    {
        nof_px_to_fill = this->GetLedIdxOfLut(idx + 1) - start_idx;
    }
    this->m_led_strip_p->fill(color, start_idx, nof_px_to_fill);
}


//*****************************************************************************
// description:
//   Show White Pixel
//*****************************************************************************
void LedRow::SetPixel(uint16_t start_idx, uint16_t width, uint16_t space, uint16_t nof_repeat, uint32_t color)
{
    uint16_t px = 0;
    uint16_t idx = 0;
    uint16_t cnt = 0;
    uint16_t end_idx = 0;
    uint16_t nof_px_to_fill = 0;
    uint16_t additional_px = 0;
    
    if (   (start_idx >= LED_ROW_LENGTH)
        || (width == 0))
    { return; }
    
    if ((start_idx + width) > LED_ROW_LENGTH)
    {
        width = LED_ROW_LENGTH - start_idx;
    }

    for (cnt = 0; cnt < nof_repeat; cnt++)
    {
        nof_px_to_fill = 0;
        end_idx = start_idx + width;
        if (end_idx >= LED_ROW_LENGTH) {
            end_idx = LED_ROW_LENGTH - 1;
            additional_px = 1;
        }
        if (this->GetLedIdxOfLut(start_idx) <= this->GetLedIdxOfLut(end_idx))
        {
            idx = this->GetLedIdxOfLut(start_idx);
            nof_px_to_fill = (this->GetLedIdxOfLut(end_idx) - idx);
        }
        else
        {
            idx = this->GetLedIdxOfLut((end_idx));
            nof_px_to_fill = (this->GetLedIdxOfLut(start_idx) - idx);  
        }

        nof_px_to_fill += additional_px;
        this->m_led_strip_p->fill(color, idx, nof_px_to_fill);
      
        start_idx += space + width;      
        // check if offset is further than the rowlength
        if (start_idx >= LED_ROW_LENGTH) { break; }
    }
}
