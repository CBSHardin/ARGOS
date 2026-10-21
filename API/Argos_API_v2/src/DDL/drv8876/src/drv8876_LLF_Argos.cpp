
#include "../interface_drv8876_LLF.hpp"
#include "../drv8876_LLF_Argos.hpp"

#include "../../../PHAL/gpio/gpio_interrupts.hpp"

#include <delegate.h>

#include <Arduino.h>




void drv8876_LLF_Argos::initLLHadware()
{

  //Serial.println("mPinEN: "+String(mPinEN)+" mPinPH: "+String(mPinPH)+" mPinSleep_N: "+String(mPinSleep_N)+" mPinFault_N: "+String(mPinFault_N));

  pinMode(mPinEN, OUTPUT);
  pinMode(mPinPH, OUTPUT);
  pinMode(mPinSleep_N, OUTPUT);

  digitalWrite(mPinSleep_N, LOW);
  digitalWrite(mPinEN, LOW);

  pinMode(mPinFault_N, INPUT);


  //Init timer
  delegate<void> timerTimeoutDelegate;
  timerTimeoutDelegate.setCallback<drv8876_LLF_Argos, &drv8876_LLF_Argos::callbackOnTimerTimeout>(this);

  faultWindowTimer.registerTimerCb(timerTimeoutDelegate);
  faultWindowTimer.begin(OSAL::iTimer::ONE_SHOT, 0);

}


void drv8876_LLF_Argos::setPinSleep_NState(bool state)
{

  switch(state)
  {
    case true:
      digitalWrite(mPinSleep_N, HIGH);
      break;

    case false:
      digitalWrite(mPinSleep_N, LOW);
      break;
  };

}

void drv8876_LLF_Argos::setPinPHState(bool state)
{

  switch(state)
  {
    case true:
      digitalWrite(mPinPH, HIGH);
      break;

    case false:
      digitalWrite(mPinPH, LOW);
      break;
  };

}

bool drv8876_LLF_Argos::getPinFault_NState()
{

  if(digitalRead(mPinFault_N) == HIGH)
    return true;

  else
    return false;

}

void drv8876_LLF_Argos::setDutyCyclePWMSignalPinEN(uint8_t dutyCycle)
{

  analogWrite(mPinEN, dutyCycle);

}

void drv8876_LLF_Argos::setFrecuencyPWMSignalPinEN(uint16_t frecuency)
{

  //Nada

}


void drv8876_LLF_Argos::deinitLLHadware()
{

  faultWindowTimer.end();

  faultWindowTimer.unregisterTimerCb();

  digitalWrite(mPinSleep_N, LOW);
  digitalWrite(mPinEN, LOW);

  pinMode(mPinEN, INPUT);
  pinMode(mPinPH, INPUT);
  pinMode(mPinSleep_N, INPUT);

}


void drv8876_LLF_Argos::registerNFaultInterruptCallbackFunction(void (*callbackOnNFaultChange)(void*), void* callbackArgument)
{

  mCallbackArgument = callbackArgument;
  mOnNFaultFunc = callbackOnNFaultChange;

  delegate<void> callbackDelegate;

  callbackDelegate.setCallback<drv8876_LLF_Argos, &drv8876_LLF_Argos::callbackOnFaultINT>(this);

  Argos::GpioInterrupts.enable(GpioPinToIntSig(mPinFault_N), callbackDelegate, Argos::GpioInterrupts.FALLING_EDGE);

}

void drv8876_LLF_Argos::registerTimerCallbackFunction(void (*callbackOnTimerEvent)(void*), void* callbackArgument)
{

  mCallbackArgument = callbackArgument;
  mOnTimerTimeoutFunc = callbackOnTimerEvent;

}

void drv8876_LLF_Argos::startOnceTimer(uint16_t timerTimeOut)
{

  faultWindowTimer.changePeriod(timerTimeOut);
  faultWindowTimer.start();

}

void drv8876_LLF_Argos::stopOnceTimer()
{

  faultWindowTimer.stop();

}


void drv8876_LLF_Argos::callbackOnFaultINT()
{

  (*mOnNFaultFunc)(mCallbackArgument);

}

void drv8876_LLF_Argos::callbackOnTimerTimeout()
{

  (*mOnTimerTimeoutFunc)(mCallbackArgument);

}
