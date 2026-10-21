
#include "../dfrobot_chasis_power_train_encoders.hpp"

using namespace Argos;

void DFRobotChasisPowerTrainEncoders::begin()
{
  delegate<void, float, uint16_t> callbackDelegate;

  mEncoderM1.begin();
  mEncoderM2.begin();
  mEncoderM3.begin();
  mEncoderM4.begin();


  callbackDelegate.setCallback<DFRobotChasisPowerTrainEncoders, &DFRobotChasisPowerTrainEncoders::encoderM1Callback>(this);
  mEncoderM1.registerOnSpeedSampleUpdateCallbackD(callbackDelegate);

  callbackDelegate.setCallback<DFRobotChasisPowerTrainEncoders, &DFRobotChasisPowerTrainEncoders::encoderM2Callback>(this);
  mEncoderM2.registerOnSpeedSampleUpdateCallbackD(callbackDelegate);

  callbackDelegate.setCallback<DFRobotChasisPowerTrainEncoders, &DFRobotChasisPowerTrainEncoders::encoderM3Callback>(this);
  mEncoderM3.registerOnSpeedSampleUpdateCallbackD(callbackDelegate);

  callbackDelegate.setCallback<DFRobotChasisPowerTrainEncoders, &DFRobotChasisPowerTrainEncoders::encoderM4Callback>(this);
  mEncoderM4.registerOnSpeedSampleUpdateCallbackD(callbackDelegate);

  mState = true;

}

bool DFRobotChasisPowerTrainEncoders::getStatus()
{

  return mState;

}

float DFRobotChasisPowerTrainEncoders::getRPM(uint8_t motor)
{

  float ret;

  switch(motor)
  {

    case 1:
      ret = mEncoderM1.getRPM();
      break;

    case 2:
      ret = mEncoderM2.getRPM();
      break;

    case 3:
      ret = mEncoderM3.getRPM();
      break;

    case 4:
      ret = mEncoderM4.getRPM();
      break;

    default:
      break;

  };

  return ret;

}

uint16_t DFRobotChasisPowerTrainEncoders::getEncoderCount(uint8_t motor)
{

  uint16_t ret;

  switch(motor)
  {

    case 1:
      ret = mEncoderM1.getEncoderCount();
      break;

    case 2:
      ret = mEncoderM2.getEncoderCount();
      break;

    case 3:
      ret = mEncoderM3.getEncoderCount();
      break;

    case 4:
      ret = mEncoderM4.getEncoderCount();
      break;

    default:
      break;

  };

  return ret;

}

const uint8_t DFRobotChasisPowerTrainEncoders::getNumberOfEncoders()
{

  return 4;

}

uint8_t DFRobotChasisPowerTrainEncoders::getEncoderStepsPerRevolution(uint8_t motor)
{

  return 25;

}


void DFRobotChasisPowerTrainEncoders::clearEncoderCount(uint8_t motor)
{


  switch(motor)
  {

    case 1:
      mEncoderM1.clearEncoderCount();
      break;

    case 2:
      mEncoderM2.clearEncoderCount();
      break;

    case 3:
      mEncoderM3.clearEncoderCount();
      break;

    case 4:
      mEncoderM4.clearEncoderCount();
      break;

    default:
      break;

  };

}


void DFRobotChasisPowerTrainEncoders::end()
{

  mEncoderM1.end();
  mEncoderM2.end();
  mEncoderM3.end();
  mEncoderM4.end();

  mState = false;

}


void DFRobotChasisPowerTrainEncoders::encoderM1Callback(float rpm, uint16_t count)
{

  updatesPublisher.call(1, rpm, count);

}

void DFRobotChasisPowerTrainEncoders::encoderM2Callback(float rpm, uint16_t count)
{

  updatesPublisher.call(2, rpm, count);
}

void DFRobotChasisPowerTrainEncoders::encoderM3Callback(float rpm, uint16_t count)
{

  updatesPublisher.call(3, rpm, count);

}

void DFRobotChasisPowerTrainEncoders::encoderM4Callback(float rpm, uint16_t count)
{

  updatesPublisher.call(4, rpm, count);

}
