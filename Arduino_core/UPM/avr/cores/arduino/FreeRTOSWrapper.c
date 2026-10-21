/******************************************************************************
                                 FreeRTOSWrapper
*******************************************************************************
En este fichero estan las redefiniciones de las funciones del Core de Arduino
que serán re-implementadas para ser compatibles con FreeRTOS.
*******************************************************************************/
#include <Arduino.h>

#define EXCLUDE_FREE_RTOS_WRAPPER

#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>


void RTOSdelay(unsigned long ms)
{

  const TickType_t xDelay = (TickType_t)ms/portTICK_PERIOD_MS;

  vTaskDelay(xDelay);
}



void RTOSdelayMicroseconds(unsigned int us)
{

  //Evitaremos los cambios de contexto durante la ejecución
  vTaskSuspendAll();

  delayMicroseconds(us);

  xTaskResumeAll();
}
