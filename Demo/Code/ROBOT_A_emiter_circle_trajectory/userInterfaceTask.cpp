/////////////////////////////////////////////
// Tarea que procesa la interfaz de usuario
//

#include <Arduino.h>
#include "RB_sensor.h"
#include "PIDController.h"
#include "motor_controller_task.h"

#include "waypointFollower.h"

#include "sensorTask.h"

#include "robotStates.h"

#include "imu.h"

#define lookUpTableAddress 0


//// Prototipos ///
void printMenu();
void processUserInput();

extern float thetaMag;
extern float lastThetaMag;
extern float lastThetaMag1;

extern float lastThetaMag2;
extern float northAngleRefOrigin;

extern imu IMU;


//////////// USER INTERFACE VARIABLES /////////
char opt=0;
String line="";
char cmdBuff[20];
char cmdBuffIndex = 0;
bool msgAvailable = false;
String readLine(bool waitForInput = true);

////////// Variables //////////
long userInterfaceTaskTimeStamp;

extern PIDController angleController;
extern PIDController speedController;

extern float robotPose[];

extern boolean inRange;

extern states state;



void userInterfaceTaskInit()
{
/*
      addWaypointToPlan({0.0f, 0.25f});
      addWaypointToPlan({0.25f, 0.25f});
      addWaypointToPlan({0.25f, 0.0f});
      addWaypointToPlan({0.0f, 0.0f});
*/
/*
      addWaypointToPlan({0.0f, 1.5f});
      addWaypointToPlan({0.5f, 1.5f});
      addWaypointToPlan({1.0f, 1.0f});
      addWaypointToPlan({1.5f, 1.0f});
      addWaypointToPlan({2.0f, 1.5f});
      addWaypointToPlan({2.5f, 1.5f});
      addWaypointToPlan({2.5f, 0.0f});
      addWaypointToPlan({0.0f, 0.0f});
*/

      addWaypointToPlan({0.0f, 1.5f});
      addWaypointToPlan({1.0f, 1.5f});
      addWaypointToPlan({1.0f, 0.0f});
      addWaypointToPlan({0.0f, 0.0f});
      addWaypointToPlan({0.0f, 0.01f});

/*
      //L
      addWaypointToPlan({0.0f, 1.5f});
      addWaypointToPlan({-1.0f, 1.5f});
      addWaypointToPlan({0.0f, 1.5f});
      addWaypointToPlan({0.0f, 0.0f});
*/

}


void userInterfaceTaskDo()
{

  switch(state)
  {

    //Imprimimos el menu
    case MENU:
      printMenu();

      state = MENU_WAITING;

      break;

    case MENU_WAITING:
      processUserInput();

      break;


    case RUNNING:

      if(millis() - userInterfaceTaskTimeStamp >1000)
      {

        Serial.println("X: "+String(robotPose[0]) + "   Y: "+String(robotPose[1])+ "   theta: "+String(robotPose[2]));

       // Serial.println("theta mag: "+String(thetaMag));

        if(getPlanExecutionStatus() == WPF_FINISHED)
          state = MENU;
/*
        if(inRange)
          Serial.println("Obj detectado");
        else
          Serial.println("Obj no detectado");
          */

        if (Serial.available() > 0)
        {
          opt  = Serial.read();

          if(opt == 'X' or opt == 'x')
          {

            //Paramos los motores, y pasamos al menu
            state = MENU;

            setM1angularSpeed(0);
            setM2angularSpeed(0);
            setM3angularSpeed(0);
            setM4angularSpeed(0);

            turnOffAllEmiters();

            Serial.println("Prueba abortada\n");

          }
          else if(opt > 32)
            Serial.print("# Opcion incorrecta\n$: ");

          opt=0;
        }

        userInterfaceTaskTimeStamp = millis();
      }

      break;

  }

}



////////////////////////// USER INTERFACE FUNCTIONS //////////////////

