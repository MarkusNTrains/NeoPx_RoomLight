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
    this->m_light_hdl_p = new LightHdl();
    this->m_scene_cloud_p = new LightScene_Cloud(this, this->m_light_hdl_p);
    this->m_scene_lightning_p = new LightScene_Lightning(this, this->m_light_hdl_p);
    this->m_scene_sun_p = new LightScene_Sun(this, this->m_light_hdl_p);
    this->m_update_time_ms = 0;

#if (ROOM_LIGHT == ROOM_LIGHT_MARKUSNTRAINS)
    this->ChangeLightScene(LightScene::OfficeTable, 100);
#else
    this->ChangeLightScene(LightScene::LightOn, 150);
#endif
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
    this->m_light_hdl_p->SetBrightness_Fade(brightness);
    
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
            break;
        
        case LightScene::LightOff:
            this->m_light_hdl_p->SetBrightness_Fade(0);
            break;
        
        case LightScene::Disco:
            this->m_light_hdl_p->Clear();
            break;

        case LightScene::MoBa:
            this->LightScene_MoBa_Enter(brightness);
            break;

        case LightScene::Night:
            this->m_scene_sun_p->Night_Enter();
            break;
          
        case LightScene::OfficeTable:
            this->LightScene_OfficeTable_Enter(brightness);
            break;
        
        case LightScene::Rainbow:
            this->m_light_hdl_p->Clear();
            this->m_rainbow_firstPixelHue = 0;
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
            this->m_light_hdl_p->Clear();
            break;

        default:
            break;
    }
}


//*****************************************************************************
// description:
//   Statemachine
//*****************************************************************************
void LightSceneHdl::Tasks()
{
    if (millis() - this->m_update_time_ms > TMO_TILL_NEXT_UPDATE_MS)
    {
        this->m_update_time_ms += TMO_TILL_NEXT_UPDATE_MS;
        this->m_light_hdl_p->UpdateBrightness();

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
                LightScene_UserSetting_Task();
                break;

            default:
                break;
        }
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
    this->m_light_hdl_p->SetLedArea(0, 20, 0, 0);  
    this->m_light_hdl_p->SetLedArea(120, 140, 0, 0);  
    this->m_light_hdl_p->SetLedArea(0, 140, 1, (LedRow::LED_ROW_NOF - 1));  
    this->m_light_hdl_p->Show();
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
    this->m_light_hdl_p->SetLedArea(0, LedRow::LED_ROW_LENGTH, 0, 0);  
    this->m_light_hdl_p->SetLedArea(0, 30, 1, 2);  
    this->m_light_hdl_p->SetLedArea(LedRow::LED_ROW_LENGTH - 30, LedRow::LED_ROW_LENGTH, 1, 2);  
    this->m_light_hdl_p->SetLedArea(0, LedRow::LED_ROW_LENGTH, 3, 3);  
    this->m_light_hdl_p->Show();
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
    //uint32_t color = Adafruit_NeoPixel::Color(0, 0, 0, 255);
    this->m_light_hdl_p->SetLedArea(0, LedRow::LED_ROW_LENGTH, 0, (LedRow::LED_ROW_NOF - 1));  
    this->m_light_hdl_p->Show();
}


//*****************************************************************************
// description:
//   MovingDot_Task
//*****************************************************************************
void LightSceneHdl::LightScene_Disco_Task(void)
{
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


//*****************************************************************************
// description:
//   Show Rainbow with moving white dots
//*****************************************************************************
void LightSceneHdl::LightScene_WhiteOverRainbow_Task(void) 
{
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


//*****************************************************************************
// description:
//   Power Off
//*****************************************************************************
void LightSceneHdl::LightScene_LightOff_Task(void)
{
    this->m_light_hdl_p->Show();
}


//*****************************************************************************
// description:
//   User Setting
//*****************************************************************************
void LightSceneHdl::LightScene_UserSetting_Task(void)
{
    this->m_light_hdl_p->UpdateLedArea();    
    this->m_light_hdl_p->Show();    
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


