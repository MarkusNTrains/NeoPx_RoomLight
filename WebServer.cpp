/*--------------------------------------------------------------
  Program:      eth_websrv_SD_Ajax_24_out

  Description:  Arduino web server allows 24 outputs to be
                switched on and off using checkboxes.
                The web page is stored on the micro SD card.
  
  Hardware:     Arduino MEGA 2560 and official Arduino Ethernet
                shield.
                Tested with 24 LEDs connected from pin 26 to 49
                Can't use pins 50 to 53 when Ethernet shield
                plugged in, these pins are for SPI.
                2Gb micro SD card formatted FAT16.
                
  Software:     Developed using Arduino 1.0.6 software
                Should be compatible with Arduino 1.0 +
                SD card contains web page called index.htm
  
  References:   - WebServer example by David A. Mellis and 
                  modified by Tom Igoe
                - SD card examples by David A. Mellis and
                  Tom Igoe
                - Ethernet library documentation:
                  http://arduino.cc/en/Reference/Ethernet
                - SD Card library documentation:
                  http://arduino.cc/en/Reference/SD
                - Based on code from the Ethernet shield
                  tutorial:
                  http://startingelectronics.org/tutorials/arduino/ethernet-shield-web-server-tutorial/SD-card-IO/

  Date:         4. September 2019
 
  Author:       W.A. Smith, http://startingelectronics.org
                enhanced by MarkusNTrains
--------------------------------------------------------------*/


//-----------------------------------------------------------------------------
// includes
#include "WebServer.h"


//-----------------------------------------------------------------------------
// static module variable



//*****************************************************************************
// description:
//   constructor
//*****************************************************************************
WebServer::WebServer(LedScene* led_scene)
{
    //int pin;
		
    byte mac[] = { 0x10, 0x0D, 0x7F, 0xBF, 0xCA, 0x49 }; // MAC address from Ethernet shield sticker under board
    IPAddress ip(192, 168, 0, 250);    // IP address, may need to change depending on network
    //IPAddress ip(192, 168, 1, 249);    // IP address, may need to change depending on network
    m_server = new EthernetServer(80);            // server
    m_led_scene = led_scene;
    
    // disable Ethernet chip slave select
    pinMode(10, OUTPUT);
    digitalWrite(10, HIGH);
    
    // initialize SD card
  #ifdef IS_DEBUG_MODE
    Serial.println("Initializing SD card...");
  #endif
    if (!SD.begin(4)) 
    {
      #ifdef IS_DEBUG_MODE
        Serial.println("ERROR - SD card initialization failed!");
      #endif
        return;    // init failed
    }
  #ifdef IS_DEBUG_MODE
    Serial.println("SUCCESS - SD card initialized.");
  #endif
    
    // check for index.htm file
    if (!SD.exists("index.htm")) 
    {
      #ifdef IS_DEBUG_MODE
        Serial.println("ERROR - Can't find index.htm file!");
      #endif
        return;  // can't find index file
    }
  #ifdef IS_DEBUG_MODE
    Serial.println("SUCCESS - Found index.htm file.");
  #endif
    
    
    // pins 26 to 49 are outputs
    /*for (pin = 26; pin <= 49; pin++) 
    {
        pinMode(pin, OUTPUT);    // set pins as outputs
        digitalWrite(pin, LOW);  // switch the output pins off
    }*/

    Ethernet.begin(mac, ip);  // initialize Ethernet device


    // start the server
    m_server->begin();           // start to listen for clients

  #ifdef IS_DEBUG_MODE
    Serial.print("server is at ");
    Serial.println(Ethernet.localIP());     
  #endif
}


//*****************************************************************************
// description:
//   destructor
//*****************************************************************************
WebServer::~WebServer()
{

}


