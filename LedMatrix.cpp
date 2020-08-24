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
LedMatrix::LedMatrix(void)
{
  m_led_strip = new Adafruit_NeoPixel(900, 22, NEO_GRBW + NEO_KHZ800);
  m_led_strip->begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  m_led_strip->clear();
  m_led_strip->setBrightness(255); // Set brigthness for all neo pixels
  m_led_strip->show();            // Turn OFF all pixels ASAP

  m_led_strip_sbh = new Adafruit_NeoPixel(300, 24, NEO_GRBW + NEO_KHZ800);
  m_led_strip_sbh->begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  m_led_strip_sbh->clear();
  m_led_strip_sbh->setBrightness(255); // Set brigthness for all neo pixels
  m_led_strip_sbh->show();            // Turn OFF all pixels ASAP

  m_led_row[0] = new LedRow(m_led_strip, &m_led_matrix[0][0], 600, 900);
  m_led_row[1] = new LedRow(m_led_strip, &m_led_matrix[1][0], 599, 300);
  m_led_row[2] = new LedRow(m_led_strip, &m_led_matrix[2][0], 0, 299);
  m_led_row[3] = new LedRow(m_led_strip_sbh, &m_led_matrix[3][0], 0, 299);
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
  m_led_row[row]->SetPixel(col, color);
}


//*****************************************************************************
// description:
//   Clear
//*****************************************************************************
void LedMatrix::Clear(void)
{
  m_led_strip->clear();
  m_led_strip_sbh->clear();  
}
