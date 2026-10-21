

#ifndef _IMPL_PHAL_GPIO_H__
#define _IMPL_PHAL_GPIO_H__

#include "../../PHAL/gpio/interfaceGpio.h"

#include <stdint.h>

#include <avr/io.h>
#include <avr/interrupt.h>


class implPHALGpio : public InterfaceGpioHAL
{

  public:

    implPHALGpio(uint8_t* pRegPORTx, uint8_t* pRegDDRx, uint8_t* pRegPINx) : \
      gpioRegisters{pRegPORTx, pRegDDRx, pRegPINx}
    {};


    inline void setPinDir(uint8_t GPIOpin, PinDir_t dir)
    {

      //Serial.println("port.pRegDDRx : 0x"+String((uint16_t)port.pRegDDRx, HEX));

      uint8_t sreg = SREG;

      //Critical section
      cli();

      if(dir == OUT)
        *(gpioRegisters.pRegDDRx) = *(gpioRegisters.pRegDDRx) | (1 << GPIOpin);
      else
        *(gpioRegisters.pRegDDRx) = *(gpioRegisters.pRegDDRx) & (~(1 << GPIOpin));


      SREG = sreg;

    }


    /*Precondition: pin must be configured as input*/
    void enablePinPullUpResistors(uint8_t GPIOpin)
    {

      writePin(GPIOpin, PinState_t::SET);

    }

    void disablePinPullUpResistors(uint8_t GPIOpin)
    {

      writePin(GPIOpin, PinState_t::CLEAR);

    }


    void indirectPinWrite(uint8_t GPIOpin, PinState_t state)
    {

      this->writePin(GPIOpin, state);

    }


    inline void writePin(uint8_t GPIOpin, PinState_t state)
    {

      uint8_t sreg = SREG;

      //Critical section
      cli();

      if(state == SET)
        *(gpioRegisters.pRegPORTx) = *(gpioRegisters.pRegPORTx) | (1 << GPIOpin);
      else
        *(gpioRegisters.pRegPORTx) = *(gpioRegisters.pRegPORTx) & (~(1 << GPIOpin));


      SREG = sreg;

    }


    PinState_t readPin(uint8_t GPIOpin)
    {

      return (*(gpioRegisters.pRegPINx) >> GPIOpin) & 0x01;

    }



    void writePort(uint8_t portValue)
    {

      *(gpioRegisters.pRegPORTx) = portValue;

    }


    uint8_t readPort()
    {

      return *(gpioRegisters.pRegPINx);

    }


  private:

    typedef struct
    {

      uint8_t* pRegPORTx;
      uint8_t* pRegDDRx;
      uint8_t* pRegPINx;

    }
    GpioPort_t;

    GpioPort_t gpioRegisters;


//  inline void enableInterrupt()

//  inline void dissableInterrupt();

};


#endif
