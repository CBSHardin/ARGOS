
#ifndef __ARGOS_API_QUAD_ENCODER_H__
#define __ARGOS_API_QUAD_ENCODER_H__

#include <stdlib.h>
#include <stdint.h>

#include <iTimer.hpp>
#include <TimerFreeRTOS.hpp>

//Este encoder usará el algoritmo presentado en el paper:
// An Embedded System for Position and Speed Measurement Adopting Incremental Encoder
// https://www.researchgate.net/publication/3175158


class quadEncoder
{

  public:

    quadEncoder(uint8_t pinChannel_1, uint8_t pinChannel_2, uint8_t pinEnableN, uint8_t encoderStepsPerRevolution, uint32_t observationWindowPeriodMs, float minRPM, bool spinInverted = false) :
    mPinChannel_1(pinChannel_1),
    mPinChannel_2(pinChannel_2),
    mPinEnableN(pinEnableN),
    mEncoderStepsPerRevolution(encoderStepsPerRevolution),
    mObservationWindowPeriodUs(observationWindowPeriodMs*1000),
    mObservationWindowTimer(mRealTimer),
    mRpm(0),
    mElapsedWindowTimeMaxUs(((float)60*1000000)/((float)minRPM*encoderStepsPerRevolution)),
    mSpinInverted(spinInverted)
    {};

    void begin();

    float getRPM();

    uint16_t getEncoderCount();

    void clearEncoderCount();

    void registerOnSpeedSampleUpdateCallbackD(delegate<void, float, uint16_t> callbackDelegate);

    void unregisterOnSpeedSampleUpdateCallbackD();

    void end();


  private:

    //Config parameters
    const uint8_t mPinChannel_1;
    const uint8_t mPinChannel_2;
    const uint8_t mPinEnableN;
    const uint8_t mEncoderStepsPerRevolution;
    const bool mSpinInverted;

    uint16_t mEncoderCounter;

    //Speed algorithm related
    const uint32_t mObservationWindowPeriodUs;
    uint32_t mObservationWindowStartTimeStamp;
    uint32_t mLastEncoderPulseTimeStamp;
    uint32_t mDeltaT_last;
    uint32_t mElapsedWindowTimeMaxUs;
    uint16_t mDeltaN;
    uint8_t mChannel_2_lastValue;

    float mRpm;

    delegate<void, float, uint16_t> mOnSpeedSampleUpdateCallbackDelegate;

    void encoderPulseEventCallback();
    void observationWindowTimerTimeoutCallback();

    //Timer related
    OSAL::TimerFreeRTOS mRealTimer;
    OSAL::iTimer& mObservationWindowTimer;

};


#endif
