/*******************************************************************************
Project   RoomLight

  This is an OpenSource Project.
  You can use, share or improve this project. If you improve this source code
  please share with the comunity or at least with the author of the original 
  source code
  
  Created 04. September 2019 by MarkusNTrains
================================================================================
$HeadURL:  $
$Id:  $
*******************************************************************************/



//-----------------------------------------------------------------------------
// includes
#include "LedStrip.h"
#include "WebServer.h"


//-----------------------------------------------------------------------------
// defines
#define IS_DEBUG_MODE


//-----------------------------------------------------------------------------
// static module variable
// MAC address from Ethernet shield sticker under board
static WebServer* s_web_server;
static LedStrip* s_led_strip;



//*****************************************************************************
// description:
//   initialize
//*****************************************************************************
void setup()
{
#ifdef IS_DEBUG_MODE
  Serial.begin(115200);
#endif

  s_led_strip = new LedStrip(6, 16, 2);
  //s_web_server = new WebServer();

  s_led_strip->ShowOfficeTableWW(20);
}


//*****************************************************************************
// description:
//   Statemachine
//*****************************************************************************
void loop()
{
  s_led_strip->Tasks();
  //s_web_server->Tasks();
}
