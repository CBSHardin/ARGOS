
template<class T>
void InterfacePowerTrainEncoders<T>::begin()
{
  static_cast<T*>(this)->begin();
}


template<class T>
bool InterfacePowerTrainEncoders<T>::getStatus()
{
  return static_cast<T*>(this)->getStatus();
}


template<class T>
float InterfacePowerTrainEncoders<T>::getRPM(uint8_t motor)
{
  return static_cast<T*>(this)->getRPM(motor);
}


template<class T>
uint16_t InterfacePowerTrainEncoders<T>::getEncoderCount(uint8_t motor)
{
  return static_cast<T*>(this)->getEncoderCount(motor);
}


template<class T>
const uint8_t InterfacePowerTrainEncoders<T>::getNumberOfEncoders()
{
  return static_cast<T*>(this)->getNumberOfEncoders();
}


template<class T>
uint8_t InterfacePowerTrainEncoders<T>::getEncoderStepsPerRevolution(uint8_t motor)
{
  return static_cast<T*>(this)->getEncoderStepsPerRevolution(motor);
}

template<class T>
void InterfacePowerTrainEncoders<T>::clearEncoderCount(uint8_t motor)
{
  static_cast<T*>(this)->clearEncoderCount(motor);
}


template<class T>
bool InterfacePowerTrainEncoders<T>::registerOnSpeedSampleUpdateCallbackD(delegate<void, uint8_t, float, uint16_t> callbackDelegate)
{
  return updatesPublisher.registerOnUpdateCallbackDelegate(callbackDelegate);
}


template<class T>
void InterfacePowerTrainEncoders<T>::unregisterOnSpeedSampleUpdateCallbackD(delegate<void, uint8_t, float, uint16_t> callbackDelegate)
{
  return updatesPublisher.unregisterOnUpdateCallback(callbackDelegate);
}


template<class T>
void InterfacePowerTrainEncoders<T>::end()
{
  static_cast<T*>(this)->end();
}
