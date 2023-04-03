
/*******************************************************************************
Project   RoomLight

  This is an OpenSource Project.
  You can use, share or improve this project. If you improve this source code
  please share with the comunity or at least with the author of the original
  source code

  Created 03. April 2023 by MarkusNTrains
================================================================================
$HeadURL:  $
$Id:  $
*******************************************************************************/


#ifndef _WEBSITE_H
#define _WEBSITE_H


//-----------------------------------------------------------------------------
// includes
#include <Ethernet.h>


//----------------------------------------------------------------------------
// Function Prototypes
void Website_SendToClient(EthernetClient* client);



#endif  // _WEBSITE_H
