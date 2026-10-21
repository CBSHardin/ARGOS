

#include <argosAPI.h>

//#include "DDL/quadEncoder/quadEncoder.hpp"

#include <FreeRTOS.h>
#include <task.h>

#include <delegate.h>


//quadEncoder encoderM1(11, 26, 38, 25, 100, 1.0);


void powerTrainEncodersCallback(uint8_t motor, float rpm, uint16_t count)
{

  Serial.println("M"+String(motor)+" Rpm: "+String(rpm));
/*
  Serial.println("stack wm: "+String(uxTaskGetStackHighWaterMark(NULL)) );
  Serial.flush();
  */
}

void powerTrainEncodersSecondConsumerCallback(uint8_t motor, float rpm, uint16_t count)
{

  Serial.println("Consumer 2  M"+String(motor)+" Rpm: "+String(rpm));
/*
  Serial.println("stack wm: "+String(uxTaskGetStackHighWaterMark(NULL)) );
  Serial.flush();
  */
}


void setup()
{
  Serial.begin(115200);

  delay(300);

  delegate<void, uint8_t, float, uint16_t> callbackDelegate;
  callbackDelegate.setCallback<powerTrainEncodersCallback>();

  Argos::PowerTrainEncoders.registerOnSpeedSampleUpdateCallbackD(callbackDelegate);

  callbackDelegate.setCallback<powerTrainEncodersSecondConsumerCallback>();

  Argos::PowerTrainEncoders.registerOnSpeedSampleUpdateCallbackD(callbackDelegate);

  Argos::PowerTrainEncoders.begin();

  Serial.println("Encoder test rpm: ");
  Serial.flush();


  //Instanciación de los diferentes elementos
  //Instanciación de los diferentes elementos
  Argos::PowerTrain.begin();

  //Avanza, y se para
  Argos::PowerTrain.setSpinDirection(1, Argos::PowerTrain.FORWARD);
  Argos::PowerTrain.setSpinDirection(2, Argos::PowerTrain.FORWARD);
  Argos::PowerTrain.setSpinDirection(3, Argos::PowerTrain.FORWARD);
  Argos::PowerTrain.setSpinDirection(4, Argos::PowerTrain.FORWARD);

  Argos::PowerTrain.setDutyCycle(1, 200);
  Argos::PowerTrain.setDutyCycle(2, 200);
  Argos::PowerTrain.setDutyCycle(3, 200);
  Argos::PowerTrain.setDutyCycle(4, 200);

  delay(2000);

  Argos::PowerTrain.setDutyCycle(1, 0);
  Argos::PowerTrain.setDutyCycle(2, 0);
  Argos::PowerTrain.setDutyCycle(3, 0);
  Argos::PowerTrain.setDutyCycle(4, 0);

  Argos::PowerTrain.setSpinDirection(1, Argos::PowerTrain.BACKWARD);
  Argos::PowerTrain.setSpinDirection(2, Argos::PowerTrain.BACKWARD);
  Argos::PowerTrain.setSpinDirection(3, Argos::PowerTrain.BACKWARD);
  Argos::PowerTrain.setSpinDirection(4, Argos::PowerTrain.BACKWARD);

  delay(1000);

  Argos::PowerTrain.setDutyCycle(1, 200);
  Argos::PowerTrain.setDutyCycle(2, 200);
  Argos::PowerTrain.setDutyCycle(3, 200);
  Argos::PowerTrain.setDutyCycle(4, 200);

  delay(2000);

  Argos::PowerTrain.setDutyCycle(1, 0);
  Argos::PowerTrain.setDutyCycle(2, 0);
  Argos::PowerTrain.setDutyCycle(3, 0);
  Argos::PowerTrain.setDutyCycle(4, 0);


  //Giro a la derecha
  Argos::PowerTrain.setSpinDirection(1, Argos::PowerTrain.FORWARD);
  Argos::PowerTrain.setSpinDirection(2, Argos::PowerTrain.BACKWARD);
  Argos::PowerTrain.setSpinDirection(3, Argos::PowerTrain.FORWARD);
  Argos::PowerTrain.setSpinDirection(4, Argos::PowerTrain.BACKWARD);

  Argos::PowerTrain.setDutyCycle(1, 170);
  Argos::PowerTrain.setDutyCycle(2, 170);
  Argos::PowerTrain.setDutyCycle(3, 170);
  Argos::PowerTrain.setDutyCycle(4, 170);

  delay(1000);

  Argos::PowerTrain.setDutyCycle(1, 0);
  Argos::PowerTrain.setDutyCycle(2, 0);
  Argos::PowerTrain.setDutyCycle(3, 0);
  Argos::PowerTrain.setDutyCycle(4, 0);

  delay(200);

  //Giro a la izquierda
  Argos::PowerTrain.setSpinDirection(1, Argos::PowerTrain.BACKWARD);
  Argos::PowerTrain.setSpinDirection(2, Argos::PowerTrain.FORWARD);
  Argos::PowerTrain.setSpinDirection(3, Argos::PowerTrain.BACKWARD);
  Argos::PowerTrain.setSpinDirection(4, Argos::PowerTrain.FORWARD);

  Argos::PowerTrain.setDutyCycle(1, 170);
  Argos::PowerTrain.setDutyCycle(2, 170);
  Argos::PowerTrain.setDutyCycle(3, 170);
  Argos::PowerTrain.setDutyCycle(4, 170);

  delay(1000);

  Argos::PowerTrain.setDutyCycle(1, 0);
  Argos::PowerTrain.setDutyCycle(2, 0);
  Argos::PowerTrain.setDutyCycle(3, 0);
  Argos::PowerTrain.setDutyCycle(4, 0);


}


void loop()
{

//Serial.println("Rpm: "+String(encoderM1.getRPM()));

  delay(1000);

 //Serial.println("stack wm: "+String(uxTaskGetStackHighWaterMark(NULL)) );

  //Serial.println("heap wm: "+String(xPortGetMinimumEverFreeHeapSize()) );

}
