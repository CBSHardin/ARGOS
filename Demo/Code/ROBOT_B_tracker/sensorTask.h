////////////////////////////////////////////////////////////////////////////////////////////
// Tarea que se ejecutarara para actualizar los sensores: magnetometro y sensor de posición
//

#include <Arduino.h>

#ifndef _SENSOR_TASK_H__
#define _SENSOR_TASK_H__

/////////// Prototipos /////////
void lowBatteryGuardianInit();
void initConfigMagnetometer();
float getAngleToNorthCompensated();
void ISR_MAG();
void magnetometerScaleCompensation(int16_t axisXInput, int16_t axisYInput, float scaleX, float scaleY, float* axisXoutput, float* axisYoutput);
void lowBatteryGuardian();
void encodersWatchdog(); 

void magnetometerCalibration();

void sensorTaskInit();
void sensorTaskDo();

#endif
