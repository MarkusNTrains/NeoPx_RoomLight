#include "Arduino.h"
/*******************************************************************************
Project   RoomLight

  This is an OpenSource Project.
  You can use, share or improve this project. If you improve this source code
  please share with the comunity or at least with the author of the original 
  source code
  
  Created 24. August 2020 by MarkusNTrains
================================================================================
$HeadURL:  $
$Id:  $
*******************************************************************************/


//-----------------------------------------------------------------------------
// includes
#include "LightSceneHdl.h"


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
LightSceneHdl::LightSceneHdl()
{
    this->m_datastore_p = new Datastore();
    this->m_light_hdl_p = new LightHdl(this->m_datastore_p);
    this->m_scene_cloud_p = new LightScene_Cloud(this, this->m_light_hdl_p);
    this->m_scene_lightning_p = new LightScene_Lightning(this, this->m_light_hdl_p);
    this->m_scene_sun_p = new LightScene_Sun(this, this->m_light_hdl_p);
    this->m_scene_userSetting_p = new LightScene_UserSetting(this, this->m_light_hdl_p, this->m_datastore_p);
    this->m_brightnessUpdate_timestamp_ms = 0;
    this->m_scene = LightScene::LightOff;
    this->m_last_scene = LightScene::LightOff;
    this->m_task_timestamp_ms = 0;

    // get last lightscene from datastore
    this->ChangeLightScene((LightScene)(this->m_datastore_p->GetParameter(Datastore::ParameterId::LightScene)));
}


//*****************************************************************************
// description:
//   destructor
//*****************************************************************************
LightSceneHdl::~LightSceneHdl()
{
    delete this->m_light_hdl_p;
    delete this->m_scene_cloud_p;
    delete this->m_scene_lightning_p;
    delete this->m_scene_sun_p;
}


//*****************************************************************************
// description:
//   ChangeLightScene
//*****************************************************************************
void LightSceneHdl::ChangeLightScene(LightScene scene)
{
    this->ChangeLightScene(scene, this->m_light_hdl_p->GetBrightness());
}


//*****************************************************************************
// description:
//   ChangeLightScene
//*****************************************************************************
void LightSceneHdl::ChangeLightScene(LightScene scene, uint8_t brightness)
{
    if (this->m_scene != scene)
    {
        this->m_last_scene = this->m_scene;
        this->m_scene = scene;
    }
    
    //--- enter light scene -----
    bool save_light_scene = false;
    switch (scene)
    {
        case LightScene::Cloud:
            this->m_scene_cloud_p->Enter();
            break;

        case LightScene::Day:
            this->m_scene_sun_p->Day_Enter();
            break;

        case LightScene::Lightning:
            this->m_scene_lightning_p->Enter();
            break;

        case LightScene::LightOn:
            this->LightScene_LightOn_Enter(brightness);
            save_light_scene = true;
            break;
        
        case LightScene::LightOff:
            this->m_light_hdl_p->SetBrightness_Fade(0);
            break;
        
        case LightScene::Disco:
            this->m_light_hdl_p->Clear();
            save_light_scene = true;
            break;

        case LightScene::MoBa:
            this->LightScene_MoBa_Enter(brightness);
            save_light_scene = true;
            break;

        case LightScene::Night:
            this->m_scene_sun_p->Night_Enter();
            break;
          
        case LightScene::OfficeTable:
            this->LightScene_OfficeTable_Enter(brightness);
            save_light_scene = true;
            break;
        
        case LightScene::Rainbow:
            this->m_light_hdl_p->Clear();
            this->m_rainbow_firstPixelHue = 0;
            save_light_scene = true;
            break;
            
        case LightScene::Sbh:
            //
            break;

        case LightScene::Sunrise:
            this->m_scene_sun_p->Sunrise_Enter();
            break;
            
        case LightScene::Sunset:
            this->m_scene_sun_p->Sunset_Enter();
            break;
          
        case LightScene::UserSetting:
            this->m_scene_userSetting_p->Enter();
            save_light_scene = true;
            break;

        default:
            break;
    }

    // save light scene if needed
    if (save_light_scene == true)
    {
        this->m_datastore_p->SetParameter(Datastore::ParameterId::LightScene, (uint8_t)scene);
    }
}


