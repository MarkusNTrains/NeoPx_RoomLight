/*******************************************************************************
Project   RoomLight

  This is an OpenSource Project.
  You can use, share or improve this project. If you improve this source code
  please share with the comunity or at least with the author of the original 
  source code
  
  Created 20. April 2023 by MarkusNTrains
================================================================================
$HeadURL:  $
$Id:  $
*******************************************************************************/


//-----------------------------------------------------------------------------
// includes
#include "Parameter.h"



//*****************************************************************************
// description:
//   constructor
//*****************************************************************************
Parameter::Parameter(uint32_t default_value, uint32_t min, uint32_t max, uint8_t width)
{
    this->m_value = default_value;
    this->m_default_value = default_value;
    this->m_min = min;
    this->m_max = max;
    this->m_width = width;
    this->m_addr = 0;    
}


//*****************************************************************************
// description:
//   destructor
//*****************************************************************************
Parameter::~Parameter()
{
}


//*****************************************************************************
// description:
//   set value to default value
//*****************************************************************************
void Parameter::Reset()
{
    this->m_value = this->m_default_value;
}


//*****************************************************************************
// description:
//   set value
// return:
//   true if value did change
//*****************************************************************************
bool Parameter::SetValue(uint32_t value) 
{
    if (this->m_value != value)
    {
        this->m_value = value;
        return true;
    }

    return false;
}
