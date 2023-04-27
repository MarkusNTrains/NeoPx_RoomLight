/*******************************************************************************
Project   RoomLight

  This is an OpenSource Project.
  You can use, share or improve this project. If you improve this source code
  please share with the comunity or at least with the author of the original 
  source code
  
  Created 4. November 2021 by MarkusNTrains
================================================================================
$HeadURL:  $
$Id:  $
*******************************************************************************/


#ifndef _LED_AREA_H
#define _LED_AREA_H


//----------------------------------------------------------------------------
// include
#include "common.h"


//----------------------------------------------------------------------------
// define


//----------------------------------------------------------------------------
// class
class LedArea
{
	  public:
      LedArea();
      ~LedArea();
		
        uint16_t GetColumnStart(void);
        uint16_t GetColumnEnd(void);
        uint16_t GetRowStart(void);
        uint16_t GetRowEnd(void);
        uint32_t GetColor(void);
        void Get(LedArea* area);
        void Set(LedArea* area);
        void Set(uint16_t xs, uint16_t xe, uint16_t ys, uint16_t ye);
        
    private:    
        //--- Memeber Variable ---
        uint16_t m_xs;
        uint16_t m_xe;
        uint16_t m_ys;
        uint16_t m_ye;
};

#endif  // _LED_ROW_H
