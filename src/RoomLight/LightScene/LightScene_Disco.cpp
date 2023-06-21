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
#include "LightScene_Disco.h"



//-----------------------------------------------------------------------------
// typedef



//*****************************************************************************
// description:
//   constructor
//*****************************************************************************
LightScene_Disco::LightScene_Disco(LightHdl* light_hdl, Datastore* datastore_p) : 
    LightScene(light_hdl, datastore_p, TASK_TmoMs, Parameter::Id::SceneDisco_Brightness, Parameter::Id::Unknown)
{
}


//*****************************************************************************
// description:
//   destructor
//*****************************************************************************
LightScene_Disco::~LightScene_Disco()
{
}


//*****************************************************************************
// description:
//   Enter
//*****************************************************************************
void LightScene_Disco::Enter()
{
    this->m_light_hdl_p->SetLedArea_DoNotChangeBlackLED(0, LedRow::LED_ROW_NOF, 0, LedRow::LED_ROW_LENGTH, Adafruit_NeoPixel::Color(0, 0, 1, 0));
    this->m_light_hdl_p->SetBrightness_Fade((uint8_t)this->m_datastore_p->GetParameter(this->m_brightness_param_id));
    //this->m_light_hdl_p->SetColor(this->m_datastore_p->GetParameter(this->m_color_param_id));
    this->TaskHdl();
}


//*****************************************************************************
// description:
//   Exit
//*****************************************************************************
void LightScene_Disco::Exit()
{
}


//*****************************************************************************
// description:
//   Task
// return:
//   true if LightHdl::Show() needs to be called, else false
//*****************************************************************************
bool LightScene_Disco::TaskHdl()
{
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

    this->m_light_hdl_p->SetLedArea_DoNotChangeBlackLED(col_start, col_end, row_start, row_end, color);
    
    return true;
}


