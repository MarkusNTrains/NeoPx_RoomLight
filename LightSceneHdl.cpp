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
    this->m_scene_lightning_p = new LightScene_Lightning(this, this->m_light_hdl_p);
    this->m_update_time_ms = 0;

    this->ChangeLightScene(LIGHTSCENE_OfficeTableWW, 100);
}


//*****************************************************************************
// description:
//   destructor
//*****************************************************************************
LightSceneHdl::~LightSceneHdl()
{
    delete this->m_light_hdl_p;
    delete this->m_scene_lightning_p;
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
    this->SetBrightness(brightness);
    
    switch (scene)
    {
        case LIGHTSCENE_OfficeTableWW:
            this->LightScene_OfficeTableWW_Enter(brightness);
            break;
        
        case LIGHTSCENE_LightOnWW:
            this->LightScene_LightOnWW_Enter(brightness);
            break;
        
        case LIGHTSCENE_Sbh:
            //
            break;

        case LIGHTSCENE_Sunrise:
            this->m_sunrise_sun_height = 0;
            this->m_sunrise_sun_pos = 0;
            break;
            
        case LIGHTSCENE_Sunset:
            //m_pixel->setBrightness(brightness); // Set brigthness for all neo pixels
            break;
          
        case LIGHTSCENE_PowerOff:
            this->m_light_hdl_p->SetBrightness_Fade(0);
            break;
        
        case LIGHTSCENE_Rainbow:
            this->m_light_hdl_p->Clear();
            this->LightScene_WhiteOverRainbow_Task(10, 5);
            break;
            
        case LIGHTSCENE_MovingDot:
            break;

        case LIGHTSCENE_Lightning:
            this->m_scene_lightning_p->Enter();
            break;

        case LIGHTSCENE_MoBa:
            this->LightScene_MoBa_Enter(brightness);
            break;

        case LIGHTSCENE_UserSetting:
            this->m_light_hdl_p->Clear();
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
            case LIGHTSCENE_OfficeTableWW:
                LightScene_OfficeTableWW_Task();
                break;
        
            case LIGHTSCENE_LightOnWW:
                LightScene_LightOnWW_Task();
                break;
        
            case LIGHTSCENE_Sunrise:
                LightScene_Sunrise_Task();
                break;
            
            case LIGHTSCENE_PowerOff:
                LightScene_PowerOff_Task();
                break;

            case LIGHTSCENE_Rainbow:
                this->LightScene_WhiteOverRainbow_Task(75, 5);
                break;
            
            case LIGHTSCENE_MovingDot:
                LightScene_MovingDot_Task();
                break;
            
            case LIGHTSCENE_Idle:
                break;

            case LIGHTSCENE_UserSetting:
                LightScene_UserSetting_Task();
                break;

            case LIGHTSCENE_Lightning:
                this->m_scene_lightning_p->Task();
                break;
                
            case LIGHTSCENE_MoBa:
                this->LightScene_MoBa_Task();
                break;

            default:
                break;
        }
    }
}


//*****************************************************************************
// description:
//   LightScene_OfficeTableWW_Enter
//*****************************************************************************
void LightSceneHdl::LightScene_OfficeTableWW_Enter(uint16_t brightness)
{
    this->m_light_hdl_p->SetBrightness_Fade(brightness);
    this->m_light_hdl_p->Clear();
}


