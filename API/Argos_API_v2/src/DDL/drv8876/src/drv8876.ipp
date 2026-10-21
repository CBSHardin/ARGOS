

template <class T>
void drv8876<T>::enable()
{

  mLLF.initLLHadware();

  mLLF.registerTimerCallbackFunction(&onTimerTimeoutCallback, this);

  mLLF.registerNFaultInterruptCallbackFunction(&onPinFaultNChangeCallback, this);

  mLLF.setPinSleep_NState(true);

}

template <class T>
void drv8876<T>::disable()
{

  mLLF.setPinSleep_NState(false);
  mLLF.deinitLLHadware();

}

template <class T>
bool drv8876<T>::getFault_NState()
{

  return  mLLF.getPinFault_NState();

}

template <class T>
void drv8876<T>::setSpinDirection(spinDir_t dir)
{

  switch(dir)
  {
    case spinDir_t::FORWARD:
      mLLF.setPinPHState(true);
      break;

    case spinDir_t::BACKWARD:
      mLLF.setPinPHState(false);
      break;

  };

}



template <class T>
void drv8876<T>::setDutyCycle(uint8_t dutyCycle)
{

  //Tenemos que mapear los valores
  mLLF.setDutyCyclePWMSignalPinEN( (((unsigned int)mDutyCycleLimit)*((unsigned int)dutyCycle))/255  );

}

template <class T>
void drv8876<T>::registerOnDriverFaultCallback(void (*callbackFunc)(void*), void* callbackFuncArgument)
{

  m_pOverCurrentFaultCbF = callbackFunc;

  mCallbackFuncArgument = callbackFuncArgument;

}


template <class T>
void drv8876<T>::onTimerTimeoutCallback(void* instance)
{

  //Si el fallo sigue presentandose, desconectamos los motores, y
  //llamamos a la callback registrada por el usuario
  if(static_cast<drv8876<T>*>(instance)->getFault_NState() == false)
  {
    static_cast<drv8876<T>*>(instance)->mLLF.stopOnceTimer();

    static_cast<drv8876<T>*>(instance)->setDutyCycle(0);

    if(static_cast<drv8876<T>*>(instance)->m_pOverCurrentFaultCbF != NULL)
    {
      (*static_cast<drv8876<T>*>(instance)->m_pOverCurrentFaultCbF)(static_cast<drv8876<T>*>(instance)->mCallbackFuncArgument);
    }

  }

}


template <class T>
void drv8876<T>::onPinFaultNChangeCallback(void* instance)
{


  //Comprobamos el estado del pin, si esta a nivel bajo, es que se ha producido
  //un fallo, y activamos el timer de desconexión de emergencia.
  if(static_cast<drv8876<T>*>(instance)->getFault_NState() == false)
  {

    static_cast<drv8876<T>*>(instance)->mLLF.startOnceTimer(static_cast<drv8876<T>*>(instance)->mOverCurrentEventWindowTimeMs);

  }


}
