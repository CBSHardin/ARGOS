///////////////////////////////////////////////
// Motor controller task
///////////////////////////////////////////////
// This task controlls the speed of the motors.
// The angular speed can be set using the functions:
// setSpeedMx(int16_t angularSpeed);
// To stop the motor, you should set the angularSpeed to 0.

#include <Arduino.h>
#include "encoders.h"
#include "motorDrivers.h"
#include "PIDController.h"


double M1_set_RPM = 0;
double M2_set_RPM = 0;
double M3_set_RPM = 0;
double M4_set_RPM = 0;

enum {INIT, ACTIVE} state = INIT;

//PIDController M4PID( 0.015, 0.001, 0.001, 0, 255);
/*
PIDController M1PID( 1.41, 0.04,  12.55, -255, 255);

PIDController M2PID( 1.41, 0.04,  12.55, -255, 255);

PIDController M3PID( 1.41, 0.04,  12.55, -255, 255);

PIDController M4PID( 1.41, 0.04,  12.55, -255, 255);
*/
/*
 * Controlador con un poco de overshoot
PIDController M1PID( 0.015, 0.02, 0.7, 0, 255);

PIDController M2PID( 0.015, 0.02, 0.7, 0, 255);

PIDController M3PID( 0.015, 0.02, 0.7, 0, 255);

PIDController M4PID( 0.015, 0.02, 0.7, 0, 255);
*/


//PID ajustado nuevo ( 0.003, 0.007133, 0.00031542, 0, 255);
PIDController M1PID( 0.003, 0.007133, 0.00031542, -180, 180);

PIDController M2PID( 0.003, 0.007133, 0.00031542, -180, 180);

PIDController M3PID( 0.003, 0.007133, 0.00031542, -180, 180);
//PIDController M3PID( 0.00192, 0.00456512, 0.0002018688, -255, 255);

PIDController M4PID( 0.003, 0.007133, 0.00031542, -180, 180);


void motorControllerTask()
{

  double PWMM1Output, PWMM2Output, PWMM3Output, PWMM4Output;

  //Configure ports and inerruptions for to use the drivers
  //and the encoders.
  if(state == INIT)
  {

    initMotorDrivers();
    
    initEncoders();
    
    setPowerMotorDrivers(true);

    state = ACTIVE;
   
  }
  else
  {
    //Todo sistema de control debe modificar sus salidas al mismo tiempo
    //Execute the PID task
    if(M1_set_RPM == 0 && getSignedRPMM1()<= 0.06) 
      PWMM1Output = 0;
    else
      PWMM1Output = M1PID.computeOutput(M1_set_RPM, getSignedRPMM1());
      
    if(M2_set_RPM == 0 && getSignedRPMM2()<= 0.06) 
      PWMM2Output = 0;
    else
      PWMM2Output = M2PID.computeOutput(M2_set_RPM, getSignedRPMM2()) ;
     

    if(M3_set_RPM == 0 && getSignedRPMM3()<= 0.06) 
      PWMM3Output = 0;
    else
      PWMM3Output = M3PID.computeOutput(M3_set_RPM, getSignedRPMM3()) ;
      
    if(M4_set_RPM == 0 && getSignedRPMM4()<= 0.06) 
      PWMM4Output = 0;
    else    
      PWMM4Output = M4PID.computeOutput(M4_set_RPM, getSignedRPMM4()) ;


    //Actualizamos todas las salidas a la vez
    setDirAndPWMM1(PWMM1Output);
    setDirAndPWMM2(PWMM2Output);
    setDirAndPWMM3(PWMM3Output);
    setDirAndPWMM4(PWMM4Output);
    
  }
  
}


void setM1RPM(double rpm)
{
  M1_set_RPM = rpm;
}


void setM2RPM(double rpm)
{
  M2_set_RPM = rpm;
}


void setM3RPM(double rpm)
{
  M3_set_RPM = rpm;
}


void setM4RPM(double rpm)
{
  M4_set_RPM = rpm;
}


void setM1angularSpeed(double w)
{

  M1_set_RPM = 30.0*w/(3.141592653589); 
  
}


void setM2angularSpeed(double w)
{

  M2_set_RPM = 30.0*w/(3.141592653589); 
  
}


void setM3angularSpeed(double w)
{

  M3_set_RPM = 30.0*w/(3.141592653589); 

}


void setM4angularSpeed(double w)
{

  M4_set_RPM = 30.0*w/(3.141592653589); 
  
}


void resetStateMotorController()
{

  M1PID.resetState();
  M2PID.resetState();
  M3PID.resetState();
  M4PID.resetState();
  
}
