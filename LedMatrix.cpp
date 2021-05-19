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
    this->m_led_strip = new Adafruit_NeoPixel(300, 26, NEO_GRBW + NEO_KHZ800);
    this->m_led_strip->begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
    
    this->m_led_strip_2 = new Adafruit_NeoPixel(285, 28, NEO_GRBW + NEO_KHZ800);
    this->m_led_strip_2->begin(); // INITIALIZE NeoPixel strip object (REQUIRED)

    this->m_led_strip_3 = new Adafruit_NeoPixel(285, 24, NEO_GRBW + NEO_KHZ800);
    this->m_led_strip_3->begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
    
    this->m_led_strip_4 = new Adafruit_NeoPixel(285, 22, NEO_GRBW + NEO_KHZ800);
    this->m_led_strip_4->begin(); // INITIALIZE NeoPixel strip object (REQUIRED)


    this->Clear();
    this->SetBrightness(0);
    this->Show();
    
    this->m_led_row[0] = new LedRow(m_led_strip, 0);
    this->m_led_row[1] = new LedRow(m_led_strip_2, 1);
    this->m_led_row[2] = new LedRow(m_led_strip_3, 2);
    this->m_led_row[3] = new LedRow(m_led_strip_4, 3);
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
        m_led_row[row]->SetPixel(xs, ((xe+1)-xs), 0, 1, color);
    }
}


//*****************************************************************************
// description:
//   Clear
//*****************************************************************************
void LedMatrix::Clear(void)
{
    m_led_strip->clear();
    m_led_strip_2->clear();  
    m_led_strip_3->clear();  
    m_led_strip_4->clear();  
}


//*****************************************************************************
// description:
//   Show
//*****************************************************************************
void LedMatrix::Show(void)
{
    m_led_strip->show();
    m_led_strip_2->show();  
    m_led_strip_3->show();  
    m_led_strip_4->show();  
}


//*****************************************************************************
// description:
//   SetBrightness
//*****************************************************************************
void LedMatrix::SetBrightness(uint8_t brightness)
{
    m_led_strip->setBrightness(brightness); // Set brigthness for all neo pixels
    m_led_strip_2->setBrightness(brightness); // Set brigthness for all neo pixels    
    m_led_strip_3->setBrightness(brightness); // Set brigthness for all neo pixels    
    m_led_strip_4->setBrightness(brightness); // Set brigthness for all neo pixels    
}
