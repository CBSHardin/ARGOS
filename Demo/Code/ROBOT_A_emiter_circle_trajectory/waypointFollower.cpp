////////////////////////////////////////
//
// Waypoint follower
//
///////////////////////////////////////
// Esta libreria implementa las funciones de navegación
// por waypoints. Una vez se de la orden de inicio del plan
// el robot seguira los waypoints y se detendrá cuando el plan se haya completado.

#include <Arduino.h>
#include "waypointFollower.h"
#include "motor_controller_task.h"
#include "PIDController.h"
#include "RB_sensor.h"

waypointsFollowerStates waypointsFollowerStatus = WPF_STOPPED;

typedef enum {WPFC_STOPPED, WPFC_AIMING, WPFC_TRAVELING} waypointsPoseControllerStates;

waypointsPoseControllerStates controllerStates = WPFC_STOPPED;


extern float robotPose[] ; //Pose {x, y, theta}

/////////////// CONTROL
#define sequenceDurationMs 40000

long startSequenceTimeStamp = 0;

float v_lineal = 0.20;

#define R 0.25;



float u_angle_objetive;
float u_distance = 0;

extern float real_w;

posVector2D plannedWaypoints[MAX_NUMBER_WAYPOINTS];

posVector2D travelingOrigin = {0, 0};

uint8_t storageIndex = 0;

uint8_t targetWaypointIndex = 0;

float e_angle, e_speed, w;

////////////////////// ODOMETRY MODULE /////////////////
//#define WHEEL_RADIUS 0.03418d  //metros
//#define WHEEL_RADIUS 0.034d  //metros
//#define WHEEL_RADIUS 0.0319d
#define WHEEL_RADIUS 0.0327
//#define L 0.22d
//#define L 0.27d
#define L 0.18d

#define T 10   //Periodo de muestreo odometria



/////////////////////// CONTROLADORES ////////////////////
/*
PIDController angleController(1, 0, 0, -20, 20); //Kp=12.5, PID inestable, lo bajamos a la mitad, Ki=3, PID inestable, lo bajamos a la mitad
PIDController speedController(0.3, 0, 0, -5, 5); //Kp=12.5, PID inestable, lo bajamos a la mitad, Ki=3, PID inestable, lo bajamos a la mitad

PIDController angleControllerTraveling(1, 0, 0, -20, 20); //Kp=12.5, PID inestable, lo bajamos a la mitad, Ki=3, PID inestable, lo bajamos a la mitad

*/

PIDController angleController(1.5, 0, 0, -20, 20); //Kp=12.5, PID inestable, lo bajamos a la mitad, Ki=3, PID inestable, lo bajamos a la mitad
PIDController speedController(0.4, 0, 0, -5, 5); //Kp=12.5, PID inestable, lo bajamos a la mitad, Ki=3, PID inestable, lo bajamos a la mitad

PIDController angleControllerTraveling(2.0, 0, 0, -20, 20); //Kp=12.5, PID inestable, lo bajamos a la mitad, Ki=3, PID inestable, lo bajamos a la mitad


long lastTimeStampSupervisorTask = 0;
long lastTimeStampControllerTask  = 0;


//////////////////// PROTOTIPOS //////////////////
void aimingStateControlLoop();
void travelingStateControlLoop();
void controlLoopSubtask();
void goalSupervisorSubtask();


//devuelve false si hubo algún error
boolean addWaypointToPlan(posVector2D waypoint)
{
  if(storageIndex < MAX_NUMBER_WAYPOINTS)
  {

    plannedWaypoints[storageIndex].x = waypoint.x;
    plannedWaypoints[storageIndex].y = waypoint.y;

    storageIndex++;

    return true;

  }
  else
    return false;

}


void startPlanExecution()
{

  waypointsFollowerStatus = WPF_RUNNING;

  controllerStates = WPFC_TRAVELING;

  w = v_lineal/R;

  double Wright = (2*v_lineal-w*L)/(2*WHEEL_RADIUS);

  double Wleft = (2*v_lineal+w*L)/(2*WHEEL_RADIUS);

  //Fijamos como velocidad de consigna
  setM1angularSpeed(Wright);
  setM2angularSpeed(Wleft);
  setM3angularSpeed(Wright);
  setM4angularSpeed(Wleft);

  turnOnAllEmiters();
  
  startSequenceTimeStamp = millis();
  
}

void pausePlanExecution()
{


}

void resumePlanExecution();

void deletePlan();

void restartPlanExecution();

waypointsFollowerStates getPlanExecutionStatus()
{

  return waypointsFollowerStatus;

}


void waypointsFollowerInitTask()
{

}

void waypointsFollowerDoTask()
{

  if(millis() - lastTimeStampSupervisorTask > stateSupervisorPeriodMillis)
  {

    goalSupervisorSubtask();

    lastTimeStampSupervisorTask = millis();
  }


}


