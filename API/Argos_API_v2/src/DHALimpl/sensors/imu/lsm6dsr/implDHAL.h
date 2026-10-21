
#ifndef _IMPL_DHAL_LSM6DSR_H__
#define _IMPL_DHAL_LSM6DSR_H__

#include "../../../../DHAL/sensors/imu/interfaceImuHAL.h"

#include "../../../../utils/delegate/delegate.h"

#include <stdint.h>

//Esta clase implementa las funciones definidas en la interfaz de la imu,
//y las conecta con las funciones de la biblioteca deviceDriverLater.
//Es una forma de fachada.



class implDHAL_lsm6dsr : public interfaceImuHAL
{

  public:

    implDHAL_lsm6dsr(uint8_t taskPriority);

    //Interface defined functions
    void begin(samplingMode mode);

    void begin(samplingMode mode, uint16_t samplingRateAccelerometer, uint16_t samplingRateGyroscope);

    bool getStatus();


    void setSamplingMode(samplingMode mode);

    samplingMode getSamplingMode();


    void setSamplingRateAccelerometer(uint16_t samplingRate);

    uint16_t getSamplingRateAccelerometer();


    void setSamplingRateGyroscope(uint16_t samplingRate);

    uint16_t getSamplingRateGyroscope();



    void takeSampleAccelerometer();

    void takeSampleGyroscope();


    void getAngularSpeed(bool takeNewSample, floatVector3d sample);

    void getLinealAcceleration(bool takeNewSample, floatVector3d sample);



    bool setOnUpdateAccelerometerCallback(delegate<void, floatVector3d> callbackDelegate);

    bool setOnUpdateGyroscopeCallback(delegate<void, floatVector3d> callbackDelegate);

    void removeOnUpdateAccelerometerCallback(delegate<void, floatVector3d> callbackDelegate);

    void removeOnUpdateGyroscopeCallback(delegate<void, floatVector3d> callbackDelegate);


    void beginAccelerometerCalibration();

    void endAccelerometerCalibration();


    void beginGyroscopeCalibration();

    void endGyroscopeCalibration();


    bool doAccelerometerSelftest();

    bool doGyroscopeSelftest();


    void end();

  private:

    delegate<void, floatVector3d> callbackFunc;


};


#endif
