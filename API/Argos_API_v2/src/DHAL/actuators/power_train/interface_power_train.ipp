
template<class T>
void InterfacePowerTrain<T>::begin()
{
  static_cast<T*>(this)->begin();
};


template<class T>
bool InterfacePowerTrain<T>::getStatus()
{
  return static_cast<T*>(this)->getStatus();
};

template<class T>
void InterfacePowerTrain<T>::registerOnFaultCallbackDelegate(delegate<void, uint8_t> callbackDelegate)
{
  static_cast<T*>(this)->registerOnFaultCallbackDelegate(callbackDelegate);
};

template<class T>
void InterfacePowerTrain<T>::unregisterOnFaultCallbackDelegate()
{
  static_cast<T*>(this)->unregisterOnFaultCallbackDelegate();
};

template<class T>
uint8_t InterfacePowerTrain<T>::getNumberOfMotors()
{
  return static_cast<T*>(this)->getNumberOfMotors();
};

template<class T>
void InterfacePowerTrain<T>::setDutyCycle(uint8_t motor, uint8_t dutyCycle)
{
  static_cast<T*>(this)->setDutyCycle(motor, dutyCycle);
};


template<class T>
void InterfacePowerTrain<T>::setSpinDirection(uint8_t motor, spinDir_t dir)
{
  static_cast<T*>(this)->setSpinDirection(motor, dir);
};

template<class T>
void InterfacePowerTrain<T>::end()
{
  static_cast<T*>(this)->end();
};
