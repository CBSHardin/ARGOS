
//Y si aqui quisieras usar RTOS, podríamoshacerlo.
//El codigo que hay por debajo debe ser hardware agnostic para mejorar la
//compatibilidad.

//Pero si por ejemplo, queremos usar una IMU simulada, podríamos hacerlo igual.
//Solo hay que cambiar la carpeta que implementa la imu, o mejor dicho, cambiar
//el archivo imuHalImplementation.c

#include "../implDHAL.h"

#include <Arduino.h>

implDHAL_lsm6dsr::implDHAL_lsm6dsr(uint8_t taskPriority)
{

  //Serial.println("imu lsm6dsr construida. Prioridad: "+String(taskPriority));


}

//Interface defined functions
void implDHAL_lsm6dsr::begin(samplingMode mode)
{

  Serial.println("imu lsm6dsr begin");




}

void implDHAL_lsm6dsr::begin(samplingMode mode, uint16_t samplingRateAccelerometer, uint16_t samplingRateGyroscope)
{



}

bool implDHAL_lsm6dsr::getStatus()
{



}


void implDHAL_lsm6dsr::setSamplingMode(samplingMode mode)
{

  Serial.println("imu lsm6dsr sampling mode: "+String(mode));


}


interfaceImuHAL::samplingMode implDHAL_lsm6dsr::getSamplingMode()
{



}


void implDHAL_lsm6dsr::setSamplingRateAccelerometer(uint16_t samplingRate)
{



}

uint16_t implDHAL_lsm6dsr::getSamplingRateAccelerometer()
{



}


void implDHAL_lsm6dsr::setSamplingRateGyroscope(uint16_t samplingRate)
{



}

uint16_t implDHAL_lsm6dsr::getSamplingRateGyroscope()
{



}



void implDHAL_lsm6dsr::takeSampleAccelerometer()
{



}

void implDHAL_lsm6dsr::takeSampleGyroscope()
{



}


void implDHAL_lsm6dsr::getAngularSpeed(bool takeNewSample, floatVector3d sample)
{



}

void implDHAL_lsm6dsr::getLinealAcceleration(bool takeNewSample, floatVector3d sample)
{



}



bool implDHAL_lsm6dsr::setOnUpdateAccelerometerCallback(delegate<void, floatVector3d> callbackDelegate)
{

  callbackFunc = callbackDelegate;

}




bool implDHAL_lsm6dsr::setOnUpdateGyroscopeCallback(delegate<void, floatVector3d> callbackDelegate)
{



}

void implDHAL_lsm6dsr::removeOnUpdateAccelerometerCallback(delegate<void, floatVector3d> callbackDelegate)
{



}

void implDHAL_lsm6dsr::removeOnUpdateGyroscopeCallback(delegate<void, floatVector3d> callbackDelegate)
{



}


void implDHAL_lsm6dsr::beginAccelerometerCalibration()
{



}

void implDHAL_lsm6dsr::endAccelerometerCalibration()
{



}


void implDHAL_lsm6dsr::beginGyroscopeCalibration()
{



}

void implDHAL_lsm6dsr::endGyroscopeCalibration()
{



}


bool implDHAL_lsm6dsr::doAccelerometerSelftest()
{

  floatVector3d res = {10001.0f, 1970.0f, 1943.0f};

  callbackFunc(res);

}

bool implDHAL_lsm6dsr::doGyroscopeSelftest()
{



}


void implDHAL_lsm6dsr::end()
{



}
