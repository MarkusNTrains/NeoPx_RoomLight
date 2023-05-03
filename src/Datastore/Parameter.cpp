/*##############################################################################

  THIS IS GENERATED CODE (use RoomLight_Parameter.ods)
    ==> DO NOT EDIT MANUALLY !!!

##############################################################################*/


/*******************************************************************************
Project   RoomLight

  This is an OpenSource Project.
  You can use, share or improve this project. If you improve this source code
  please share with the comunity or at least with the author of the original
  source code

  Created 27.04.2023 00:00:15 by MarkusNTrains
================================================================================
$HeadURL: parameter.h $
$Id: Parameter 27.04.2023 00:00:15 automatic gen. $
*******************************************************************************/

/*
Defines the Parameter of RoomLight device and their properties.
 Usage of this file allows to have configuration parameter specific information
 only within this file. See RoomLight_Parameter.ods for more Information.
*/

//------------------------------------------------------------------------------
// include
#include "Parameter.h"
#include <Arduino.h>




//*****************************************************************************
// description:
//   constructor
//*****************************************************************************
Parameter::Parameter()
{

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
//   set parameter to default value
//*****************************************************************************
void Parameter::Reset(Id id)
{
    if (id >= Id::Nof) {
        return;  // invalid parameter id
    }

    parameter_t param;
    memcpy_P(&param, &PARAM_TABLE[id], sizeof(parameter_t));

    uint16_t addr = param.addr;

    switch (param.width)
    {
        case 4:
            this->m_buffer[addr++] = (param.default_value >> 24) & 0xFF;
        case 3:
            this->m_buffer[addr++] = (param.default_value >> 16) & 0xFF;
        case 2:
            this->m_buffer[addr++] = (param.default_value >> 8) & 0xFF;
        case 1:
            this->m_buffer[addr++] = (param.default_value) & 0xFF;
            break;

        default:
            break;
    }
}


//*****************************************************************************
// description:
//   set all parameter to default value
//*****************************************************************************
void Parameter::ResetAll()
{
    for (uint8_t id = 0; id < Parameter::Id::Nof; id++)
    {
        this->Reset(id);
    }
}


//*****************************************************************************
// description:
//   get value
// return:
//   value
//*****************************************************************************
uint32_t Parameter::GetValue(Id id)
{
    if (id >= Id::Nof) {
        return;  // invalid parameter id
    }

    parameter_t param;
    memcpy_P(&param, &PARAM_TABLE[id], sizeof(parameter_t));

    uint16_t addr = param.addr;
    uint32_t value = 0;

    switch (param.width)
    {
        case 4:
            value |= ((uint32_t)this->m_buffer[addr++] << 24) & 0xFF000000;
        case 3:
            value |= ((uint32_t)this->m_buffer[addr++] << 16) & 0xFF0000;
        case 2:
            value |= ((uint32_t)this->m_buffer[addr++] << 8) & 0xFF00;
        case 1:
            value |= ((uint32_t)this->m_buffer[addr++]) & 0xFF;
            break;

        default:
            break;
    }

    return value;
}


//*****************************************************************************
// description:
//   set value
// return:
//   true if value did change
//*****************************************************************************
bool Parameter::SetValue(Id id, uint32_t value) 
{
    if (id >= Id::Nof) {
        return;  // invalid parameter id
    }

    parameter_t param;
    memcpy_P(&param, &PARAM_TABLE[id], sizeof(parameter_t));

    if (value < param.min) {
        value = param.min;
    }
    if (value > param.max) {
        value = param.max;
    }

    if (this->GetValue(id) != value)
    {
        uint16_t addr = param.addr;
        switch (param.width)
        {
            case 4:
                this->m_buffer[addr++] = (uint8_t)((value >> 24) & 0xFF);
            case 3:
                this->m_buffer[addr++] = (uint8_t)((value >> 16) & 0xFF);
            case 2:
                this->m_buffer[addr++] = (uint8_t)((value >> 8) & 0xFF);
            case 1:
                this->m_buffer[addr++] = (uint8_t)((value) & 0xFF);
                break;

            default:
                break;
        }
        return true;
    }

    return false;
}


//*****************************************************************************
// description:
//   GetAddr
//*****************************************************************************
static uint16_t Parameter::GetAddr(Id id)
{
    if (id >= Id::Nof) {
        return;  // invalid parameter id
    }

    parameter_t param;
    memcpy_P(&param, &PARAM_TABLE[id], sizeof(parameter_t));

    return param.addr;
}


//*****************************************************************************
// description:
//   GetWidth
//*****************************************************************************
static uint8_t Parameter::GetWidth(Id id)
{
    if (id >= Id::Nof) {
        return;  // invalid parameter id
    }

    parameter_t param;
    memcpy_P(&param, &PARAM_TABLE[id], sizeof(parameter_t));

    return param.width;
}


