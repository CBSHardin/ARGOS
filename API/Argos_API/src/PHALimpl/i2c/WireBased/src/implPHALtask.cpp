
#include "../implPHAL.h"
#include "implPHALi2cOperations.h"

#include <Arduino.h>


void static implPHAL_i2c_Wire_PDL::taskLoopExecutor(implPHAL_i2c_Wire_PDL *instancePtr)
{

  instancePtr->taskLoop();

}



void implPHAL_i2c_Wire_PDL::taskLoop()
{

  I2cGkCmd_t recvCmd = {};

  uint8_t retAsync;

  uint8_t errAsync;

  uint8_t asyncReadBuffer[32];


  while(1)
  {

    //First, we get the number of messages available within the queue
    if(uxQueueMessagesWaiting(mCmdQueueHandler) != 0)
    {

      //We extract the message, and process it:
      xQueueReceive(mCmdQueueHandler, &recvCmd, 0);

      switch(recvCmd.cmd)
      {

        case Cmds::READ_SYNC:

          *(recvCmd.readSyncData.retValuePtr) = i2c_read(recvCmd.address, recvCmd.readSyncData.outputBufferPtr, recvCmd.readSyncData.numberOfBytesToRead);

          //Resume the task
          xTaskNotifyIndexed(recvCmd.readSyncData.toNotifyTaskHandle, 0, Notifications::SYNC_OP_COMPLETED, eSetBits);

          vTaskPrioritySet(mTaskHandler, mTaskPriority);

          break;



        case Cmds::READ_ASYNC:

          retAsync = i2c_read(recvCmd.address, asyncReadBuffer, recvCmd.readAsyncData.numberOfBytesToRead);

          //Send data to the callback
          recvCmd.readAsyncData.callbackDelegate(asyncReadBuffer, retAsync);

          vTaskPrioritySet(mTaskHandler, mTaskPriority);

          break;



        case Cmds::WRITE_SYNC:

          *(recvCmd.writeSyncData.retValuePtr) = i2c_write(recvCmd.address, recvCmd.writeSyncData.inputBufferPtr, recvCmd.writeSyncData.numberOfBytesToWrite);

          //Resume the task
          xTaskNotifyIndexed(recvCmd.writeSyncData.toNotifyTaskHandle, 0, Notifications::SYNC_OP_COMPLETED, eSetBits);

          vTaskPrioritySet(mTaskHandler, mTaskPriority);

          break;



        case Cmds::WRITE_ASYNC:

          retAsync = i2c_write(recvCmd.address, recvCmd.writeAsyncData.inputBufferPtr, recvCmd.writeAsyncData.numberOfBytesToWrite);

          //Send data to the callback
          recvCmd.writeAsyncData.callbackDelegate(retAsync);

          vTaskPrioritySet(mTaskHandler, mTaskPriority);

          break;



        case Cmds::TRANSACTION_SYNC:

          recvCmd.transactionSyncData.retValuePtr[0] = i2c_transaction(recvCmd.address, recvCmd.transactionSyncData.inputBufferPtr, recvCmd.transactionSyncData.numberOfBytesToWrite, recvCmd.transactionSyncData.outputBufferPtr, recvCmd.transactionSyncData.numberOfBytesToRead, &recvCmd.transactionSyncData.retValuePtr[1]);

          //Resume the task
          xTaskNotifyIndexed(recvCmd.transactionSyncData.toNotifyTaskHandle, 0, Notifications::SYNC_OP_COMPLETED, eSetBits);

          vTaskPrioritySet(mTaskHandler, mTaskPriority);

          break;



        case Cmds::TRANSACTION_ASYNC:


          retAsync = i2c_transaction(recvCmd.address, recvCmd.transactionAsyncData.inputBufferPtr, recvCmd.transactionAsyncData.numberOfBytesToWrite, asyncReadBuffer, recvCmd.transactionAsyncData.numberOfBytesToRead, &errAsync);

          //Send data to the callback
          recvCmd.transactionAsyncData.callbackDelegate(asyncReadBuffer, retAsync, errAsync);

          vTaskPrioritySet(mTaskHandler, mTaskPriority);


          break;

      };




    }
    //If there is no more messages, we set the task in suspend mode, waiting for
    //notification
    else
    {

      uint32_t receiveNotification = 0;

      while((receiveNotification & Notifications::NEW_CMD_AT_QUEUE) == 0)
        xTaskNotifyWait(0, Notifications::NEW_CMD_AT_QUEUE, &receiveNotification, portMAX_DELAY);

    }

  }


}
