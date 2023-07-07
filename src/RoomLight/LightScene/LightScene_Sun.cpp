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
LightScene_Sun::LightScene_Sun(RoomLightHdl* parent, LightHdl* light_hdl, Datastore* datastore_p)
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

    this->m_light_hdl_p->SetBrightness_Instantly(Parameter::BRIGHTNESS_Max);
    this->m_light_hdl_p->SetLedArea_DoNotChangeBlackLED(0, LedRow::LED_ROW_LENGTH, 0, LedRow::LED_ROW_NOF, Adafruit_NeoPixel::Color(0, 0, this->m_twilight_brightness, 0));
    this->m_light_hdl_p->Show();
}


//*****************************************************************************
// description:
//   Exit
//*****************************************************************************
void LightScene_Sun::Sunrise_Leave()
{
    this->m_light_hdl_p->SetBrightness_Instantly(this->m_day_brightness_white);
    this->m_light_hdl_p->SetColor(LightScene_Day::COLOR);
    this->m_scene_hdl_p->ChangeScene((uint8_t)RoomLightHdl::SceneID::Day);   
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
                    this->m_day_color.color = Adafruit_NeoPixel::Color(this->m_red_max, this->m_green_max, this->m_blue_max, 0);
                    this->m_state = Fading; 
                    this->m_fading_progress_rgb = FADING_Max;  
                }

                break;
            }
            case Fading:
            {
                if (this->m_day_color.color == Adafruit_NeoPixel::Color(0, 0, 0, this->m_day_brightness_white))
                {
                    this->Sunrise_Leave();
                }
                else
                {
                    // fade white
                    uint16_t tmp = (this->m_day_color.white / FADING_Factor) + 1;
                    if ((this->m_day_color.white + tmp) < this->m_day_brightness_white) 
                    {
                        this->m_day_color.white += tmp;
                    }
                    else 
                    {
                        this->m_day_color.white = this->m_day_brightness_white;
                    }

                    // fade rgb
                    if (this->m_day_color.white > ((this->m_day_brightness_white * 2) / 5))
                    {
                        tmp = (this->m_fading_progress_rgb / FADING_Factor) + 1;
                        if (this->m_fading_progress_rgb > tmp)
                        {
                            this->m_fading_progress_rgb -= tmp;
                            this->m_day_color.red = (this->m_day_color.red * this->m_fading_progress_rgb) / FADING_Max;
                            this->m_day_color.green = (this->m_day_color.green * this->m_fading_progress_rgb) / FADING_Max;
                            this->m_day_color.blue = (this->m_day_color.blue * this->m_fading_progress_rgb) / FADING_Max;
                        }
                        else
                        {
                            this->m_fading_progress_rgb = 0;
                            this->m_day_color.red = 0;
                            this->m_day_color.green = 0;
                            this->m_day_color.blue = 0;
                        }
                    }

                    this->m_light_hdl_p->SetColor(this->m_day_color.color);
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
    this->m_fading_progress_rgb = 0;
    this->m_day_color.color = Adafruit_NeoPixel::Color(0, 0, 0, this->m_day_brightness_white);
    this->m_task_timestamp_ms = millis();
    this->m_state = Fading;
    
    this->m_light_hdl_p->SetBrightness_Instantly(Parameter::BRIGHTNESS_Max);
    this->m_light_hdl_p->SetLedArea_DoNotChangeBlackLED(0, LedRow::LED_ROW_LENGTH, 0, LedRow::LED_ROW_NOF, this->m_day_color.color);  
    this->m_light_hdl_p->Show();
}


//*****************************************************************************
// description:
//   Exit
//*****************************************************************************
void LightScene_Sun::Sunset_Leave()
{
    this->m_light_hdl_p->SetColor(LightScene_Night::COLOR);
    this->m_light_hdl_p->SetBrightness_Instantly(this->m_night_brightness);
    this->m_scene_hdl_p->ChangeScene((uint8_t)RoomLightHdl::SceneID::Night);   
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
                if (this->m_day_color.color == Adafruit_NeoPixel::Color(this->m_red_max, this->m_green_max, this->m_blue_max, 0))
                {
                    this->m_state = Sunset;   
                }
                else
                {
                    // fade rgb
                    uint16_t tmp = (this->m_fading_progress_rgb / FADING_Factor) + 1;
                    if (this->m_fading_progress_rgb + tmp < FADING_Max)
                    {
                        this->m_fading_progress_rgb += tmp;
                        this->m_day_color.red = (this->m_red_max * this->m_fading_progress_rgb) / FADING_Max;
                        this->m_day_color.green = (this->m_green_max * this->m_fading_progress_rgb) / FADING_Max;
                        this->m_day_color.blue = (this->m_blue_max * this->m_fading_progress_rgb) / FADING_Max;
                    }
                    else
                    {
                        this->m_fading_progress_rgb = FADING_Max;
                        this->m_day_color.red = this->m_red_max;
                        this->m_day_color.green = this->m_green_max;
                        this->m_day_color.blue = this->m_blue_max;
                    }

                    // fade white
                    if (this->m_day_color.red > ((this->m_day_brightness_rgb * 2) / 5))
                    {
                        tmp = (this->m_day_color.white / FADING_Factor) + 1;
                        if (this->m_day_color.white > tmp) 
                        {
                            this->m_day_color.white -= tmp;
                        }
                        else 
                        {
                            this->m_day_color.white = 0;
                        }
                    }
                    this->m_light_hdl_p->SetColor(this->m_day_color.color);
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
                    this->Sunset_Leave();
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
    uint16_t red;
    uint16_t green;
    uint16_t blue;
    float brightness = 0;
    float asin_alpha = 0;
    uint32_t hypothenuse = 0;
    uint32_t color = 0;
    uint32_t pow_sunHeight = pow(this->m_sun_height, 2);
    uint32_t brightness_sunHeight = ((uint32_t)Parameter::BRIGHTNESS_Max * this->m_sun_height);

    for (cnt = 0; cnt < LedRow::LED_ROW_LENGTH; cnt++)
    {
        // calculate pixel color ----------------------------------------------
        hypothenuse = sqrt(pow_sunHeight + pow(STRIPE_PixelDistanceMM * cnt, 2));
        asin_alpha = brightness_sunHeight / hypothenuse;
        brightness = asin_alpha / (float)Parameter::BRIGHTNESS_Max;

        red = Parameter::BRIGHTNESS_Max * brightness;
        if (this->m_twilight_brightness > TWILIGHT_RedOffsetToBlue) {
            red += (this->m_twilight_brightness - TWILIGHT_RedOffsetToBlue);
        }
        if (red > this->m_red_max) {
            red = this->m_red_max;
        }

        green = asin_alpha * brightness;
        if (this->m_twilight_brightness > TWILIGHT_GreenOffsetToBlue) {
            green += (this->m_twilight_brightness - TWILIGHT_GreenOffsetToBlue);
        }
        if (green > this->m_green_max) {
            green = this->m_green_max;
        }

        if (this->m_sun_height > 0) {
            blue = (cnt * (this->m_day_brightness_rgb / (LedRow::LED_ROW_LENGTH / 2))) + blue;
        }
        blue = (blue * brightness) + this->m_twilight_brightness;
        if (blue > this->m_blue_max) {
            blue = this->m_blue_max;
        }
        else if (blue < 1) {
            blue = 1;
        }

        // update pixel -------------------------------------------------------
        color = Adafruit_NeoPixel::Color(red, green, blue, 0);
        if ((this->m_sun_pos + cnt) < LedRow::LED_ROW_LENGTH)
        {
            pixel_idx = (this->m_sun_pos + cnt);
            this->m_light_hdl_p->SetLedArea_DoNotChangeBlackLED(pixel_idx, pixel_idx, 0, (LedRow::LED_ROW_NOF - 1), color);
        }

        if (this->m_sun_pos >= cnt)
        {
            pixel_idx = (this->m_sun_pos - cnt);
            this->m_light_hdl_p->SetLedArea_DoNotChangeBlackLED(pixel_idx, pixel_idx, 0, (LedRow::LED_ROW_NOF - 1), color);
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
    this->m_day_brightness_white = (uint8_t)this->m_datastore_p->GetParameter(Parameter::Id::SceneDay_Brightness);
    this->m_day_brightness_rgb = ((this->m_day_brightness_white * 5) / 3);
    if (this->m_day_brightness_rgb > Parameter::BRIGHTNESS_Max) 
    {
        this->m_day_brightness_rgb = Parameter::BRIGHTNESS_Max;
    }
    this->m_red_max = (uint8_t)((RED_Max * this->m_day_brightness_rgb) / Parameter::BRIGHTNESS_Max);
    this->m_green_max = (uint8_t)((GREEN_Max * this->m_day_brightness_rgb) / Parameter::BRIGHTNESS_Max);
    this->m_blue_max = (uint8_t)((BLUE_Max * this->m_day_brightness_rgb ) / Parameter::BRIGHTNESS_Max);
    this->m_night_brightness = this->m_datastore_p->GetParameter(Parameter::Id::SceneNight_Brightness);
}

