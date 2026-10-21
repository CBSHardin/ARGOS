

#ifndef _INTERFACE_IMU_HAL_H__
#define _INTERFACE_IMU_HAL_H__


#include <stdint.h>

#include "../../../utils/mathTypes.h"

#include "../../../utils/delegate/delegate.h"



//Supongamos que la interfaz de la IMU es:
class interfaceImuHAL
{

  public:

    enum samplingMode : uint8_t {ON_REQUEST, PERIODIC};

    virtual void begin(samplingMode mode) = 0;

    virtual void begin(samplingMode mode, uint16_t samplingRateAccelerometer, uint16_t samplingRateGyroscope) = 0;

    virtual bool getStatus() = 0;


    virtual void setSamplingMode(samplingMode mode) = 0;

    virtual samplingMode getSamplingMode() = 0;


    virtual void setSamplingRateAccelerometer(uint16_t samplingRate);

    virtual uint16_t getSamplingRateAccelerometer() = 0;


    virtual void setSamplingRateGyroscope(uint16_t samplingRate);

    virtual uint16_t getSamplingRateGyroscope() = 0;



    virtual void takeSampleAccelerometer() = 0;

    virtual void takeSampleGyroscope() = 0;


    virtual void getAngularSpeed(bool takeNewSample, floatVector3d sample) = 0;

    virtual void getLinealAcceleration(bool takeNewSample, floatVector3d sample) = 0;



    virtual bool setOnUpdateAccelerometerCallback(delegate<void, floatVector3d> callbackDelegate) = 0;

    virtual bool setOnUpdateGyroscopeCallback(delegate<void, floatVector3d> callbackDelegate) = 0;

    virtual void removeOnUpdateAccelerometerCallback(delegate<void, floatVector3d> callbackDelegate) = 0;

    virtual void removeOnUpdateGyroscopeCallback(delegate<void, floatVector3d> callbackDelegate) = 0;
  


    virtual void beginAccelerometerCalibration();

    virtual void endAccelerometerCalibration();


    virtual void beginGyroscopeCalibration();

    virtual void endGyroscopeCalibration();


    virtual bool doAccelerometerSelftest();

    virtual bool doGyroscopeSelftest();



    virtual void end() = 0;

  /*

    virtual bool setOnUpdateAccelerometerCallback(void (*funcCallbackPtr)(floatVector3d)) = 0;

    virtual bool setOnUpdateGyroscopeCallback(void (*funcCallbackPtr)(floatVector3d)) = 0;

    virtual void removeOnUpdateAccelerometerCallback(void (*funcCallbackPtr)(floatVector3d)) = 0;

    virtual void removeOnUpdateGyroscopeCallback(void (*funcCallbackPtr)(floatVector3d)) = 0;


    //Callback system for member functions
    template <class T>
    virtual bool setOnUpdateAccelerometerCallback(void (T::*memberFuncCallbackPtr)(floatVector3d)) = 0;

    template <class T>
    virtual bool setOnUpdateGyroscopeCallback(void (T::*memberFuncCallbackPtr)(floatVector3d)) = 0;

    template <class T>
    virtual void removeOnUpdateAccelerometerCallback(void (T::*memberFuncCallbackPtr)(floatVector3d)) = 0;

    template <class T>
    virtual void removeOnUpdateGyroscopeCallback(void (T::*memberFuncCallbackPtr)(floatVector3d)) = 0;
  */



};

#endif
