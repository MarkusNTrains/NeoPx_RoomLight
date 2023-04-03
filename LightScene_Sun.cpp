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
    this->m_light_hdl_p->Clear();
    this->m_light_hdl_p->SetColor(DAY_COLOR);
    this->m_light_hdl_p->SetBrightness_Fade(DAY_BRIGHTNESS_WHITE);
}


//*****************************************************************************
// description:
//   Task
//*****************************************************************************
void LightScene_Sun::Day_Task(void)
{
    this->m_light_hdl_p->SetLedArea(0, LedRow::LED_ROW_LENGTH, 0, LedRow::LED_ROW_NOF);  
    this->m_light_hdl_p->Show();    
}


//*****************************************************************************
// description:
//   Enter
//*****************************************************************************
void LightScene_Sun::Night_Enter(void)
{
    this->m_light_hdl_p->Clear();
    this->m_light_hdl_p->SetColor(NIGHT_COLOR);
    this->m_light_hdl_p->SetBrightness_Fade(NIGHT_BRIGHTNESS);
}


//*****************************************************************************
// description:
//   Task
//*****************************************************************************
void LightScene_Sun::Night_Task(void)
{
    this->m_light_hdl_p->SetLedArea(0, LedRow::LED_ROW_LENGTH, 0, LedRow::LED_ROW_NOF);  
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
    this->m_twilight_brightness = NIGHT_BRIGHTNESS;
    this->m_state = LIGHTSCENESUN_STATE_Sunrise;

    this->m_light_hdl_p->SetBrightness_Instantly(255);
    this->m_light_hdl_p->SetColor(Adafruit_NeoPixel::Color(0, 0, NIGHT_BRIGHTNESS, 0));
    this->m_light_hdl_p->SetLedArea(0, LedRow::LED_ROW_LENGTH, 0, LedRow::LED_ROW_NOF);  
    this->m_light_hdl_p->Show();
}


//*****************************************************************************
// description:
//   Exit
//*****************************************************************************
void LightScene_Sun::Sunrise_Exit(void)
{
    this->m_light_hdl_p->SetBrightness_Instantly(DAY_BRIGHTNESS_WHITE);
    this->m_light_hdl_p->SetColor(DAY_COLOR);
    this->m_scene_hdl_p->ChangeLightScene(LightScene::Day);   
}


//*****************************************************************************
// description:
//   Task
//*****************************************************************************
void LightScene_Sun::Sunrise_Task(void)
{
    switch (this->m_state)
    {
        case LIGHTSCENESUN_STATE_Sunrise:
        {
            this->CalculateAndShow_Sunlight();

            if (this->m_twilight_brightness < SUNRISE_BIRGHTNESS)
            {
                this->m_twilight_brightness++;
            }
            else if (this->m_sun_height < SUN_MAX_HEIGHT)
            {
                this->m_sun_height += (this->m_sun_height / 10) + 1;
                this->m_sun_pos = this->m_sun_pos % LedRow::LED_ROW_LENGTH;
            }
            else
            {
                this->m_day_color = Adafruit_NeoPixel::Color(RED_MAX, GREEN_MAX, BLUE_MAX, 0);
                this->m_state = LIGHTSCENESUN_STATE_Fading;   
            }

            break;
        }
        case LIGHTSCENESUN_STATE_Fading:
        {
            if (this->m_day_color == Adafruit_NeoPixel::Color(0, 0, 0, DAY_BRIGHTNESS_WHITE))
            {
                this->Sunrise_Exit();
            }
            else
            {
                uint16_t tmp = 0;
                uint8_t red = (uint8_t)(this->m_day_color >> 16);
                uint8_t green = (uint8_t)(this->m_day_color >>  8);
                uint8_t blue = (uint8_t)this->m_day_color;
                uint8_t white = (uint8_t)(this->m_day_color >> 24);

                tmp = (white / 10) + 1;
                if ((white + tmp) < DAY_BRIGHTNESS_WHITE) 
                {
                    white += tmp;
                }
                else 
                {
                    white = DAY_BRIGHTNESS_WHITE;
                }

                if (white > ((DAY_BRIGHTNESS_WHITE * 2) / 5))
                {
                    tmp = (red / 10) + 1;
                    if (tmp < red) {
                        red -= tmp;
                    }
                    else {
                        red = 0;
                    }

                    tmp = (green / 10) + 1;
                    if (tmp < green) {
                        green -= tmp;
                    }
                    else {
                        green = 0;
                    }

                    tmp = (blue / 10) + 1;
                    if (tmp < blue) {
                        blue -= tmp;
                    }
                    else {
                        blue = 0;
                    }                
                }

                this->m_day_color = Adafruit_NeoPixel::Color(red, green, blue, white);
                this->m_light_hdl_p->SetColor(this->m_day_color);
                this->m_light_hdl_p->Show();    
            }
            break;  
        } 

        default:
            break;
    }
}


//*****************************************************************************
// description:
//   Enter
//*****************************************************************************
void LightScene_Sun::Sunset_Enter(void)
{
    this->m_sun_height = SUN_MAX_HEIGHT;
    this->m_sun_pos = LedRow::LED_ROW_LENGTH - 1;
    this->m_twilight_brightness = SUNRISE_BIRGHTNESS;
    this->m_state = LIGHTSCENESUN_STATE_Fading;
    this->m_day_color = Adafruit_NeoPixel::Color(0, 0, 0, DAY_BRIGHTNESS_WHITE);

    this->m_light_hdl_p->SetBrightness_Instantly(255);
    this->m_light_hdl_p->SetColor(this->m_day_color);
    this->m_light_hdl_p->SetLedArea(0, LedRow::LED_ROW_LENGTH, 0, LedRow::LED_ROW_NOF);  
    this->m_light_hdl_p->Show();
}


