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
#include "LightScene.h"
#include "WebServer.h"


//-----------------------------------------------------------------------------
// defines


//-----------------------------------------------------------------------------
// static module variable
// MAC address from Ethernet shield sticker under board
static LightSceneHdl* s_led_scene;
static WebServer* s_web_server;



//*****************************************************************************
// description:
//   initialize
//*****************************************************************************
void setup()
{
  #ifdef IS_DEBUG_MODE
    Serial.begin(115200);       // for debugging
  #endif
    
    delay(50);
    s_led_scene = new LightSceneHdl();
    s_web_server = new WebServer(s_led_scene);
}


//*****************************************************************************
// description:
//   Statemachine
//*****************************************************************************
void loop()
{
    s_led_scene->Tasks();
    s_web_server->Tasks();
}
