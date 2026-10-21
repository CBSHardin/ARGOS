

#include <argosAPI.h>

#include "DHAL/actuators/power_train/implementations/dfrobot_chasis/dfrobot_chasis_power_train.hpp"

#include <delegate.h>



void driverFaultDetectedCallback(uint8_t motor)
{
  Serial.println("Error, driver fault detected: "+String(motor));
}


void setup()
{

  Serial.begin(115200);


  Argos::PowerTrain.begin();

  pinMode(15, INPUT);
  pinMode(14, INPUT);
  pinMode(70, INPUT);
  pinMode(71, INPUT);

  delegate<void, uint8_t> driverFaultDelegate;

  driverFaultDelegate.setCallback<driverFaultDetectedCallback>();

  Argos::PowerTrain.registerOnFaultCallbackDelegate(driverFaultDelegate);

  Argos::PowerTrain.setSpinDirection(1, Argos::PowerTrain.FORWARD);
  Argos::PowerTrain.setSpinDirection(2, Argos::PowerTrain.FORWARD);
  Argos::PowerTrain.setSpinDirection(3, Argos::PowerTrain.FORWARD);
  Argos::PowerTrain.setSpinDirection(4, Argos::PowerTrain.FORWARD);

  Argos::PowerTrain.setDutyCycle(1, 120);
  Argos::PowerTrain.setDutyCycle(2, 120);
  Argos::PowerTrain.setDutyCycle(3, 120);
  Argos::PowerTrain.setDutyCycle(4, 120);

/*
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

*/

}


void loop()
{


}
