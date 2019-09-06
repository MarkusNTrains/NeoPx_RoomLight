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
//static Adafruit_NeoPixel m_pixel(16, 6, NEO_GRB + NEO_KHZ800);



//*****************************************************************************
// description:
//   constructor
//*****************************************************************************
LedStrip::LedStrip(uint8_t px_pin, uint16_t nof_px, uint8_t nof_row)
{
  m_nof_px = nof_px;
  m_nof_row = nof_row;
  m_state = OFFICE_TABLE_WW;
  m_current_brightness = 0;
  m_desired_brightness = 20;
  m_update_time_ms = millis();
  
  //Adafruit_NeoPixel pixels(num_of_neo_px, neo_px_pin, NEO_GRB + NEO_KHZ800);
  m_pixel = new Adafruit_NeoPixel(nof_px, px_pin, NEO_GRBW + NEO_KHZ800);
  m_pixel->begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  m_pixel->clear();
  m_pixel->setBrightness(255); // Set brigthness for all neo pixels
  m_pixel->show();            // Turn OFF all pixels ASAP
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
//   ChangeLightScene
//*****************************************************************************
void LedStrip::ChangeLightScene(light_scene_t scene)
{
  m_state = scene;
  
  switch (scene)
  {
    case OFFICE_TABLE_WW:
      ShowOfficeTableWW_Enter(10);
      break;

    case LIGHT_ON_WW:
      LightOnWW_Enter(20);
      break;

    case SUNRISE:
      m_pixel->setBrightness(50); // Set brigthness for all neo pixels
      break;
      
    case POWER_OFF:
      break;

    case DISCO:
    case MOVING_DOT:
      this->m_state = MOVING_DOT;
      break;
      
    default:
      break;
  }
}


//*****************************************************************************
// description:
//   Statemachine
//*****************************************************************************
void LedStrip::Tasks()
{
  if (millis() > this->m_update_time_ms + TMO_TILL_NEXT_UPDATE_MS)
  {
    this->m_update_time_ms = millis();
    
    switch (this->m_state)
    {
      case OFFICE_TABLE_WW:
        ShowOfficeTableWW_Task();
        break;

      case LIGHT_ON_WW:
        LightOnWW_Task();
        break;
        
      case SUNRISE:
        Sunrise_Task();
        break;
        
      case POWER_OFF:
        PowerOff_Task();
        break;
        
      case DISCO:
      case MOVING_DOT:
        MovingDot_Task();
        break;
        
      case IDLE:
        break;
        
      default:
        break;
    }
  }
}


//*****************************************************************************
// description:
//   Show White Pixel
//*****************************************************************************
void LedStrip::ShowOfficeTableWW_Enter(uint16_t brightness)
{
  this->m_desired_brightness = brightness;
}


//*****************************************************************************
// description:
//   Show White Pixel
//*****************************************************************************
void LedStrip::ShowOfficeTableWW_Task(void)
{
  this->m_current_brightness++;  
  this->m_pixel->setBrightness(this->m_current_brightness); // Set brigthness for all neo pixels
  uint32_t color = this->m_pixel->Color(0, 0, 0, 255);
  this->SetPixel(3, 10, 5, 5, color);
  
  if (this->m_current_brightness >= this->m_desired_brightness)
  {
    this->m_state = IDLE;
  }
}


//*****************************************************************************
// description:
//   Show White Pixel
//*****************************************************************************
void LedStrip::LightOnWW_Enter(uint16_t brightness)
{
  this->m_desired_brightness = brightness;
}


//*****************************************************************************
// description:
//   Show White Pixel
//*****************************************************************************
void LedStrip::LightOnWW_Task(void)
{
  this->m_current_brightness++;  
  this->m_pixel->setBrightness(this->m_current_brightness); // Set brigthness for all neo pixels

  uint32_t color = this->m_pixel->Color(0, 0, 0, 255);
  this->SetPixel(0, m_nof_px, 0, 1, color);
  
  if (this->m_current_brightness >= this->m_desired_brightness)
  {
    this->m_state = IDLE;
  }
}


//*****************************************************************************
// description:
//   Sunrise_Task
//*****************************************************************************
#define PIXEL_DISTANCE_MM 30  // distance between neo pixels in mm
#define PIXEL_BRIGHTNESS 100
#define SUN_MAX_HEIGHT 1000
static int32_t s_sun_height = 0;
static uint32_t s_sun_pos = 0;
void LedStrip::Sunrise_Task(void)
{
  uint16_t pixel;
  uint16_t cnt;
  uint8_t red[m_nof_px];
  uint8_t green[m_nof_px];
  uint8_t blue[m_nof_px];
  float brightness = 0;
  float asin_alpha = 0;
  uint32_t hypothenuse = 0;

  for (cnt = 0; cnt < ((m_nof_px + 1) / 2); cnt++)
  {
    hypothenuse = sqrt(pow(s_sun_height, 2) + pow(PIXEL_DISTANCE_MM * cnt, 2));
    asin_alpha = (255 * s_sun_height) / hypothenuse;
    brightness = asin_alpha / 255;
    
    red[cnt] = 255 * brightness;
    red[(m_nof_px - 1) - cnt] = red[cnt];
    
    green[cnt] = asin_alpha * brightness;
    green[(m_nof_px - 1) - cnt] = green[cnt];
    
    blue[cnt] = ((green[cnt] * s_sun_height) / (SUN_MAX_HEIGHT * 2)) * brightness;
    blue[(m_nof_px - 1) - cnt] =  blue[cnt];
    
#ifdef IS_DEBUG_MODE
    Serial.print("c ");
    Serial.print(hypothenuse);
    Serial.print("  brigth ");
    Serial.print(brightness);
    Serial.print("  green ");
    Serial.println(green[cnt]);
#endif
  }
  
  for (cnt = 0; cnt < m_nof_px; cnt++)
  {
    pixel = (s_sun_pos + cnt) % m_nof_px;
    m_pixel->setPixelColor(pixel, m_pixel->Color(red[cnt], green[cnt], blue[cnt]));
  }

  m_pixel->show();   // Send the updated pixel colors to the hardware.  

  if (s_sun_height < SUN_MAX_HEIGHT)
  {
    s_sun_height++;
  }
  s_sun_pos = s_sun_pos % m_nof_px;

}


//*****************************************************************************
// description:
//   MovingDot_Task
//*****************************************************************************
void LedStrip::MovingDot_Task(void)
{
  m_pixel->clear();
  m_pixel->setPixelColor(m_px, m_pixel->Color(0, 0, 0, 255));
  m_pixel->show();
  m_px++;
  if (m_px >= m_nof_px)
  {
    m_px = 0;
  }
}


//*****************************************************************************
// description:
//   Power Off
//*****************************************************************************
void LedStrip::PowerOff_Task(void)
{
  if (this->m_current_brightness == 0)
  {
    this->m_pixel->setBrightness(20); // Set brigthness for all neo pixels
    this->m_state = IDLE;
  }
  else
  {
    this->m_current_brightness--;
    this->m_pixel->setBrightness(this->m_current_brightness); // Set brigthness for all neo pixels
    this->m_pixel->show();
  }
}


//*****************************************************************************
// description:
//   Show White Pixel
//*****************************************************************************
void LedStrip::SetPixel(uint16_t start_pos, uint16_t width, uint16_t space, uint16_t nof_repeat, uint32_t color)
{
  uint16_t px = 0;
  uint16_t idx = 0;
  uint16_t row = 0;
  uint16_t cnt = 0;
  uint16_t offset = 0;
  uint16_t row_length = m_nof_px / m_nof_row;
  
  this->m_pixel->clear();

  if (start_pos >= row_length)
  { return; }
  
  if ((start_pos + width) > row_length)
  {
    width = row_length - start_pos;
  }

  for (row = 0; row < m_nof_row; row++)
  {
    offset = start_pos;
    for (cnt = 0; cnt < nof_repeat; cnt++)
    {
      for (px = 0; px < width; px++)
      {
        // check if position is further than the rowlength
        if ((offset + px) >= row_length) { break; } 
        
        if (row % 2)
        {
          idx = ((row + 1) * row_length) - 1 - offset - px;  // this is required if the pixel signal is zigzagged through the LED strips        
        }
        else
        {
          idx = (row * row_length) + offset + px;
        }
        m_pixel->setPixelColor(idx, color);    
      }
      
      offset += space + width;      
      // check if offset is further than the rowlength
      if (offset >= row_length) { break; }
    }
  }
  
  m_pixel->show();   // Send the updated pixel colors to the hardware.
}
