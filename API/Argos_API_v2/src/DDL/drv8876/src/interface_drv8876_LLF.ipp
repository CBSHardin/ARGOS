

template <class T>
void interface_drv8876_LLF<T>::initLLHadware()
{

  static_cast<T*>(this)->initLLHadware();

}

template <class T>
void interface_drv8876_LLF<T>::setPinSleep_NState(bool state)
{

  static_cast<T*>(this)->setPinSleep_NState(state);

}

template <class T>
void interface_drv8876_LLF<T>::setPinPHState(bool state)
{

  static_cast<T*>(this)->setPinPHState(state);

}

template <class T>
bool interface_drv8876_LLF<T>::getPinFault_NState()
{

  return static_cast<T*>(this)->getPinFault_NState();

}

template <class T>
void interface_drv8876_LLF<T>::setDutyCyclePWMSignalPinEN(uint8_t dutyCycle)
{

  static_cast<T*>(this)->setDutyCyclePWMSignalPinEN(dutyCycle);

}

template <class T>
void interface_drv8876_LLF<T>::setFrecuencyPWMSignalPinEN(uint16_t frecuency)
{

  static_cast<T*>(this)->setFrecuencyPWMSignalPinEN(frecuency);

}

template <class T>
void interface_drv8876_LLF<T>::registerNFaultInterruptCallbackFunction(void (*callbackOnNFaultChange)(void*), void* callbackArgument)
{

  static_cast<T*>(this)->registerNFaultInterruptCallbackFunction(callbackOnNFaultChange, callbackArgument);

}

template <class T>
void interface_drv8876_LLF<T>::registerTimerCallbackFunction(void (*callbackOnTimerEvent)(void*), void* callbackArgument)
{

  static_cast<T*>(this)->registerTimerCallbackFunction(callbackOnTimerEvent, callbackArgument);

}

template <class T>
void interface_drv8876_LLF<T>::startOnceTimer(uint16_t timerTimeOutMs)
{

  static_cast<T*>(this)->startOnceTimer(timerTimeOutMs);

}

template <class T>
void interface_drv8876_LLF<T>::stopOnceTimer()
{

  static_cast<T*>(this)->stopOnceTimer();

}

template <class T>
void interface_drv8876_LLF<T>::deinitLLHadware()
{

  static_cast<T*>(this)->deinitLLHadware();

}
