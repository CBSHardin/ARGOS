///////////////////////////////////////////////
//
//       Range and Bearing Sensor  - PoC
//      
//////////////////////////////////////////////
// ARGOS - SWARM ROBOTICS PLATFORM
//////////////////////////////////////////////
//             ROBOT A - Emiter 
//////////////////////////////////////////////
// by Carlos Berbell Sánchez

#include "imu.h"

#include "fuelGauge.h"
#include "RB_sensor.h"
#include "motorDrivers.h"
#include "PIDController.h"
#include "motor_controller_task.h"
#include "sensorTask.h"
#include "controlSystemTask.h"
#include "userInterfaceTask.h"

#include "robotStates.h"

#include "waypointFollower.h"

states state = MENU;


/////////////////// CONTROL VARIABLES ///////////////////
extern uint8_t sensorSelector; //0 odometry, 1 magnetometer

extern PIDController angleController;


////////////////////// ODOMETRY MODULE /////////////////
extern float robotPose[] ; //Pose {x, y, theta}


////////// MAGNETOMETER RELATED VARIABLES /////////
extern float northAngleRefOrigin;
extern float thetaMag;


///////////////// TASKS TIME STAMPS VARIABLES ///////////////////
long lastTimeTestSupervisor;


void setup() {

  delay(100);
  Serial.begin(115200);  // start serial for output

  Serial.println("=============== Robot to robot tracking using RB sensor ===============");
Serial.println("Init ok");

  sensorTaskInit();

  controlSystemTaskInit();

  waypointsFollowerInitTask();

  userInterfaceTaskInit();

  
  init_R_B_sensor_tx();
  
  setPowerRB_sensor_tx(true);

  turnOffAllEmiters();
  
  startExcitation();


  //Añadimos un waypoint

}

void loop() {

  sensorTaskDo();

  controlSystemTaskDo();

  waypointsFollowerDoTask();

  userInterfaceTaskDo();

}
