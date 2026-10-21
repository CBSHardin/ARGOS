

#include "../implPHAL.h"


#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include "../../../../PDL/i2c/Wire.h"

#include <Arduino.h>


implPHAL_i2c_Wire_PDL::implPHAL_i2c_Wire_PDL(uint8_t taskPriority)
{

  mTaskPriority = taskPriority;

  mStatus = false;   //Gatekeeper has not been init

}



void implPHAL_i2c_Wire_PDL::begin()
{

  if(mStatus == false)
  {

    mStatus = true;

    //Inite i2c driver
    Wire.begin();

    Wire.setClock(400000);

    //Queue for the commands
    mCmdQueueHandler = xQueueCreateStatic(I2C_WIRE_PDL_CMD_QUEUE_SLOTS, sizeof(I2cGkCmd_t), mCmdQueueBuffer, &mCmdQueueDataStruct);

    mTaskHandler = xTaskCreateStatic(&taskLoopExecutor, "I2C_GK", I2C_WIRE_PDL_TASK_STACK_SIZE, (void*) this, mTaskPriority, mTaskStackBuffer, &mTCBbuffer);


  }

}



uint8_t implPHAL_i2c_Wire_PDL::read(uint8_t address, uint8_t* outBuffer, uint8_t numberOfBytesToBeReaded, uint8_t bufferSize, uint8_t op_priority )
{

  uint32_t receiveNotification = 0;

  uint8_t ret;

  //Esta funcion es sincrona, así que, insertara la peticion en la cola de comandos
  I2cGkCmd_t cmd =
    {
      Cmds::READ_SYNC,
      address
    };

  cmd.readSyncData.outputBufferPtr = outBuffer;
  cmd.readSyncData.toNotifyTaskHandle = xTaskGetCurrentTaskHandle();

  if(numberOfBytesToBeReaded > bufferSize)
    cmd.readSyncData.numberOfBytesToRead = bufferSize;
  else
    cmd.readSyncData.numberOfBytesToRead = numberOfBytesToBeReaded;


  cmd.readSyncData.retValuePtr = &ret;


  if(xQueueSend(mCmdQueueHandler, &cmd, 0) == errQUEUE_FULL)
    return 0;


  //If the priority of the operation is high, we elevate the priority of the
  //i2c task
  if(mTaskPriority < op_priority)
    vTaskPrioritySet(mTaskHandler, op_priority);


  //Send a notification to the i2c gatekeeper task to inform that a new
  //element has been posted on the queue.
  xTaskNotify(mTaskHandler, Notifications::NEW_CMD_AT_QUEUE, eSetBits);

  //Now, we pause the taks execution until we receive the op_completed notification
  while((receiveNotification & Notifications::SYNC_OP_COMPLETED) == 0)

    xTaskNotifyWaitIndexed(0, 0, Notifications::SYNC_OP_COMPLETED, &receiveNotification, portMAX_DELAY);


  return ret;

}



bool implPHAL_i2c_Wire_PDL::readAsync(uint8_t address, uint8_t numberOfBytesToBeReaded, uint8_t op_priority, delegate<void, uint8_t*, uint8_t> onReadCompleteCallback)
{

  //Esta funcion es sincrona, así que, insertara la peticion en la cola de comandos
  I2cGkCmd_t cmd =
    {
      Cmds::READ_ASYNC,
      address
    };

  cmd.readAsyncData.numberOfBytesToRead = numberOfBytesToBeReaded;

  cmd.readAsyncData.callbackDelegate = onReadCompleteCallback;



  if(xQueueSend(mCmdQueueHandler, &cmd, 0) == errQUEUE_FULL)
    return false;


  //If the priority of the operation is high, we elevate the priority of the
  //i2c task
  if(mTaskPriority < op_priority)
    vTaskPrioritySet(mTaskHandler, op_priority);


  //Send a notification to the i2c gatekeeper task to inform that a new
  //element has been posted on the queue.
  xTaskNotify(mTaskHandler, Notifications::NEW_CMD_AT_QUEUE, eSetBits);


  return true;

}



uint8_t implPHAL_i2c_Wire_PDL::write(uint8_t address, uint8_t* data, uint8_t numberOfBytesToWrite, uint8_t op_priority)
{

  uint32_t receiveNotification = 0;

  uint8_t ret;

  //Esta funcion es sincrona, así que, insertara la peticion en la cola de comandos
  I2cGkCmd_t cmd =
    {
      Cmds::WRITE_SYNC,
      address
    };

  cmd.writeSyncData.toNotifyTaskHandle = xTaskGetCurrentTaskHandle();

  cmd.writeSyncData.inputBufferPtr = data;

  cmd.writeSyncData.numberOfBytesToWrite = numberOfBytesToWrite;

  cmd.writeSyncData.retValuePtr = &ret;


  if(xQueueSend(mCmdQueueHandler, &cmd, 0) == errQUEUE_FULL)
    return 4; //other error


  //If the priority of the operation is high, we elevate the priority of the
  //i2c task
  if(mTaskPriority < op_priority)
    vTaskPrioritySet(mTaskHandler, op_priority);


  //Send a notification to the i2c gatekeeper task to inform that a new
  //element has been posted on the queue.
  xTaskNotify(mTaskHandler, Notifications::NEW_CMD_AT_QUEUE, eSetBits);


  //Now, we pause the taks execution until we receive the op_completed notification
  //This is a sync function, so the task will be suspended until the gatekeeper
  //process the cmd. The gatekeeper will be who resume the task

  while((receiveNotification & Notifications::SYNC_OP_COMPLETED) == 0)

    xTaskNotifyWaitIndexed(0, 0, Notifications::SYNC_OP_COMPLETED, &receiveNotification, portMAX_DELAY);


  return ret;


}



