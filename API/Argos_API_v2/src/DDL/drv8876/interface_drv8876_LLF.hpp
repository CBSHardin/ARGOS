
#ifndef __ARGOS_API_DDL_INTERFACE_DRV8876_LLF_H__
#define __ARGOS_API_DDL_INTERFACE_DRV8876_LLF_H__

#include <stdint.h>

//LowerLevelFunctions Interface, implementa todas las funciones de bajo nivel
//para que el driver sea hardware agsnostic.

template <class T>
class interface_drv8876_LLF
{

  public:

    void initLLHadware();

    void setPinSleep_NState(bool state);

    void setPinPHState(bool state);

    bool getPinFault_NState();

    void setDutyCyclePWMSignalPinEN(uint8_t dutyCycle);

    void setFrecuencyPWMSignalPinEN(uint16_t frecuency);

    void registerNFaultInterruptCallbackFunction(void (*callbackOnNFaultChange)(void*), void* callbackArgument);

    void registerTimerCallbackFunction(void (*callbackOnTimerEvent)(void*), void* callbackArgument);

    void startOnceTimer(uint16_t timerTimeOutMs);

    void stopOnceTimer();

    void deinitLLHadware();

};

#include "src/interface_drv8876_LLF.ipp"

#endif
