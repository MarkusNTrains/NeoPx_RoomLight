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
    uint8_t idx = 0;
    
#if (ROOM_LIGHT == ROOM_LIGHT_MARKUSNTRAINS)
    this->m_led_strip[0] = new Adafruit_NeoPixel(300, 26, NEO_GRBW + NEO_KHZ800);
    this->m_led_strip[1] = new Adafruit_NeoPixel(285, 28, NEO_GRBW + NEO_KHZ800);
    this->m_led_strip[2] = new Adafruit_NeoPixel(285, 24, NEO_GRBW + NEO_KHZ800);
    this->m_led_strip[3] = new Adafruit_NeoPixel(285, 22, NEO_GRBW + NEO_KHZ800);
    for (idx = 0; idx < LED_STRIP_NOF; idx++)
    {
        this->m_led_strip[idx]->begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
    }

    this->Clear();
    this->SetBrightness(0);
    this->Show();
    
    this->m_led_row[0] = new LedRow(m_led_strip[0], 0);
    this->m_led_row[1] = new LedRow(m_led_strip[1], 1);
    this->m_led_row[2] = new LedRow(m_led_strip[2], 2);
    this->m_led_row[3] = new LedRow(m_led_strip[3], 3);
#else
    this->m_led_strip[0] = new Adafruit_NeoPixel(66, 22, NEO_GRBW + NEO_KHZ800);
    this->m_led_strip[1] = new Adafruit_NeoPixel(240, 24, NEO_GRBW + NEO_KHZ800);
    this->m_led_strip[2] = new Adafruit_NeoPixel(240, 26, NEO_GRBW + NEO_KHZ800);
    this->m_led_strip[3] = new Adafruit_NeoPixel(240, 28, NEO_GRBW + NEO_KHZ800);
    this->m_led_strip[4] = new Adafruit_NeoPixel(240, 30, NEO_GRBW + NEO_KHZ800);
    this->m_led_strip[5] = new Adafruit_NeoPixel(66, 32, NEO_GRBW + NEO_KHZ800);
    for (idx = 0; idx < LED_STRIP_NOF; idx++)
    {
        this->m_led_strip[idx]->begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
    }

    this->Clear();
    this->SetBrightness(0);
    this->Show();
    
    this->m_led_row[0] = new LedRow(m_led_strip[0], 0);
    this->m_led_row[1] = new LedRow(m_led_strip[1], 1);
    this->m_led_row[2] = new LedRow(m_led_strip[2], 2);
    this->m_led_row[3] = new LedRow(m_led_strip[3], 3);
    this->m_led_row[4] = new LedRow(m_led_strip[4], 4);
    this->m_led_row[5] = new LedRow(m_led_strip[5], 5);
#endif
}


//*****************************************************************************
// description:
//   destructor
//*****************************************************************************
LedMatrix::~LedMatrix()
{
    uint8_t idx = 0;
    
    for (idx = 0; idx < LED_STRIP_NOF; idx++)
    {
        delete this->m_led_strip[idx];
    }

    for (idx = 0; idx < LedRow::LED_ROW_NOF; idx++)
    {
        delete this->m_led_row[idx];
    }
}


//*****************************************************************************
// description:
//   GetPixelColor
// return:
//   color WRGB
//*****************************************************************************
uint32_t LedMatrix::GetPixelColor(uint16_t row, uint16_t column)
{
    if ((row < LedRow::LED_ROW_NOF) && (column < LedRow::LED_ROW_LENGTH)) 
    {
        return this->m_led_row[row]->GetPixelColor(column);
    }

    return 0;
}


//*****************************************************************************
// description:
//   SetPixel
//*****************************************************************************
void LedMatrix::SetPixel(uint16_t row, uint16_t column, uint32_t color)
{
    if ((row < LedRow::LED_ROW_NOF) && (column < LedRow::LED_ROW_LENGTH)) 
    {
        this->m_led_row[row]->SetPixel(column, color);
    }
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
void LedMatrix::SetPixelArray(uint16_t xs, uint16_t xe, uint16_t ys, uint16_t ye, uint32_t color)
{
    uint16_t row = 0;

    if (xs >= LedRow::LED_ROW_LENGTH)
    {
        xs = LedRow::LED_ROW_LENGTH - 1;
    }

    if (xe >= LedRow::LED_ROW_LENGTH)
    {
        xe = LedRow::LED_ROW_LENGTH - 1;
    }

    if (ys >= LedRow::LED_ROW_NOF)
    {
        ys = LedRow::LED_ROW_NOF - 1;
    }

    if (ye >= LedRow::LED_ROW_NOF)
    {
        ye = LedRow::LED_ROW_NOF - 1;
    }

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
    uint8_t idx = 0;
    
    for (idx = 0; idx < LED_STRIP_NOF; idx++)
    {
        m_led_strip[idx]->clear();
    }
}


//*****************************************************************************
// description:
//   Show
//*****************************************************************************
void LedMatrix::Show(void)
{
    uint8_t idx = 0;

    for (idx = 0; idx < LED_STRIP_NOF; idx++)
    {
        m_led_strip[idx]->show();
    }
}


//*****************************************************************************
// description:
//   SetBrightness
//*****************************************************************************
void LedMatrix::SetBrightness(uint8_t brightness)
{
    uint8_t idx = 0;

    for (idx = 0; idx < LED_STRIP_NOF; idx++)
    {
        m_led_strip[idx]->setBrightness(brightness); // Set brigthness for all neo pixels
    }
}


//*****************************************************************************
// description:
//   SetColor
//*****************************************************************************
void LedMatrix::SetColor(uint32_t color)
{
    uint16_t col;
    uint8_t row;

    for (row = 0; row < LedRow::LED_ROW_NOF; row++)
    {
        for (col = 0; col < LedRow::LED_ROW_LENGTH; col++)
        {
            if (this->m_led_row[row]->GetPixelColor(col) > 0)
            {
                this->m_led_row[row]->SetPixel(col, color);
            }
        }
    }
}
