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


#ifndef _WEBSERVER_H
#define _WEBSERVER_H


//----------------------------------------------------------------------------
// include
#include <SPI.h>
#include <Ethernet.h>
#include <SD.h>



//----------------------------------------------------------------------------
// define
#define REQ_BUF_SZ 60  // size of buffer used to capture HTTP requests


//----------------------------------------------------------------------------
// class
class Webserver
{
	public:
		Webserver(void);
		~Webserver();
    void Tasks(void);
		
		
	private:		
		File webFile;                     // the web page file on the SD card
		char HTTP_req[REQ_BUF_SZ] = {0};  // buffered HTTP request stored as null terminated string
		char req_index = 0;               // index into HTTP_req buffer
		unsigned char LED_state[3] = {0}; // stores the states of the LEDs, 1 bit per LED

		void SetLEDs(void);
		void XML_response(EthernetClient cl);
		void StrClear(char *str, char length);
		char StrContains(char *str, char *sfind);
};

#endif  // _WEBSERVER_H
