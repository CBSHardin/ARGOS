
#ifndef __ARGOS_API_DDL_DRV8876_H__
#define __ARGOS_API_DDL_DRV8876_H__

//Esta clase implementa el control del driver drv8876, que incluye habilitacion y
//deshabilitacion, detección de fallo y control PWM. Lo complicado del asunto,
//es que debe apoyarse en la capa inferior, PDL, para poder manejar el PWM,
//y el GPIO.

#include <stdint.h>
#include <stdlib.h>
#include "interface_drv8876_LLF.hpp"

#include <Arduino.h>

template <class T>
class drv8876
{

  public:

    typedef enum : uint8_t {FORWARD, BACKWARD} spinDir_t;

    drv8876(interface_drv8876_LLF<T>& LLF, uint8_t dutyCycleLimit,  uint16_t overCurrentEventWindowTimeMs) :
    mLLF(LLF), mDutyCycleLimit(dutyCycleLimit), mOverCurrentEventWindowTimeMs(overCurrentEventWindowTimeMs),
    m_pOverCurrentFaultCbF(NULL)
    {};

    void enable();

    void disable();

    bool getFault_NState();

    void setSpinDirection(spinDir_t dir);

    //Esta función establecerá un valor máximo para el duty cycle,
    //una vez establecido, los valores de PWM se mapearan entre 0 y este.
    //void setDutyCycleLimit(uint8_t limit);

    void setDutyCycle(uint8_t dutyCycle);

    void registerOnDriverFaultCallback(void (*callbackFunc)(void*), void* callbackFuncArgument);


  private:

    typedef void (callbackFunction_t)(void*);

    callbackFunction_t* m_pOverCurrentFaultCbF;

    void* mCallbackFuncArgument;

    const interface_drv8876_LLF<T>& mLLF;

    const uint8_t mDutyCycleLimit;

    const uint16_t mOverCurrentEventWindowTimeMs;

    //Callback functions for timer rutines and ISR rutines
    static void onTimerTimeoutCallback(void* instance);

    static void onPinFaultNChangeCallback(void* instance);

};

#include "src/drv8876.ipp"


#endif