void controlLoopSubtask()
{

  switch(controllerStates)
  {

    case WPFC_STOPPED:
    /*
      setM1angularSpeed(0);
      setM2angularSpeed(0);
      setM3angularSpeed(0);
      setM4angularSpeed(0);
      */

      //Reseteamos los controladores


      break;


    case WPFC_AIMING:
      aimingStateControlLoop();
      //Serial.println("Aiming control loop ite");
      break;

    case WPFC_TRAVELING:
      travelingStateControlLoop();
    //Serial.println("Traveling control loop ite");
      break;
      

  }
}


void goalSupervisorSubtask()
{

   switch(controllerStates)
   {
      case WPFC_STOPPED:
         break;

      case WPFC_TRAVELING:
        if(millis()-startSequenceTimeStamp >=sequenceDurationMs)
        {
  
            controllerStates = WPFC_STOPPED;
            waypointsFollowerStatus = WPF_FINISHED;

            setM1angularSpeed(0);
            setM2angularSpeed(0);
            setM3angularSpeed(0);
            setM4angularSpeed(0);

            turnOffAllEmiters();
          
        }
        break;

   }

}



void aimingStateControlLoop()
{
  //Serial.println("Ejecutar control loop");
   //Con odometria
  //float w = angleController.computeOutput(u_angle_objetive, robotPose[2]);

  //Para evitar discontinuidades en el error

  //u_angle_objetive = atan2( (plannedWaypoints[targetWaypointIndex].y - robotPose[1] ), (plannedWaypoints[targetWaypointIndex].x - robotPose[0] ) );

  //Si no hay objetivo, el error será 0
  e_angle = (u_angle_objetive - robotPose[2]);

  e_angle = atan2(sin(e_angle), cos(e_angle) );

  w = angleController.computeOutput(e_angle , 0);

  //  Serial.println("plannedWaypoints[targetWaypointIndex].x: "+String(plannedWaypoints[targetWaypointIndex].x) + "  plannedWaypoints[targetWaypointIndex].y: "+String(plannedWaypoints[targetWaypointIndex].y) + "   X: "+String(robotPose[0]) + "   Y: "+String(robotPose[1])+"   theta: "+String(robotPose[2]) +"  e_angle: "+String(e_angle) );


  //Convertimos a valores para el control de motores
  float Wright = (2*v_lineal-w*L)/(2*WHEEL_RADIUS);

  float Wleft = (2*v_lineal+w*L)/(2*WHEEL_RADIUS);

//  Serial.println("Wright: "+String(Wright)+ "  Wleft: "+String(Wleft)+ "   X: "+String(robotPose[0]) + "   Y: "+String(robotPose[1]) + " theta: "+String(robotPose[2]));


  //Fijamos como velocidad de consigna
  setM1angularSpeed(Wright);
  setM2angularSpeed(Wleft);
  setM3angularSpeed(Wright);
  setM4angularSpeed(Wleft);



}


void travelingStateControlLoop()
{


  //u_angle_objetive = atan2( (plannedWaypoints[targetWaypointIndex].y - robotPose[1] ), (plannedWaypoints[targetWaypointIndex].x - robotPose[0] ) );

  //u_angle_objetive = atan2(sin(u_angle_objetive), cos(u_angle_objetive));


  //Si no hay objetivo, el error será 0

  e_speed = ( (u_distance - sqrt(pow(robotPose[0] - travelingOrigin.x, 2) + pow(robotPose[1]-travelingOrigin.y, 2 )) ) )/u_distance;   //Sensor RB

  //if(plannedWaypoints[targetWaypointIndex].x - robotPose[0])

//  Serial.println("e_speed: "+String(e_speed));
  e_angle = 0 +(u_angle_objetive - robotPose[2]);   //Sensor RB

  e_angle = atan2(sin(e_angle), cos(e_angle) );

 // e_angle = 0;

  v_lineal = speedController.computeOutput(e_speed, 0);

  w = angleControllerTraveling.computeOutput(e_angle, 0);

  #ifdef debug
  
    Serial.println("u_angle_objetive: "+String(u_angle_objetive) + "   robotPose[2]:"+String(robotPose[2])+"  e_angle: "+String(atan2(sin(e_angle), cos(e_angle) )));

    Serial.println("plannedWaypoints[targetWaypointIndex].x: "+String(plannedWaypoints[targetWaypointIndex].x) + "  plannedWaypoints[targetWaypointIndex].y: "+String(plannedWaypoints[targetWaypointIndex].y) + "   X: "+String(robotPose[0]) + "   Y: "+String(robotPose[1])+"  e_speed: "+String(e_speed) );

    Serial.println("w: "+String(w));

  #endif

  

  //Convertimos a valores para el control de motores
  double Wright = (2*v_lineal-w*L)/(2*WHEEL_RADIUS);

  double Wleft = (2*v_lineal+w*L)/(2*WHEEL_RADIUS);

  //Fijamos como velocidad de consigna
  setM1angularSpeed(Wright);
  setM2angularSpeed(Wleft);
  setM3angularSpeed(Wright);
  setM4angularSpeed(Wleft);


}
