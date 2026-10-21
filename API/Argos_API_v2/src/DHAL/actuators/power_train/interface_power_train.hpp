
#ifndef __ARGOS_API_POWER_TRAIN_H__
#define __ARGOS_API_POWER_TRAIN_H__

#include <stdint.h>
#include <delegate.h>

#include "../../../config.h"

namespace Argos
{

//Usaremos el idiom CRTP para implementar poliformismo estático, es decir,
//en tiempo de compilación.

template <class T>
class InterfacePowerTrain
{

public:

  typedef enum : uint8_t {FORWARD, BACKWARD} spinDir_t;

  void begin();

  bool getStatus();

  void registerOnFaultCallbackDelegate(delegate<void, uint8_t> callbackDelegate);

  void unregisterOnFaultCallbackDelegate();

  uint8_t getNumberOfMotors();

  void setDutyCycle(uint8_t motor, uint8_t dutyCycle);

  void setSpinDirection(uint8_t motor, spinDir_t dir);

  void end();

};

//Archivo de implementación del template
//#include "DHAL/actuators/power_train/power_train.ipp"
#include "interface_power_train.ipp"

}


//Instance for DFRobot_chasis
#ifdef USE_DFROBOT_CHASIS
  #include "implementations/dfrobot_chasis/dfrobot_chasis_power_train.hpp"

  namespace Argos
  {

   class DFRobotChasisPowerTrain;   //Advance definition

   extern InterfacePowerTrain<DFRobotChasisPowerTrain>& PowerTrain;

  }

#endif

#endif
