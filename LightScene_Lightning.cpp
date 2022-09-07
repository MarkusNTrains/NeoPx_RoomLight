/*******************************************************************************
Project   RoomLight

  This is an OpenSource Project.
  You can use, share or improve this project. If you improve this source code
  please share with the comunity or at least with the author of the original 
  source code
  
  Created 7. September 2022 by MarkusNTrains
================================================================================
$HeadURL:  $
$Id:  $
*******************************************************************************/


//-----------------------------------------------------------------------------
// includes
#include "LightScene_Lightning.h"


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
LightScene_Lightning::LightScene_Lightning(LightSceneHdl* parent, LightHdl* light_hdl)
{
    this->m_scene_hdl_p = parent;
    this->m_light_hdl_p = light_hdl;

    this->m_state = LIGHTNING_STATE_Unknown;
    this->m_flash_timestamp_ms = 0;
    this->m_flash_pause_ms = 0;
    this->m_nof_flashes = 0;
    this->m_flash_counter = 0;
}


//*****************************************************************************
// description:
//   destructor
//*****************************************************************************
LightScene_Lightning::~LightScene_Lightning()
{
}


//*****************************************************************************
// description:
//   Enter
//*****************************************************************************
void LightScene_Lightning::Enter(void)
{
    this->m_state = LIGHTNING_STATE_Dimming;
    this->m_last_brightness = this->m_light_hdl_p->GetBrightness();
    //this->m_desired_brightness = BackgroundBrightness;
}


//*****************************************************************************
// description:
//   Exit
//*****************************************************************************
void LightScene_Lightning::Exit(void)
{
    this->m_scene_hdl_p->ChangeLightScene(this->m_scene_hdl_p->GetLastScene(), this->m_last_brightness);
}


//*****************************************************************************
// description:
//   Task
//*****************************************************************************
void LightScene_Lightning::Task(void)
{
    switch (this->m_state)
    {
        case LIGHTNING_STATE_Dimming:
            /*if ((this->m_desired_brightness != m_current_brightness) || (this->m_color != COLOR_BLUE))
            {
                uint8_t color_change_factor = 20;
                uint8_t red = (this->m_color & COLOR_RED) >> 16;
                uint8_t green = (this->m_color & COLOR_GREEN) >> 8;
                uint8_t blue = (this->m_color & COLOR_BLUE);
                uint8_t white = (this->m_color & COLOR_WHITE) >> 24;
                
                red = this->UpdateValueTo(red, 0, color_change_factor);
                green = this->UpdateValueTo(green, 0, color_change_factor);
                blue = this->UpdateValueTo(blue, 0xFF, color_change_factor/2);
                white = this->UpdateValueTo(white, 0, color_change_factor);
                
                this->m_color = Adafruit_NeoPixel::Color(red, green, blue, white);
                this->m_led_matrix->SetPixelArray(0, LedRow::LED_ROW_LENGTH, 0, 3, this->m_color);
                this->UpdateBrightness();  
                this->m_led_matrix->Show();
            }
            else*/
            {
                // set overall brightness to maximum and the background color to current brightness, so that flashes will bi visible
                this->m_light_hdl_p->SetBrightness_Instantly(255);
                this->m_light_hdl_p->SetLedArea(0, LedRow::LED_ROW_LENGTH, 0, 3, Adafruit_NeoPixel::Color(0, 0, BackgroundBrightness, 0));
                this->m_light_hdl_p->Show();

                this->m_nof_flashes = (rand() % 10) + 10;
                this->m_flash_counter = 0;
                this->m_flash_timestamp_ms = millis();
                this->m_flash_pause_ms = 5000;
                this->m_state = LIGHTNING_STATE_FlashActive;
            }
            break;

        case LIGHTNING_STATE_FlashActive:
            if (millis() >= this->m_flash_timestamp_ms + this->m_flash_pause_ms)
            {
                if (this->m_flash_counter >= this->m_nof_flashes)
                {
                    this->Exit();
                }
                else 
                {
                    // show next flash
                    srand(millis());
                    this->m_flash_timestamp_ms = millis();
                    this->m_flash_pause_ms = rand() % 5000;

                    uint32_t flash_color = Adafruit_NeoPixel::Color(0, 0, 0, 255);
                    uint8_t flash_start_pos = rand() % (LedRow::LED_ROW_LENGTH - MaxFlashWidth);
                    uint8_t flash_width = rand() % MaxFlashWidth;
                    uint32_t flash_length_ms = rand() % MaxFlashLengthMs;
                    uint8_t flash_row = rand() % LedRow::LED_ROW_NOF;

                    // show flash
                    this->m_light_hdl_p->SetLedArea(flash_start_pos, flash_start_pos + flash_width, flash_row, flash_row, flash_color);
                    this->m_light_hdl_p->Show();
                    delay(flash_length_ms);

                    // hide flash
                    this->m_light_hdl_p->SetLedArea(0, LedRow::LED_ROW_LENGTH, flash_row, flash_row, Adafruit_NeoPixel::Color(0, 0, BackgroundBrightness, 0));
                    this->m_light_hdl_p->Show();

                    this->m_flash_counter++;
                }
            }
            break;

        default:
            break;
    }
}