//*****************************************************************************
// description:
//   Statemachine
//*****************************************************************************
void LightSceneHdl::Tasks()
{
    // run datsatore task
    this->m_datastore_p->Task();

    // update brightness
    if (millis() - this->m_brightnessUpdate_timestamp_ms > BRIGHTNESS_UPDATE_TMO_MS)
    {
        this->m_brightnessUpdate_timestamp_ms += BRIGHTNESS_UPDATE_TMO_MS;
        this->m_light_hdl_p->UpdateBrightness();
    }

    // run scene tasks
    switch (this->m_scene)
    {
        case LightScene::Cloud:
            this->m_scene_cloud_p->Task();
            break;
            
        case LightScene::Day:
            this->m_scene_sun_p->Day_Task();
            break;

        case LightScene::Idle:
            break;

        case LightScene::Lightning:
            this->m_scene_lightning_p->Task();
            break;
            
        case LightScene::LightOn:
            LightScene_LightOn_Task();
            break;
    
        case LightScene::LightOff:
            LightScene_LightOff_Task();
            break;

        case LightScene::MoBa:
            this->LightScene_MoBa_Task();
            break;

        case LightScene::Disco:
            LightScene_Disco_Task();
            break;
        
        case LightScene::Night:
            this->m_scene_sun_p->Night_Task();
            break;
        
        case LightScene::OfficeTable:
            LightScene_OfficeTable_Task();
            break;
    
        case LightScene::Rainbow:
            this->LightScene_WhiteOverRainbow_Task();
            break;
        
        case LightScene::Sunrise:
            this->m_scene_sun_p->Sunrise_Task();
            break;

        case LightScene::Sunset:
            this->m_scene_sun_p->Sunset_Task();
            break;
        
        case LightScene::UserSetting:
            this->m_scene_userSetting_p->Task();
            break;

        default:
            break;
    }
}


//*****************************************************************************
// description:
//   Get Light Scene
//*****************************************************************************
LightScene LightSceneHdl::GetLightScene(void)
{
    return this->m_scene;
}


//*****************************************************************************
// description:
//   Get Last Light Scene
//*****************************************************************************
LightScene LightSceneHdl::GetLastScene(void)
{
    return this->m_last_scene;
}


//*****************************************************************************
// description:
//   GetLightHdl
//*****************************************************************************
LightHdl* LightSceneHdl::GetLightHdl(void)
{
    return this->m_light_hdl_p;
}


//*****************************************************************************
// description:
//   LightScene_OfficeTableWW_Enter
//*****************************************************************************
void LightSceneHdl::LightScene_OfficeTable_Enter(uint16_t brightness)
{
    this->m_light_hdl_p->SetBrightness_Fade(brightness);
    this->m_light_hdl_p->Clear();
}


//*****************************************************************************
// description:
//   ShowOfficeTable_Task
//*****************************************************************************
void LightSceneHdl::LightScene_OfficeTable_Task(void)
{
    if (millis() - this->m_task_timestamp_ms > TASK_SceneOfficeTable_TmoMs)
    {
        this->m_task_timestamp_ms = millis();

        this->m_light_hdl_p->SetLedArea(0, 20, 0, 0);  
        this->m_light_hdl_p->SetLedArea(120, 140, 0, 0);  
        this->m_light_hdl_p->SetLedArea(0, 140, 1, (LedRow::LED_ROW_NOF - 1));  
        this->m_light_hdl_p->Show();
    }
}


