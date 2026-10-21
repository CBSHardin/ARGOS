
#include "quadEncoder.hpp"

#include "../../PHAL/gpio/gpio_interrupts.hpp"

#include <criticalSection.h>


using namespace Argos;

void quadEncoder::begin()
{

  delegate<void> callbackDelegate;

  pinMode(mPinChannel_1, INPUT);
  pinMode(mPinChannel_2, INPUT);
  pinMode(mPinEnableN, OUTPUT);

  mLastEncoderPulseTimeStamp = 0;
  mDeltaT_last = 0;
  mDeltaN = 0;
  mEncoderCounter = 0;
  mRpm = 0;

  //Interrupt config
  callbackDelegate.setCallback<quadEncoder, &quadEncoder::encoderPulseEventCallback>(this);

  GpioInterrupts.enable(GpioPinToIntSig(mPinChannel_1), callbackDelegate, GpioInterrupts.FALLING_EDGE);

  //Timer config
  callbackDelegate.setCallback<quadEncoder, &quadEncoder::observationWindowTimerTimeoutCallback>(this);

  mObservationWindowTimer.registerTimerCb(callbackDelegate);

  mObservationWindowTimer.begin(OSAL::iTimer::PERIODIC, mObservationWindowPeriodUs/1000);

  mObservationWindowTimer.start();

  digitalWrite(mPinEnableN, LOW);
  

}

float quadEncoder::getRPM()
{

  float rpmBuff;

  enterCriticalSection_OSAL();

  rpmBuff = mRpm;

  exitCriticalSection_OSAL();

  return rpmBuff;

}

uint16_t quadEncoder::getEncoderCount()
{

  float encoderCounterBuff;

  enterCriticalSection_OSAL();

  encoderCounterBuff = mEncoderCounter;

  exitCriticalSection_OSAL();

  return encoderCounterBuff;

}

void quadEncoder::clearEncoderCount()
{

  enterCriticalSection_OSAL();

  mEncoderCounter = 0;

  exitCriticalSection_OSAL();

}


void quadEncoder::registerOnSpeedSampleUpdateCallbackD(delegate<void, float, uint16_t> callbackDelegate)
{

   mOnSpeedSampleUpdateCallbackDelegate = callbackDelegate;

}

void quadEncoder::unregisterOnSpeedSampleUpdateCallbackD()
{

  mOnSpeedSampleUpdateCallbackDelegate = delegate<void, float, uint16_t>();

}

void quadEncoder::end()
{

  digitalWrite(mPinEnableN, HIGH);

  pinMode(mPinEnableN, INPUT);

  GpioInterrupts.disable(GpioPinToIntSig(mPinChannel_1));

  mObservationWindowTimer.stop();

  mObservationWindowTimer.end();


}


void quadEncoder::encoderPulseEventCallback()
{

 //Serial.println("Encoder pulse detected!");

  mLastEncoderPulseTimeStamp = micros();

  mChannel_2_lastValue = digitalRead(mPinChannel_2);

  ++mEncoderCounter;

  ++mDeltaN;

}


void quadEncoder::observationWindowTimerTimeoutCallback()
{

  uint32_t actualTimeStamp = micros();

  uint32_t deltaT = actualTimeStamp - mLastEncoderPulseTimeStamp;

  uint32_t elapseTimeObservationWindow = actualTimeStamp-mObservationWindowStartTimeStamp;


  if(mDeltaN != 0)
  {

    mRpm = ((float)mDeltaN*60*1000000)/((float)(( elapseTimeObservationWindow -deltaT+mDeltaT_last)*mEncoderStepsPerRevolution));

    mDeltaN = 0;

    mDeltaT_last = deltaT;

    mObservationWindowStartTimeStamp = actualTimeStamp;

    if(mSpinInverted == true)
    {

      //Check spin direcction
      if(mChannel_2_lastValue == true)
      {
        mRpm = -mRpm;
      }

    }
    else
    {

      //Check spin direcction
      if(mChannel_2_lastValue == false)
      {
        mRpm = -mRpm;
      }

    }


    //Call delegate to inform of the update value in speed
    mOnSpeedSampleUpdateCallbackDelegate(mRpm, mEncoderCounter);

  }
  else
  {

    if((elapseTimeObservationWindow > mElapsedWindowTimeMaxUs) && mRpm !=0)
    {
      mRpm = 0;

      //Call delegate to inform of the update value in speed
      mOnSpeedSampleUpdateCallbackDelegate(mRpm, mEncoderCounter);

    }

  }

}
