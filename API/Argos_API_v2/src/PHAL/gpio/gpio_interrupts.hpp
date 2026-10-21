
#ifndef _GPIO_INTERRUPTS_PHAL_H__
#define _GPIO_INTERRUPTS_PHAL_H__

#include <avr/interrupt.h>
#include <avr/io.h>

#include "../../utils/delegate/delegate.h"

#include <Arduino.h>
#include "gpio_interrupts_pins.hpp"

namespace Argos
{

class GpioInterruptsPHAL
{

  public:

    typedef enum : uint8_t
    {
      int0,
      int1,
      int2,
      int3,
      int4,
      int5,
      int6,
      int7,
      pcint0, // = 8
      pcint1,
      pcint2,
      pcint3,
      pcint4,
      pcint5,
      pcint6,
      pcint7,
      pcint8,
      pcint9, // = 16
      pcint10,
      pcint11,
      pcint12,
      pcint13,
      pcint14,
      pcint15,
      pcint16,
      pcint17,
      pcint18,
      pcint19,
      pcint20,
      pcint21,
      pcint22,
      pcint23,
      NOT_INTERRUPT
    }
    IntSignal_t;


    typedef enum : uint8_t
    {
      LOW_LEVEL = LOW,    //Only available for INTx interrupts
      ANY_EDGE = CHANGE,
      RISING_EDGE = RISING,
      FALLING_EDGE = FALLING
    }
    TriggerMode_t;

    void enable(IntSignal_t intSig, delegate<void> callbackDelegate, TriggerMode_t mode);

    void disable(IntSignal_t intSig);

    void changeTriggerMode(IntSignal_t intSig, TriggerMode_t mode);


    inline static interruptRecv(IntSignal_t intID)
    {

      //Call the delegate
      mCallbacksTable[intID]();

    }


    //Received interrupt signal
    inline static void int0Signal()
    {

      interruptRecv(int0);

    }

    inline static void int1Signal()
    {

      interruptRecv(int1);

    }

    inline static void int2Signal()
    {

      interruptRecv(int2);

    }

    inline static void int3Signal()
    {

      interruptRecv(int3);

    }

    inline static void int4Signal()
    {

      interruptRecv(int4);

    }

    inline static void int5Signal()
    {

      interruptRecv(int5);

    }

    inline static void int6Signal()
    {

      interruptRecv(int6);

    }

    inline static void int7Signal()
    {

      interruptRecv(int7);

    }


    inline static void pcint0_7Signal(uint8_t actualPCINTState)
    {

      uint8_t triggeredInterruptions = 0;

      triggeredInterruptions |= actualPCINTState & (~mPortLastStatePCINT0_7) & mInterruptRisignEdgeMaskPCINT0_7;

      triggeredInterruptions |= (~actualPCINTState) & mPortLastStatePCINT0_7 & mInterruptFallingEdgeMaskPCINT0_7;

      uint8_t intIndex = IntSignal_t::pcint0;


      for(uint8_t i = 1 ; i != 0 ; i = i << 1 )
      {

        if(triggeredInterruptions & i)
        {

          interruptRecv(intIndex);

        }

        ++intIndex;

      }

      mPortLastStatePCINT0_7 = actualPCINTState;

    }



    inline static void pcint8_15Signal(uint8_t actualPCINTState)
    {

      uint8_t triggeredInterruptions = 0;

      triggeredInterruptions |= actualPCINTState & (~mPortLastStatePCINT8_15) & mInterruptRisignEdgeMaskPCINT8_15;

      triggeredInterruptions |= (~actualPCINTState) & mPortLastStatePCINT8_15 & mInterruptFallingEdgeMaskPCINT8_15;

      uint8_t intIndex = IntSignal_t::pcint8;

      for(uint8_t i = 1 ; i != 0 ; i = i << 1 )
      {

        if(triggeredInterruptions & i)
        {

          interruptRecv(intIndex);

        }

        ++intIndex;

      }

      mPortLastStatePCINT8_15 = actualPCINTState;

    }



    inline static void pcint16_23Signal(uint8_t actualPCINTState)
    {

      uint8_t triggeredInterruptions = 0;

      triggeredInterruptions |= actualPCINTState & (~mPortLastStatePCINT16_23) & mInterruptRisignEdgeMaskPCINT16_23;

      triggeredInterruptions |= (~actualPCINTState) & mPortLastStatePCINT16_23 & mInterruptFallingEdgeMaskPCINT16_23;

      uint8_t intIndex = IntSignal_t::pcint16;


      for(uint8_t i = 1 ; i != 0 ; i = i << 1 )
      {

        if(triggeredInterruptions & i)
        {

          interruptRecv(intIndex);

        }

        ++intIndex;

      }

      mPortLastStatePCINT16_23 = actualPCINTState;

    }


    //void getTableSize()
    //{

    //  Serial.println("Tamaño tabla: "+String(sizeof(mCallbacksTable)));
    //}


  private:


    static delegate<void> mCallbacksTable[IntSignal_t::pcint23 + 1];

    static uint8_t mInterruptRisignEdgeMaskPCINT0_7,  mInterruptRisignEdgeMaskPCINT8_15,  mInterruptRisignEdgeMaskPCINT16_23;

    static uint8_t mInterruptFallingEdgeMaskPCINT0_7,  mInterruptFallingEdgeMaskPCINT8_15,  mInterruptFallingEdgeMaskPCINT16_23;

    static uint8_t mPortLastStatePCINT0_7, mPortLastStatePCINT8_15, mPortLastStatePCINT16_23;



};


extern GpioInterruptsPHAL GpioInterrupts;

}

#endif
