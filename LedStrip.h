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
		LedStrip(uint8_t px_pin, uint16_t nof_px, uint8_t nof_row);
		~LedStrip();
    void Tasks(void);
		void ShowWhite(uint16_t start_pos, uint16_t width, uint8_t brightness);
		
	private:	
    Adafruit_NeoPixel m_pixel;	
    uint16_t m_nof_px;
    uint8_t m_nof_row;
};

#endif  // _LED_STRIP_H
