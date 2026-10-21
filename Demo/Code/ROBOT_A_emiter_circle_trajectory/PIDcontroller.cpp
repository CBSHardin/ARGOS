/////////////////////////////////////////////
//              PID Controller
/////////////////////////////////////////////

#include <Arduino.h>
#include "PIDController.h"



PIDController::PIDController(double P, double I, double D, double outputLowerLimit, double outputUpperLimit)
{

  kp = P;

  ki = I;

  kd = D;

  //Compute the terms for the difference equations
  k1 = kp + ki + kd;
  k2 = -kp - 2*kd;
  k3 = kd;

  //Set the output limits
  outLowerLimit = outputLowerLimit;
  outUpperLimit = outputUpperLimit;
  
  
}



double PIDController::computeOutput(double input, double feedback)
{

  e2 = e1;

  e1 = e;

  e = input - feedback;

  delta_u = k1*e + k2*e1 +k3*e2;

  u = u + delta_u;

  if(u >= outUpperLimit)
    u = outUpperLimit;

  else if( u < outLowerLimit)
    u = outLowerLimit;

  return u;
  
}


double PIDController::getKp()
{
  return kp;
}


double PIDController::getKi()
{
  return ki;
}


double PIDController::getKd()
{
  return kd;
}


double PIDController::getOuputLowerLimit()
{
  return outLowerLimit;
}


double  PIDController::getOutputUpperLimit()
{
  return outUpperLimit;
}


void PIDController::setKp(double P)
{
  kp = P;

  //Compute the terms for the difference equations
  k1 = kp + ki + kd;
  k2 = -kp - 2*kd;
  k3 = kd;
}

void PIDController::setKi(double I)
{
  ki = I;

  //Compute the terms for the difference equations
  k1 = kp + ki + kd;
  k2 = -kp - 2*kd;
  k3 = kd;
}

void PIDController::setKd(double D)
{
  kd = D;

  //Compute the terms for the difference equations
  k1 = kp + ki + kd;
  k2 = -kp - 2*kd;
  k3 = kd;
}

void PIDController::setOuputLowerLimit(double outputLowerLimit)
{
  outLowerLimit = outputLowerLimit;
}

void PIDController::setOuputUpperLimit(double outputUpperLimit)
{
  outUpperLimit = outputUpperLimit;
}

void PIDController::resetState()
{

  e = 0;
  e1 = 0;
  e2 = 0;
  u = 0;
  delta_u = 0;
}
