/*
 * FreeRTOS V202112.00
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */

/*
 * Creates all the demo application tasks, then starts the scheduler.  The WEB
 * documentation provides more details of the demo application tasks.
 *
 * Main. c also creates a task called "Check".  This only executes every three
 * seconds but has the highest priority so is guaranteed to get processor time.
 * Its main function is to check that all the other tasks are still operational.
 * Each task that does not flash an LED maintains a unique count that is
 * incremented each time the task successfully completes its function.  Should
 * any error occur within such a task the count is permanently halted.  The
 * check task inspects the count of each task to ensure it has changed since
 * the last time the check task executed.  If all the count variables have
 * changed all the tasks are still executing error free, and the check task
 * toggles an LED.  Should any task contain an error at any time the LED toggle
 * will stop.
 *
 * The LED flash and communications test tasks do not maintain a count.
 */

/*
Changes from V1.2.0

	+ Changed the baud rate for the serial test from 19200 to 57600.

Changes from V1.2.3

	+ The integer and comtest tasks are now used when the cooperative scheduler
	  is being used.  Previously they were only used with the preemptive
	  scheduler.

Changes from V1.2.5

	+ Set the baud rate to 38400.  This has a smaller error percentage with an
	  8MHz clock (according to the manual).

Changes from V2.0.0

	+ Delay periods are now specified using variables and constants of
	  TickType_t rather than unsigned long.

Changes from V2.6.1

	+ The IAR and WinAVR AVR ports are now maintained separately.

Changes from V4.0.5

	+ Modified to demonstrate the use of co-routines.

*/

#include <stdlib.h>
#include <string.h>

#define GCC_MEGA_AVR
#ifdef GCC_MEGA_AVR
	/* EEPROM routines used only with the WinAVR compiler. */
	#include <avr/eeprom.h>
#endif


#include "FreeRTOSWrapper.h"

/* Scheduler include files. */
#include "FreeRTOS.h"
#include "task.h"


//#include "partest.h"
#include "wiring_private.h"
#include "Arduino.h"

#define mainCHECK_TASK_PRIORITY			( tskIDLE_PRIORITY + 3 )

//Comando para el enlazador. Guarda la función en la sección .lowtext,
//la cual esta situada antes de la sección .text, lo que intenta garantizar
//que las tareas se almacenen en los primeros 128KiB de memoria (64Ki words).
//Este atributo debe aplicarse a todas las tareas de FreeRtos, ya que
//el compilador avr-gcc solo usa 16 bits para las direcciones de memoria, por
//tanto, solo puede direccionar 64Ki words (128 KBytes). Utilizando la opción
//-mrelax, se generan trampolines que permiten direccionar todo el espacio
//de memoria, pero si intentamos leer la posición de una función, por las
//limitaciones del compilador, nos devolvera una dirección truncada a 2 bytes.
//Si la función reside en una dirección más hallá de 0x010000, se producirá
//un error, porque la dirección de salto apuntará a una dirección diferente
//a la dirección de la función.
//Situando las funciones que pueden ser llamadas por el RTOS en los primeros 64KiW
//garantizamos el el tercer byte del PC siempre sea 0, por lo que nunca se producirá
//el error.
#define PUT_IN_THE_FIRST_128K   __attribute__ ((section (".lowtext")))

//////////////////////////////////// Tasks ////////////////////////////////////
void ArduinoSketch(void *param) PUT_IN_THE_FIRST_128K;

extern
"C"{
// Prototipos:
void vApplicationIdleHook( void ) PUT_IN_THE_FIRST_128K;

void vApplicationGetIdleTaskMemory( StaticTask_t *(*ppxIdleTaskTCBBuffer), StackType_t *(*ppxIdleTaskStackBuffer), uint32_t *pulIdleTaskStackSize );

void vApplicationGetTimerTaskMemory( StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize );
}



int main( void )
{

	//Stack and TCB for the ArduinoSketch task
	static StaticTask_t xArduinoSketchTaskTCB;
	static StackType_t uxArduinoSketchTaskStack[ configMINIMAL_STACK_SIZE ];

	/*Arduino init Timers*/
	init();

	//Arduino's sketch task:
	xTaskCreateStatic( ArduinoSketch, "sket", configMINIMAL_STACK_SIZE, NULL, mainCHECK_TASK_PRIORITY, uxArduinoSketchTaskStack, &xArduinoSketchTaskTCB );

	vTaskStartScheduler();

	//It won't reach here
	return 0;

}


//////////////////////////// Arduino's sketch task /////////////////////////////
void ArduinoSketch(void* param)
{

	//Setup
	setup();

	for(;;)
	{

		//Loop
		loop();

		if (serialEventRun) serialEventRun();

	}

}


////////////////////////////////// Idle task //////////////////////////////////
void vApplicationIdleHook( void )
{
}


// Memory configuration function for static allocation of the idle task
void vApplicationGetIdleTaskMemory( StaticTask_t *(*ppxIdleTaskTCBBuffer), StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{

	static StaticTask_t xIdleTaskTCB;
	static StackType_t uxIdleTaskStack[ configMINIMAL_STACK_SIZE ];

  *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;

  *ppxIdleTaskStackBuffer =  uxIdleTaskStack;

  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;

}

// Memory configuration function for static allocation of the timer task
void vApplicationGetTimerTaskMemory( StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize )
{

  static StaticTask_t xTimerTaskTCB;
  static StackType_t uxTimerTaskStack[ configTIMER_TASK_STACK_DEPTH ];

  *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;

  *ppxTimerTaskStackBuffer = uxTimerTaskStack;

  *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;

}
