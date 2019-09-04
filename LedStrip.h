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
      POWER_ON = 0,
      IDLE,
    } state_t;
    
		LedStrip(uint8_t px_pin, uint16_t nof_px, uint8_t nof_row);
		~LedStrip();
    void Tasks(void);
		void ShowOfficeTableWarmWhite(uint16_t brightness);
		
	private:	
    const uint32_t TMO_TILL_NEXT_UPDATE_MS = 100;
    
    Adafruit_NeoPixel* m_pixel;	
    uint16_t m_nof_px;
    uint8_t m_nof_row;
    state_t m_state;
    uint8_t m_current_brightness;
    uint8_t m_desired_brightness;
    uint32_t m_update_time_ms;

    void SetPixel(uint16_t start_pos, uint16_t width, uint16_t space, uint16_t nof_repeat, uint32_t color);
};

#endif  // _LED_STRIP_H
