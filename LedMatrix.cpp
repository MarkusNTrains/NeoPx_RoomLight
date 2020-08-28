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
#include "LedMatrix.h"


//-----------------------------------------------------------------------------
// typedef


//-----------------------------------------------------------------------------
// define


//-----------------------------------------------------------------------------
// static module variable



//*****************************************************************************
// description:
//   constructor
//*****************************************************************************
LedMatrix::LedMatrix()
{
    m_led_strip = new Adafruit_NeoPixel(900, 22, NEO_GRBW + NEO_KHZ800);
    m_led_strip->begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
    m_led_strip->clear();
    m_led_strip->setBrightness(255); // Set brigthness for all neo pixels
    m_led_strip->show();            // Turn OFF all pixels ASAP
    
    m_led_strip_2 = new Adafruit_NeoPixel(284, 24, NEO_GRBW + NEO_KHZ800);
    m_led_strip_2->begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
    m_led_strip_2->clear();
    m_led_strip_2->setBrightness(255); // Set brigthness for all neo pixels
    m_led_strip_2->show();            // Turn OFF all pixels ASAP
    
    m_led_row[0] = new LedRow(m_led_strip, 0);
    m_led_row[1] = new LedRow(m_led_strip, 1);
    m_led_row[2] = new LedRow(m_led_strip, 2);
    m_led_row[3] = new LedRow(m_led_strip_2, 3);
}


//*****************************************************************************
// description:
//   destructor
//*****************************************************************************
LedMatrix::~LedMatrix()
{
}


//*****************************************************************************
// description:
//   SetPixel
//*****************************************************************************
void LedMatrix::SetPixel(uint16_t row, uint16_t col, uint32_t color)
{
    this->Clear();
    this->m_led_row[row]->SetPixel(col, color);
    this->Show();
}


//*****************************************************************************
// description:
//   SetPixelArray
// parameter:
//   xs: 0 - (LED_MATRIX_NOF_COL-1) x_start
//   xe: 0 - (LED_MATRIX_NOF_COL-1) x_end
//   ys: 0 - (LED_MATRIX_NOF_ROW-1) y_start
//   ye: 0 - (LED_MATRIX_NOF_ROW-1) y_end
//*****************************************************************************
void LedMatrix::SetPixelArray(uint16_t xs, uint16_t xe, uint8_t ys, uint8_t ye, uint32_t color)
{
    uint16_t row = 0;

    this->Clear();
    for (row = ys; row <= ye; row++)
    {
        m_led_row[row]->SetPixel(xs, xe-xs, 0, 1, color);
    }
    this->Show();
}


//*****************************************************************************
// description:
//   Clear
//*****************************************************************************
void LedMatrix::Clear(void)
{
    m_led_strip->clear();
    m_led_strip_2->clear();  
}


//*****************************************************************************
// description:
//   Show
//*****************************************************************************
void LedMatrix::Show(void)
{
    m_led_strip->show();
    m_led_strip_2->show();  
}
