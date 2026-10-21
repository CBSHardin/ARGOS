/////////////////////////////////////////////
//              PID Controller
/////////////////////////////////////////////


#ifndef __PID_CONTROLLER_H_
#define __PID_CONTROLLER_H_


class PIDController
{
  public:

    PIDController(double P, double I, double D, double outputLowerLimit, double outputUpperLimit);

 
    double computeOutput(double input, double sensorOutput);

    double getKp();

    double getKi();

    double getKd();

    double getOuputLowerLimit();

    double getOutputUpperLimit();
    

    void setKp(double P);

    void setKi(double I);

    void setKd(double D);

    void setOuputLowerLimit(double outputLowerLimit);

    void setOuputUpperLimit(double outputUpperLimit);

    void resetState();


  private:

    double kp, ki, kd;

    double k1, k2, k3;

    double e, e1, e2, u, delta_u;

    double outLowerLimit, outUpperLimit;
  
};

#endif
