

#ifndef _INTERFACE_GPIO_HAL_H__
#define _INTERFACE_GPIO_HAL_H__

#include <stdint.h>


class InterfaceGpioHAL
{

  public:

    typedef enum : uint8_t {SET = 0xff, CLEAR = 0x00} PinState_t;

    typedef enum : uint8_t {IN = 0, OUT = 1} PinDir_t;


    void setPinDir(uint8_t GPIOpin, PinDir_t dir){};


    /*Precondition: pin must be configured as input*/
    void enablePinPullUpResistors(uint8_t GPIOpin){};


    void disablePinPullUpResistors(uint8_t GPIOpin){};




    void writePin(uint8_t GPIOpin, PinState_t state){};


    PinState_t readPin(uint8_t GPIOpin){};


    void writePort(uint8_t portValue){};


    uint8_t readPort(){};

};

#endif