//*****************************************************************************
// description:
//   LightScene_MoBa_Enter
//*****************************************************************************
void LightSceneHdl::LightScene_MoBa_Enter(uint16_t brightness)
{
    this->m_light_hdl_p->SetBrightness_Fade(brightness);
    this->m_light_hdl_p->Clear();
}


//*****************************************************************************
// description:
//   LightScene_MoBa_Task
//*****************************************************************************
void LightSceneHdl::LightScene_MoBa_Task(void)
{
    if (millis() - this->m_task_timestamp_ms > TASK_SceneMoBa_TmoMs)
    {
        this->m_task_timestamp_ms = millis();

        this->m_light_hdl_p->SetLedArea(0, LedRow::LED_ROW_LENGTH, 0, 0);  
        this->m_light_hdl_p->SetLedArea(0, 30, 1, 2);  
        this->m_light_hdl_p->SetLedArea(LedRow::LED_ROW_LENGTH - 30, LedRow::LED_ROW_LENGTH, 1, 2);  
        this->m_light_hdl_p->SetLedArea(0, LedRow::LED_ROW_LENGTH, 3, 3);  
        this->m_light_hdl_p->Show();
    }
}


//*****************************************************************************
// description:
//   Show White Pixel
//*****************************************************************************
void LightSceneHdl::LightScene_LightOn_Enter(uint16_t brightness)
{
    this->m_light_hdl_p->SetBrightness_Fade(brightness);
    this->m_light_hdl_p->Clear();
}


//*****************************************************************************
// description:
//   Show White Pixel
//*****************************************************************************
void LightSceneHdl::LightScene_LightOn_Task(void)
{
    if (millis() - this->m_task_timestamp_ms > TASK_SceneLightOn_TmoMs)
    {
        this->m_task_timestamp_ms = millis();
        
        //uint32_t color = Adafruit_NeoPixel::Color(0, 0, 0, 255);
        this->m_light_hdl_p->SetLedArea(0, LedRow::LED_ROW_LENGTH, 0, (LedRow::LED_ROW_NOF - 1));  
        this->m_light_hdl_p->Show();
    }
}


//*****************************************************************************
// description:
//   Disco Task
//*****************************************************************************
void LightSceneHdl::LightScene_Disco_Task(void)
{
    if (millis() - this->m_task_timestamp_ms > TASK_SceneDisco_TmoMs)
    {
        this->m_task_timestamp_ms = millis();

        /*m_pixel->clear();
        m_pixel->setPixelColor(m_moving_dot_current_px, Adafruit_NeoPixel::Color(0, 0, 0, this->m_light_hdl_p->m_desired_brightness));
        m_pixel->show();
        m_moving_dot_current_px++;
        if (m_moving_dot_current_px >= m_nof_px)
        {
            m_moving_dot_current_px = 0;
        }*/

        srand(millis());

        uint16_t col_start = rand() % LedRow::LED_ROW_LENGTH;
        uint16_t col_end = col_start + (rand() % LedRow::LED_ROW_LENGTH);
        uint16_t row_start = rand() % LedRow::LED_ROW_NOF;
        uint16_t row_end = row_end + rand() % LedRow::LED_ROW_NOF;
        uint32_t color = 0;
        /*switch (rand() % 3)
        {
            case 0:
                color = Adafruit_NeoPixel::Color(rand() % 255, rand() % 255, 0, 0);
                break;

            case 1:
                color = Adafruit_NeoPixel::Color(rand() % 255, 0, rand() % 255, 0);
                break;

            case 2:
                color = Adafruit_NeoPixel::Color(0, rand() % 255, rand() % 255, 0);
                break;

            default:
                break;
        }*/

        switch (rand() % 12)
        {
            case 0:
                color = Adafruit_NeoPixel::Color(255, 0, 0, 0);
                break;

            case 1:
                color = Adafruit_NeoPixel::Color(0, 255, 0, 0);
                break;

            case 2:
                color = Adafruit_NeoPixel::Color(0, 0, 255, 0);
                break;

            case 3:
                color = Adafruit_NeoPixel::Color(255, 255, 0, 0);
                break;

            case 4:
                color = Adafruit_NeoPixel::Color(255, 0, 255, 0);
                break;

            case 5:
                color = Adafruit_NeoPixel::Color(0, 255, 255, 0);
                break;

            case 6:
                color = Adafruit_NeoPixel::Color(255, 127, 0, 0);
                break;

            case 7:
                color = Adafruit_NeoPixel::Color(127, 255, 0, 0);
                break;

            case 8:
                color = Adafruit_NeoPixel::Color(255, 0, 127, 0);
                break;

            case 9:
                color = Adafruit_NeoPixel::Color(127, 0, 255, 0);
                break;

            case 10:
                color = Adafruit_NeoPixel::Color(0, 255, 127, 0);
                break;

            case 11:
                color = Adafruit_NeoPixel::Color(0, 127, 255, 0);
                break;

            default:
                break;
        }

        this->m_light_hdl_p->SetLedArea(col_start, col_end, row_start, row_end, color);
        this->m_light_hdl_p->Show();
    }
}


