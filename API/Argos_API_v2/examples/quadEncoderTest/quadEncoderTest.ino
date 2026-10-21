

#include <argosAPI.h>

#include "DDL/quadEncoder/quadEncoder.hpp"

#include <FreeRTOS.h>
#include <task.h>

#include <delegate.h>


quadEncoder encoderM1(11, 26, 38, 25, 100, 1.0);


void encoderM1Callback(float rpm, uint16_t count)
{
  /*
  Serial.println("Rpm: "+String(encoderM1.getRPM()));

  Serial.println("stack wm: "+String(uxTaskGetStackHighWaterMark(NULL)) );
  Serial.flush();
  */
}



void setup()
{
  Serial.begin(115200);

  delay(300);

  delegate<void, float, uint16_t> callbackDelegate;
  callbackDelegate.setCallback<encoderM1Callback>();

  encoderM1.registerOnSpeedSampleUpdateCallbackD(callbackDelegate);

  encoderM1.begin();

  Serial.println("Encoder init rpm: "+String(encoderM1.getRPM()));
  Serial.flush();


  //Instanciación de los diferentes elementos
  Argos::PowerTrain.begin();

  Argos::PowerTrain.setSpinDirection(1, Argos::PowerTrain.FORWARD);
  Argos::PowerTrain.setDutyCycle(1, 120);


  //Serial.println("stack wm: "+String(uxTaskGetStackHighWaterMark(NULL)) );

//  Serial.println("heap wm: "+String(xPortGetMinimumEverFreeHeapSize()) );

}


void loop()
{

Serial.println("Rpm: "+String(encoderM1.getRPM()));

  delay(1000);

 //Serial.println("stack wm: "+String(uxTaskGetStackHighWaterMark(NULL)) );

  //Serial.println("heap wm: "+String(xPortGetMinimumEverFreeHeapSize()) );

}
