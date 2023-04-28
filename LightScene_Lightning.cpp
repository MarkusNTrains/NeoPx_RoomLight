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
#include "LightScene_Sun.h"


//-----------------------------------------------------------------------------
// typedef



//*****************************************************************************
// description:
//   constructor
//*****************************************************************************
LightScene_Lightning::LightScene_Lightning(LightSceneHdl* parent, LightHdl* light_hdl, Datastore* datastore_p)
{
    this->m_scene_hdl_p = parent;
    this->m_light_hdl_p = light_hdl;
    this->m_datastore_p = datastore_p;

    this->m_task_timestamp_ms = 0;
    this->m_task_tmo_ms = 0;
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
void LightScene_Lightning::Enter()
{
    this->m_last_scene_brightness = this->m_light_hdl_p->GetBrightness();
    this->m_last_scene_color.color = this->m_light_hdl_p->GetColor();

    this->m_background_color.red = (((uint16_t)this->m_last_scene_color.red * this->m_last_scene_brightness) / 255);
    this->m_background_color.green = (((uint16_t)this->m_last_scene_color.green * this->m_last_scene_brightness) / 255);
    this->m_background_color.blue = (((uint16_t)this->m_last_scene_color.blue * this->m_last_scene_brightness) / 255);
    this->m_background_color.white = (((uint16_t)this->m_last_scene_color.white * this->m_last_scene_brightness) / 255);

    this->m_light_hdl_p->SetBrightness_Instantly(255);
    this->m_light_hdl_p->SetColor(this->m_background_color.color);

    this->m_task_tmo_ms = TASK_TmoMs;
    this->m_state = State::Darken;
}


//*****************************************************************************
// description:
//   Exit
//*****************************************************************************
void LightScene_Lightning::Exit()
{
    this->m_light_hdl_p->SetBrightness_Instantly(this->m_last_scene_brightness);
    this->m_light_hdl_p->SetColor(this->m_last_scene_color.color);
    this->m_scene_hdl_p->ChangeLightScene(this->m_scene_hdl_p->GetLastScene());
}


//*****************************************************************************
// description:
//   Task
// return:
//   true if LightHdl::Show() needs to be called, else false
//*****************************************************************************
bool LightScene_Lightning::Task()
{
    bool is_update_needed = false;

    if (millis() >= this->m_task_timestamp_ms + this->m_task_tmo_ms)
    {
        this->m_task_timestamp_ms = millis();

        switch (this->m_state)
        {
            case State::Darken:
            {
                wrgb_color_t target_color;
                target_color.color = Adafruit_NeoPixel::Color(0, 0, this->m_datastore_p->GetParameter(Parameter::Id::SceneNight_Brightness), 0);
                
                bool is_fading_finished = true;
                is_fading_finished &= this->FadeColor(target_color.red, &this->m_background_color.red);
                is_fading_finished &= this->FadeColor(target_color.green, &this->m_background_color.green);
                is_fading_finished &= this->FadeColor(target_color.blue, &this->m_background_color.blue);
                is_fading_finished &= this->FadeColor(target_color.white, &this->m_background_color.white);

                this->m_light_hdl_p->SetColor(this->m_background_color.color);
                is_update_needed = true;

                if (is_fading_finished == true)
                {
                    this->m_state = State::Lightning;
                    this->m_nof_flashes = (rand() % (FLASH_MaxNof - FLASH_MinNof)) + FLASH_MinNof;
                    this->m_flash_counter = 0;
                    this->m_task_tmo_ms = 1000;

                    // set overall brightness to maximum and the background color to current brightness, so that flashes will bi visible
                    //this->m_light_hdl_p->Clear();
                    //this->m_light_hdl_p->SetLedArea(0, (LedRow::LED_ROW_LENGTH - 1), 0, (LedRow::LED_ROW_NOF - 1), this->m_current_color.color);
                }
                break;
            }
            case State::Lightning:
            {
                if (this->m_flash_counter >= this->m_nof_flashes)
                {
                    this->m_state = State::BrightenUp;
                    this->m_task_tmo_ms = TASK_TmoMs;
                }
                else 
                {
                    // show next flash
                    srand(millis());
                    this->m_task_tmo_ms = rand() % FLASH_MaxWaitTimeMs;

                    uint8_t nof_flashes_followed = rand() % FLASH_MaxNofAfterEachOther;
                    uint16_t flash_start_pos = 0;
                    uint16_t flash_length_px = 0;
                    uint32_t flash_duration_ms = 0;
                    uint16_t flash_row = 0;
                    bool is_led_dark[FLASH_MaxLengthPx + 1];

                    for (uint8_t cnt = 0; cnt < nof_flashes_followed; cnt++)
                    {
                        flash_start_pos = rand() % (LedRow::LED_ROW_LENGTH - FLASH_MaxLengthPx);
                        flash_length_px = rand() % (FLASH_MaxLengthPx + 1);
                        flash_duration_ms = rand() % (FLASH_MaxDurationMs + 1);
                        flash_row = rand() % LedRow::LED_ROW_NOF;

                        // find dark leds
                        for (uint8_t px = 0; px <= flash_length_px; px++)
                        {
                            if (this->m_light_hdl_p->GetLedColor(flash_row, flash_start_pos + px) == LightHdl::COLOR_BLACK)
                            {
                                is_led_dark[px] = true;
                            }
                            else
                            {
                                is_led_dark[px] = false;
                            }
                        }

                        // show flash
                        this->m_light_hdl_p->SetLedArea(flash_start_pos, flash_start_pos + flash_length_px, flash_row, flash_row, LightHdl::COLOR_WHITE);
                        this->m_light_hdl_p->Show();

                        // wait 
                        delay(flash_duration_ms);

                        // hide flash
                        for (uint8_t px = 0; px <= flash_length_px; px++)
                        {
                            if (is_led_dark[px] == true)
                            {
                                this->m_light_hdl_p->SetLedColor(flash_row, flash_start_pos + px, LightHdl::COLOR_BLACK);
                            }
                            else
                            {
                                this->m_light_hdl_p->SetLedColor(flash_row, flash_start_pos + px, this->m_background_color.color);
                            }
                        }
                        this->m_light_hdl_p->Show();
                    }

                    this->m_flash_counter++;
                }
                break;
            }
            case BrightenUp:
            {
                wrgb_color_t target_color;
                target_color.red = (((uint16_t)this->m_last_scene_color.red * this->m_last_scene_brightness) / 255);
                target_color.green = (((uint16_t)this->m_last_scene_color.green * this->m_last_scene_brightness) / 255);
                target_color.blue = (((uint16_t)this->m_last_scene_color.blue * this->m_last_scene_brightness) / 255);
                target_color.white = (((uint16_t)this->m_last_scene_color.white * this->m_last_scene_brightness) / 255);

                bool is_fading_finished = true;
                is_fading_finished &= this->FadeColor(target_color.red, &this->m_background_color.red);
                is_fading_finished &= this->FadeColor(target_color.green, &this->m_background_color.green);
                is_fading_finished &= this->FadeColor(target_color.blue, &this->m_background_color.blue);
                is_fading_finished &= this->FadeColor(target_color.white, &this->m_background_color.white);

                this->m_light_hdl_p->SetColor(this->m_background_color.color);
                is_update_needed = true;

                if (is_fading_finished == true)
                {
                    this->Exit();
                }
                    
                break;
            }
            default:
            {
                this->Exit();
                break;
            }
        }
    }

    return is_update_needed;
}


//*****************************************************************************
// description:
//   Task
// return:
//   true if current_color did reach target_color
//*****************************************************************************
static bool LightScene_Lightning::FadeColor(uint8_t target_color, uint8_t* current_color)
{
    uint16_t change_value = 0;
    bool is_target_reached = false;

    if (target_color < *current_color)
    {
        uint8_t change_value = (*current_color / FADING_Factor) + 1;
        if (change_value < *current_color) 
        {
            *current_color -= change_value;
        }
        else 
        {
            *current_color = 0;
        }

        if (target_color > *current_color) 
        {
            *current_color = target_color;
            is_target_reached = true;
        }
    }
    else if (target_color > *current_color)
    {
        uint8_t change_value = (*current_color / FADING_Factor) + 1;
        if (target_color > ((uint16_t)*current_color + change_value)) 
        {
            *current_color += change_value;
        }
        else 
        {
            *current_color = target_color;
            is_target_reached = true;
        }
    }
    else
    {
        is_target_reached = true;
    }

    return is_target_reached;
}