bool implPHAL_i2c_Wire_PDL::writeAsync(uint8_t address, uint8_t* data, uint8_t numberOfBytesToWrite, uint8_t op_priority, delegate<void, uint8_t> onTransmissionCompleteCallback)
{


  //Esta funcion es sincrona, así que, insertara la peticion en la cola de comandos
  I2cGkCmd_t cmd =
    {
      Cmds::WRITE_ASYNC,
      address
    };

  cmd.writeAsyncData.numberOfBytesToWrite = numberOfBytesToWrite;

  cmd.writeAsyncData.inputBufferPtr = data;

  cmd.writeAsyncData.callbackDelegate = onTransmissionCompleteCallback;



  if(xQueueSend(mCmdQueueHandler, &cmd, 0) == errQUEUE_FULL)
    return false;


  //If the priority of the operation is high, we elevate the priority of the
  //i2c task
  if(mTaskPriority < op_priority)
    vTaskPrioritySet(mTaskHandler, op_priority);


  //Send a notification to the i2c gatekeeper task to inform that a new
  //element has been posted on the queue.
  xTaskNotify(mTaskHandler, Notifications::NEW_CMD_AT_QUEUE, eSetBits);


  return true;

}



uint8_t implPHAL_i2c_Wire_PDL::transaction(uint8_t address, uint8_t* dataToWrite, uint8_t numberOfBytesToWrite, uint8_t* outBuffer, uint8_t numberOfBytesToBeReaded, uint8_t bufferSize, uint8_t op_priority, uint8_t *errorCodePtr)
{

  uint32_t receiveNotification = 0;

  uint8_t retValues[2];

  //Esta funcion es sincrona, así que, insertara la peticion en la cola de comandos
  I2cGkCmd_t cmd =
    {
      Cmds::TRANSACTION_SYNC,
      address
    };

  cmd.transactionSyncData.outputBufferPtr = outBuffer;
  cmd.transactionSyncData.toNotifyTaskHandle = xTaskGetCurrentTaskHandle();

  if(numberOfBytesToBeReaded > bufferSize)
    cmd.transactionSyncData.numberOfBytesToRead = bufferSize;
  else
    cmd.transactionSyncData.numberOfBytesToRead = numberOfBytesToBeReaded;

  cmd.transactionSyncData.inputBufferPtr = dataToWrite;

  cmd.transactionSyncData.numberOfBytesToWrite = numberOfBytesToWrite;

  cmd.transactionSyncData.retValuePtr = retValues;


  if(xQueueSend(mCmdQueueHandler, &cmd, 0) == errQUEUE_FULL)
  {
    *errorCodePtr = 4;
    return 0;
  }


  //If the priority of the operation is high, we elevate the priority of the
  //i2c task
  if(mTaskPriority < op_priority)
    vTaskPrioritySet(mTaskHandler, op_priority);


  //Send a notification to the i2c gatekeeper task to inform that a new
  //element has been posted on the queue.
  xTaskNotify(mTaskHandler, Notifications::NEW_CMD_AT_QUEUE, eSetBits);


  //Now, we pause the taks execution until we receive the op_completed notification
  //This is a sync function, so the task will be suspended until the gatekeeper
  //process the cmd. The gatekeeper will be who resume the task

  while((receiveNotification & Notifications::SYNC_OP_COMPLETED) == 0)

    xTaskNotifyWaitIndexed(0, 0, Notifications::SYNC_OP_COMPLETED, &receiveNotification, portMAX_DELAY);


  if(errorCodePtr != 0)
    *errorCodePtr = retValues[1];

  return retValues[0];

}



bool implPHAL_i2c_Wire_PDL::transactionAsync(uint8_t address, uint8_t* dataToWrite, uint8_t numberOfBytesToWrite,  uint8_t numberOfBytesToBeReaded, uint8_t op_priority, delegate<void, uint8_t*, uint8_t, uint8_t> onTransactionCompleteCallback)
{

  //Esta funcion es sincrona, así que, insertara la peticion en la cola de comandos
  I2cGkCmd_t cmd =
    {
      Cmds::TRANSACTION_ASYNC,
      address
    };

  cmd.transactionAsyncData.numberOfBytesToWrite = numberOfBytesToWrite;

  cmd.transactionAsyncData.inputBufferPtr = dataToWrite;

  cmd.transactionAsyncData.numberOfBytesToRead = numberOfBytesToBeReaded;

  cmd.transactionAsyncData.callbackDelegate = onTransactionCompleteCallback;



  if(xQueueSend(mCmdQueueHandler, &cmd, 0) == errQUEUE_FULL)
    return false;


  //If the priority of the operation is high, we elevate the priority of the
  //i2c task
  if(mTaskPriority < op_priority)
    vTaskPrioritySet(mTaskHandler, op_priority);


  //Send a notification to the i2c gatekeeper task to inform that a new
  //element has been posted on the queue.
  xTaskNotify(mTaskHandler, Notifications::NEW_CMD_AT_QUEUE, eSetBits);


  return true;


}



void implPHAL_i2c_Wire_PDL::end()
{

  if(mStatus == true)
  {

    mStatus = false;

    //Inite i2c driver
    Wire.end();

    //Delete queue and task
    vQueueDelete(mCmdQueueHandler);

    vTaskDelete(mTaskHandler);

  }


}
