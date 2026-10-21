
#ifndef __ARGOS_API_POWER_TRAIN_H__
#define __ARGOS_API_POWER_TRAIN_H__

#include <stdint.h>
#include <delegate.h>

namespace Argos
{

//Usaremos el idiom CRTP para implementar poliformismo estático, es decir,
//en tiempo de compilación.

template <class T>
class PowerTrain
{

public:

  void begin();

  bool getStatus();

  void registerOnFaultCallbackDelegate(delegate<void, uint8_t> callbackDelegate);

  void unregisterOnFaultCallbackDelegate();

  uint8_t getNumberOfMotors();

  void setDutyCycle(uint8_t motor, uint8_t dutyCycle);

  uint8_t getDutyCycle(uint8_t motor);

  void end();

};

//Archivo de implementación del template
//#include "DHAL/actuators/power_train/power_train.ipp"
#include "power_train.ipp"

}

#endif
