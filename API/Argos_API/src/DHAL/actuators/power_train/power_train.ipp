
template<class T>
void PowerTrain<T>::begin();
{
  static_cast(T*)(this)->begin();
};


template<class T>
bool PowerTrain<T>::getStatus();
{
  return static_cast(T*)(this)->getStatus();
};

template<class T>
void PowerTrain<T>::registerOnFaultCallbackDelegate(delegate<void, uint8_t> callbackDelegate);
{
  static_cast(T*)(this)->registerOnFaultCallbackDelegate(callbackDelegate);
};

template<class T>
void PowerTrain<T>::unregisterOnFaultCallbackDelegate();
{
  static_cast(T*)(this)->unregisterOnFaultCallbackDelegate();
};

template<class T>
uint8_t PowerTrain<T>::getNumberOfMotors();
{
  return static_cast(T*)(this)->getNumberOfMotors();
};

template<class T>
void PowerTrain<T>::setDutyCycle(uint8_t motor, uint8_t dutyCycle);
{
  static_cast(T*)(this)->setDutyCycle(motor, dutyCycle);
};


template<class T>
uint8_t PowerTrain<T>::getDutyCycle(uint8_t motor);
{
  return static_cast(T*)(this)->getDutyCycle(motor);
};

template<class T>
void PowerTrain<T>::end();
{
  static_cast(T*)(this)->end();
};
