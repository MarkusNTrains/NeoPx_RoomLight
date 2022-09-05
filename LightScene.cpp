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
#include "LightScene.h"
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
LightSceneHdl::LightSceneHdl()
{
    this->m_current_brightness = 0;
    this->m_desired_brightness = 0;
    this->m_color = COLOR_WHITE;
    this->m_update_time_ms = 0;
    this->m_led_matrix = new LedMatrix();
    this->m_led_area = new LedArea();

    this->ChangeLightScene(LIGHTSCENE_OfficeTableWW, 100);
}


//*****************************************************************************
// description:
//   destructor
//*****************************************************************************
LightSceneHdl::~LightSceneHdl()
{
    delete this->m_led_matrix;
    delete this->m_led_area;
}


//*****************************************************************************
// description:
//   ChangeLightScene
//*****************************************************************************
void LightSceneHdl::ChangeLightScene(LightScene scene)
{
    this->ChangeLightScene(scene, this->m_desired_brightness);
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
    this->m_desired_brightness = brightness;
    
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
        
        case LIGHTSCENE_Sunset:
            //m_pixel->setBrightness(brightness); // Set brigthness for all neo pixels
            break;
          
        case LIGHTSCENE_PowerOff:
            this->m_desired_brightness = 0;
            break;
        
        case LIGHTSCENE_Rainbow:
            this->m_led_matrix->Clear();
            this->LightScene_WhiteOverRainbow_Task(10, 5);
            break;
            
        case LIGHTSCENE_MovingDot:
            break;

        case LIGHTSCENE_Lightning:
            this->m_lightning_state = LIGHTNING_STATE_Dimming;
            this->m_last_brightness = this->m_current_brightness;
            this->m_desired_brightness = LIGHTNING_BackgroundBrightness;
            break;

        case LIGHTSCENE_Moba:
            this->LightScene_MoBa_Enter(brightness);
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
//   Statemachine
//*****************************************************************************
void LightSceneHdl::Tasks()
{
    if (millis() - this->m_update_time_ms > TMO_TILL_NEXT_UPDATE_MS)
    {
        this->m_update_time_ms += TMO_TILL_NEXT_UPDATE_MS;
    
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
                LightScene_Lightning_Task();
                break;
                
            case LIGHTSCENE_Moba:
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
    this->m_desired_brightness = brightness;
    this->m_led_matrix->Clear();
}


//*****************************************************************************
// description:
//   ShowOfficeTableWW_Task
//*****************************************************************************
void LightSceneHdl::LightScene_OfficeTableWW_Task(void)
{
    this->m_led_matrix->SetPixelArray(0, 20, 0, 0, this->m_color);  
    this->m_led_matrix->SetPixelArray(120, 140, 0, 0, this->m_color);  
    this->m_led_matrix->SetPixelArray(0, 140, 1, 3, this->m_color);  
    this->UpdateBrightness();
    this->m_led_matrix->Show();
}


//*****************************************************************************
// description:
//   LightScene_MoBa_Enter
//*****************************************************************************
void LightSceneHdl::LightScene_MoBa_Enter(uint16_t brightness)
{
    this->m_desired_brightness = brightness;
    this->m_led_matrix->Clear();
}


//*****************************************************************************
// description:
//   LightScene_MoBa_Task
//*****************************************************************************
void LightSceneHdl::LightScene_MoBa_Task(void)
{
    this->m_led_matrix->SetPixelArray(0, LedRow::LED_ROW_LENGTH, 0, 0, this->m_color);  
    this->m_led_matrix->SetPixelArray(0, 30, 1, 2, this->m_color);  
    this->m_led_matrix->SetPixelArray(LedRow::LED_ROW_LENGTH - 30, LedRow::LED_ROW_LENGTH, 1, 2, this->m_color);  
    this->m_led_matrix->SetPixelArray(0, LedRow::LED_ROW_LENGTH, 3, 3, this->m_color);  
    this->UpdateBrightness();
    this->m_led_matrix->Show();
}


//*****************************************************************************
// description:
//   Show White Pixel
//*****************************************************************************
void LightSceneHdl::LightScene_LightOnWW_Enter(uint16_t brightness)
{
    this->m_desired_brightness = brightness;
    this->m_led_matrix->Clear();
}


//*****************************************************************************
// description:
//   Show White Pixel
//*****************************************************************************
void LightSceneHdl::LightScene_LightOnWW_Task(void)
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
void LightSceneHdl::LightScene_Sunrise_Task(void)
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
void LightSceneHdl::LightScene_MovingDot_Task(void)
{
    /*m_pixel->clear();
    m_pixel->setPixelColor(m_moving_dot_current_px, Adafruit_NeoPixel::Color(0, 0, 0, m_desired_brightness));
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
void LightSceneHdl::LightScene_PowerOff_Task(void)
{
    this->UpdateBrightness();  
    this->m_led_matrix->Show();
}


//*****************************************************************************
// description:
//   User Setting
//*****************************************************************************
void LightSceneHdl::LightScene_UserSetting_Task(void)
{
    this->UpdateBrightness();  
    this->m_led_matrix->SetPixelArray(this->m_led_area->GetColumnStart(), this->m_led_area->GetColumnEnd(), this->m_led_area->GetRowStart(), this->m_led_area->GetRowEnd(), this->m_color);    
    this->m_led_matrix->Show();    
}


//*****************************************************************************
// description:
//   Handle ligthing 
//*****************************************************************************
void LightSceneHdl::LightScene_Lightning_Task(void)
{
    switch (this->m_lightning_state)
    {
        case LIGHTNING_STATE_Dimming:
            if ((this->m_desired_brightness != m_current_brightness) || (this->m_color != COLOR_BLUE))
            {
                uint8_t color_change_factor = 20;
                uint8_t red = (this->m_color & COLOR_RED) >> 16;
                uint8_t green = (this->m_color & COLOR_GREEN) >> 8;
                uint8_t blue = (this->m_color & COLOR_BLUE);
                uint8_t white = (this->m_color & COLOR_WHITE) >> 24;
                
                red = this->UpdateValueTo(red, 0, color_change_factor);
                green = this->UpdateValueTo(green, 0, color_change_factor);
                blue = this->UpdateValueTo(blue, 0xFF, color_change_factor/2);
                white = this->UpdateValueTo(white, 0, color_change_factor);
                
                this->m_color = Adafruit_NeoPixel::Color(red, green, blue, white);
                this->m_led_matrix->SetPixelArray(0, LedRow::LED_ROW_LENGTH, 0, 3, this->m_color);
                this->UpdateBrightness();  
                this->m_led_matrix->Show();
            }
            else
            {
                // set overall brightness to maximum and the background color to current brightness, so that flashes will bi visible
                this->m_led_matrix->SetBrightness(255);
                this->m_led_matrix->SetPixelArray(0, LedRow::LED_ROW_LENGTH, 0, 3, Adafruit_NeoPixel::Color(0, 0, LIGHTNING_BackgroundBrightness, 0));
                this->m_led_matrix->Show();

                this->m_lightning_nof_flashes = rand() % 10;
                this->m_lightning_flash_counter = 0;
                this->m_lightning_flash_timestamp_ms = millis();
                this->m_lightning_flash_pause_ms = 5000;
                this->m_lightning_state = LIGHTNING_STATE_FlashActive;
            }
            break;

        case LIGHTNING_STATE_FlashActive:
            if (millis() >= this->m_lightning_flash_timestamp_ms + this->m_lightning_flash_pause_ms)
            {
                if (this->m_lightning_flash_counter >= this->m_lightning_nof_flashes)
                {
                    // exit lightning task
                    this->ChangeLightScene(this->m_last_scene, this->m_last_brightness);
                }
                else 
                {
                    // show next flash
                    srand(millis());
                    this->m_lightning_flash_timestamp_ms = millis();
                    this->m_lightning_flash_pause_ms = rand() % 5000;

                    uint32_t flash_color = Adafruit_NeoPixel::Color(0, 0, 0, 255);
                    uint8_t flash_start_pos = rand() % 2;
                    uint8_t flash_width = rand() % 10;
                    uint32_t flash_length_ms = rand() % LIGHTNING_MaxFlashLengthMs;

                    // show flash
                    this->m_led_matrix->SetPixelArray(flash_start_pos, flash_start_pos + flash_width, 0, 3, flash_color);
                    this->m_led_matrix->Show();
                    delay(flash_length_ms);

                    // hide flash
                    this->m_led_matrix->SetPixelArray(0, LedRow::LED_ROW_LENGTH, 0, 3, Adafruit_NeoPixel::Color(0, 0, LIGHTNING_BackgroundBrightness, 0));
                    this->m_led_matrix->Show();

                    this->m_lightning_flash_counter++;
                }
            }
            break;

        default:
            break;
    }
}


//*****************************************************************************
// description:
//   Get Led Area
//*****************************************************************************
LedArea* LightSceneHdl::GetLedArea(void)
{
    return this->m_led_area;  
}


//*****************************************************************************
// description:
//   Set Led Area
//*****************************************************************************
void LightSceneHdl::SetLedArea(uint16_t xs, uint16_t xe, uint16_t ys, uint16_t ye)
{
    this->m_led_area->Set(xs, xe, ys, ye, this->m_color);
    this->m_led_matrix->Clear();
    this->m_led_matrix->SetPixelArray(xs, xe, ys, ye, this->m_color);    
}


//*****************************************************************************
// description:
//   Get Brightness
//*****************************************************************************
uint8_t LightSceneHdl::GetBrightness(void)
{
    return this->m_desired_brightness;
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
    }

    this->ChangeLightScene(m_scene, brightness);
}


//*****************************************************************************
// description:
//   Update Brightness
//*****************************************************************************
void LightSceneHdl::UpdateBrightness(void)
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
        // IDLE;
    }    
    
    this->m_led_matrix->SetBrightness(this->m_current_brightness); // Set brigthness for all neo pixels
}


//*****************************************************************************
// description:
//   Get Color
//*****************************************************************************
uint32_t LightSceneHdl::GetColor(void)
{
    return this->m_color;
}


//*****************************************************************************
// description:
//   Set Color
// parameter:
//   color: RGBW --> 8888
//*****************************************************************************
void LightSceneHdl::SetColor(uint32_t color)
{
    this->m_color = color;
}


//*****************************************************************************
// description:
//   Get Color
// parameter:
//   current_value: 
//   desired_value: value to change
//   change_val: max change of value
//*****************************************************************************
uint8_t LightSceneHdl::UpdateValueTo(uint8_t current_value, uint8_t desired_value, uint8_t factor)
{
    if (current_value < desired_value)
    {
        if (((current_value + ((current_value / factor) + 1))) < desired_value)
        {
            current_value += (current_value / factor) + 1;
        }
        else
        {
            current_value = desired_value;
        }
    }
    else if (current_value > desired_value)
    {
        if (((current_value - ((current_value / factor) + 1))) > desired_value)
        {
            current_value -= (current_value / factor) + 1;
        }
        else
        {
            current_value = desired_value;      
        }
    }
    else
    {
        // IDLE;
    }  

    return current_value;  
}
