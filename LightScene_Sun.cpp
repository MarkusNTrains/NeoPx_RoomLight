/*******************************************************************************
Project   RoomLight

  This is an OpenSource Project.
  You can use, share or improve this project. If you improve this source code
  please share with the comunity or at least with the author of the original 
  source code
  
  Created 8. September 2022 by MarkusNTrains
================================================================================
$HeadURL:  $
$Id:  $
*******************************************************************************/


//-----------------------------------------------------------------------------
// includes
#include "LightScene_Sun.h"


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
LightScene_Sun::LightScene_Sun(LightSceneHdl* parent, LightHdl* light_hdl)
{
    this->m_scene_hdl_p = parent;
    this->m_light_hdl_p = light_hdl;

}


//*****************************************************************************
// description:
//   destructor
//*****************************************************************************
LightScene_Sun::~LightScene_Sun()
{
}


//*****************************************************************************
// description:
//   Enter
//*****************************************************************************
void LightScene_Sun::Day_Enter(void)
{
    this->m_light_hdl_p->SetColor(Adafruit_NeoPixel::Color(0,0,0,255));
    this->m_light_hdl_p->SetBrightness_Fade(DAY_BRIGHTNESS);
    this->m_light_hdl_p->Clear();
}


//*****************************************************************************
// description:
//   Task
//*****************************************************************************
void LightScene_Sun::Day_Task(void)
{
    this->m_light_hdl_p->SetLedArea(0, LedRow::LED_ROW_LENGTH, 0, 0);  
    this->m_light_hdl_p->SetLedArea(0, 30, 1, 2);  
    this->m_light_hdl_p->SetLedArea(LedRow::LED_ROW_LENGTH - 30, LedRow::LED_ROW_LENGTH, 1, 2);  
    this->m_light_hdl_p->SetLedArea(0, LedRow::LED_ROW_LENGTH, 3, 3);  
    this->m_light_hdl_p->Show();    
}


//*****************************************************************************
// description:
//   Enter
//*****************************************************************************
void LightScene_Sun::Night_Enter(void)
{
    this->m_light_hdl_p->SetColor(NIGHT_COLOR);
    this->m_light_hdl_p->SetBrightness_Fade(NIGHT_BRIGHTNESS);
    this->m_light_hdl_p->Clear();
}


//*****************************************************************************
// description:
//   Task
//*****************************************************************************
void LightScene_Sun::Night_Task(void)
{
    this->m_light_hdl_p->SetLedArea(0, LedRow::LED_ROW_LENGTH, 0, 0);  
    this->m_light_hdl_p->SetLedArea(0, 30, 1, 2);  
    this->m_light_hdl_p->SetLedArea(LedRow::LED_ROW_LENGTH - 30, LedRow::LED_ROW_LENGTH, 1, 2);  
    this->m_light_hdl_p->SetLedArea(0, LedRow::LED_ROW_LENGTH, 3, 3);  
    this->m_light_hdl_p->Show();    
}


//*****************************************************************************
// description:
//   Enter
//*****************************************************************************
void LightScene_Sun::Sunrise_Enter(void)
{
    this->m_sun_height = 0;
    this->m_sun_pos = 0;

    this->m_light_hdl_p->SetColor(NIGHT_COLOR);
    this->m_light_hdl_p->SetBrightness_Instantly(255);
}


//*****************************************************************************
// description:
//   Exit
//*****************************************************************************
void LightScene_Sun::Sunrise_Exit(void)
{
    this->m_scene_hdl_p->ChangeLightScene(LightScene::Day);   
}


//*****************************************************************************
// description:
//   Task
//*****************************************************************************
void LightScene_Sun::Sunrise_Task(void)
{
    uint16_t PIXEL_NOF = LedRow::LED_ROW_LENGTH;
    uint16_t pixel;
    uint16_t cnt;
    uint8_t red[PIXEL_NOF];
    uint8_t green[PIXEL_NOF];
    uint8_t blue[PIXEL_NOF];
    float brightness = 0;
    float asin_alpha = 0;
    uint32_t hypothenuse = 0;
    uint32_t color = 0;
    uint8_t tmp_color = 0;

    for (cnt = 0; cnt < PIXEL_NOF; cnt++)
    {
        hypothenuse = sqrt(pow(this->m_sun_height, 2) + pow(PIXEL_DISTANCE_MM * cnt, 2));
        asin_alpha = (255 * this->m_sun_height) / hypothenuse;
        brightness = asin_alpha / 255;

        red[cnt] = 255 * brightness;
        //red[(PIXEL_NOF - 1) - cnt] = red[cnt];

        green[cnt] = asin_alpha * brightness;
        if (green[cnt] > DAY_BRIGHTNESS) {
            green[cnt] = DAY_BRIGHTNESS;
        }
        //green[(PIXEL_NOF - 1) - cnt] = green[cnt];

        tmp_color = (green[cnt] * this->m_sun_height) / (SUN_MAX_HEIGHT * 2);
        tmp_color = (cnt * (DAY_BRIGHTNESS / (PIXEL_NOF / 2))) + tmp_color;
        if (tmp_color > DAY_BRIGHTNESS) {
            tmp_color = DAY_BRIGHTNESS;
        }
        blue[cnt] = tmp_color * brightness;
        //blue[cnt] = ((green[cnt] * m_sunrise_sun_height) / (SUN_MAX_HEIGHT * 2)) * brightness;
        //blue[(PIXEL_NOF - 1) - cnt] =  blue[cnt];

        #ifdef IS_DEBUG_MODE
        Serial.print("c ");
        Serial.print(asin_alpha);
        Serial.print("  brigth ");
        Serial.print(brightness);
        Serial.print("  green ");
        Serial.println(green[cnt]);
        #endif
    }

    for (cnt = 0; cnt < PIXEL_NOF; cnt++)
    {
        pixel = (this->m_sun_pos + cnt) % PIXEL_NOF;
        color = Adafruit_NeoPixel::Color(red[cnt], green[cnt], blue[cnt], 0);
        this->m_light_hdl_p->SetLedArea(pixel, pixel, 0, (LedRow::LED_ROW_NOF - 1), color);
    }

    // Send the updated pixel colors to the hardware.  
    this->m_light_hdl_p->Show();    

    if (this->m_sun_height < SUN_MAX_HEIGHT)
    {
        this->m_sun_height += 10;
    }
    else
    {
        this->Sunrise_Exit();     
    }
    this->m_sun_pos = this->m_sun_pos % PIXEL_NOF;
}


//*****************************************************************************
// description:
//   Enter
//*****************************************************************************
void LightScene_Sun::Sunset_Enter(void)
{
    this->m_sun_height = SUN_MAX_HEIGHT;
    this->m_sun_pos = 0;

    this->m_light_hdl_p->SetColor(DAY_COLOR);
    this->m_light_hdl_p->SetBrightness_Instantly(255);
}


//*****************************************************************************
// description:
//   Exit
//*****************************************************************************
void LightScene_Sun::Sunset_Exit(void)
{
    this->m_scene_hdl_p->ChangeLightScene(LightScene::Night);   
}


//*****************************************************************************
// description:
//   Task
//*****************************************************************************
void LightScene_Sun::Sunset_Task(void)
{
}