//*****************************************************************************
// description:
//   Statemachine
//*****************************************************************************
void WebServer::Tasks()
{
    EthernetClient client = m_server->available();  // try to get client

    if (client)   // got client?
	{
        boolean currentLineIsBlank = true;
        while (client.connected()) 
		{
            if (client.available())    // client data available to read
			{
                char c = client.read(); // read 1 byte (character) from client
  #ifdef IS_DEBUG_MODE
                Serial.print(c);
  #endif
                // limit the size of the stored received HTTP request
                // buffer first part of HTTP request in HTTP_req array (string)
                // leave last element in array as 0 to null terminate string (REQ_BUF_SZ - 1)
                if (req_index < (REQ_BUF_SZ - 1)) 
			    {
                    HTTP_req[req_index] = c;          // save HTTP request character
                    req_index++;
                }
                
                // last line of client request is blank and ends with \n
                // respond to client only after last line received
                if (c == '\n' && currentLineIsBlank) 
			    {
                    // send a standard http response header
                    client.println("HTTP/1.1 200 OK");
                    // remainder of header follows below, depending on if
                    // web page or XML page is requested
                    // Ajax request - send XML file
                    if (StrContains(HTTP_req, "ajax_inputs")) 
			        {
                        // send rest of HTTP header
                        client.println("Content-Type: text/xml");
                        client.println("Connection: keep-alive");
                        client.println();

                        this->HandleRequest();
                        
                        // send XML file containing input states
                        XML_response(client);
                    }
                    else   // web page request
			        {
                        // send rest of HTTP header
                        client.println("Content-Type: text/html");
                        client.println("Connection: keep-alive");
                        client.println();

                        // send web page
                        webFile = SD.open("index.htm");        // open web page file
                        if (webFile) 
			            {
                            while(webFile.available()) 
			                {
                                client.write(webFile.read()); // send web page to client
                            }
                            webFile.close();
                        }
                    }
                    
                    // display received HTTP request on serial port
                    //Serial.print(HTTP_req);
                    // reset buffer index and all buffer elements to 0
                    req_index = 0;
                    StrClear(HTTP_req, REQ_BUF_SZ);
                    break;
                }
                
                // every line of text received from the client ends with \r\n
                if (c == '\n') 
				{
                    // last character on line of received text
                    // starting new line with next character read
                    currentLineIsBlank = true;
                } 
                else if (c != '\r') 
				{
                    // a text character was received from client
                    currentLineIsBlank = false;
                }
            } 
        } 
        
        delay(50);      // give the web browser time to receive the data
        client.stop(); // close the connection
    } // end if (client)
}


//*****************************************************************************
// description:
//   checks if received HTTP request is switching on/off LEDs
//   also saves the state of the LEDs
//*****************************************************************************
void WebServer::HandleRequest(void)
{
    char param_c[8];
    uint16_t param = 0;
    uint16_t param2 = 0;
    uint16_t param3 = 0;
    uint16_t param4 = 0;
    char* start_pos = 0;
    char* end_pos = 0;
    uint16_t cnt = 0;
    char needle_scene[] = "LightScene";
    char needle_brightness[] = "SetBrightness";
    char needle_color[] = "SetColor";
    char needle_set_led_area[] = "SetArea";

    // find LightScene ---------------------------------------------------------
    if (StrContains(HTTP_req, needle_scene))
    {
        this->m_action = ACTION_SetLightSecene;
        
        start_pos = strstr(HTTP_req, needle_scene);
        if (start_pos == 0) return;  // no String found

        end_pos = strstr(start_pos, "&");
        if (end_pos == 0) return;  // no String found

        start_pos += sizeof(needle_scene);

        for (cnt = 0; cnt < end_pos - start_pos; cnt++)
        {
            param_c[cnt] = start_pos[cnt];
            param_c[cnt+1] = '\0';
        }
        param = atoi(param_c);

        this->m_led_scene->ChangeLightScene(param, 255);
    }
    
    // find brightness ---------------------------------------------------------
    else if (StrContains(HTTP_req, needle_brightness))
    {
        this->m_action = ACTION_SetBrightness;
        
        start_pos = strstr(HTTP_req, needle_brightness);
        if (start_pos == 0) return;  // no String found

        end_pos = strstr(start_pos, "&");
        if (end_pos == 0) return;  // no String found

        start_pos += sizeof(needle_brightness);      

        for (cnt = 0; cnt < end_pos - start_pos; cnt++)
        {
            param_c[cnt] = start_pos[cnt];
            param_c[cnt+1] = '\0';
        }
        param = atoi(param_c);

        this->m_led_scene->SetBrightness(param);
    }
    
    // find set led area data -------------------------------------------------
    else if (StrContains(HTTP_req, needle_set_led_area))
    {
        this->m_action = ACTION_SetLedArea;
        
        start_pos = strstr(HTTP_req, needle_set_led_area);
        if (start_pos == 0) return;  // no String found

        end_pos = strstr(start_pos, "-");
        if (end_pos == 0) return;  // no String found

        start_pos += (sizeof(needle_set_led_area) + 1);      

        for (cnt = 0; cnt < end_pos - start_pos; cnt++)
        {
            param_c[cnt] = start_pos[cnt];
            param_c[cnt+1] = '\0';
        }
        param = atoi(param_c);        

        start_pos = end_pos+1;
        end_pos = strstr(start_pos, "Y");
        if (end_pos == 0) return;  // no String found

        for (cnt = 0; cnt < end_pos - start_pos; cnt++)
        {
            param_c[cnt] = start_pos[cnt];
            param_c[cnt+1] = '\0';
        }
        param2 = atoi(param_c);
        
        start_pos = end_pos+1;
        end_pos = strstr(start_pos, "-");
        if (end_pos == 0) return;  // no String found

        for (cnt = 0; cnt < end_pos - start_pos; cnt++)
        {
            param_c[cnt] = start_pos[cnt];
            param_c[cnt+1] = '\0';
        }
        param3 = atoi(param_c);
        
        start_pos = end_pos+1;
        end_pos = strstr(start_pos, "&");
        if (end_pos == 0) return;  // no String found

        for (cnt = 0; cnt < end_pos - start_pos; cnt++)
        {
            param_c[cnt] = start_pos[cnt];
            param_c[cnt+1] = '\0';
        }
        param4 = atoi(param_c);
        
      #ifdef IS_DEBUG_MODE
        Serial.println(param);
        Serial.println(param2);
        Serial.println(param3);
        Serial.println(param4);
      #endif

        uint32_t color = Adafruit_NeoPixel::Color(0, 0, 0, 255);
        this->m_led_scene->ChangeLightScene(USER_SETTING);
        this->m_led_scene->SetLedArea(param, param2, param3, param4, color);
    }

    else
    {
        this->m_action = ACTION_Unknown;
    }

    
    /*for (i = 0; i < 3; i++) 
	  {
        for (j = 1; j <= 0x80; j <<= 1) 
		    {
            sprintf(str_on,  "LED%d=%d", LED_num, 1);
            sprintf(str_off, "LED%d=%d", LED_num, 0);
            if (StrContains(HTTP_req, str_on)) 
			      {
                LED_state[i] |= (unsigned char)j;  // save LED state
                digitalWrite(LED_num + 25, HIGH);
                Serial.print(LED_num);
                Serial.println(" ON");
            }
            else if (StrContains(HTTP_req, str_off)) 
			      {
                LED_state[i] &= (unsigned char)(~j);  // save LED state
                digitalWrite(LED_num + 25, LOW);
                Serial.print(LED_num);
                Serial.println(" OFF");
            }
            LED_num++;
        }
    }*/    
}



