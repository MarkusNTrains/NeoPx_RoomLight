/*******************************************************************************
Project   RoomLight

  This is an OpenSource Project.
  You can use, share or improve this project. If you improve this source code
  please share with the comunity or at least with the author of the original 
  source code
  
  Created 04. September 2019 by MarkusNTrains
================================================================================
$HeadURL:  $
$Id:  $
*******************************************************************************/


//-----------------------------------------------------------------------------
// includes
#include "LedStrip.h"


//-----------------------------------------------------------------------------
// static module variable



//*****************************************************************************
// description:
//   constructor
//*****************************************************************************
LedStrip::LedStrip(uint8_t px_pin, uint16_t nof_px, uint8_t nof_row)
{
  m_nof_px = nof_px;
  m_nof_row = nof_row;
  
  //Adafruit_NeoPixel pixels(num_of_neo_px, neo_px_pin, NEO_GRB + NEO_KHZ800);
  m_pixel = new Adafruit_NeoPixel(nof_px, px_pin, NEO_GRBW + NEO_KHZ800);
  m_pixel.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
}


//*****************************************************************************
// description:
//   destructor
//*****************************************************************************
LedStrip::~LedStrip()
{

}


//*****************************************************************************
// description:
//   Statemachine
//*****************************************************************************
void LedStrip::Tasks()
{
}


//*****************************************************************************
// description:
//   Show White Pixel
//*****************************************************************************
void LedStrip::ShowWhite(uint16_t start_pos, uint16_t width, uint8_t brightness)
{
  uint16_t px = 0;
  uint16_t idx = 0;
  uint8_t row = 0;
  uint16_t row_length = m_nof_px / m_nof_row;
  
  m_pixel.clear();

  if (start_pos >= row_length)
  { return; }
  
  if ((start_pos + width) > row_length)
  {
    width = row_length - start_pos;
  }

  for (row = 0; row < m_nof_row; row+2)
  {
    for (px = 0; px < width; px++)
    {
      if (row % 2)
      {
        idx = (row * row_length) + start_pos + px;  // this is required if the pixel signal is zigzagged through the LED strips        
      }
      else
      {
        idx = (row * row_length) + start_pos + px;
      }
      m_pixel.setPixelColor(idx, m_pixel.Color(0, 0, 0, brightness));    
    }
  }
  
  m_pixel.show();   // Send the updated pixel colors to the hardware.
}
