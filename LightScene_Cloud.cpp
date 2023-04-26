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

    this->m_scene_color = this->m_light_hdl_p->GetColor();
    this->m_start_next_cloud_idx = 0;
    this->m_start_cloud_timestamp_ms = 0;
    this->m_start_next_cloud_tmo_ms = LIGHTSCENE_CLOUD_Task_TmoMs;
    this->m_task_hdl_timestamp_ms = 0;
    this->m_task_cycle_cnt = 0;

    srand(millis());
    this->m_nof_clouds = (rand() % ((LIGHTSCENE_CLOUD_Cloud_MaxNof + 1) - LIGHTSCENE_CLOUD_Cloud_MinNof)) + LIGHTSCENE_CLOUD_Cloud_MinNof;
    for (cnt = 0; cnt < this->m_nof_clouds; cnt++) 
    {
        this->m_cloud_p[cnt] = new SkyCloud();
        this->m_cloud_p[cnt]->is_enable = false;
        this->m_cloud_p[cnt]->position_px = 0;
        this->m_cloud_p[cnt]->row = rand() % LedRow::LED_ROW_NOF;
        this->m_cloud_p[cnt]->width = (rand() % ((LIGHTSCENE_CLOUD_Cloud_MaxWidth + 1) - LIGHTSCENE_CLOUD_Cloud_MinWidth)) + LIGHTSCENE_CLOUD_Cloud_MinWidth;;
        this->m_cloud_p[cnt]->length_px = (rand() % ((LIGHTSCENE_CLOUD_Cloud_MaxLengthPx + 1) - LIGHTSCENE_CLOUD_Cloud_MinLengthPx)) + LIGHTSCENE_CLOUD_Cloud_MinLengthPx;
        this->m_cloud_p[cnt]->speed = (rand() % ((LIGHTSCENE_CLOUD_Cloud_MaxSpeed + 1) - LIGHTSCENE_CLOUD_Cloud_MinSpeed)) + LIGHTSCENE_CLOUD_Cloud_MinSpeed;
        this->m_cloud_p[cnt]->darkness = (rand() % ((LIGHTSCENE_CLOUD_Cloud_MaxDarkness + 1) - LIGHTSCENE_CLOUD_Cloud_MinDarkness)) + LIGHTSCENE_CLOUD_Cloud_MinDarkness;
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

            srand(millis());
            this->m_start_next_cloud_tmo_ms = (rand() % ((LIGHTSCENE_CLOUD_Cloud_AddNewMaxLengthMs + 1) / this->m_nof_clouds))  + LIGHTSCENE_CLOUD_Cloud_AddNextMinTmoMs;
        }
    }

    //--- move clouds ---------------------------------------------------------
    if (millis() >= this->m_task_hdl_timestamp_ms + LIGHTSCENE_CLOUD_Task_TmoMs) 
    {
        this->m_task_hdl_timestamp_ms = millis();
        this->m_task_cycle_cnt++;

        bool do_update = false;
        uint16_t cnt = 0;
        uint16_t nof_active_clouds = 0;
        uint16_t start_pos;
        uint16_t end_pos;
        uint16_t column;
        uint8_t row;
        uint32_t color = 0;
        uint32_t darkness = 0;

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
                color = 0;
                darkness = this->m_cloud_p[cnt]->darkness;
                // white color
                if (darkness < ((this->m_scene_color >> 24) & 0xFF)) 
                {
                    color |= (this->m_scene_color & 0xFF000000) - (darkness << 24);
                }
                // red color
                if (darkness < ((this->m_scene_color >> 16) & 0xFF)) 
                {
                    color |= (this->m_scene_color & 0xFF0000) - (darkness << 16);
                }
                // green color
                if (darkness < ((this->m_scene_color >> 8) & 0xFF)) 
                {
                    color |= (this->m_scene_color & 0xFF00) - (darkness << 8);
                }
                // blue color
                if (darkness < (this->m_scene_color & 0xFF)) 
                {
                    color |= (this->m_scene_color & 0xFF) - darkness;
                }
                
                
                /*switch (cnt % 6)
                {
                    case 0:
                        color = 0x00FF0000;
                        break;

                    case 1:
                        color = 0x0000FF00;
                        break;

                    case 2:
                        color = 0x000000FF;
                        break;

                    case 3:
                        color = 0x00FFFF00;
                        break;

                    case 4:
                        color = 0x00FF00FF;
                        break;

                    case 5:
                        color = 0x0000FFFF;
                        break;
                }*/

                //--- set cloud into stripe -----------------------------------
                /*for (row = this->m_cloud_p[cnt]->row; row < (this->m_cloud_p[cnt]->row + this->m_cloud_p[cnt]->width); row++) 
                {
                    if (row >= LedRow::LED_ROW_NOF) 
                    {
                        break;
                    }

                    for (column = start_pos; column <= end_pos; column++) 
                    {
                        if (column < LedRow::LED_ROW_LENGTH)
                        {
                            if (this->m_light_hdl_p->GetLedColor(row, column) > color) 
                            {
                                this->m_light_hdl_p->SetLed(row, column, color);
                            }
                        }
                    }
                }*/
                this->m_light_hdl_p->SetLedArea(start_pos, end_pos, this->m_cloud_p[cnt]->row, this->m_cloud_p[cnt]->row + this->m_cloud_p[cnt]->width, color);
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