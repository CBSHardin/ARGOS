
#include "../dfrobot_chasis_power_train.hpp"

#include <Arduino.h>

using namespace Argos;

void DFRobotChasisPowerTrain::begin()
{

//Serial.println("mDr8876FunctionsM1.mPinEN:  "+String(mDr8876FunctionsM1.mPinEN));

//drv8876LLHALArgosImpl dutThing(69, 0, 0, 0);

//Serial.println("dutThing.mPinEN:  "+String((dutThing.mPinEN)  ));
//canary =10;

//Serial.println("canary:  "+String((canary)  ));


  mDriverM1.enable();
  mDriverM2.enable();
  mDriverM3.enable();
  mDriverM4.enable();

  mDriverM1.setDutyCycle(0);
  mDriverM2.setDutyCycle(0);
  mDriverM3.setDutyCycle(0);
  mDriverM4.setDutyCycle(0);

  mDriverM1.registerOnDriverFaultCallback(&onDriverM1FaultCallback, this);
  mDriverM2.registerOnDriverFaultCallback(&onDriverM2FaultCallback, this);
  mDriverM3.registerOnDriverFaultCallback(&onDriverM3FaultCallback, this);
  mDriverM4.registerOnDriverFaultCallback(&onDriverM4FaultCallback, this);

  mState = true;

}



bool DFRobotChasisPowerTrain::getStatus()
{

  return mState;

}



void DFRobotChasisPowerTrain::registerOnFaultCallbackDelegate(delegate<void, uint8_t> callbackDelegate)
{

  mOnDriverFaultCbD = callbackDelegate;

}



void DFRobotChasisPowerTrain::unregisterOnFaultCallbackDelegate()
{

  mOnDriverFaultCbD = delegate<void, uint8_t>();

}



uint8_t DFRobotChasisPowerTrain::getNumberOfMotors()
{

  return 4;

}



void DFRobotChasisPowerTrain::setDutyCycle(uint8_t motor, uint8_t dutyCycle)
{

  switch(motor)
  {
    case 1:
      mDriverM1.setDutyCycle(dutyCycle);
      break;

    case 2:
      mDriverM2.setDutyCycle(dutyCycle);
      break;

    case 3:
      mDriverM3.setDutyCycle(dutyCycle);
      break;

    case 4:
      mDriverM4.setDutyCycle(dutyCycle);
      break;

    default:
      break;

  };

}



void DFRobotChasisPowerTrain::setSpinDirection(uint8_t motor, spinDir_t dir)
{

  switch(dir)
  {
    case spinDir_t::FORWARD:

      switch(motor)
      {
        case 1:
          mDriverM1.setSpinDirection(mDriverM1.FORWARD);
          break;

        case 2:
          mDriverM2.setSpinDirection(mDriverM2.BACKWARD);
          break;

        case 3:
          mDriverM3.setSpinDirection(mDriverM3.FORWARD);
          break;

        case 4:
          mDriverM4.setSpinDirection(mDriverM4.BACKWARD);
          break;

        default:
          break;

      };

      break;

    case spinDir_t::BACKWARD:
      switch(motor)
      {
        case 1:
          mDriverM1.setSpinDirection(mDriverM1.BACKWARD);
          break;

        case 2:
          mDriverM2.setSpinDirection(mDriverM2.FORWARD);
          break;

        case 3:
          mDriverM3.setSpinDirection(mDriverM3.BACKWARD);
          break;

        case 4:
          mDriverM4.setSpinDirection(mDriverM4.FORWARD);
          break;

        default:
          break;

      };

      break;

  };

}


void DFRobotChasisPowerTrain::end()
{

  mDriverM1.setDutyCycle(0);
  mDriverM2.setDutyCycle(0);
  mDriverM3.setDutyCycle(0);
  mDriverM4.setDutyCycle(0);

  mDriverM1.disable();
  mDriverM2.disable();
  mDriverM3.disable();
  mDriverM4.disable();

  mState = false;

}


static void DFRobotChasisPowerTrain::onDriverM1FaultCallback(void* instance)
{

  static_cast<DFRobotChasisPowerTrain*>(instance)->mOnDriverFaultCbD(1);

}


static void DFRobotChasisPowerTrain::onDriverM2FaultCallback(void* instance)
{

  static_cast<DFRobotChasisPowerTrain*>(instance)->mOnDriverFaultCbD(2);

}


static void DFRobotChasisPowerTrain::onDriverM3FaultCallback(void* instance)
{

  static_cast<DFRobotChasisPowerTrain*>(instance)->mOnDriverFaultCbD(3);

}


static void DFRobotChasisPowerTrain::onDriverM4FaultCallback(void* instance)
{

  static_cast<DFRobotChasisPowerTrain*>(instance)->mOnDriverFaultCbD(4);

}
