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
    this->m_last_brightness = this->m_light_hdl_p->GetBrightness();
    // set overall brightness to maximum and the background color to current brightness, so that flashes will bi visible
    this->m_light_hdl_p->Clear();
    this->m_light_hdl_p->SetBrightness_Instantly(255);
    this->m_light_hdl_p->SetLedArea(0, LedRow::LED_ROW_LENGTH, 0, (LedRow::LED_ROW_NOF - 1), Adafruit_NeoPixel::Color(0, 0, LightScene_Sun::NIGHT_BRIGHTNESS, 0));
    this->m_light_hdl_p->Show();

    this->m_nof_flashes = (rand() % 10) + MIN_NOF_FLASHES;
    this->m_flash_counter = 0;
    this->m_flash_timestamp_ms = millis();
    this->m_flash_pause_ms = 5000;
}


//*****************************************************************************
// description:
//   Exit
//*****************************************************************************
void LightScene_Lightning::Exit(void)
{
    this->m_light_hdl_p->SetBrightness_Instantly(this->m_last_brightness);
    this->m_scene_hdl_p->ChangeLightScene(this->m_scene_hdl_p->GetLastScene());
}


//*****************************************************************************
// description:
//   Task
//*****************************************************************************
void LightScene_Lightning::Task(void)
{
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
            uint8_t flash_start_pos = rand() % (LedRow::LED_ROW_LENGTH - MAX_FLASH_WIDTH_PX);
            uint8_t flash_width = rand() % MAX_FLASH_WIDTH_PX;
            uint32_t flash_length_ms = rand() % MAX_FLASH_LENGHT_MS;
            uint8_t flash_row = rand() % LedRow::LED_ROW_NOF;

            // show flash
            this->m_light_hdl_p->SetLedArea(flash_start_pos, flash_start_pos + flash_width, flash_row, flash_row, flash_color);
            this->m_light_hdl_p->Show();
            delay(flash_length_ms);

            // hide flash
            this->m_light_hdl_p->SetLedArea(0, LedRow::LED_ROW_LENGTH, flash_row, flash_row, Adafruit_NeoPixel::Color(0, 0, LightScene_Sun::NIGHT_BRIGHTNESS, 0));
            this->m_light_hdl_p->Show();

            this->m_flash_counter++;
        }
    }
}