//*****************************************************************************
// description:
//   ShowOfficeTableWW_Task
//*****************************************************************************
void LightSceneHdl::LightScene_OfficeTableWW_Task(void)
{
    this->m_light_hdl_p->SetLedArea(0, 20, 0, 0);  
    this->m_light_hdl_p->SetLedArea(120, 140, 0, 0);  
    this->m_light_hdl_p->SetLedArea(0, 140, 1, LedRow::LED_ROW_NOF);  
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
void LightSceneHdl::LightScene_LightOnWW_Enter(uint16_t brightness)
{
    this->m_light_hdl_p->SetBrightness_Fade(brightness);
    this->m_light_hdl_p->Clear();
}


//*****************************************************************************
// description:
//   Show White Pixel
//*****************************************************************************
void LightSceneHdl::LightScene_LightOnWW_Task(void)
{
    //uint32_t color = Adafruit_NeoPixel::Color(0, 0, 0, 255);
    this->m_light_hdl_p->SetLedArea(0, LedRow::LED_ROW_LENGTH, 0, LedRow::LED_ROW_NOF);  
    this->m_light_hdl_p->Show();
}


//*****************************************************************************
// description:
//   Sunrise_Task
//*****************************************************************************
#define PIXEL_DISTANCE_MM 16  // distance between neo pixels in mm
#define PIXEL_BRIGHTNESS 100
#define SUN_MAX_HEIGHT 10000
void LightSceneHdl::LightScene_Sunrise_Task(void)
{
    uint16_t PIXEL_NOF = LedRow::LED_ROW_LENGTH;
    uint16_t pixel;
    uint16_t cnt;
    uint8_t red[PIXEL_NOF];
    uint8_t green[PIXEL_NOF];
    uint8_t blue[PIXEL_NOF];
    float brightness = 0;
    float asin_alpha = 0;
    uint32_t hypothenuse = 0;
    uint32_t color = 0;
    uint8_t tmp_color = 0;

    for (cnt = 0; cnt < PIXEL_NOF; cnt++)
    {
        hypothenuse = sqrt(pow(m_sunrise_sun_height, 2) + pow(PIXEL_DISTANCE_MM * cnt, 2));
        asin_alpha = (255 * m_sunrise_sun_height) / hypothenuse;
        brightness = asin_alpha / 255;

        red[cnt] = 255 * brightness;
        //red[(PIXEL_NOF - 1) - cnt] = red[cnt];

        green[cnt] = asin_alpha * brightness;
        if (green[cnt] > PIXEL_BRIGHTNESS) {
            green[cnt] = PIXEL_BRIGHTNESS;
        }
        //green[(PIXEL_NOF - 1) - cnt] = green[cnt];

        tmp_color = (green[cnt] * m_sunrise_sun_height) / (SUN_MAX_HEIGHT * 2);
        tmp_color = (cnt * (PIXEL_BRIGHTNESS / (PIXEL_NOF / 2))) + tmp_color;
        if (tmp_color > PIXEL_BRIGHTNESS) {
            tmp_color = PIXEL_BRIGHTNESS;
        }
        blue[cnt] = tmp_color * brightness;
        //blue[cnt] = ((green[cnt] * m_sunrise_sun_height) / (SUN_MAX_HEIGHT * 2)) * brightness;
        //blue[(PIXEL_NOF - 1) - cnt] =  blue[cnt];

        #ifdef IS_DEBUG_MODE
        Serial.print("c ");
        Serial.print(asin_alpha);
        Serial.print("  brigth ");
        Serial.print(brightness);
        Serial.print("  green ");
        Serial.println(green[cnt]);
        #endif
    }

    for (cnt = 0; cnt < PIXEL_NOF; cnt++)
    {
        pixel = (m_sunrise_sun_pos + cnt) % PIXEL_NOF;
        color = Adafruit_NeoPixel::Color(red[cnt], green[cnt], blue[cnt], 0);
        this->m_light_hdl_p->SetLedArea(pixel, pixel, 0, LedRow::LED_ROW_NOF, color);
    }

    // Send the updated pixel colors to the hardware.  
    this->m_light_hdl_p->SetBrightness_Instantly(255);
    this->m_light_hdl_p->Show();    

    if (m_sunrise_sun_height < SUN_MAX_HEIGHT)
    {
        m_sunrise_sun_height += 10;
    }
    else
    {
        this->m_light_hdl_p->SetColor(Adafruit_NeoPixel::Color(0,0,0,255));
        this->ChangeLightScene(LIGHTSCENE_MoBa);        
    }
    m_sunrise_sun_pos = m_sunrise_sun_pos % PIXEL_NOF;
}


//*****************************************************************************
// description:
//   MovingDot_Task
//*****************************************************************************
void LightSceneHdl::LightScene_MovingDot_Task(void)
{
    /*m_pixel->clear();
    m_pixel->setPixelColor(m_moving_dot_current_px, Adafruit_NeoPixel::Color(0, 0, 0, this->m_light_hdl_p->m_desired_brightness));
    m_pixel->show();
    m_moving_dot_current_px++;
    if (m_moving_dot_current_px >= m_nof_px)
    {
        m_moving_dot_current_px = 0;
    }*/
}


//*****************************************************************************
// description:
//   Show Rainbow with moving white dots
//*****************************************************************************
void LightSceneHdl::LightScene_WhiteOverRainbow_Task(int whiteSpeed, int whiteLength) 
{
    if (whiteLength >= LedRow::LED_ROW_LENGTH) 
    {
        whiteLength = LedRow::LED_ROW_LENGTH - 1;
    }
    
    int      head          = whiteLength - 1;
    int      tail          = 0;
    int      loops         = 3;
    int      loopNum       = 0;
    uint32_t lastTime      = millis();
    static uint32_t firstPixelHue = 0;
    uint32_t color = 0;

    for (int idx = 0; idx < LedRow::LED_ROW_LENGTH; idx++)   // For each pixel in strip...
    {
        /*if (((idx >= tail) && (idx <= head)) ||      //  If between head & tail...
         ((tail > head) && ((idx >= tail) || (idx <= head)))) 
        {
        color = Adafruit_NeoPixel::Color(0, 0, 0, 255);
        this->m_light_hdl_p->m_led_matrix->SetPixelArray(idx, idx, 0, LedRow::LED_ROW_NOF, color);  
        } 
        else */
        {                                             // else set rainbow
            int pixelHue = firstPixelHue + (idx * 65536L / LedRow::LED_ROW_LENGTH);
            color = Adafruit_NeoPixel::gamma32(Adafruit_NeoPixel::ColorHSV(pixelHue));
            this->m_light_hdl_p->SetLedArea(idx, idx, 0, LedRow::LED_ROW_NOF, color);  
        }
    }

    // Update strip with new contents
    this->m_light_hdl_p->Show();
    // There's no delay here, it just runs full-tilt until the timer and
    // counter combination below runs out.

    firstPixelHue += 542; // Advance just a little along the color wheel

    if ((millis() - lastTime) > whiteSpeed)  // Time to update head/tail?
    {
        if (++head >= LedRow::LED_ROW_LENGTH)       // Advance head, wrap around
        {
            head = 0;
            if (++loopNum >= loops) 
            { return; }
        }
        if (++tail >= LedRow::LED_ROW_LENGTH)    // Advance tail, wrap around
        {
            tail = 0;
        }
        lastTime = millis();                   // Save time of last movement
    }
}


//*****************************************************************************
// description:
//   Power Off
//*****************************************************************************
void LightSceneHdl::LightScene_PowerOff_Task(void)
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
    if (this->m_scene == LIGHTSCENE_PowerOff)
    {
        this->m_scene = this->m_last_scene;
        this->ChangeLightScene(m_scene, brightness);
    }
    else
    {
        this->m_light_hdl_p->SetBrightness_Fade(brightness);
    }
}


