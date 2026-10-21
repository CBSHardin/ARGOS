
#include "../gpio_interrupts.hpp"

#include <FreeRTOS.h>
#include <task.h>

#include <Arduino.h>
#include <avr/io.h>

using namespace Argos;

delegate<void> GpioInterruptsPHAL::mCallbacksTable[];

uint8_t GpioInterruptsPHAL::mInterruptRisignEdgeMaskPCINT0_7;
uint8_t GpioInterruptsPHAL::mInterruptRisignEdgeMaskPCINT8_15;
uint8_t GpioInterruptsPHAL::mInterruptRisignEdgeMaskPCINT16_23;

uint8_t GpioInterruptsPHAL::mInterruptFallingEdgeMaskPCINT0_7;
uint8_t GpioInterruptsPHAL::mInterruptFallingEdgeMaskPCINT8_15;
uint8_t GpioInterruptsPHAL::mInterruptFallingEdgeMaskPCINT16_23;

uint8_t GpioInterruptsPHAL::mPortLastStatePCINT0_7;
uint8_t GpioInterruptsPHAL::mPortLastStatePCINT8_15;
uint8_t GpioInterruptsPHAL::mPortLastStatePCINT16_23;

namespace Argos
{

GpioInterruptsPHAL GpioInterrupts;

}


ISR(PCINT0_vect)          //External interrupt_zero ISR
{

  uint8_t actualState = PINB;

  GpioInterrupts.pcint0_7Signal(actualState);


}


ISR(PCINT1_vect)
{

  uint8_t actualState = 0x01 & PINE;

  actualState |=  ( (0x7f & PINJ) << 1 );

  GpioInterrupts.pcint8_15Signal(actualState);

}


ISR(PCINT2_vect)
{

  uint8_t actualState = PINK;

  GpioInterrupts.pcint16_23Signal(actualState);

}



void GpioInterruptsPHAL::enable(IntSignal_t intSig, delegate<void> callbackDelegate, TriggerMode_t mode)
{

  //This is a critical section
  taskENTER_CRITICAL();

  if(intSig == NOT_INTERRUPT)
  {
    taskEXIT_CRITICAL();
    return;
  }

  //Register the interrupt in the table
  mCallbacksTable[intSig] = callbackDelegate;

  //First, we check if the interrupt is handle by the Arduino HAL. All INTx
  //interrupts are handler by Arduino
  if(intSig <=  IntSignal_t::int7 )
  {

    switch(intSig)
    {

      case int0:
        attachInterrupt(IntSignal_t::int0, int0Signal, mode);
        break;


      case int1:
        attachInterrupt(IntSignal_t::int1, int1Signal, mode);
        break;


      case int2:
        attachInterrupt(IntSignal_t::int2, int2Signal, mode);
        break;


      case int3:
        attachInterrupt(IntSignal_t::int3, int3Signal, mode);
        break;


      case int4:
        attachInterrupt(IntSignal_t::int4, int4Signal, mode);
        break;


      case int5:
        attachInterrupt(IntSignal_t::int5, int5Signal, mode);
        break;


      case int6:
        attachInterrupt(IntSignal_t::int6, int6Signal, mode);
        break;


      case int7:
        attachInterrupt(IntSignal_t::int7, int7Signal, mode);
        break;


    }


  }

  //Register the PCINT interrupt
  else
  {

    switch(mode)
    {

      case TriggerMode_t::ANY_EDGE:

        if(intSig <= IntSignal_t::pcint7 )
        {

          uint8_t intBit = (1 << (intSig-IntSignal_t::pcint0));

          PCMSK0 = PCMSK0 | intBit;

          PCICR = PCICR | 0x01;

          mInterruptRisignEdgeMaskPCINT0_7 = mInterruptRisignEdgeMaskPCINT0_7 | intBit;

          mInterruptFallingEdgeMaskPCINT0_7 = mInterruptFallingEdgeMaskPCINT0_7 | intBit;


        }
        else if( intSig <= IntSignal_t::pcint15)
        {

          uint8_t intBit = (1 << (intSig-IntSignal_t::pcint8));

          PCMSK1 = PCMSK1 | intBit;

          PCICR = PCICR | 0x02;

          mInterruptRisignEdgeMaskPCINT8_15 = mInterruptRisignEdgeMaskPCINT8_15 | intBit;

          mInterruptFallingEdgeMaskPCINT8_15 = mInterruptFallingEdgeMaskPCINT8_15 | intBit;

        }
        else
        {

          uint8_t intBit =  (1 << (intSig-IntSignal_t::pcint16));

          PCMSK2 = PCMSK2 | intBit;

          PCICR = PCICR | 0x04;

          mInterruptRisignEdgeMaskPCINT16_23 = mInterruptRisignEdgeMaskPCINT16_23 | intSig;

          mInterruptFallingEdgeMaskPCINT16_23 = mInterruptFallingEdgeMaskPCINT16_23 | intSig;

        }

        break;



      case TriggerMode_t::RISING_EDGE:

        if(intSig <= IntSignal_t::pcint7 )
        {

          uint8_t intBit = (1 << (intSig-IntSignal_t::pcint0));

          PCMSK0 = PCMSK0 | intBit;

          PCICR = PCICR | 0x01;

          mInterruptRisignEdgeMaskPCINT0_7 = mInterruptRisignEdgeMaskPCINT0_7 | intBit;

        }
        else if( intSig <= IntSignal_t::pcint15)
        {

          uint8_t intBit = (1 << (intSig-IntSignal_t::pcint8));

          PCMSK1 = PCMSK1 | intBit;

          PCICR = PCICR | 0x02;

          mInterruptRisignEdgeMaskPCINT8_15 = mInterruptRisignEdgeMaskPCINT8_15 | intBit;

        }
        else
        {

          uint8_t intBit = (1 << (intSig-IntSignal_t::pcint16));

          PCMSK2 = PCMSK2 | intBit;

          PCICR = PCICR | 0x04;

          mInterruptRisignEdgeMaskPCINT16_23 = mInterruptRisignEdgeMaskPCINT16_23 | intBit;

        }


        break;


      case TriggerMode_t::FALLING_EDGE:


        if(intSig <= IntSignal_t::pcint7 )
        {

          uint8_t intBit = (1 << (intSig-IntSignal_t::pcint0));

          PCMSK0 = PCMSK0 | intBit;

          PCICR = PCICR | 0x01;

          mInterruptFallingEdgeMaskPCINT0_7 = mInterruptFallingEdgeMaskPCINT0_7 | intBit;


        }
        else if( intSig <= IntSignal_t::pcint15)
        {

          uint8_t intBit = (1 << (intSig-IntSignal_t::pcint8));

          PCMSK1 = PCMSK1 | intBit;

          PCICR = PCICR | 0x02;

          mInterruptFallingEdgeMaskPCINT8_15 = mInterruptFallingEdgeMaskPCINT8_15 | intBit;

        }
        else
        {

          uint8_t intBit = (1 << (intSig-IntSignal_t::pcint16));

          PCMSK2 = PCMSK2 | intBit;

          PCICR = PCICR | 0x04;

          mInterruptFallingEdgeMaskPCINT16_23 = mInterruptFallingEdgeMaskPCINT16_23 | intBit;

        }

        break;

    }


  }

  taskEXIT_CRITICAL();

}



