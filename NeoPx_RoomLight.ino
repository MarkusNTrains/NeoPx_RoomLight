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
#include "common.h"
#include "LightSceneHdl.h"
#include "WebServer.h"

#include <avr/wdt.h>



//-----------------------------------------------------------------------------
// defines


//-----------------------------------------------------------------------------
// static module variable
// MAC address from Ethernet shield sticker under board
static LightSceneHdl* s_lightSceneHdl_p;
static WebServer* s_webServer_p;



//*****************************************************************************
// description:
//   initialize
//*****************************************************************************
void setup()
{
  #if (IS_DEBUG_MODE == ON)
    Serial.begin(115200);       // for debugging
  #endif
    
    delay(50);
    s_lightSceneHdl_p = new LightSceneHdl();
    s_webServer_p = new WebServer(s_lightSceneHdl_p);

    wdt_enable(WDTO_2S);
}


//*****************************************************************************
// description:
//   Statemachine
//*****************************************************************************
void loop()
{
    s_lightSceneHdl_p->Tasks();
    s_webServer_p->Tasks();
    wdt_reset();
}
