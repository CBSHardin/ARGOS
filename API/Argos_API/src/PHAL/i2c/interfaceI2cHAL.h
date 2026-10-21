/*******************************************************************************
* InterfaceI2CHAL
********************************************************************************
* This is the hardware agnostic I2C Hadware Abstraction Layer interface for
* I2C peripherals.
*
* All the transfer functions encodes a single I2C operation:
* -read: read N bytes of data from an I2C device.
* -write: write N bytes of data to an I2C device.
* -transaction: write N bytes of data and without realease I2C bus, read K bytes
* of data.
*
* Functions can be sync or async. Syncs functions will block the execution of
* the caller context until the operation is completed. Async functions will
* return always, and when the operation is completed the registered callback
* will be called.
*
* Apart of this, all operation has a priority field. If the priority passed is
* greater than the priority of the I2C task, the I2C task's priority will be set
* to the priority parameter. Once the operation is completed, the I2C task's
* priority will be set to the I2C task's base priority.
*******************************************************************************/


#ifndef _INTERFACE_I2C_HAL_H__
#define _INTERFACE_I2C_HAL_H__

#include <stdint.h>

#include "../../utils/delegate/delegate.h"


class interfaceI2cHAL
{

  public:

    /**
    * This function will initialice the I2C HAL and I2C peripheral.
    */
    virtual void begin() = 0;



    /**
    * This operation will read N bytes from the I2C device. It is a blocking
    * or sync function, which means that it will not return until the operation
    * is completed.
    *
    * @param address device's I2C address.
    * @param *outBuffer read data output buffer pointer.
    * @param numberOfBytesToBeReaded number of bytes that will be readed.
    * @param bufferSize maximum number of bytes that the buffer can hold.
    * @param op_priority priority of the I2C task.
    *
    * @return number of readed bytes.
    */
    virtual uint8_t read(uint8_t address, uint8_t *outBuffer, uint8_t numberOfBytesToBeReaded, uint8_t bufferSize, uint8_t op_priority ) = 0;


    /**
    * This operation will read N bytes from the I2C device. It is a non-blocking
    * or async function, which means that it always return. Once the operation
    * is completed, the registered callback will be called.
    *
    * Two parameters will be passed to the function called: first a pointer to a
    * buffer that holds the readed data; second, the number of bytes readed. It
    * must be noted that the buffer address and the data only will exist during
    * the function's execution. Therefore, the data should be copied somewhere
    * else before the functions ends.
    *
    * Callback function proptype:
    * void callbackRecv(uint8_t *bufferPtr, uint8_t numberOfBytesReaded);
    *
    * @param address device's I2C address.
    * @param numberOfBytesToBeReaded number of bytes that will be readed.
    * @param op_priority priority of the I2C task.
    * @param onReadCompleteCallback delegate that holds the callback function.
    *
    * @return true if the operation is correctly registered. False, if the
    * operation can not be registered.
    */
    virtual bool readAsync(uint8_t address, uint8_t numberOfBytesToBeReaded, uint8_t op_priority, delegate<void, uint8_t*, uint8_t> onReadCompleteCallback) = 0;



    /**
    * This operation will write N bytes to the I2C device. It is a blocking
    * or sync function, which means that it will not return until the operation
    * is completed.
    *
    * @param address device's I2C address.
    * @param *data write data buffer pointer.
    * @param numberOfBytesToWrite number of bytes that will be wrotten.
    * @param op_priority priority of the I2C task.
    *
    * @return I2C error code after the operation is completed.
    */
    virtual uint8_t write(uint8_t address, uint8_t* data, uint8_t numberOfBytesToWrite, uint8_t op_priority) = 0;


    /**
    * This operation will write N bytes to the I2C device. It is a non-blocking
    * or async function, which means that it always return. Once the operation
    * is completed, the registered callback will be called.
    *
    * One parameter will be passed to the callback function: the error code of
    * write operation.
    *
    * Callback function proptype:
    * void callbackWrotten(uint8_t errorCode);
    *
    * @param address device's I2C address.
    * @param numberOfBytesToWrite number of bytes that will be wrotten.
    * @param op_priority priority of the I2C task.
    * @param onTransmissionCompleteCallback delegate that holds the callback
    * function.
    *
    * @return true if the operation is correctly registered. False, if the
    * operation can not be registered.
    */
    virtual bool writeAsync(uint8_t address, uint8_t* data, uint8_t numberOfBytesToWrite, uint8_t op_priority, delegate<void, uint8_t> onTransmissionCompleteCallback) = 0;



    /**
    * This operation will write N bytes to the I2C device, then, without
    * realease the bus, it will read K bytes from the device. It is a blocking
    * or sync function, which means that it will not return until the operation
    * is completed.
    *
    * @param address device's I2C address.
    * @param *data write data buffer pointer.
    * @param numberOfBytesToWrite number of bytes that will be wrotten.
    * @param *outBuffer read data output buffer pointer.
    * @param numberOfBytesToBeReaded number of bytes that will be readed.
    * @param bufferSize maximum number of bytes that the buffer can hold.
    * @param op_priority priority of the I2C task.
    * @param *errorCodePtr pointer to a variable that will hold the error code
    * after the operation.
    *
    * @return number of bytes readed.
    */
    virtual uint8_t transaction(uint8_t address, uint8_t* dataToWrite, uint8_t numberOfBytesToWrite, uint8_t* outBuffer, uint8_t numberOfBytesToRead, uint8_t bufferSize, uint8_t op_priority, uint8_t *errorCodePtr) = 0;


    /**
    * This operation will write N bytes to the I2C device, then, without
    * realease the bus, it will read K bytes from the device. It is a non-blocking
    * or async function, which means that it always return. Once the operation
    * is completed, the registered callback will be called.
    *
    * Three parameter will be passed to the callback function: first a pointer
    * to a buffer that holds the readed data; second, the number of bytes
    * readed; third, the error code of write operation. It must be noted that
    * the buffer address and the data only will exist during the function's
    * execution. Therefore, the data should be copied somewhere else before the
    * functions ends.
    *
    * Callback function proptype:
    * void callbackTransaction(uint8_t *bufferPtr, uint8_t numberOfBytesReaded, uint8_t errorCode);
    *
    *
    * @param address device's I2C address.
    * @param *dataToWrite write data buffer pointer.
    * @param numberOfBytesToWrite number of bytes that will be wrotten.
    * @param numberOfBytesToBeReaded number of bytes that will be readed.
    * @param op_priority priority of the I2C task.
    * @param onTransactionCompleteCallback delegate that holds the callback
    * function.
    *
    * @return true if the operation is correctly registered. False, if the
    * operation can not be registered.
    */
    virtual bool transactionAsync(uint8_t address, uint8_t* dataToWrite, uint8_t numberOfBytesToWrite,  uint8_t numberOfBytesToBeReaded, uint8_t op_priority, delegate<void, uint8_t*, uint8_t, uint8_t> onTransactionCompleteCallback) = 0;



    /**
    * This function will de-initialice the I2C HAL and I2C peripheral.
    */
    virtual void end() = 0;

};


#endif
