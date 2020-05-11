/*******************************************************************************
Project   Train TimeTable

  This is an OpenSource Project.
  You can use, share or improve this project. If you improve this source code
  please share with the comunity or at least with the author of the original 
  source code
  
  Created 09. May 2019 by MarkusNTrains
================================================================================
$HeadURL:  $
$Id:  $
*******************************************************************************/


#ifndef _LED_STRIP_H
#define _LED_STRIP_H


//----------------------------------------------------------------------------
// include
#include "Arduino.h"
#include <Adafruit_NeoPixel.h>


//----------------------------------------------------------------------------
// define


//----------------------------------------------------------------------------
// class
class LedStrip
{
	public:
   typedef enum 
   {
      OFFICE_TABLE_WW = 0,
      LIGHT_ON_WW,
      DISCO,
      SUNSET,
      SUNRISE,
      POWER_OFF,
      MOVING_DOT,
      SBH,
      IDLE,
    } light_scene_t;
    
		LedStrip(uint8_t px_pin, uint16_t nof_px, uint8_t nof_row);
		~LedStrip();
    void Tasks(void);
    void ChangeLightScene(light_scene_t scene, uint8_t brightness);
    void SetBrightness(uint8_t brightness);
		
	private:	
    const uint32_t TMO_TILL_NEXT_UPDATE_MS = 20;
    
    light_scene_t m_light_scene;
    light_scene_t m_state;
    uint8_t m_current_brightness;
    uint8_t m_desired_brightness;
    uint32_t m_update_time_ms;
    uint16_t m_px;

    Adafruit_NeoPixel* m_pixel;  
    Adafruit_NeoPixel* m_pixel_sbh;  
    uint16_t m_nof_px;
    uint8_t m_nof_row;


    //void ShowOfficeTableWW_Enter(uint16_t brightness);
    void ShowOfficeTableWW_Task(void);
    //void LightOnWW_Enter(uint16_t brightness);
    void LightOnWW_Task(void);
    void SbhWW_Task(void);
    void Sunrise_Task(void);
    void PowerOff_Task(void);
    void MovingDot_Task(void);
    void SetPixel(uint16_t start_pos, uint16_t width, uint16_t space, uint16_t nof_repeat, uint32_t color);
    void UpdateBrightness(void);
};

#endif  // _LED_STRIP_H
