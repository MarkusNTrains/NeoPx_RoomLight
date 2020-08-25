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
#include "WebServer.h"


//-----------------------------------------------------------------------------
// defines
#define IS_DEBUG_MODE


//-----------------------------------------------------------------------------
// static module variable
// MAC address from Ethernet shield sticker under board
static LedScene* s_led_scene;
static WebServer* s_web_server;



//*****************************************************************************
// description:
//   initialize
//*****************************************************************************
void setup()
{
    delay(50);
    s_led_scene = new LedScene();
    s_web_server = new WebServer(s_led_scene);
}


//*****************************************************************************
// description:
//   Statemachine
//*****************************************************************************
void loop()
{
    //s_led_strip->Tasks();
    s_web_server->Tasks();
}
