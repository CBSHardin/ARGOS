////////////////////////////////////////////////////////////////////////////////////////////
// Tarea que se ejecutarara para actualizar los sensores: magnetometro y sensor de posición
//


#include "sensorTask.h"

#include "imu.h"
#include "fuelGauge.h"
#include "motorDrivers.h"
#include "RB_sensor.h"

#include "robotStates.h"


#define lowBatLevel 7300
//fuelGauge batteryMonitor;

//////////// Variables //////////
extern states state;

long lastTimeBatteryGuardian;

sensorsValuesMatrix ADCValues;


///////////////// RB sensor variables ////////////
float distance_to_emiter;
float angle_to_emiter;
boolean inRange;


////////// MAGNETOMETER RELATED VARIABLES /////////

imu IMU;

float northAngleRefOrigin;
float thetaMag;

boolean intMagRecieved = false;
/*
int16_t offsetX = -110, offsetY = -1;
float scaleX = 0.9712f, scaleY = 1.0306;
*/

/*Robot nuevo
 * */
int16_t offsetX = -19, offsetY = 38;
float scaleX = 0.9851f, scaleY = 1.0153;

/* Robot viejo
int16_t offsetX = -96, offsetY = -2;
float scaleX = 0.9719f, scaleY = 1.0297;
*/
/////////////// LOW BATTERY GUARDIAN VARIABLES /////////
#define lowBatLevel 7300
fuelGauge batteryMonitor;




void sensorTaskInit()
{

  //Encedemos el sensor RX
  init_R_B_sensor_rx();

  setPowerRB_sensor_rx(true);

  lowBatteryGuardianInit();

  initConfigMagnetometer();

  
  northAngleRefOrigin = getAngleToNorthCompensated();

  IMU.setMagnetometerDataRate(ODR_M_100);

  IMU.attachMagnetometerDRDYInterruptISR(ISR_MAG);
  IMU.setMagnetometerCaptureMode(CONTINUOUS_MODE);

  
  
}



void sensorTaskDo()
{

  switch(state)
  {

    case RUNNING:

      //Capturamos datos de la distancia al objetivo
      if(readSensorsADCValuesNonBlocking(ADCValues))
       {

          uint32_t measures[8];

          computeSensorMeasures(ADCValues, measures);


          if(computeDistanceAndBearing(measures, &distance_to_emiter, &angle_to_emiter))
          {
/*
            for(uint8_t i = 0; i < 8 ; +i++)
              Serial.println("Medida["+String(i)+"]: "+String(measures[i]));
              */

            inRange = true;

          }
          else
            inRange = false;

       }

      break;

  }

  //if(intMagRecieved == true)
  if(digitalRead(INTERFACE_IMU_INT_MAG) == HIGH)
  {
    vector3D data;

    float axisXcomp, axisYcomp;
  
    IMU.getMagneticVector(data);

    magnetometerScaleCompensation(data[0], data[1], scaleX, scaleY, &axisXcomp, &axisYcomp);
  
    thetaMag = atan2(axisYcomp, axisXcomp)-northAngleRefOrigin;

    thetaMag = atan2(sin(thetaMag), cos(thetaMag));
    
    intMagRecieved = false;

   // Serial.println("Inte magnetometro");
  }


  if(millis()-lastTimeBatteryGuardian>1000)
  {

    lowBatteryGuardian();

    lastTimeBatteryGuardian = millis();

  }





}



//////////////////// MAGNETOMETER RELATED FUNCTIONS /////////////////
void initConfigMagnetometer()
{

  IMU.resetMagnetometer();
  IMU.setMagnetometerPowerMode(M_HIGH_RESOLUTION_MODE);
  IMU.setMagnetometerDataRate(ODR_M_100);
  IMU.setMagnetometerCaptureMode(IDLE_MODE);
  IMU.setMagnetometerFilter(true);
  IMU.setMagnetometerOffsetCompContinuousMode(true);
  IMU.setMagnetometerOffsetCompSingleMode(true);
  IMU.setMagnetometerDRDYInterrupt(true);
  IMU.setMagnetometerOffsetPulseSetFreq(SET_PULSE_EVERY_63_ODR);

  //Compensación de hard iron
 // int16_t offsetX = -57;    old
//  int16_t offsetY = 35;

  IMU.setMagnetometerHardIronCompensation(offsetX, offsetY, 0);

}


void getSampleSModeMagnetometer(int16_t* axisX, int16_t* axisY)
{

  vector3D data, data2;

  //Establecemos el ángulo de origen para el magnetómetro, usamos la compensación de offset
  IMU.setMagnetometerCaptureMode(SINGLE_MODE);
  while (digitalRead(INTERFACE_IMU_INT_MAG) == LOW);
  IMU.getMagneticVector(data);

  IMU.setMagnetometerCaptureMode(SINGLE_MODE);
  while (digitalRead(INTERFACE_IMU_INT_MAG) == LOW);
  IMU.getMagneticVector(data2);

  *axisY = (data[1] + data2[1])/2;

  *axisX = (data[0] + data2[0])/2;

}


void magnetometerScaleCompensation(int16_t axisXInput, int16_t axisYInput, float scaleX, float scaleY, float* axisXoutput, float* axisYoutput)
{

  *axisXoutput = ((float)axisXInput)*scaleX;

  *axisYoutput = ((float)axisYInput)*scaleY;

}

float getAngleToNorthCompensated()
{

  //Capturamos la muestra que hará de referencia
  int16_t axisXSample, axisYSample;
  float axisXSampleComp, axisYSampleComp;

  getSampleSModeMagnetometer(&axisXSample, &axisYSample);

  magnetometerScaleCompensation(axisXSample, axisYSample, scaleX, scaleY, &axisXSampleComp, &axisYSampleComp);

  return atan2(axisYSampleComp, axisXSampleComp);

}


void magnetometerCalibration()
{

  Serial.println("Iniciando calibración");

  //Encendemos los motores para iniciar la rotación
  setDirAndPWMM1(40);
  setDirAndPWMM2(-40);
  setDirAndPWMM3(40);
  setDirAndPWMM4(-40);


  IMU.setMagnetometerHardIronCompensation(0, 0, 0);

  delay(100);

  IMU.magnetometerCalibrationXY(6000, &offsetX, &offsetY, &scaleX, &scaleY );

  //Desconectamos motores
  setDirAndPWMM1(0);
  setDirAndPWMM2(0);
  setDirAndPWMM3(0);
  setDirAndPWMM4(0);

  //Mostramos valores de calibración
  Serial.println("Calibración terminada: \n\taxis_x_hard_iron="+String(offsetX)+"  \n\taxis_y_hard_iron="+String(offsetY)+"  \n\taxis_x_scale="+String(scaleX, 4)+ "  \n\taxis_y_scale="+String(scaleY, 4));

  IMU.setMagnetometerHardIronCompensation(offsetX, offsetY, 0);

}

void ISR_MAG()
{
  intMagRecieved = true;

}



///////////////////// LOW BAT GUARDIAN //////////////////

void lowBatteryGuardianInit()
{
  DDRJ |= 0x80;
  PORTJ &= ~0x80;
}

void lowBatteryGuardian()
{

  uint16_t voltage;

  fuelGauge::errCode err = batteryMonitor.getVoltage(&voltage);

  if(voltage < lowBatLevel)
    PORTJ |= 0x80;
  else
    PORTJ &= ~0x80;
}