void GpioInterruptsPHAL::disable(IntSignal_t intSig)
{

  //This is a critical section
  taskENTER_CRITICAL();

  if(intSig == NOT_INTERRUPT)
  {
    taskEXIT_CRITICAL();
    return;
  }

  //First, we check if the interrupt is handle by the Arduino HAL. All INTx
  //interrupts are handler by Arduino
  if(intSig <=  IntSignal_t::int7 )
  {

    detachInterrupt(intSig);

  }

  //Register the PCINT interrupt
  else
  {

    if(intSig <= IntSignal_t::pcint7 )
    {

      uint8_t intBitMask = ~(1 << (intSig-IntSignal_t::pcint0));

      PCMSK0 = PCMSK0 & intBitMask;

      mInterruptRisignEdgeMaskPCINT0_7 = mInterruptRisignEdgeMaskPCINT0_7 & intBitMask;

      mInterruptFallingEdgeMaskPCINT0_7 = mInterruptFallingEdgeMaskPCINT0_7 & intBitMask;

      if((mInterruptRisignEdgeMaskPCINT0_7 | mInterruptFallingEdgeMaskPCINT0_7) == 0)
        PCICR = PCICR & (~0x01);


    }
    else if( intSig <= IntSignal_t::pcint15)
    {

      uint8_t intBitMask = ~(1 << (intSig-IntSignal_t::pcint8));

      PCMSK1 = PCMSK1 & intBitMask;

      mInterruptRisignEdgeMaskPCINT8_15 = mInterruptRisignEdgeMaskPCINT8_15 & intBitMask;

      mInterruptFallingEdgeMaskPCINT8_15 = mInterruptFallingEdgeMaskPCINT8_15 & intBitMask;

      if((mInterruptRisignEdgeMaskPCINT8_15 | mInterruptFallingEdgeMaskPCINT8_15) == 0)
        PCICR = PCICR & (~0x02);

    }
    else
    {

      uint8_t intBitMask = ~(1 << (intSig-IntSignal_t::pcint16));

      PCMSK2 = PCMSK2 & intBitMask;

      mInterruptRisignEdgeMaskPCINT16_23 = mInterruptRisignEdgeMaskPCINT16_23 & intBitMask;

      mInterruptFallingEdgeMaskPCINT16_23 = mInterruptFallingEdgeMaskPCINT16_23 & intBitMask;

      if((mInterruptRisignEdgeMaskPCINT16_23 | mInterruptFallingEdgeMaskPCINT16_23) == 0)
        PCICR = PCICR & (~0x04);

    }

  }

  taskEXIT_CRITICAL();

}



