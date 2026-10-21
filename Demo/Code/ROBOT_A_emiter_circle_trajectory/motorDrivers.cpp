////////////////////////////////////////////
//        motorDrivers.cpp
////////////////////////////////////////////
//This file contains the implementation of low level control functions
//of motor system and the pin definitions for the hardware
//of the motors.

#include <Arduino.h>
#include "motorDrivers.h"
#include <util/atomic.h> 


volatile uint8_t faultsDrivers = 0;


void initMotorDrivers()
{

  pinMode(M1_SPIN_DIR, OUTPUT);
  pinMode(M2_SPIN_DIR, OUTPUT);  
  pinMode(M3_SPIN_DIR, OUTPUT);
  pinMode(M4_SPIN_DIR, OUTPUT);  

  digitalWrite(M1_SPIN_DIR, LOW);
  digitalWrite(M2_SPIN_DIR, LOW);
  digitalWrite(M3_SPIN_DIR, LOW);
  digitalWrite(M4_SPIN_DIR, LOW);

  pinMode( DRIVER_SLEEP_CRTL, OUTPUT);
  digitalWrite(DRIVER_SLEEP_CRTL, LOW);

  pinMode(M1_PWM, OUTPUT);
  pinMode(M2_PWM, OUTPUT);
  pinMode(M3_PWM, OUTPUT);
  pinMode(M4_PWM, OUTPUT);

  digitalWrite(M1_PWM, LOW);
  digitalWrite(M2_PWM, LOW);
  digitalWrite(M3_PWM, LOW);
  digitalWrite(M4_PWM, LOW);

  analogReference(EXTERNAL);

  pinMode(M1_DRIVER_FAULT, INPUT);
  pinMode(M2_DRIVER_FAULT, INPUT);
  pinMode(M3_DRIVER_FAULT, INPUT);
  pinMode(M4_DRIVER_FAULT, INPUT);

  pinMode(M1_CURRENT_SENSOR, INPUT);
  pinMode(M2_CURRENT_SENSOR, INPUT);
  pinMode(M3_CURRENT_SENSOR, INPUT);
  pinMode(M4_CURRENT_SENSOR, INPUT);

  //Config interrupts for detect faults in the driver motors
  PCMSK2 = PCMSK2 | 0xf0;
  PCICR = PCICR | 0x40;
  
}


void setPowerMotorDrivers(boolean power)
{
  
  if(power)
    digitalWrite(DRIVER_SLEEP_CRTL, HIGH);
  else
    digitalWrite(DRIVER_SLEEP_CRTL, LOW);
  
}


void setDirM1(motorSpinDir dir)
{

  if(dir == forward)
    digitalWrite(M1_SPIN_DIR, HIGH);
  else
    digitalWrite(M1_SPIN_DIR, LOW);
  
}


void setDirM2(motorSpinDir dir)
{

  if(dir == forward)
    digitalWrite(M2_SPIN_DIR, LOW);
  else
    digitalWrite(M2_SPIN_DIR, HIGH);
    
}


void setDirM3(motorSpinDir dir)
{
  
  if(dir == forward)
    digitalWrite(M3_SPIN_DIR, HIGH );
  else
    digitalWrite(M3_SPIN_DIR, LOW);
  
}


void setDirM4(motorSpinDir dir)
{
  
  if(dir == forward)
    digitalWrite(M4_SPIN_DIR, LOW);
  else
    digitalWrite(M4_SPIN_DIR, HIGH);
  
}


void setPWMM1(uint8_t pwm)
{

  analogWrite(M1_PWM, pwm);

}


void setPWMM2(uint8_t pwm)
{

  analogWrite(M2_PWM, pwm);
  
}


void setPWMM3(uint8_t pwm)
{
  
  analogWrite(M3_PWM, pwm);
  
}


void setPWMM4(uint8_t pwm)
{

  analogWrite(M4_PWM, pwm);
  
}


void setDirAndPWMM1(int16_t signedPWM)
{
  
  if(signedPWM >= 0)
  {
    setDirM1(forward);

    setPWMM1(signedPWM);
  } 
  else
  {
    setDirM1(backward);
    setPWMM1((signedPWM*-1));
  }
  
}


void setDirAndPWMM2(int16_t signedPWM)
{
  
  if(signedPWM >= 0)
  {
    setDirM2(forward);

    setPWMM2(signedPWM);
  } 
  else
  {
    setDirM2(backward);
    setPWMM2((signedPWM*-1));
  }
  
}


void setDirAndPWMM3(int16_t signedPWM)
{
  
  if(signedPWM >= 0)
  {
    setDirM3(forward);

    setPWMM3(signedPWM);
  } 
  else
  {
    setDirM3(backward);
    setPWMM3((signedPWM*-1));
  }
  
}


void setDirAndPWMM4(int16_t signedPWM)
{
  
  if(signedPWM >= 0)
  {
    setDirM4(forward);

    setPWMM4(signedPWM);
  } 
  else
  {
    setDirM4(backward);
    setPWMM4((signedPWM*-1));
  }
  
}



//Devuelve el estado del driver de los motores en el nibble bajo de 
//byte de retorno
uint8_t getMotorDriverFaultStatus()
{

  ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
  {

    return faultsDrivers;
    
  }

}


void clearMotorDriverFaultStatus()
{


  ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
  {

    faultsDrivers = 0;
    
  }
  
}


uint16_t getMotorDriverCurrentM1()
{

  return analogRead(M1_CURRENT_SENSOR);
  
}

uint16_t getMotorDriverCurrentM2()
{

  return analogRead(M2_CURRENT_SENSOR);
  
}


uint16_t getMotorDriverCurrentM3()
{

  return analogRead(M3_CURRENT_SENSOR);
  
}


uint16_t getMotorDriverCurrentM4()
{
  
  return analogRead(M4_CURRENT_SENSOR);
  
}


//Interruption routine for to detect faults in the motor drivers
ISR(PCINT2_vect)
{

  if(digitalRead(M1_DRIVER_FAULT) == LOW)
    faultsDrivers |= 0x01;

  if(digitalRead(M2_DRIVER_FAULT) == LOW)
    faultsDrivers |= 0x02;

  if(digitalRead(M3_DRIVER_FAULT) == LOW)
    faultsDrivers |= 0x04;
  
  if(digitalRead(M4_DRIVER_FAULT)== LOW)
    faultsDrivers |= 0x08;  
    
}
