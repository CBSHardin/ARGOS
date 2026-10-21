

#ifndef _ON_UPDATE_CALLBACK_MANAGER_H__
#define _ON_UPDATE_CALLBACK_MANAGER_H__


#include "mathTypes.h"

#include <stdint.h>

#include <FreeRTOS.h>
#include <task.h>

//Lo más sencillo, seria registrar delegados en vez de callbacks, pero eso
//oscurece muchísimo el código. Así que, este módulo operará por debajo
//de las funciones de registro de callbacks de cada objeto. Basicamente,
//lo que se mantendrá en las tablas de registro serán delegados, y lo que se
//manipularan serán delegados, pero el usuario nunca los verá. Así la magia
//estara bien protegida.


template <class D, uint8_t MAX_NUMBER_OF_DELEGATES>
class onUpdateCallbackManager
{

  public:

    bool registerOnUpdateCallbackDelegate(D delegado)
    {

        bool ret = false;

        //Critical section
        taskENTER_CRITICAL();

          for(uint8_t i = 0 ; i < MAX_NUMBER_OF_DELEGATES ; i++)
          {

            if(delegatesTable[i].entryIsOccupied == false)
            {
              delegatesTable[i].delegate = delegado;

              delegatesTable[i].entryIsOccupied = true;

              ret = true;

              break;

            }

          }

        taskEXIT_CRITICAL();

        return ret;

    }



    bool unregisterOnUpdateCallback(D delegate)
    {

      bool ret = false;

      //Critical section
      taskENTER_CRITICAL();

        for(uint8_t i = 0 ; i < MAX_NUMBER_OF_DELEGATES ; i++)
        {

          if(delegatesTable[i].delegate == delegate)
          {

            delegatesTable[i].entryIsOccupied = false;

            ret = true;

            break;

          }

        }

      taskEXIT_CRITICAL();

      return ret;

    }


/*
  bool registerOnUpdateNotification(uint32_t expectedNotification)

  bool unregisterOnUpdateNotification() = 0;

  bool waitUntilGetSample() = 0;
*/


    //No parameters callback, just a signal
    inline void call()
    {

      for(uint8_t i = 0 ; i < MAX_NUMBER_OF_DELEGATES ; i++)
      {

        //Critical section
        taskENTER_CRITICAL();

        if(delegatesTable[i].entryIsOccupied)
          delegatesTable[i].delegate();

        taskEXIT_CRITICAL();

      }

    }



    //One parameter
    template <typename P1>
    inline void call(P1 param1)
    {

      for(uint8_t i = 0 ; i < MAX_NUMBER_OF_DELEGATES ; i++)
      {

        //Critical section
        taskENTER_CRITICAL();

        if(delegatesTable[i].entryIsOccupied)
          delegatesTable[i].delegate(param1);

        taskEXIT_CRITICAL();

      }

    }



    //Two parameter
    template <typename P1, typename P2>
    inline void call(P1 param1, P2 param2)
    {

      for(uint8_t i = 0 ; i < MAX_NUMBER_OF_DELEGATES ; i++)
      {

        //Critical section
        taskENTER_CRITICAL();

        if(delegatesTable[i].entryIsOccupied)
          delegatesTable[i].delegate(param1, param2);

        taskEXIT_CRITICAL();

      }

    }



    //Three parameter
    template <typename P1, typename P2, typename P3>
    inline void call(P1 param1, P2 param2, P3 param3)
    {

      for(uint8_t i = 0 ; i < MAX_NUMBER_OF_DELEGATES ; i++)
      {

        //Critical section
        taskENTER_CRITICAL();

        if(delegatesTable[i].entryIsOccupied)
          delegatesTable[i].delegate(param1, param2, param3);

        taskEXIT_CRITICAL();

      }

    }



    //Four parameter
    template <typename P1, typename P2, typename P3, typename P4>
    inline void call(P1 param1, P2 param2, P3 param3, P4 param4)
    {

      for(uint8_t i = 0 ; i < MAX_NUMBER_OF_DELEGATES ; i++)
      {

        //Critical section
        taskENTER_CRITICAL();

        if(delegatesTable[i].entryIsOccupied)
          delegatesTable[i].delegate(param1, param2, param3, param4);

        taskEXIT_CRITICAL();

      }

    }



    //Five parameter
    template <typename P1, typename P2, typename P3, typename P4, typename P5>
    inline void call(P1 param1, P2 param2, P3 param3, P4 param4, P5 param5)
    {

      for(uint8_t i = 0 ; i < MAX_NUMBER_OF_DELEGATES ; i++)
      {

        //Critical section
        taskENTER_CRITICAL();

        if(delegatesTable[i].entryIsOccupied)
          delegatesTable[i].delegate(param1, param2, param3, param4, param5);

        taskEXIT_CRITICAL();

      }

    }



    //Six parameter
    template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
    inline void call(P1 param1, P2 param2, P3 param3, P4 param4, P5 param5, P6 param6)
    {

      for(uint8_t i = 0 ; i < MAX_NUMBER_OF_DELEGATES ; i++)
      {

        //Critical section
        taskENTER_CRITICAL();

        if(delegatesTable[i].entryIsOccupied)
          delegatesTable[i].delegate(param1, param2, param3, param4, param5, param6);

        taskEXIT_CRITICAL();

      }

    }





  private:

    typedef struct
    {
      bool entryIsOccupied;
      D delegate;
    }
    tableEntry;

    //Delegate table
    tableEntry delegatesTable[MAX_NUMBER_OF_DELEGATES];


};


#endif
