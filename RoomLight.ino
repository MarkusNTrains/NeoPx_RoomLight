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
#include "Webserver.h"


//-----------------------------------------------------------------------------
// static module variable
// MAC address from Ethernet shield sticker under board
static Webserver* s_webserver;



//*****************************************************************************
// description:
//   initialize
//*****************************************************************************
void setup()
{
    s_webserver = new Webserver();
}


//*****************************************************************************
// description:
//   Statemachine
//*****************************************************************************
void loop()
{
    s_webserver->Tasks();
}