void processUserInput()
{

    line  = readLine(false);

    //Iniciamos el test
    if(line.charAt(0) == 'S' or line.charAt(0) == 's')
    {
     // state = TEST_RUNING_MOVING;

      angleController.resetState();
      resetStateMotorController();

      Serial.println("Prueba iniciada con coeficientes: \n Controlador de angulo:  Kp = "+String(angleController.getKp(), 6) +"  Ki = "+String(angleController.getKi(), 6)+"  Kd = "+String(angleController.getKd(), 6));
      Serial.println("Upper limit = " +String(angleController.getOutputUpperLimit()) + " lower limit = "+String(angleController.getOuputLowerLimit())+"\n"  );

      Serial.println("Controlador de velocidad:   Kp = "+String(speedController.getKp(), 6) +"  Ki = "+String(speedController.getKi(), 6)+"  Kd = "+String(speedController.getKd(), 6));
      Serial.println("Upper limit = " +String(speedController.getOutputUpperLimit()) + " lower limit = "+String(speedController.getOuputLowerLimit())+"\n"  );

      state = RUNNING;
/*
      addWaypointToPlan({0.5f, 0.0f});
      addWaypointToPlan({0.5f, 0.5f});
      addWaypointToPlan({0.0f, 0.5f});
      addWaypointToPlan({0.0f, 0.0f});
      */
/*
      addWaypointToPlan({0.0f, 0.5f});
      addWaypointToPlan({0.5f, 0.5f});
*/
      

      
/*
      addWaypointToPlan({0.0f, 0.25f});
      addWaypointToPlan({0.05f, 0.25f});
      addWaypointToPlan({0.1f, 0.20f});
      addWaypointToPlan({0.15f, 0.20f});
      addWaypointToPlan({0.20f, 0.25f});
      addWaypointToPlan({0.25f, 0.25f});
      addWaypointToPlan({0.25f, 0.0f});
      addWaypointToPlan({0.0f, 0.0f});

      
*/
  northAngleRefOrigin = getAngleToNorthCompensated()-PI/2;
 // lastThetaMag = getAngleToNorthCompensated();
 lastThetaMag = 0;

  lastThetaMag1 = 0;
  lastThetaMag2 = 0;

  IMU.setMagnetometerDataRate(ODR_M_100);
  IMU.attachMagnetometerDRDYInterruptISR(ISR_MAG);
  IMU.setMagnetometerCaptureMode(CONTINUOUS_MODE);

      robotPose[0] = 0;
      robotPose[1] = 0;
      robotPose[2] = PI/2;

      

  
      startPlanExecution();

      userInterfaceTaskTimeStamp = millis();

    }
    //Cambio de constante Kp
    else if(line.charAt(0) == 'P' or line.charAt(0) == 'p')
    {

      if(line.charAt(2) == 'A' or line.charAt(2) =='a')
        angleController.setKp(line.substring(3).toFloat());

      else if(line.charAt(2) == 'S' or line.charAt(2) =='s')
        speedController.setKp(line.substring(3).toFloat());

      Serial.print("$: ");
    }

    //Cambio de constante Ki
    else if(line.charAt(0) == 'I' or line.charAt(0) == 'i')
    {
      if(line.charAt(2) == 'A' or line.charAt(2) =='a')
          angleController.setKi(line.substring(3).toFloat());

      else if(line.charAt(2) == 'S' or line.charAt(2) =='s')
         speedController.setKi(line.substring(3).toFloat());

      Serial.print("$: ");
    }

    //Cambio de constante Kd
    else if(line.charAt(0) == 'D' or line.charAt(0) == 'd')
    {

      if(line.charAt(2) == 'A' or line.charAt(2) =='a')
          angleController.setKd(line.substring(3).toFloat());

      else if(line.charAt(2) == 'S' or line.charAt(2) =='s')
          speedController.setKd(line.substring(3).toFloat());

      Serial.print("$: ");
    }

    //Cambio de limite superior
    else if(line.charAt(0) == 'U' or line.charAt(0) == 'u')
    {

      if(line.charAt(2) == 'A' or line.charAt(2) =='a')
          angleController.setOuputUpperLimit(line.substring(3).toFloat());

      else if(line.charAt(2) == 'S' or line.charAt(2) =='s')
          speedController.setOuputUpperLimit(line.substring(3).toFloat());

      Serial.print("$: ");
    }

    //Cambio de limite inferior
    else if(line.charAt(0) == 'L' or line.charAt(0) == 'l')
    {

      if(line.charAt(2) == 'A' or line.charAt(2) =='a')
          angleController.setOuputLowerLimit(line.substring(3).toFloat());

      else if(line.charAt(2) == 'S' or line.charAt(2) =='s')
          speedController.setOuputLowerLimit(line.substring(3).toFloat());

      Serial.print("$: ");
    }

    //Calibración magnetómetro
    else if(line.charAt(0) == 'c' or line.charAt(0) == 'C')
    {

      magnetometerCalibration();

    }

    //Muestra los resultados de la prueba
    else if(line.charAt(0) == 'G' or line.charAt(0) == 'g')
    {

      Serial.println("Posición del robot ");

      Serial.println("X: "+String(robotPose[0]) + "   Y: "+String(robotPose[1])+ "   theta: "+String(robotPose[2]));


      Serial.print("\n$: ");
    }

    //Carga el valor de los valores de la eeprom de la LUT
    else if(line.charAt(0) == 'e' or line.charAt(0) == 'E')
    {

      loadLookUpTableFromEEPROM(lookUpTableAddress);

      Serial.println("LUT almacenada en la EEPROM cargada en la RAM");

      Serial.print("\n$: ");

    }


    else if(opt > 32)
      Serial.print("# Opcion incorrecta\n$: ");


}

void printMenu()
{

  Serial.print("Comandos: \n\t\"S\" - inicio \n\t\"X\" - abortar/detener  \n\t\"C\" - calibrar magnetómetro  \n\t\"G\" - mostrar resultados \n\tA: controlador de angulo,  S: controlador de velocidad     \n\tP [A{Kp gain value}] [S{Kp gainValue}] - modificar ganancia proporcional \n\tI [A{Ki gain value}] [S{Ki gain value}]  - modificar ganancia integral \n\tD [A{Ki gain value}] [S{Ki gain value}] - modificar ganancia derivativa \n\tU [A{Upper limit value}] [S{Upper limit value}] - modificar el valor máximo de salida \n\tL [A{Lower limit value}] [S{Lower limit value}] - modificar el valor mínimo de salida \n$: ");

}

String readLine(bool waitForInput = true)
{

  if(waitForInput == true)
    Serial.print(">");

  do
  {

    if(Serial.available() > 0)
    {

        cmdBuff[cmdBuffIndex] = Serial.read();

       // Serial.print(cmdBuff[cmdBuffIndex]);

        if(cmdBuff[cmdBuffIndex]=='\n' || cmdBuff[cmdBuffIndex]=='\r' )
        {
          msgAvailable = true;

          cmdBuff[cmdBuffIndex] = 0;

         // Serial.println();
        }
        else
          cmdBuffIndex++;

    }

  }
  while(waitForInput && !msgAvailable);

  if(msgAvailable == true)
  {
      String msg = String(cmdBuff);

      cmdBuffIndex = 0;

      msgAvailable = false;

      return msg;

  }
  else
  {

    String msg ="";

    return msg;

  }


}
