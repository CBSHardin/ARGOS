

#ifndef _IMPL_PHAL_I2C_OPERATIONS_H__
#define _IMPL_PHAL_I2C_OPERATIONS_H__


#include <stdint.h>
#include "../../../../PDL/i2c/Wire.h"

#include <Arduino.h>


inline uint8_t i2c_read(uint8_t address, uint8_t* outBuffer, uint8_t numberOfBytesToRead)
{

  uint8_t ret;

  // Critical section
  vTaskSuspendAll();

    ret = Wire.requestFrom(address, numberOfBytesToRead, (uint8_t)1);

  xTaskResumeAll();


  for(uint8_t i = 0 ; i < ret ; i++)
    outBuffer[i] = Wire.read();

  return ret;

}



inline uint8_t i2c_write(uint8_t address, uint8_t* inBuffer, uint8_t numberOfBytesToWrite)
{

  uint8_t ret;


  Wire.beginTransmission(address);
  Wire.write(inBuffer, numberOfBytesToWrite);


  // Critical section
  vTaskSuspendAll();

    ret = Wire.endTransmission(1);

  xTaskResumeAll();

  return ret;

}


inline uint8_t i2c_transaction(uint8_t address, uint8_t *inBuffer, uint8_t numberOfBytesToWrite, uint8_t* outBuffer, uint8_t numberOfBytesToRead, uint8_t *errCode)
{

  uint8_t recvBytes;

  Wire.beginTransmission(address);
  Wire.write(inBuffer, numberOfBytesToWrite);

  // Critical section
  vTaskSuspendAll();

    *errCode = Wire.endTransmission(0);    //Not release the bus

    recvBytes = Wire.requestFrom(address, numberOfBytesToRead, (uint8_t)1);

  xTaskResumeAll();

  //Read all the bytes
  for(uint8_t i = 0 ; i < recvBytes ; i++)
    outBuffer[i] = Wire.read();


  return recvBytes;

}




#endif
