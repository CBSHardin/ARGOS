
#ifndef __ARGOS_API_DDL_DRV8876_LLF_ARGOS_H__
#define __ARGOS_API_DDL_DRV8876_LLF_ARGOS_H__

#include <stdint.h>

#include <iTimer.hpp>
#include <TimerFreeRTOS.hpp>

//Este es el adaptador o la implementación de bajo nivel de las funciones que
//el driver hadware agnostic necesita para poder ejecutar su función.
//La interfaz debe incluir algunos detalles como los pines a los que se conecta
//y esas cosas. Cada instancia del driver recibirá una referencia a una version
//diferente de este objeto, lo que permitirá implementar el power_train con
//varias instancias.

class drv8876_LLF_Argos : public interface_drv8876_LLF<drv8876_LLF_Argos>
{

  public:

    drv8876_LLF_Argos(uint8_t pinEN, uint8_t pinPH, uint8_t pinSleep_N, uint8_t pinFault_N) :
    mPinEN(pinEN), mPinPH(pinPH), mPinSleep_N(pinSleep_N), mPinFault_N(pinFault_N),
    mOnNFaultFunc(NULL), faultWindowTimer(faultWindowTimerRTOS){};


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

  private:

    const uint8_t mPinEN;
    const uint8_t mPinPH;
    const uint8_t mPinSleep_N;
    const uint8_t mPinFault_N;

    void callbackOnFaultINT();
    void callbackOnTimerTimeout();
    void* mCallbackArgument;

    typedef void (callbackFunc_t)(void*);

    callbackFunc_t* mOnNFaultFunc;
    callbackFunc_t* mOnTimerTimeoutFunc;

    //Timer related
    OSAL::TimerFreeRTOS faultWindowTimerRTOS;

    OSAL::iTimer& faultWindowTimer;

};


#endif
