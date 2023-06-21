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


#ifndef _WEB_SERVER_H
#define _WEB_SERVER_H


//----------------------------------------------------------------------------
// include
#include <SPI.h>
#include <Ethernet.h>
#include <SD.h>
#include "../common.h"
#include "../LightSourceHdl.h"



//-----------------------------------------------------------------------------
// Info
// PROGMEM -> store data in flash
//   to read out the stored data from PROGMEM use one macro of <avr/pgmspace.h>
//   - e.g. (char*)pgm_read_word(&(PARAM_IN_PROGMEM))
//   - or use memcpy_P
//      data_type_t param_SRAM
//      memcpy_P(&param_SRAM, &PARAM_IN_PROGMEM, sizeof(data_type_t))
//   more infos: https://www.arduino.cc/reference/en/language/variables/utilities/progmem/


//----------------------------------------------------------------------------
// define
#define USE_SD_CARD
#undef USE_SD_CARD


//----------------------------------------------------------------------------
// const
const static char WEBSERVER_Request_Needle_Source[] PROGMEM = "SetSource";
const static char WEBSERVER_Request_Needle_Scene[] PROGMEM = "SetScene";
const static char WEBSERVER_Request_Needle_brightness[] PROGMEM = "SetBrightness";
const static char WEBSERVER_Request_Needle_Color[] PROGMEM = "SetColor";
const static char WEBSERVER_Request_Needle_White[] PROGMEM = "SetWhite";
const static char WEBSERVER_Request_Needle_SetLedArea[] PROGMEM = "SetArea";
const static char WEBSERVER_Request_Needle_GetCurrentData[] PROGMEM = "GetCurrentData";
const static char WEBSERVER_Request_Needle_GetInfo[] PROGMEM = "GetInfo";
const static uint8_t WEBSERVER_Request_Needle_MaxLength = 16;  // musst longer than the longest needle + 1


//----------------------------------------------------------------------------
// class
class WebServer
{
    public:
        WebServer(LightSourceHdl* light_source_hdl);
        ~WebServer();
        void Tasks();		
        
    private:
        enum Action
        {
            ACTION_SetLightSource = 0,
            ACTION_SetLightScene,
            ACTION_SetBrightness,
            ACTION_SetColor,
            ACTION_SetWhite,
            ACTION_SetLedArea,
            ACTION_GetInfo,
            ACTION_Unknown
        };
        
        //--- task ---
        const static uint8_t TASK_RequestBufferLength = 100;     // size of read buffer (reads a complete line) 
        const static uint8_t TASK_SmallBufferSize = 50;          // a smaller buffer for results

        EthernetServer* m_server;
        File webFile;                                // the web page file on the SD card
        LightSourceHdl* m_lightSourceHdl_p;
        Action m_action;

        void HandleRequest(char* http_request);
        uint32_t HttpRequestExtractOneParameter(char* http_request, char* needle, uint8_t needle_length);
        void StrClear(char *str, char length);
        char StrContains(char *str, char *sfind);   
        void PrintHardwareInfo();
        void SendXML(EthernetClient* client);
        void SendWebPage(EthernetClient* client);
        void Send404(EthernetClient* client);
        void SendFavicon(EthernetClient* client);
};

#endif  // _WEB_SERVER_H
