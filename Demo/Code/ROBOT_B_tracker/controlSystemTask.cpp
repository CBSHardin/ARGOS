//////////////////////////////////
// Esta tarea ejecutará los sistemas de control del robot
//

#include "controlSystemTask.h"

#include "PIDController.h"
#include "encoders.h"

#include "motor_controller_task.h"
#include "motorDrivers.h"
#include "RB_sensor.h"

#include "robotStates.h"

#define distanceBetweenSamples 0.02f   //2 centímetros
#define dist_final 4.0f               //4 metros

///////////// Prototipos //////////////
extern void motorControllerTask();
void odometryComputeState();
void controlLoopBearingController();
void controlLoopBearingAndSpeedController();


/////////////////// CONTROL VARIABLES ///////////////////
PIDController angleController(2, 0, 0, -20, 20); //Kp=12.5, PID inestable, lo bajamos a la mitad, Ki=3, PID inestable, lo bajamos a la mitad
PIDController speedController(2, 0, 0, -20, 20); //Kp=12.5, PID inestable, lo bajamos a la mitad, Ki=3, PID inestable, lo bajamos a la mitad

float u_angle_objetive = 0;
float u_distance = 0.2;

//float v_lineal = -0.15;  //5 centimetros segundo
float v_lineal = 0;  //0 centimetros segundo

uint8_t sensorSelector = 0; //0 odometry, 1 magnetometer

extern float distance_to_emiter;
extern float angle_objetive;
extern boolean inRange;



////////////////////// ODOMETRY MODULE /////////////////
//#define WHEEL_RADIUS 0.03418d  //metros
//#define WHEEL_RADIUS 0.034d  //metros
//#define WHEEL_RADIUS 0.0319d
#define WHEEL_RADIUS 0.0327
//#define L 0.22d
#define L 0.18d
#define T 10   //Periodo de muestreo odometria

float robotPose[] = {0, 0, 0} ; //Pose {x, y, theta}

extern float robotLastPose[];

extern float thetaMag;


///////////// Variables ///////////////////
long lastTimeStampMotorControllerTask;

long lastTimeStampOdometryModuleTask;

long lastTimeOdometryDeltaTime;

long lastTimeControllerTask;

long lastTimeSlowingManeuverDelay;

extern states state;

states lastState = state;



void controlSystemTaskInit()
{
  
  initMotorDrivers();
  
  setPowerMotorDrivers(true);

  //Load LUT from EEPROM
  loadLookUpTableFromEEPROM(0);

  
  lastTimeStampMotorControllerTask = millis();

  //lastTimeStampOdometryModuleTask = millis();

  lastTimeControllerTask = millis();
  
}


void controlSystemTaskDo()
{

  switch(state)
  {

    
    case MENU:
    case MENU_WAITING:
      
      //Tarea del control de velocidad de motores
      if(millis() - lastTimeStampMotorControllerTask  >= 5)
      {  
        
        motorControllerTask();    //Debe ejecutarse cada 5ms
    
        lastTimeStampMotorControllerTask = millis();
        
      }

      lastState = state;

    break;


    case RUNNING:

      if(state != lastState)
      {
        lastTimeControllerTask = millis();
        lastTimeStampMotorControllerTask = millis();
        
        lastState = state;
      }
    
      //Tarea del control de velocidad de motores
      if(millis() - lastTimeStampMotorControllerTask  >= 5)
      {  
        
        motorControllerTask();    //Debe ejecutarse cada 5ms
    
        lastTimeStampMotorControllerTask = millis();
        
      }
  
      //Tarea del control de posición del robot
      if(millis() - lastTimeControllerTask >= 25)   //Original 10
      {
        
      //  controlLoopBearingController();

        controlLoopBearingAndSpeedController();
    
        lastTimeControllerTask = millis();
        
      }


      break;
    
  }
  

/*
  //Tarea de muestreo de odometria
  if(millis() - lastTimeStampOdometryModuleTask >= T)
  {  
      odometryComputeState();

      lastTimeStampOdometryModuleTask = millis();
  }
*//*

  //Tarea del control de posición del robot
  if(millis() - lastTimeControllerTask >= 12)   //Original 10
  {
    controlLoop();

    lastTimeControllerTask = millis();

    //Vigilamos si el robot a alcanzado la distancia final
    if(dist_final <= sqrt(pow(robotPose[0], 2) + pow(robotPose[1], 2) ) )
    {

    //  state = WAITING_USER_INPUT;

      endTest();

      //Si es así, computamos los resultados
      computeResults();

      saveLookUpTableInEEPROM(lookUpTableAddress);

  //    lastTimeTestSupervisor = millis();

      Serial.println("Prueba finalizada, distancia alcanzada");
      Serial.println("d = "+String(sqrt(pow(robotPose[0], 2) + pow(robotPose[1], 2) )));
      Serial.println("Pose: x="+String(robotPose[0]) + "  y="+String(robotPose[1])+"  theta="+String(robotPose[2])+"  theta_mag="+String(thetaMag));

     // printOptions();
      
    }
    //Comprobamos si el robot se ha desplazado un incremento suficiente para realizar al captura
    else if( sqrt(pow(robotPose[0], 2) + pow(robotPose[1], 2) ) -  sqrt(pow(robotLastPose[0], 2) + pow(robotLastPose[1], 2) ) >= distanceBetweenSamples)
    {
      
      //Paramos los motores, y cambiamos al estado de captura
      endTest();

      lastTimeSlowingManeuverDelay = millis();

      //state = TEST_RUNING_SAMPLING;

      //Guardamos la pose actual en la pose anterior
      robotLastPose[0] = robotPose[0];
      robotLastPose[1] = robotPose[1];
      robotLastPose[2] = robotPose[2];

    }
  }
*/
  
}





