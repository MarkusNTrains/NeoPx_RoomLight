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


//-----------------------------------------------------------------------------
// typedef



//-----------------------------------------------------------------------------
// static module variable



//*****************************************************************************
// description:
//   constructor
//*****************************************************************************
LedRow::LedRow(Adafruit_NeoPixel* led_strip_p, uint16_t* lookup_table_p, uint16_t row_length)
{
    this->m_led_strip_p = led_strip_p;
    this->m_lookup_table_p = lookup_table_p;
    this->m_length = row_length;
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
//   Send the updated pixel colors to the hardware.
//*****************************************************************************
void LedRow::Show(void)
{
    this->m_led_strip_p->show();    
}


//*****************************************************************************
// description:
//   Set Pixel
//*****************************************************************************
void LedRow::SetPixel(uint16_t idx, uint32_t color)
{
    this->m_led_strip_p->setPixelColor(this->m_lookup_table_p[idx], color);
        Serial.println(idx);
        Serial.println(this->m_lookup_table_p[idx]);
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
    uint16_t offset = 0;
    uint16_t nof_px_to_fill = 0;
    
    if (start_idx >= this->m_length)
    { return; }
    
    if ((start_idx + width) > this->m_length)
    {
        width = this->m_length - start_idx;
    }
    
    offset = start_idx;
    for (cnt = 0; cnt < nof_repeat; cnt++)
    {
        /*for (px = 0; px < width; px++)
        {
            // check if position is further than the rowlength
            if ((offset + px) >= this->m_length) { break; }
            
            idx = offset + px;
        Serial.println(idx);
            this->m_led_strip_p->setPixelColor(this->m_lookup_table_p[idx], color);    
        }*/
        if (this->m_lookup_table_p[offset] <= this->m_lookup_table_p[(offset + width)])
        {
            idx = this->m_lookup_table_p[offset];
            nof_px_to_fill = (this->m_lookup_table_p[(offset + width)] - idx);
        }
        else
        {
            idx = this->m_lookup_table_p[(offset + width)];
            nof_px_to_fill = (this->m_lookup_table_p[(offset)] - idx);            
        }
        this->m_led_strip_p->fill(color, idx, nof_px_to_fill);
        Serial.println(nof_px_to_fill);
        Serial.println(offset);
        Serial.println(m_lookup_table_p[offset]);
        //Serial.println(idx);
        //Serial.println((this->m_lookup_table_p[offset + (width)] - idx));
        Serial.println("end");
      
        offset += space + width;      
        // check if offset is further than the rowlength
        if (offset >= this->m_length) { break; }
    }
}
