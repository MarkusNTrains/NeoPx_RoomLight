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
#include "src/common.h"
#include "src/LightScene/LightSceneHdl.h"
#include "src/Webserver/WebServer.h"

#ifdef __AVR__
  #include <avr/wdt.h>
#endif


//-----------------------------------------------------------------------------
// defines


//-----------------------------------------------------------------------------
// extern


//-----------------------------------------------------------------------------
// static module variable
static LightSceneHdl* s_lightSceneHdl_p;
static WebServer* s_webServer_p;

#if (IS_DEBUG_MODE == ON)
const static uint32_t MAIN_PrintFreeMemoryTmoMs = 2000;
static uint32_t s_main_printFreeMemoryTimestampMs = 0;
#endif


//-----------------------------------------------------------------------------
// function prototype
uint16_t GetAvailableMemory();



//*****************************************************************************
// description:
//   initialize
//*****************************************************************************
void setup()
{
#if (IS_DEBUG_MODE == ON)
    Serial.begin(115200);       // for debugging
    Serial.println(F("\n\n************************************"));
    Serial.println(F("Start Room Light"));
    Serial.print(F("Free Memory: "));
    Serial.println(GetAvailableMemory());
    delay(50);
 #endif
    
    s_lightSceneHdl_p = new LightSceneHdl();
    s_webServer_p = new WebServer(s_lightSceneHdl_p);

    wdt_enable(WDTO_8S);

#if (IS_DEBUG_MODE == ON)
    s_main_printFreeMemoryTimestampMs = millis();
    Serial.print(F("Free Memory: "));
    Serial.println(GetAvailableMemory());
#endif
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


#if (IS_DEBUG_MODE == ON)
    if (millis() - s_main_printFreeMemoryTimestampMs > MAIN_PrintFreeMemoryTmoMs)
    {
        s_main_printFreeMemoryTimestampMs = millis();
        Serial.print(F("Free Memory: "));
        Serial.println(GetAvailableMemory());
    }
#endif
}


#if (IS_DEBUG_MODE == ON)
//*****************************************************************************
// description:
//   GetAvailableMemory
//*****************************************************************************
uint16_t GetAvailableMemory() 
{
    // Use 1024 with ATmega168
    uint16_t size = 8192; // SRAM size of target
    byte *buf;
    while ((buf = (byte *) malloc(--size)) == NULL);
        free(buf);

    return size;
}
#endif


