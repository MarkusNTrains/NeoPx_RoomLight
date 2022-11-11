/*******************************************************************************
Project   RoomLight

  This is an OpenSource Project.
  You can use, share or improve this project. If you improve this source code
  please share with the comunity or at least with the author of the original 
  source code
  
  Created 10. November 2022 by MarkusNTrains
================================================================================
$HeadURL:  $
$Id:  $
*******************************************************************************/


//-----------------------------------------------------------------------------
// includes
#include "LightScene_Cloud.h"


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
LightScene_Cloud::LightScene_Cloud(LightSceneHdl* parent, LightHdl* light_hdl)
{
    uint8_t cnt = 0;

    this->m_scene_hdl_p = parent;
    this->m_light_hdl_p = light_hdl;
}


//*****************************************************************************
// description:
//   destructor
//*****************************************************************************
LightScene_Cloud::~LightScene_Cloud()
{
}


//*****************************************************************************
// description:
//   Enter
//*****************************************************************************
void LightScene_Cloud::Enter(void)
{
    uint8_t cnt = 0;

    this->m_scene_brightness = this->m_light_hdl_p->GetBrightness();
    this->m_scene_color = this->m_light_hdl_p->GetColor();
    this->m_start_next_cloud_idx = 0;
    this->m_start_cloud_timestamp_ms = 0;
    this->m_start_next_cloud_tmo_ms = TASK_HDL_TMO_MS;
    this->m_task_hdl_timestamp_ms = 0;
    this->m_task_cycle_cnt = 0;

    srand(millis());
    this->m_nof_clouds = (rand() % ((MAX_NOF_CLOUDS + 1) - MIN_NOF_CLOUDS)) + MIN_NOF_CLOUDS;
    for (cnt = 0; cnt < this->m_nof_clouds; cnt++)
    {
        this->m_cloud_p[cnt] = new SkyCloud();
        this->m_cloud_p[cnt]->is_enable = false;
        this->m_cloud_p[cnt]->position_px = 0;
        this->m_cloud_p[cnt]->row = rand() % LedRow::LED_ROW_NOF;
        this->m_cloud_p[cnt]->length_px = (rand() % ((MAX_CLOUD_WIDTH_PX + 1) - MIN_CLOUD_WIDTH_PX)) + MIN_CLOUD_WIDTH_PX;
        this->m_cloud_p[cnt]->speed = (rand() % ((MAX_CLOUD_SPEED + 1) - MIN_CLOUD_SPEED)) + MIN_CLOUD_SPEED;
        this->m_cloud_p[cnt]->darkness = (rand() % ((MAX_CLOUD_DARKNESS + 1) - MIN_CLOUD_DARKNESS)) + MIN_CLOUD_DARKNESS;
    }
}


//*****************************************************************************
// description:
//   Exit
//*****************************************************************************
void LightScene_Cloud::Exit(void)
{
    uint8_t cnt = 0;
    for (cnt = 0; cnt < this->m_nof_clouds; cnt++)
    {
        delete this->m_cloud_p[cnt];
    }

    this->m_scene_hdl_p->ChangeLightScene(this->m_scene_hdl_p->GetLastScene());
}


//*****************************************************************************
// description:
//   Task
//*****************************************************************************
void LightScene_Cloud::Task(void)
{
    //--- check if a new cloud has to be enabled -----------------------------
    if (this->m_start_next_cloud_idx < this->m_nof_clouds)
    {
        if (millis() >= this->m_start_cloud_timestamp_ms + this->m_start_next_cloud_tmo_ms)
        {
            this->m_start_cloud_timestamp_ms = millis();

            this->m_cloud_p[this->m_start_next_cloud_idx]->is_enable = true;
            this->m_start_next_cloud_idx++;

            this->m_start_next_cloud_tmo_ms = 500;
        }
    }

    //--- move clouds ---------------------------------------------------------
    if (millis() >= this->m_task_hdl_timestamp_ms + TASK_HDL_TMO_MS)
    {
        this->m_task_hdl_timestamp_ms = millis();
        this->m_task_cycle_cnt++;

        bool do_update = false;
        uint8_t cnt = 0;
        uint8_t nof_active_clouds = 0;
        uint16_t start_pos;
        uint16_t end_pos;
        uint32_t color = 0;

        this->m_light_hdl_p->SetLedArea(0, LedRow::LED_ROW_LENGTH, 0, LedRow::LED_ROW_NOF, this->m_scene_color);
        for (cnt = 0; cnt < this->m_nof_clouds; cnt++)
        {
            if (this->m_cloud_p[cnt]->is_enable == true)
            {
                nof_active_clouds++;

                //--- calculate end pos ---------------------------------------
                end_pos = this->m_cloud_p[cnt]->position_px;

                //--- calculate start pos -------------------------------------
                if (end_pos > this->m_cloud_p[cnt]->length_px)
                {
                    start_pos = end_pos - this->m_cloud_p[cnt]->length_px;
                }
                else
                {
                    start_pos = 0;
                }

                if (start_pos >= LedRow::LED_ROW_LENGTH)
                {
                    this->m_cloud_p[cnt]->is_enable = false;
                }

                //--- calculate color -----------------------------------------
                color = this->m_scene_color & 0xFFFFFF;  // clear white color
                Serial.print("color: ");
                Serial.println(color);
                if (this->m_cloud_p[cnt]->darkness < (this->m_scene_color >> 24)) 
                {
                    color = this->m_scene_color - this->m_cloud_p[cnt]->darkness << 24;
                }

                //--- set cloud into stripe -----------------------------------
                this->m_light_hdl_p->SetLedArea(start_pos, end_pos, this->m_cloud_p[cnt]->row, this->m_cloud_p[cnt]->row, color);
                do_update = true;

                //--- update position -----------------------------------------
                if ((this->m_task_cycle_cnt % this->m_cloud_p[cnt]->speed) == 0)
                {
                    this->m_cloud_p[cnt]->position_px++;
                }
            }
        }

        //--- update LED strips -----------------------------------------------
        if (do_update == true)
        {
            this->m_light_hdl_p->Show();
        }

        //--- check exit condition --------------------------------------------
        if ((nof_active_clouds == 0) && (this->m_start_next_cloud_idx >= this->m_nof_clouds))
        {
            this->Exit();
        }
    }
}


