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


#ifndef _PARAMETER_H
#define _PARAMETER_H


//----------------------------------------------------------------------------
// include
#include <stdint.h>


//----------------------------------------------------------------------------
// enum


//----------------------------------------------------------------------------
// class
class Parameter
{
	public:
        Parameter(uint32_t default_value, uint32_t min, uint32_t max, uint8_t width);
        ~Parameter();
		

        uint32_t GetValue() {
            return this->m_value;
        }
        void SetValue(uint32_t value) {
            this->m_value = value;
        }
        uint32_t GetMin() {
            return this->m_min;
        }
        uint32_t GetMax() {
            return this->m_max;
        }
        uint8_t GetWidth() {
            return this->m_width;
        }
        uint16_t GetAddr() {
            return this->m_addr;
        }
        void SetAddr(uint16_t addr) {
            this->m_addr = addr;
        }
        void Reset();

    private:
        uint32_t m_value;
        uint32_t m_default_value;
        uint32_t m_min;
        uint32_t m_max;
        uint8_t m_width;
        uint16_t m_addr;
};

#endif  // _PARAMETER_H
