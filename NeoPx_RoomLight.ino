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
// static module variable
// MAC address from Ethernet shield sticker under board
static WebServer* s_web_server;



//*****************************************************************************
// description:
//   initialize
//*****************************************************************************
void setup()
{
    s_web_server = new WebServer();
}


//*****************************************************************************
// description:
//   Statemachine
//*****************************************************************************
void loop()
{
    s_web_server->Tasks();
}