//*****************************************************************************
// description:
//   send the XML file with analog values, switch status and LED status
//*****************************************************************************
void WebServer::XML_response(EthernetClient cl)
{
    unsigned char i;
    unsigned int  j;
    
    cl.print("<?xml version = \"1.0\" ?>");
    cl.print("<inputs>");

    cl.print("<scene>");
    cl.print(m_led_scene->GetLightScene());
    cl.print("</scene>");

    if (this->m_action != ACTION_SetBrightness)
    {
        cl.print("<brightness>");
        cl.print(m_led_scene->GetBrightness());
        cl.print("</brightness>");
    }

    /*cl.print("<led_area>");
    cl.print(this->m_led_scene->GetLedRowStart());
    cl.print(this->m_led_scene->GetLedRowEnd());
    cl.print(this->m_led_scene->GetLedColumnStart());
    cl.print(this->m_led_scene->GetLedColumnEnd());
    cl.print("</led_area>");*/
  
    /*for (i = 0; i < 3; i++) 
	  {
        for (j = 1; j <= 0x80; j <<= 1) 
		    {
            cl.print("<LED>");
            if ((unsigned char)LED_state[i] & j) 
			      {
                cl.print("checked");
                //Serial.println("ON");
            }
            else 
			      {
                cl.print("unchecked");
            }
            cl.println("</LED>");
        }
    }
*/
    cl.print("</inputs>");
}


//*****************************************************************************
// description:
//   sets every element of str to 0 (clears array)
//*****************************************************************************
void WebServer::StrClear(char *str, char length)
{
    for (int i = 0; i < length; i++) 
	  {
        str[i] = 0;
    }
}


//*****************************************************************************
// description:
//   searches for the string sfind in the string str
// return:
//   returns 1 if string found
//   returns 0 if string not found
//*****************************************************************************
char WebServer::StrContains(char *str, char *sfind)
{
    char found = 0;
    char index = 0;
    char len;

    len = strlen(str);
    
    if (strlen(sfind) > len) 
    {
        return 0;
    }
    while (index < len) 
    {
        if (str[index] == sfind[found]) 
        {
            found++;
            if (strlen(sfind) == found) 
            {
                return 1;
            }
        }
        else 
        {
            found = 0;
        }
        index++;
    }

    return 0;
}
