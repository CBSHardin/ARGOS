/******************************************************************************
                                 FreeRTOSWrapper
*******************************************************************************
En este fichero estan las redefiniciones de las funciones del Core de Arduino
que serán re-implementadas para ser compatibles con FreeRTOS.
*******************************************************************************/


#ifndef __FREERTOS_WRAPPER__
#define __FREERTOS_WRAPPER__

#ifdef __cplusplus
extern
"C"{
#endif

#define   delay               RTOSdelay
#define   delayMicroseconds   RTOSdelayMicroseconds

void RTOSdelay(unsigned long ms);
void RTOSdelayMicroseconds(unsigned int us);

#ifdef __cplusplus
}
#endif

#endif
