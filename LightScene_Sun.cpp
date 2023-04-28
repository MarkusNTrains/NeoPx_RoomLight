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
#include "LightScene_Day.h"


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
LightScene_Sun::LightScene_Sun(LightSceneHdl* parent, LightHdl* light_hdl, Datastore* datastore_p)
{
    this->m_scene_hdl_p = parent;
    this->m_light_hdl_p = light_hdl;
    this->m_datastore_p = datastore_p;
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
void LightScene_Sun::Sunrise_Enter()
{
    this->m_sun_height = 0;
    this->m_sun_pos = 0;
    this->m_twilight_brightness = this->m_datastore_p->GetParameter(Parameter::Id::SceneNight_Brightness);
    this->m_state = Sunrise;
    this->m_task_timestamp_ms = millis();
    this->Update_DayParameter();

    this->m_light_hdl_p->SetBrightness_Instantly(255);
    this->m_light_hdl_p->SetColor(Adafruit_NeoPixel::Color(0, 0, this->m_twilight_brightness, 0));
    this->m_light_hdl_p->SetLedArea(0, LedRow::LED_ROW_LENGTH, 0, LedRow::LED_ROW_NOF);  
    this->m_light_hdl_p->Show();
}


//*****************************************************************************
// description:
//   Exit
//*****************************************************************************
void LightScene_Sun::Sunrise_Exit()
{
    this->m_light_hdl_p->SetBrightness_Instantly(this->m_day_brightness_white);
    this->m_light_hdl_p->SetColor(LightScene_Day::COLOR);
    this->m_scene_hdl_p->ChangeLightScene(LightSceneID::Day);   
}


//*****************************************************************************
// description:
//   Task
// return:
//   true if LightHdl::Show() needs to be called, else false
//*****************************************************************************
bool LightScene_Sun::Sunrise_Task()
{
    bool is_update_needed = false;

    if (millis() - this->m_task_timestamp_ms > TASK_TmoMs)
    {
        this->m_task_timestamp_ms = millis();
        
        switch (this->m_state)
        {
            case Sunrise:
            {
                is_update_needed = this->CalculateAndShow_Sunlight();

                if (this->m_twilight_brightness < SUNRISE_StartBrightness)
                {
                    this->m_twilight_brightness++;
                }
                else if (this->m_sun_height < SUN_MaxHeight)
                {
                    this->m_sun_height += (this->m_sun_height / 10) + 1;
                    this->m_sun_pos = this->m_sun_pos % LedRow::LED_ROW_LENGTH;
                }
                else
                {
                    this->m_day_color = Adafruit_NeoPixel::Color(this->m_red_max, this->m_green_max, this->m_blue_max, 0);
                    this->m_state = Fading;   
                }

                break;
            }
            case Fading:
            {
                if (this->m_day_color == Adafruit_NeoPixel::Color(0, 0, 0, this->m_day_brightness_white))
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
                    if ((white + tmp) < this->m_day_brightness_white) 
                    {
                        white += tmp;
                    }
                    else 
                    {
                        white = this->m_day_brightness_white;
                    }

                    if (white > ((this->m_day_brightness_white * 2) / 5))
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
                    is_update_needed = true; 
                }
                break;  
            } 

            default:
                break;
        }
    }

    return is_update_needed;
}


//*****************************************************************************
// description:
//   Enter
//*****************************************************************************
void LightScene_Sun::Sunset_Enter()
{
    this->Update_DayParameter();
    this->m_sun_height = SUN_MaxHeight;
    this->m_sun_pos = LedRow::LED_ROW_LENGTH - 1;
    this->m_twilight_brightness = SUNRISE_StartBrightness;
    this->m_state = Fading;
    this->m_day_color = Adafruit_NeoPixel::Color(0, 0, 0, this->m_day_brightness_white);
    this->m_task_timestamp_ms = millis();
    
    this->m_light_hdl_p->SetBrightness_Instantly(255);
    this->m_light_hdl_p->SetColor(this->m_day_color);
    this->m_light_hdl_p->SetLedArea(0, LedRow::LED_ROW_LENGTH, 0, LedRow::LED_ROW_NOF);  
    this->m_light_hdl_p->Show();
}