//*****************************************************************************
// description:
//   Show Rainbow with moving white dots
//*****************************************************************************
void LightSceneHdl::LightScene_WhiteOverRainbow_Task(void) 
{
    if (millis() - this->m_task_timestamp_ms > TASK_SceneRainbow_TmoMs)
    {
        this->m_task_timestamp_ms = millis();

        uint32_t color = 0;
        uint16_t pixelHue = 0;

        for (uint32_t idx = 0; idx < LedRow::LED_ROW_LENGTH; idx++)   // For each pixel in strip...
        {
            pixelHue = this->m_rainbow_firstPixelHue + (idx * 65536L / LedRow::LED_ROW_LENGTH);
            color = Adafruit_NeoPixel::gamma32(Adafruit_NeoPixel::ColorHSV(pixelHue));
            this->m_light_hdl_p->SetLedArea(idx, idx, 0, (LedRow::LED_ROW_NOF - 1), color);  
        }

        // Update strip with new contents
        this->m_light_hdl_p->Show();
        this->m_rainbow_firstPixelHue += 542; // Advance just a little along the color wheel
    }
}


//*****************************************************************************
// description:
//   Power Off Task
//*****************************************************************************
void LightSceneHdl::LightScene_LightOff_Task(void)
{
    if (millis() - this->m_task_timestamp_ms > TASK_SceneLightOff_TmoMs)
    {
        this->m_task_timestamp_ms = millis();

        this->m_light_hdl_p->Show();
    }
}


//*****************************************************************************
// description:
//   GetUserSettingArea
//*****************************************************************************
void LightSceneHdl::GetUserSettingArea(LedArea* area_p)
{
    this->m_scene_userSetting_p->GetLedArea(area_p);
}


//*****************************************************************************
// description:
//   SetUserSettingArea
//*****************************************************************************
void LightSceneHdl::SetUserSettingArea(uint16_t xs, uint16_t xe, uint16_t ys, uint16_t ye)
{
    if (this->m_scene != LightScene::UserSetting)
    {
        this->ChangeLightScene(LightScene::UserSetting);
    }

    LedArea area;
    area.Set(xs, xe, ys, ye, this->m_light_hdl_p->GetColor());
    this->m_scene_userSetting_p->SetLedArea(&area);
}


//*****************************************************************************
// description:
//   Set Brightness
//*****************************************************************************
void LightSceneHdl::SetBrightness(uint8_t brightness)
{
    if (this->m_scene == LightScene::LightOff)
    {
        this->m_scene = this->m_last_scene;
        this->ChangeLightScene(m_scene, brightness);
    }
    else
    {
        this->m_light_hdl_p->SetBrightness_Fade(brightness);
    }
}


