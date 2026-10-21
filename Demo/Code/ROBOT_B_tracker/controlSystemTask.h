//////////////////////////////////
// Esta tarea ejecutará los sistemas de control del robot
//

#include <Arduino.h>

#ifndef __CONTROL_SYSTEM_TASK__H_
#define __CONTROL_SYSTEM_TASK__H_


#define lookUpTableAddress 0

void endTest();

void computeResults();

void controlSystemTaskInit();

void controlSystemTaskDo();

#endif