//*****************************************************************************
// description:
//   Exit
//*****************************************************************************
void LightScene_Sun::Sunset_Exit()
{
    this->m_light_hdl_p->SetBrightness_Instantly(this->m_night_brightness);
    this->m_light_hdl_p->SetColor(LightScene_Night::COLOR);
    this->m_scene_hdl_p->ChangeLightScene(LightSceneID::Night);   
}


//*****************************************************************************
// description:
//   Task
// return:
//   true if LightHdl::Show() needs to be called, else false
//*****************************************************************************
bool LightScene_Sun::Sunset_Task()
{
    bool is_update_needed = false;

    if (millis() - this->m_task_timestamp_ms > TASK_TmoMs)
    {
        this->m_task_timestamp_ms = millis();
        
        switch (this->m_state)
        {
            case Fading:
            {
                if (this->m_day_color == Adafruit_NeoPixel::Color(this->m_red_max, this->m_green_max, this->m_blue_max, 0))
                {
                    this->m_state = Sunset;   
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
                    if ((tmp + red) < this->m_red_max) 
                    {
                        red += tmp;
                    }
                    else
                    {
                        red = this->m_red_max;
                    }

                    tmp = (green / 10) + 1;
                    if ((tmp + green) < this->m_green_max) 
                    {
                        green += tmp;
                    }
                    else
                    {
                        green = this->m_green_max;
                    }

                    tmp = (blue / 10) + 1;
                    if ((tmp + blue) < this->m_blue_max) 
                    {
                        blue += tmp;
                    }
                    else
                    {
                        blue = this->m_blue_max;
                    }

                    if (red > ((this->m_red_max * 2) / 5))
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
                    is_update_needed = true;
                }
                break;   
            }

            case Sunset:
            {
                is_update_needed = this->CalculateAndShow_Sunlight();

                if (this->m_sun_height > 0)
                {
                    this->m_sun_height -= (this->m_sun_height / 10) + 1;
                    this->m_sun_pos = this->m_sun_pos % LedRow::LED_ROW_LENGTH;
                }
                else if (this->m_twilight_brightness > this->m_night_brightness)
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

    return is_update_needed;
}


//*****************************************************************************
// description:
//   CalculateAndShow_Sunlight
// return:
//   true if LightHdl::Show() needs to be called, else false
//*****************************************************************************
bool LightScene_Sun::CalculateAndShow_Sunlight()
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
        hypothenuse = sqrt(pow(this->m_sun_height, 2) + pow(STRIPE_PixelDistanceMM * cnt, 2));
        asin_alpha = (255 * this->m_sun_height) / hypothenuse;
        brightness = asin_alpha / 255;

        red = 255 * brightness;
        if (red > this->m_red_max) {
            red = this->m_red_max;
        }

        green = asin_alpha * brightness;
        if (green > this->m_green_max) {
            green = this->m_green_max;
        }

        tmp_color = (green * this->m_sun_height) / (SUN_MaxHeight * 2);
        tmp_color = (cnt * (this->m_day_brightness_white / (LedRow::LED_ROW_LENGTH / 2))) + tmp_color;
        tmp_color += this->m_night_brightness;
        if (tmp_color > this->m_blue_max) {
            tmp_color = this->m_blue_max;
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

    // pixel color changed --> update needed
    return true;  
}


//*****************************************************************************
// description:
//   Update_DayParameter
//*****************************************************************************
void LightScene_Sun::Update_DayParameter()
{
    uint32_t day_brightness = this->m_datastore_p->GetParameter(Parameter::Id::SceneDay_Brightness);
    
    this->m_day_brightness_white = (uint8_t)day_brightness;
    this->m_day_brightness_rgb = (uint8_t)((day_brightness * 5) / 3);
    this->m_red_max = (uint8_t)((RED_Max * day_brightness) / 255);
    this->m_green_max = (uint8_t)((GREEN_Max * day_brightness) / 255);
    this->m_blue_max = (uint8_t)((BLUE_Max * day_brightness ) / 255);
    this->m_night_brightness = this->m_datastore_p->GetParameter(Parameter::Id::SceneNight_Brightness);
}