void GpioInterruptsPHAL::changeTriggerMode(IntSignal_t intSig, TriggerMode_t mode)
{


    //This is a critical section
    taskENTER_CRITICAL();

    if(intSig == NOT_INTERRUPT)
    {
      taskEXIT_CRITICAL();
      return;
    }

    //First, we check if the interrupt is handle by the Arduino HAL. All INTx
    //interrupts are handler by Arduino
    if(intSig <=  IntSignal_t::int7 )
    {

      detachInterrupt(intSig);

      switch(intSig)
      {

        case int0:
          attachInterrupt(IntSignal_t::int0, int0Signal, mode);
          break;


        case int1:
          attachInterrupt(IntSignal_t::int1, int1Signal, mode);
          break;


        case int2:
          attachInterrupt(IntSignal_t::int2, int2Signal, mode);
          break;


        case int3:
          attachInterrupt(IntSignal_t::int3, int3Signal, mode);
          break;


        case int4:
          attachInterrupt(IntSignal_t::int4, int4Signal, mode);
          break;


        case int5:
          attachInterrupt(IntSignal_t::int5, int5Signal, mode);
          break;


        case int6:
          attachInterrupt(IntSignal_t::int6, int6Signal, mode);
          break;


        case int7:
          attachInterrupt(IntSignal_t::int7, int7Signal, mode);
          break;


      }

    }

    //Register the PCINT interrupt
    else
    {

      switch(mode)
      {

        case TriggerMode_t::ANY_EDGE:

          if(intSig <= IntSignal_t::pcint7 )
          {

            intSig = (intSig-IntSignal_t::pcint0);

            mInterruptRisignEdgeMaskPCINT0_7 = mInterruptRisignEdgeMaskPCINT0_7 | (1 << intSig);

            mInterruptFallingEdgeMaskPCINT0_7 = mInterruptFallingEdgeMaskPCINT0_7 | (1 << intSig);


          }
          else if( intSig <= IntSignal_t::pcint15)
          {

            intSig = (intSig-IntSignal_t::pcint8);

            mInterruptRisignEdgeMaskPCINT8_15 = mInterruptRisignEdgeMaskPCINT8_15 | (1 << intSig);

            mInterruptFallingEdgeMaskPCINT8_15 = mInterruptFallingEdgeMaskPCINT8_15 | (1 << intSig);

          }
          else
          {

            intSig = (intSig-IntSignal_t::pcint16);

            mInterruptRisignEdgeMaskPCINT16_23 = mInterruptRisignEdgeMaskPCINT16_23 | (1 << intSig);

            mInterruptFallingEdgeMaskPCINT16_23 = mInterruptFallingEdgeMaskPCINT16_23 | (1 << intSig);

          }

          break;



        case TriggerMode_t::RISING_EDGE:

          if(intSig <= IntSignal_t::pcint7 )
          {

            intSig = (intSig-IntSignal_t::pcint0);

            mInterruptRisignEdgeMaskPCINT0_7 = mInterruptRisignEdgeMaskPCINT0_7 | (1 << intSig);

            mInterruptFallingEdgeMaskPCINT0_7 = mInterruptFallingEdgeMaskPCINT0_7 & (~(1 << intSig));

          }
          else if( intSig <= IntSignal_t::pcint15)
          {

            intSig = (intSig-IntSignal_t::pcint8);

            mInterruptRisignEdgeMaskPCINT8_15 = mInterruptRisignEdgeMaskPCINT8_15 | (1 << intSig);

            mInterruptFallingEdgeMaskPCINT8_15 = mInterruptFallingEdgeMaskPCINT8_15 & (~(1 << intSig));

          }
          else
          {

            intSig = (intSig-IntSignal_t::pcint16);

            mInterruptRisignEdgeMaskPCINT16_23 = mInterruptRisignEdgeMaskPCINT16_23 | (1 << intSig);

            mInterruptFallingEdgeMaskPCINT16_23 = mInterruptFallingEdgeMaskPCINT16_23 & (~(1 << intSig));

          }


          break;


        case TriggerMode_t::FALLING_EDGE:


          if(intSig <= IntSignal_t::pcint7 )
          {

            intSig = (intSig-IntSignal_t::pcint0);

            mInterruptRisignEdgeMaskPCINT0_7 = mInterruptRisignEdgeMaskPCINT0_7 & (~(1 << intSig));

            mInterruptFallingEdgeMaskPCINT0_7 = mInterruptFallingEdgeMaskPCINT0_7 | (1 << intSig);


          }
          else if( intSig <= IntSignal_t::pcint15)
          {

            intSig = (intSig-IntSignal_t::pcint8);

            mInterruptRisignEdgeMaskPCINT8_15 = mInterruptRisignEdgeMaskPCINT8_15 & (~(1 << intSig));

            mInterruptFallingEdgeMaskPCINT8_15 = mInterruptFallingEdgeMaskPCINT8_15 | (1 << intSig);

          }
          else
          {

            intSig = (intSig-IntSignal_t::pcint16);

            mInterruptRisignEdgeMaskPCINT16_23 = mInterruptRisignEdgeMaskPCINT16_23 & (~(1 << intSig));

            mInterruptFallingEdgeMaskPCINT16_23 = mInterruptFallingEdgeMaskPCINT16_23 | (1 << intSig);

          }

          break;

      }


    }

    taskEXIT_CRITICAL();

}
