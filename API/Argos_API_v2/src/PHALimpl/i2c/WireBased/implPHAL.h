

#ifndef _IMPL_PHAL_i2c_Wire_PDL_H__
#define _IMPL_PHAL_i2c_Wire_PDL_H__

#include "../../../utils/delegate/delegate.h"

#include "../../../PHAL/i2c/interfaceI2cHAL.h"

#include <stdint.h>

#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>



#define I2C_WIRE_PDL_TASK_STACK_SIZE 160
#define I2C_WIRE_PDL_CMD_QUEUE_SLOTS 10


class implPHAL_i2c_Wire_PDL : public interfaceI2cHAL
{

  public:

    implPHAL_i2c_Wire_PDL(uint8_t taskPriority);

    void begin();

    uint8_t read(uint8_t address, uint8_t* outBuffer, uint8_t numberOfBytesToBeReaded, uint8_t bufferSize, uint8_t op_priority );

    bool readAsync(uint8_t address, uint8_t numberOfBytesToBeReaded, uint8_t op_priority, delegate<void, uint8_t*, uint8_t> onReadCompleteCallback);


    uint8_t write(uint8_t address, uint8_t* data, uint8_t numberOfBytesToWrite, uint8_t op_priority);

    bool writeAsync(uint8_t address, uint8_t* data, uint8_t numberOfBytesToWrite, uint8_t op_priority, delegate<void, uint8_t> onTransmissionCompleteCallback);


    uint8_t transaction(uint8_t address, uint8_t* dataToWrite, uint8_t numberOfBytesToWrite, uint8_t* outBuffer, uint8_t numberOfBytesToRead, uint8_t bufferSize, uint8_t op_priority, uint8_t *errorCodePtr);

    bool transactionAsync(uint8_t address, uint8_t* dataToWrite, uint8_t numberOfBytesToWrite,  uint8_t numberOfBytesToBeReaded, uint8_t op_priority, delegate<void, uint8_t*, uint8_t, uint8_t> onTransactionCompleteCallback);

    void end();


  private:

    void static taskLoopExecutor(implPHAL_i2c_Wire_PDL *instancePtr);

    void taskLoop();

    uint8_t mTaskPriority;

    bool mStatus;


    //Cmds related structures
    typedef enum : uint8_t {READ_SYNC, READ_ASYNC, WRITE_SYNC, WRITE_ASYNC, TRANSACTION_SYNC, TRANSACTION_ASYNC} Cmds;


    typedef struct
    {

      uint8_t* outputBufferPtr;

      uint8_t numberOfBytesToRead;

      TaskHandle_t toNotifyTaskHandle;

      uint8_t *retValuePtr;

    }
    I2cGkCmdReadSyncData_t;



    typedef struct
    {

      uint8_t* inputBufferPtr;

      uint8_t numberOfBytesToWrite;

      TaskHandle_t toNotifyTaskHandle;

      uint8_t *retValuePtr;

    }
    I2cGkCmdWriteSyncData_t;



    typedef struct
    {

      uint8_t* outputBufferPtr;

      uint8_t numberOfBytesToRead;

      uint8_t* inputBufferPtr;

      uint8_t numberOfBytesToWrite;

      TaskHandle_t toNotifyTaskHandle;

      uint8_t *retValuePtr;

    }
    I2cGkCmdTransactionSyncData_t;



    typedef struct
    {

      uint8_t numberOfBytesToRead;

      delegate<void, uint8_t*, uint8_t> callbackDelegate;

    }
    I2cGkCmdReadAsyncData_t;



    typedef struct
    {

      uint8_t* inputBufferPtr;

      uint8_t numberOfBytesToWrite;

      delegate<void, uint8_t> callbackDelegate;

    }
    I2cGkCmdWriteAsyncData_t;



    typedef struct
    {

      uint8_t* inputBufferPtr;

      uint8_t numberOfBytesToWrite;

      uint8_t numberOfBytesToRead;

      delegate<void, uint8_t*, uint8_t, uint8_t> callbackDelegate;

    }
    I2cGkCmdTransactionAsyncData_t;


    typedef struct
    {

      Cmds cmd;

      uint8_t address;

      union
      {

        I2cGkCmdReadSyncData_t readSyncData;

        I2cGkCmdWriteSyncData_t writeSyncData;

        I2cGkCmdTransactionSyncData_t transactionSyncData;

        I2cGkCmdReadAsyncData_t readAsyncData;

        I2cGkCmdWriteAsyncData_t writeAsyncData;

        I2cGkCmdTransactionAsyncData_t transactionAsyncData;

      };

    }
    I2cGkCmd_t;


    //Notifications
    typedef enum : uint32_t {NEW_CMD_AT_QUEUE = 0x00010000, SYNC_OP_COMPLETED = 0x00020000} Notifications;


    //FreeRTOS static allocation for task and cmd queue
    StackType_t mTaskStackBuffer[I2C_WIRE_PDL_TASK_STACK_SIZE];
    StaticTask_t mTCBbuffer;

    TaskHandle_t mTaskHandler;


    uint8_t mCmdQueueBuffer[I2C_WIRE_PDL_CMD_QUEUE_SLOTS*sizeof(I2cGkCmd_t)];
    StaticQueue_t mCmdQueueDataStruct;

    QueueHandle_t mCmdQueueHandler;


};

#endif;
