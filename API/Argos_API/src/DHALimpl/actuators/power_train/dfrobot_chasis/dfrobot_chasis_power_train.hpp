
#ifndef __ARGOS_API_DFROBOT_CHASIS_POWER_TRAIN_H__
#define __ARGOS_API_DFROBOT_CHASIS_POWER_TRAIN_H__

#include <stdint.h>
#include <delegate.h>

#include "../../../../DHAL/actuators/power_train/power_train.hpp"


namespace Argos
{

class DFRobotChasisPowerTrain : public PowerTrain<DFRobotChasisPowerTrain>
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

}

#endif