//*****************************************************************************
// description:
//   Exit
//*****************************************************************************
void LightScene_Sun::Sunset_Exit(void)
{
    this->m_light_hdl_p->SetBrightness_Instantly(NIGHT_BRIGHTNESS);
    this->m_light_hdl_p->SetColor(NIGHT_COLOR);
    this->m_scene_hdl_p->ChangeLightScene(LightScene::Night);   
}


//*****************************************************************************
// description:
//   Task
//*****************************************************************************
void LightScene_Sun::Sunset_Task(void)
{
    switch (this->m_state)
    {
        case LIGHTSCENESUN_STATE_Fading:
        {
            if (this->m_day_color == Adafruit_NeoPixel::Color(RED_MAX, GREEN_MAX, BLUE_MAX, 0))
            {
                this->m_state = LIGHTSCENESUN_STATE_Sunset;   
            }
            else
            {
                uint16_t tmp = 0;
                uint8_t red = (uint8_t)(this->m_day_color >> 16);
                uint8_t green = (uint8_t)(this->m_day_color >>  8);
                uint8_t blue = (uint8_t)this->m_day_color;
                uint8_t white = (uint8_t)(this->m_day_color >> 24);
                bool is_rgb_color_ready = true;

                tmp = (red / 10) + 1;
                if ((tmp + red) < RED_MAX) 
                {
                    red += tmp;
                }
                else
                {
                    red = RED_MAX;
                }

                tmp = (green / 10) + 1;
                if ((tmp + green) < GREEN_MAX) 
                {
                    green += tmp;
                }
                else
                {
                    green = GREEN_MAX;
                }

                tmp = (blue / 10) + 1;
                if ((tmp + blue) < BLUE_MAX) 
                {
                    blue += tmp;
                }
                else
                {
                    blue = BLUE_MAX;
                }

                if (red > ((RED_MAX * 2) / 5))
                {
                    tmp = (white / 10) + 1;
                    if (tmp < white) {
                        white -= tmp;
                    }
                    else {
                        white = 0;
                    }                
                }

                this->m_day_color = Adafruit_NeoPixel::Color(red, green, blue, white);
                this->m_light_hdl_p->SetColor(this->m_day_color);
                this->m_light_hdl_p->Show();    
            }
            break;   
        }

        case LIGHTSCENESUN_STATE_Sunset:
        {
            this->CalculateAndShow_Sunlight();

            if (this->m_sun_height > 0)
            {
                this->m_sun_height -= (this->m_sun_height / 10) + 1;
                this->m_sun_pos = this->m_sun_pos % LedRow::LED_ROW_LENGTH;
            }
            else if (this->m_twilight_brightness > NIGHT_BRIGHTNESS)
            {
                this->m_twilight_brightness--;
            }
            else
            {
                this->Sunset_Exit();
            }

            break;
        }

        default:
            break;
    }
}


//*****************************************************************************
// description:
//   CalculateAndShow_Sunlight
//*****************************************************************************
void LightScene_Sun::CalculateAndShow_Sunlight(void)
{
    uint16_t pixel_idx;
    uint16_t cnt;
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    float brightness = 0;
    float asin_alpha = 0;
    uint32_t hypothenuse = 0;
    uint32_t color = 0;
    uint8_t tmp_color = 0;

    for (cnt = 0; cnt < LedRow::LED_ROW_LENGTH; cnt++)
    {
        // calculate pixel color ----------------------------------------------
        hypothenuse = sqrt(pow(this->m_sun_height, 2) + pow(PIXEL_DISTANCE_MM * cnt, 2));
        asin_alpha = (255 * this->m_sun_height) / hypothenuse;
        brightness = asin_alpha / 255;

        red = 255 * brightness;
        if (red > RED_MAX) {
            red = RED_MAX;
        }

        green = asin_alpha * brightness;
        if (green > GREEN_MAX) {
            green = GREEN_MAX;
        }

        tmp_color = (green * this->m_sun_height) / (SUN_MAX_HEIGHT * 2);
        tmp_color = (cnt * (DAY_BRIGHTNESS_WHITE / (LedRow::LED_ROW_LENGTH / 2))) + tmp_color;
        tmp_color += NIGHT_BRIGHTNESS;
        if (tmp_color > BLUE_MAX) {
            tmp_color = BLUE_MAX;
        }
        blue = (tmp_color * brightness) + this->m_twilight_brightness;

        // update pixel -------------------------------------------------------
        color = Adafruit_NeoPixel::Color(red, green, blue, 0);
        if ((this->m_sun_pos + cnt) < LedRow::LED_ROW_LENGTH)
        {
            pixel_idx = (this->m_sun_pos + cnt);
            this->m_light_hdl_p->SetLedArea(pixel_idx, pixel_idx, 0, (LedRow::LED_ROW_NOF - 1), color);
        }

        if (this->m_sun_pos >= cnt)
        {
            pixel_idx = (this->m_sun_pos - cnt);
            this->m_light_hdl_p->SetLedArea(pixel_idx, pixel_idx, 0, (LedRow::LED_ROW_NOF - 1), color);
        }
    }

    // Send the updated pixel colors to the hardware.  
    this->m_light_hdl_p->Show();    

}