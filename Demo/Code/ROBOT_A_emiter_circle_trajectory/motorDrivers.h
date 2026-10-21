////////////////////////////////////////////
//        motorDrivers.h
////////////////////////////////////////////
//This file contains the low level control functions
//of motor system and the pin definitions for the hardware
//of the motors.

#ifndef __MOTOR_DRIVERS.H_
#define __MOTOR_DRIVERS.H_

//Driver PWM pins
#define M1_PWM             7
#define M2_PWM             8
#define M3_PWM             6
#define M4_PWM             9

//Driver direction control pin
#define M1_SPIN_DIR       22
#define M2_SPIN_DIR       23
#define M3_SPIN_DIR       24
#define M4_SPIN_DIR       25

//Driver sleep control pin
#define DRIVER_SLEEP_CRTL 36

//Driver fail pin
#define M1_DRIVER_FAULT 68
#define M2_DRIVER_FAULT 69
#define M3_DRIVER_FAULT 66
#define M4_DRIVER_FAULT 67

//Driver current sense pin
#define M1_CURRENT_SENSOR A10
#define M2_CURRENT_SENSOR A11
#define M3_CURRENT_SENSOR A9
#define M4_CURRENT_SENSOR A8


typedef enum {forward, backward} motorSpinDir;

//Functions
void initMotorDrivers();

void setPowerMotorDrivers(boolean power);

void setDirM1(motorSpinDir dir);

void setDirM2(motorSpinDir dir);

void setDirM3(motorSpinDir dir);

void setDirM4(motorSpinDir dir);

void setPWMM1(uint8_t pwm);

void setPWMM2(uint8_t pwm);

void setPWMM3(uint8_t pwm);

void setPWMM4(uint8_t pwm);

void setDirAndPWMM1(int16_t signedPWM);

void setDirAndPWMM2(int16_t signedPWM);

void setDirAndPWMM3(int16_t signedPWM);

void setDirAndPWMM4(int16_t signedPWM);


//Devuelve el estado del driver de los motores en el nibble bajo de 
//byte de retorno
uint8_t getMotorDriverFaultStatus();

void clearMotorDriverFaultStatus();

uint16_t getMotorDriverCurrentM1();

uint16_t getMotorDriverCurrentM2();

uint16_t getMotorDriverCurrentM3();

uint16_t getMotorDriverCurrentM4();

#endif
