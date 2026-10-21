/*
Esta clase proporciona una interfaz hardware-agnostica para la IMU del sistema
*/

#ifndef _IMU_HAL_H__
#define _IMU_HAL_H__

#include "interfaceImuHAL.h"

#include "../../../utils/mathTypes.h"

#include "../../../utils/delegate/delegate.h"



class imuHAL
{

  public:

    imuHAL(interfaceImuHAL* instancePtr)
    {

      ptrImuInstance = instancePtr;

    }



    inline void begin(interfaceImuHAL::samplingMode mode)
    {

      ptrImuInstance->begin(mode);

    }



    inline void begin(interfaceImuHAL::samplingMode mode, uint16_t samplingRateAccelerometer, uint16_t samplingRateGyroscope)
    {

      ptrImuInstance->begin(mode, samplingRateAccelerometer, samplingRateGyroscope);

    }



    inline bool getStatus()
    {

        return ptrImuInstance->getStatus();

    }



    inline void setSamplingMode(interfaceImuHAL::samplingMode mode)
    {

      ptrImuInstance->setSamplingMode(mode);

    }



    inline interfaceImuHAL::samplingMode getSamplingMode()
    {

      return ptrImuInstance->getSamplingMode();

    }



    inline void setSamplingRateAccelerometer(uint16_t samplingRate)
    {

      ptrImuInstance->setSamplingRateAccelerometer(samplingRate);

    }



    inline uint16_t getSamplingRateAccelerometer()
    {

      return ptrImuInstance->getSamplingRateAccelerometer();

    }



    inline void setSamplingRateGyroscope(uint16_t samplingRate)
    {

      ptrImuInstance->setSamplingRateGyroscope(samplingRate);

    }



    inline uint16_t getSamplingRateGyroscope()
    {

      return ptrImuInstance->getSamplingRateGyroscope();

    }



    inline void takeSampleAccelerometer()
    {

      ptrImuInstance->takeSampleAccelerometer();

    }



    inline void takeSampleGyroscope()
    {

      ptrImuInstance->takeSampleGyroscope();

    }



    inline void getAngularSpeed(bool takeNewSample, floatVector3d sample)
    {

      return ptrImuInstance->getAngularSpeed(takeNewSample, sample);

    }



    inline void getLinealAcceleration(bool takeNewSample, floatVector3d sample)
    {

      return ptrImuInstance->getLinealAcceleration(takeNewSample, sample);

    }



    inline bool setOnUpdateAccelerometerCallback(delegate<void, floatVector3d> callbackDelegate)
    {

      return ptrImuInstance->setOnUpdateAccelerometerCallback(callbackDelegate);

    }



    inline bool setOnUpdateGyroscopeCallback(delegate<void, floatVector3d> callbackDelegate)
    {

      return ptrImuInstance->setOnUpdateGyroscopeCallback(callbackDelegate);

    }



    inline void removeOnUpdateAccelerometerCallback(delegate<void, floatVector3d> callbackDelegate)
    {

      return ptrImuInstance->removeOnUpdateAccelerometerCallback(callbackDelegate);

    }



    inline void removeOnUpdateGyroscopeCallback(delegate<void, floatVector3d> callbackDelegate)
    {

      return ptrImuInstance->removeOnUpdateGyroscopeCallback(callbackDelegate);

    }



    inline void beginAccelerometerCalibration()
    {

      ptrImuInstance->beginAccelerometerCalibration();

    }



    inline void endAccelerometerCalibration()
    {

      ptrImuInstance->endAccelerometerCalibration();

    }



    inline void beginGyroscopeCalibration()
    {

      ptrImuInstance->beginGyroscopeCalibration();

    }



    inline void endGyroscopeCalibration()
    {

      ptrImuInstance->endGyroscopeCalibration();

    }



    inline bool doAccelerometerSelftest()
    {

      return ptrImuInstance->doAccelerometerSelftest();

    }



    inline bool doGyroscopeSelftest()
    {

        return ptrImuInstance->doGyroscopeSelftest();

    }



    inline void end()
    {

      ptrImuInstance->end();

    }


  private:
    interfaceImuHAL* ptrImuInstance;

};

extern imuHAL imu;

#endif
