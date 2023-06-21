/*******************************************************************************
Project   RoomLight

  This is an OpenSource Project.
  You can use, share or improve this project. If you improve this source code
  please share with the comunity or at least with the author of the original 
  source code
  
  Created 25. April 2023 by MarkusNTrains
================================================================================
$HeadURL:  $
$Id:  $
*******************************************************************************/

#ifndef _LIGHT_SOURCE_H
#define _LIGHT_SOURCE_H


//----------------------------------------------------------------------------
// include
#include "common.h"
#include "Datastore/Datastore.h"



//----------------------------------------------------------------------------
// extern


//----------------------------------------------------------------------------
// class
class LightSource
{
    public:
        LightSource(Datastore* datastore_p);
        ~LightSource();

        virtual void Task() = 0;
        virtual uint8_t GetActiveScene() = 0;
        virtual void ChangeScene(uint8_t scene_id) = 0;
        virtual uint8_t GetBrightness() = 0;
        virtual void SetBrightness(uint8_t brightness) = 0;
        virtual uint32_t GetColor() = 0;
        virtual void SetColor(uint32_t color) = 0;

    protected:
        Datastore* m_datastore_p = nullptr;

    private:
};

#endif // _LIGHT_SOURCE_H
