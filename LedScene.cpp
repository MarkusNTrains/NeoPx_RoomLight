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
#include "LedScene.h"
#include "LedMatrix.h"


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
LedScene::LedScene()
{
    this->m_state = OFFICE_TABLE_WW;
    this->m_current_brightness = 0;
    this->m_desired_brightness = 0;
    this->m_color = COLOR_WHITE;
    this->m_update_time_ms = 0;
    this->m_led_matrix = new LedMatrix();
    this->m_led_area = new LedArea();

    this->ChangeLightScene(OFFICE_TABLE_WW, 100);
}


//*****************************************************************************
// description:
//   destructor
//*****************************************************************************
LedScene::~LedScene()
{
    delete this->m_led_matrix;
    delete this->m_led_area;
}


//*****************************************************************************
// description:
//   ChangeLightScene
//*****************************************************************************
void LedScene::ChangeLightScene(light_scene_t scene)
{
    this->ChangeLightScene(scene, this->m_desired_brightness);
}


//*****************************************************************************
// description:
//   ChangeLightScene
//*****************************************************************************
void LedScene::ChangeLightScene(light_scene_t scene, uint8_t brightness)
{
    m_light_scene = scene;
    m_state = scene;
    m_desired_brightness = brightness;
    
    switch (scene)
    {
        case OFFICE_TABLE_WW:
            this->ShowOfficeTableWW_Enter(brightness);
            break;
        
        case LIGHT_ON_WW:
            this->LightOnWW_Enter(brightness);
            break;
        
        case SBH:
            //
            break;
        
        case SUNRISE:
            //m_pixel->setBrightness(brightness); // Set brigthness for all neo pixels
            break;
          
        case POWER_OFF:
            this->m_desired_brightness = 0;
            break;
        
        case RAINBOW:
            this->m_led_matrix->Clear();
            this->WhiteOverRainbow_Task(10, 5);
            break;
            
        case MOVING_DOT:
            this->m_state = MOVING_DOT;
            break;
          
        default:
            break;
    }
}


//*****************************************************************************
// description:
//   Get Light Scene
//*****************************************************************************
light_scene_t LedScene::GetLightScene(void)
{
    return this->m_light_scene;
}


//*****************************************************************************
// description:
//   Statemachine
//*****************************************************************************
void LedScene::Tasks()
{
    if (millis() - this->m_update_time_ms > TMO_TILL_NEXT_UPDATE_MS)
    {
        this->m_update_time_ms += TMO_TILL_NEXT_UPDATE_MS;
    
        switch (this->m_state)
        {
            case OFFICE_TABLE_WW:
                ShowOfficeTableWW_Task();
                break;
        
            case LIGHT_ON_WW:
                LightOnWW_Task();
                break;
        
            case SUNRISE:
                Sunrise_Task();
                break;
            
            case POWER_OFF:
                PowerOff_Task();
                break;

            case RAINBOW:
                this->WhiteOverRainbow_Task(75, 5);
                break;
            
            case MOVING_DOT:
                MovingDot_Task();
                break;
            
            case IDLE:
                break;

            case USER_SETTING:
                UserSetting_Task();
                break;
                
            default:
                break;
        }
    }
}


//*****************************************************************************
// description:
//   Show White Pixel
//*****************************************************************************
void LedScene::ShowOfficeTableWW_Enter(uint16_t brightness)
{
    this->m_desired_brightness = brightness;
    this->m_led_matrix->Clear();
}


//*****************************************************************************
// description:
//   ShowOfficeTableWW_Task
//*****************************************************************************
void LedScene::ShowOfficeTableWW_Task(void)
{
    //uint32_t color = Adafruit_NeoPixel::Color(0, 0, 0, 255);
    this->m_led_matrix->SetPixelArray(0, 20, 0, 0, this->m_color);  
    this->m_led_matrix->SetPixelArray(120, 140, 0, 0, this->m_color);  
    this->m_led_matrix->SetPixelArray(0, 140, 1, 3, this->m_color);  
    this->UpdateBrightness();
    this->m_led_matrix->Show();
}


//*****************************************************************************
// description:
//   Show White Pixel
//*****************************************************************************
void LedScene::LightOnWW_Enter(uint16_t brightness)
{
    this->m_desired_brightness = brightness;
    this->m_led_matrix->Clear();
}


//*****************************************************************************
// description:
//   Show White Pixel
//*****************************************************************************
void LedScene::LightOnWW_Task(void)
{
    //uint32_t color = Adafruit_NeoPixel::Color(0, 0, 0, 255);
    this->m_led_matrix->SetPixelArray(0, LedRow::LED_ROW_LENGTH, 0, 3, this->m_color);  
    this->UpdateBrightness();
    this->m_led_matrix->Show();
}


