
#ifndef _GPIO_INTERRUPTS_PHAL_H__
#define _GPIO_INTERRUPTS_PHAL_H__

#include <avr/interrupt.h>
#include <avr/io.h>

#include "../../utils/delegate/delegate.h"

#include <Arduino.h>


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
      pcint23
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


    void getTableSize()
    {

      Serial.println("Tamaño tabla: "+String(sizeof(mCallbacksTable)));
    }


  private:


    delegate<void> mCallbacksTable[IntSignal_t::pcint23 + 1];

    uint8_t mInterruptRisignEdgeMaskPCINT0_7,  mInterruptRisignEdgeMaskPCINT8_15,  mInterruptRisignEdgeMaskPCINT16_23;

    uint8_t mInterruptFallingEdgeMaskPCINT0_7,  mInterruptFallingEdgeMaskPCINT8_15,  mInterruptFallingEdgeMaskPCINT16_23;


    uint8_t mPortLastStatePCINT0_7, mPortLastStatePCINT8_15, mPortLastStatePCINT16_23;



};





inline static void ISRhandler(uint8_t ISRentryIndex)
{


}


/*
pcint0.enableInterrupt(handler, type);
pcint1.enableInterrupt(handler, type);


pcint.disableInterrupt();


int1.enableInterrupt(handler, type);


gpioInterrupts.enable(interruptSignal_t::INT0, callbackBUtton, RISING_EDGE);
*/

extern GpioInterruptsPHAL gpioInterrupts;


#endif
