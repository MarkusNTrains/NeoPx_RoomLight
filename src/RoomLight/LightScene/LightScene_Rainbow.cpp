/*******************************************************************************
Project   RoomLight

  This is an OpenSource Project.
  You can use, share or improve this project. If you improve this source code
  please share with the comunity or at least with the author of the original 
  source code
  
  Created 25. April 2023 by MarkusNTrains
================================================================================
$HeadURL:  $
$Id:  $
*******************************************************************************/


//-----------------------------------------------------------------------------
// includes
#include "LightScene_Rainbow.h"



//-----------------------------------------------------------------------------
// typedef



//*****************************************************************************
// description:
//   constructor
//*****************************************************************************
LightScene_Rainbow::LightScene_Rainbow(LightHdl* light_hdl, Datastore* datastore_p) : 
    LightScene(light_hdl, datastore_p, TASK_TmoMs, Parameter::Id::SceneRainbow_Brightness, Parameter::Id::Unknown)
{
}


//*****************************************************************************
// description:
//   destructor
//*****************************************************************************
LightScene_Rainbow::~LightScene_Rainbow()
{
}


//*****************************************************************************
// description:
//   Enter
//*****************************************************************************
void LightScene_Rainbow::Enter()
{
    this->m_firstPixelHue = 0;

    this->m_light_hdl_p->SetBrightness_Fade((uint8_t)this->m_datastore_p->GetParameter(this->m_brightness_param_id));
    this->TaskHdl();
}


//*****************************************************************************
// description:
//   Exit
//*****************************************************************************
void LightScene_Rainbow::Exit()
{
}


//*****************************************************************************
// description:
//   Task
// return:
//   true if LightHdl::Show() needs to be called, else false
//*****************************************************************************
bool LightScene_Rainbow::TaskHdl()
{
    uint32_t color = 0;
    uint16_t pixelHue = 0;

    for (uint32_t idx = 0; idx < LedRow::LED_ROW_LENGTH; idx++)   // For each pixel in strip...
    {
        pixelHue = this->m_firstPixelHue + (idx * 65536L / LedRow::LED_ROW_LENGTH);
        color = Adafruit_NeoPixel::gamma32(Adafruit_NeoPixel::ColorHSV(pixelHue));
        this->m_light_hdl_p->SetLedArea_DoNotChangeBlackLED(idx, idx, 0, (LedRow::LED_ROW_NOF - 1), color);  
    }

    // Update strip with new contents
    this->m_firstPixelHue += 542; // Advance just a little along the color wheel

    return true;
}


