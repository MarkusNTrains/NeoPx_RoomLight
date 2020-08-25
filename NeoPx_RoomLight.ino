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
  #ifdef IS_DEBUG_MODE
    Serial.begin(9600);       // for debugging
  #endif
    
    delay(50);
    s_led_scene = new LedScene();
    //s_web_server = new WebServer(s_led_scene);



    /*Adafruit_NeoPixel test(900, 22, NEO_GRB + NEO_KHZ800);
    test.begin();
    test.clear();
    test.setBrightness(255);
    test.show();

    test.setPixelColor(10, test.Color(0,25,0,0));
    test.show();
    Serial.println("static");*/
}


//*****************************************************************************
// description:
//   Statemachine
//*****************************************************************************
void loop()
{
    //s_led_scene->Tasks();
    //s_web_server->Tasks();
}