//*****************************************************************************
// description:
//   Sunrise_Task
//*****************************************************************************
#define PIXEL_DISTANCE_MM 16  // distance between neo pixels in mm
#define PIXEL_BRIGHTNESS 100
#define SUN_MAX_HEIGHT 1000
static int32_t s_sun_height = 0;
static uint32_t s_sun_pos = 0;
void LedScene::Sunrise_Task(void)
{
 /* uint16_t pixel;
  uint16_t cnt;
  uint8_t red[m_nof_px];
  uint8_t green[m_nof_px];
  uint8_t blue[m_nof_px];
  float brightness = 0;
  float asin_alpha = 0;
  uint32_t hypothenuse = 0;

  for (cnt = 0; cnt < ((m_nof_px + 1) / 2); cnt++)
  {
    hypothenuse = sqrt(pow(s_sun_height, 2) + pow(PIXEL_DISTANCE_MM * cnt, 2));
    asin_alpha = (255 * s_sun_height) / hypothenuse;
    brightness = asin_alpha / 255;
    
    red[cnt] = 255 * brightness;
    red[(m_nof_px - 1) - cnt] = red[cnt];
    
    green[cnt] = asin_alpha * brightness;
    green[(m_nof_px - 1) - cnt] = green[cnt];
    
    blue[cnt] = ((green[cnt] * s_sun_height) / (SUN_MAX_HEIGHT * 2)) * brightness;
    blue[(m_nof_px - 1) - cnt] =  blue[cnt];
    
#ifdef IS_DEBUG_MODE
    Serial.print("c ");
    Serial.print(hypothenuse);
    Serial.print("  brigth ");
    Serial.print(brightness);
    Serial.print("  green ");
    Serial.println(green[cnt]);
#endif
  }
  
  for (cnt = 0; cnt < m_nof_px; cnt++)
  {
    pixel = (s_sun_pos + cnt) % m_nof_px;
    m_pixel->setPixelColor(pixel, m_pixel->Color(red[cnt], green[cnt], blue[cnt]));
  }

  m_pixel->show();   // Send the updated pixel colors to the hardware.  

  if (s_sun_height < SUN_MAX_HEIGHT)
  {
    s_sun_height++;
  }
  s_sun_pos = s_sun_pos % m_nof_px;
*/
}


//*****************************************************************************
// description:
//   MovingDot_Task
//*****************************************************************************
void LedScene::MovingDot_Task(void)
{
    /*m_pixel->clear();
    m_pixel->setPixelColor(m_px, Adafruit_NeoPixel::Color(0, 0, 0, m_desired_brightness));
    m_pixel->show();
    m_px++;
    if (m_px >= m_nof_px)
    {
        m_px = 0;
    }*/
}


//*****************************************************************************
// description:
//   Show Rainbow with moving white dots
//*****************************************************************************
void LedScene::WhiteOverRainbow_Task(int whiteSpeed, int whiteLength) 
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
        this->m_led_matrix->SetPixelArray(idx, idx, 0, 3, color);  
        } 
        else */
        {                                             // else set rainbow
            int pixelHue = firstPixelHue + (idx * 65536L / LedRow::LED_ROW_LENGTH);
            color = Adafruit_NeoPixel::gamma32(Adafruit_NeoPixel::ColorHSV(pixelHue));
            this->m_led_matrix->SetPixelArray(idx, idx, 0, 3, color);  
        }
    }

    // Update strip with new contents
    this->UpdateBrightness();
    this->m_led_matrix->Show();
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
void LedScene::PowerOff_Task(void)
{
    this->UpdateBrightness();  
    this->m_led_matrix->Show();
}


//*****************************************************************************
// description:
//   User Setting
//*****************************************************************************
void LedScene::UserSetting_Task(void)
{
    this->UpdateBrightness();  
    this->m_led_matrix->SetPixelArray(this->m_led_area->GetColumnStart(), this->m_led_area->GetColumnEnd(), this->m_led_area->GetRowStart(), this->m_led_area->GetRowEnd(), this->m_color);    
    this->m_led_matrix->Show();    
}


//*****************************************************************************
// description:
//   Get Led Area
//*****************************************************************************
LedArea* LedScene::GetLedArea(void)
{
    return this->m_led_area;  
}


//*****************************************************************************
// description:
//   Set Led Area
//*****************************************************************************
void LedScene::SetLedArea(uint16_t xs, uint16_t xe, uint16_t ys, uint16_t ye)
{
    this->m_led_area->Set(xs, xe, ys, ye, this->m_color);
    this->m_led_matrix->Clear();
    this->m_led_matrix->SetPixelArray(xs, xe, ys, ye, this->m_color);    
}


//*****************************************************************************
// description:
//   Get Brightness
//*****************************************************************************
uint8_t LedScene::GetBrightness(void)
{
    return this->m_desired_brightness;
}


//*****************************************************************************
// description:
//   Set Brightness
//*****************************************************************************
void LedScene::SetBrightness(uint8_t brightness)
{
    this->ChangeLightScene(m_light_scene, brightness);
}


//*****************************************************************************
// description:
//   Update Brightness
//*****************************************************************************
void LedScene::UpdateBrightness(void)
{
    uint8_t factor = 10;
    
    if (this->m_current_brightness < this->m_desired_brightness)
    {
        if (((this->m_current_brightness + ((this->m_current_brightness / factor) + 1))) < this->m_desired_brightness)
        {
            this->m_current_brightness += (this->m_current_brightness / factor) + 1;
        }
        else
        {
            this->m_current_brightness = this->m_desired_brightness;
        }
    }
    else if (this->m_current_brightness > this->m_desired_brightness)
    {
        if (((this->m_current_brightness - ((this->m_current_brightness / factor) + 1))) > this->m_desired_brightness)
        {
            this->m_current_brightness -= (this->m_current_brightness / factor) + 1;
        }
        else
        {
            this->m_current_brightness = this->m_desired_brightness;      
        }
    }
    else
    {
        //this->m_state = IDLE;
    }    
    
    this->m_led_matrix->SetBrightness(this->m_current_brightness); // Set brigthness for all neo pixels
}


//*****************************************************************************
// description:
//   Get Color
//*****************************************************************************
uint32_t LedScene::GetColor(void)
{
    return this->m_color;
}


//*****************************************************************************
// description:
//   Set Color
// parameter:
//   color: RGBW --> 8888
//*****************************************************************************
void LedScene::SetColor(uint32_t color)
{
    this->m_color = color;
}
