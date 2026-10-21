
#ifndef __ARGOS_API_DFROBOT_CHASIS_POWER_TRAIN_H__
#define __ARGOS_API_DFROBOT_CHASIS_POWER_TRAIN_H__

#include <stdint.h>
#include <delegate.h>

#include "../../interface_power_train.hpp"

#include "../../../../../DDL/drv8876/drv8876.hpp"
#include "../../../../../DDL/drv8876/interface_drv8876_LLF.hpp"
#include "../../../../../DDL/drv8876/drv8876_LLF_Argos.hpp"


namespace Argos
{

class DFRobotChasisPowerTrain : public InterfacePowerTrain<DFRobotChasisPowerTrain>
{

public:

  friend InterfacePowerTrain<DFRobotChasisPowerTrain>;

  DFRobotChasisPowerTrain(uint8_t dutyCycleMax, uint16_t overCurrentEventWindowTimeMs) :
  mState(false),

  mDr8876FunctionsM1(7, 22, 36, 68),
  mDr8876FunctionsM2(8, 23, 36, 69),
  mDr8876FunctionsM3(6, 24, 36, 66),
  mDr8876FunctionsM4(9, 25, 36, 67),

  mDriverM1(mDr8876FunctionsM1, dutyCycleMax, overCurrentEventWindowTimeMs),
  mDriverM2(mDr8876FunctionsM2, dutyCycleMax, overCurrentEventWindowTimeMs),
  mDriverM3(mDr8876FunctionsM3, dutyCycleMax, overCurrentEventWindowTimeMs),
  mDriverM4(mDr8876FunctionsM4, dutyCycleMax, overCurrentEventWindowTimeMs)
  {};

  void begin();

  bool getStatus();

  void registerOnFaultCallbackDelegate(delegate<void, uint8_t> callbackDelegate);

  void unregisterOnFaultCallbackDelegate();

  uint8_t getNumberOfMotors();

  void setDutyCycle(uint8_t motor, uint8_t dutyCycle);

  void setSpinDirection(uint8_t motor, spinDir_t dir);

  void end();


private:

  bool mState;

  //Funciones de acceso al hadware del driver dr8876
  drv8876_LLF_Argos mDr8876FunctionsM1;
  drv8876_LLF_Argos mDr8876FunctionsM2;
  drv8876_LLF_Argos mDr8876FunctionsM3;
  drv8876_LLF_Argos mDr8876FunctionsM4;

  //Drivers de cada motor:
  drv8876<drv8876_LLF_Argos> mDriverM1;
  drv8876<drv8876_LLF_Argos> mDriverM2;
  drv8876<drv8876_LLF_Argos> mDriverM3;
  drv8876<drv8876_LLF_Argos> mDriverM4;

  delegate<void, uint8_t> mOnDriverFaultCbD;


  static void onDriverM1FaultCallback(void* instance);
  static void onDriverM2FaultCallback(void* instance);
  static void onDriverM3FaultCallback(void* instance);
  static void onDriverM4FaultCallback(void* instance);

};

}

#endif