//////////////////////// CONTROL SYSTEMS AND ODOMETRY /////////////////////
/*
void controlLoopBearingController()
{

  //Serial.println("Ejecutar control loop");
   //Con odometria
  //float w = angleController.computeOutput(u_angle_objetive, robotPose[2]);

  //Para evitar discontinuidades en el error
  double e;

  //Si no hay objetivo, el error será 0
  if(inRange == true)
  {
    e = u_angle_objetive - angle_objetive;   //Sensor RB
  }
  else
    e = 0;

  float w = angleController.computeOutput(atan2(sin(e), cos(e) ), 0);


  //Convertimos a valores para el control de motores
  double Wright = (2*v_lineal-w*L)/(2*WHEEL_RADIUS);
    
  double Wleft = (2*v_lineal+w*L)/(2*WHEEL_RADIUS);

  //Fijamos como velocidad de consigna
  setM1angularSpeed(Wright);
  setM2angularSpeed(Wleft);
  setM3angularSpeed(Wright);
  setM4angularSpeed(Wleft); 

  
}
*/

void controlLoopBearingAndSpeedController()
{

  //Para evitar discontinuidades en el error
  double e_angle, e_speed;

  //Si no hay objetivo, el error será 0
  if(inRange == true)
  {
    e_speed = -(u_distance - distance_to_emiter);   //Sensor RB
    e_angle = u_angle_objetive - angle_objetive;   //Sensor RB
  }
  else
  {
    e_speed = 0;
    e_angle = 0;

  }

  v_lineal = speedController.computeOutput(e_speed, 0);

  if(v_lineal > 0.3)
    v_lineal = 0.3;
    
  else if(v_lineal < -0.3)
    v_lineal= -0,3;
  
  float w = angleController.computeOutput(atan2(sin(e_angle), cos(e_angle) ), 0);


  //Convertimos a valores para el control de motores
  double Wright = (2*v_lineal-w*L)/(2*WHEEL_RADIUS);
    
  double Wleft = (2*v_lineal+w*L)/(2*WHEEL_RADIUS);

  //Fijamos como velocidad de consigna
  setM1angularSpeed(Wright);
  setM2angularSpeed(Wleft);
  setM3angularSpeed(Wright);
  setM4angularSpeed(Wleft); 

  
}





///////////////////////////// ODOMETRY MODULE /////////////////////////

void odometryComputeState()
{   

  double deltaT = millis()-lastTimeOdometryDeltaTime;
  
  double RPMM1 = getSignedRPMM1();
  double RPMM2 = getSignedRPMM2();
  double RPMM3 = getSignedRPMM3();
  double RPMM4 = getSignedRPMM4();
/*
  Serial.println("RPMM1: "+String(getSignedRPMM1()));
  Serial.println("RPMM2: "+String(getSignedRPMM2()));
  Serial.println("RPMM3: "+String(getSignedRPMM3()));
  Serial.println("RPMM4: "+String(getSignedRPMM4()));
*/
  lastTimeOdometryDeltaTime = millis();

  double Wright = (RPMM1 + RPMM3)*PI/60;
  
  double Wleft = (RPMM2 + RPMM4)*PI/60;

  //Si suponemos que el muestreo es continuo, podemos aproximar
  //la odometria con segmentos rectos

 // Serial.println("Wright: "+String(Wright)+ "  Wleft: " + String(Wleft));

  double v = WHEEL_RADIUS*(Wright+Wleft)/2;
  
  double w = WHEEL_RADIUS*(Wleft-Wright)/L;


  
 // Serial.println("Time between executions: " +String(millis()-lastTimeTest));
 // lastTimeTest = millis();

  
  robotPose[0] = robotPose[0] + v*cos(robotPose[2])*deltaT/1000;
  robotPose[1] = robotPose[1] + v*sin(robotPose[2])*deltaT/1000;
  robotPose[2] = robotPose[2] + w*deltaT/1000;

  //Reducimos el ángulo a el intervalo de -pi y pi
  robotPose[2] = atan2(sin(robotPose[2]), cos(robotPose[2]));
    
}



void endTest()
{

  //Finalizados el programa
  //Detenemos motores
  setM1angularSpeed(0);
  setM2angularSpeed(0);
  setM3angularSpeed(0);
  setM4angularSpeed(0);

  setPWMM1(0);
  setPWMM2(0);
  setPWMM3(0);
  setPWMM4(0);

//  IMU.setMagnetometerCaptureMode(IDLE_MODE);
        
}



void computeResults()
{

  Serial.println("Resultados computados");
}
