///////////////////////////////////////////////
//
//    LSM303 - Inertial Measurement Unit
//
//////////////////////////////////////////////
//      ARGOS - SWARM ROBOTICS PLATFORM
//////////////////////////////////////////////
// by Carlos Berbell Sánchez
//////////////////////////////////////////////
/// \file 
/// Header file of the Inertial Measurement Unit
/// of the IC LSM303C of STMicroelectronics.

#ifndef _IMU_LSM303C_H_
#define _IMU_LSM303C_H_

#include <Arduino.h>
#include <Wire.h>


#define A_LOW_POWER_MODE       2
#define A_NORMAL_POWER_MODE    0
#define A_HIGH_RESOLUTION_MODE 1

#define M_HIGH_RESOLUTION_MODE 0
#define M_LOW_POWER_MODE       1

#define BYPASS_MODE           0
#define FIFO_MODE             1
#define STREAM_MODE           2
#define STREAM_TO_FIFO_MODE   3

#define ODR_A_POWER_DOWN  0
#define ODR_A_1Hz         1
#define ODR_A_10Hz        2
#define ODR_A_25Hz        3
#define ODR_A_50Hz        4
#define ODR_A_100Hz       5
#define ODR_A_200Hz       6
#define ODR_A_400Hz       7
#define ODR_A_1620Hz      8   //Only avaiable for low power mode
#define ODR_A_5376Hz      9   //For low power mode: 5376Hz, for High resolution or normal mode: 1344Hz

#define SCALE_2G          0
#define SCALE_4G          1
#define SCALE_8G          2
#define SCALE_16G         3

#define I2CaccAddress   0x19
#define I2CmagAddress   0x1E

#define CRTL_REG1_A     0x20
#define CRTL_REG4_A     0x23
#define FIFO_CTRL_REG_A 0x2E

#define OUT_X_L_A       0x28
#define OUT_X_H_A       0x29
#define OUT_Y_L_A       0x2A
#define OUT_Y_H_A       0x2B
#define OUT_Z_L_A       0x2C
#define OUT_Z_H_A       0x2D

#define CFG_REG_A_M     0x60
#define CFG_REG_B_M     0x61
#define CFG_REG_C_M     0x62
#define INT_CTRL_REG_M  0x63
#define STATUS_REG_M    0x67

#define ODR_M_10        0 
#define ODR_M_20        1
#define ODR_M_50        2
#define ODR_M_100       3

#define CONTINUOUS_MODE  0
#define SINGLE_MODE      1
#define IDLE_MODE        3

#define SET_PULSE_EVERY_63_ODR  0
#define SET_PULSE_ONLY_POWER_UP 1

#define OUTX_L_REG_M    0x68
#define OUTX_H_REG_M    0x69
#define OUTY_L_REG_M    0x6A
#define OUTY_H_REG_M    0x6B
#define OUTZ_L_REG_M    0x6C
#define OUTZ_H_REG_M    0X6D

#define OFFSET_X_REG_L_M  0x45
#define OFFSET_X_REG_H_M  0x46
#define OFFSET_Y_REG_L_M  0x47
#define OFFSET_Y_REG_H_M  0x48
#define OFFSET_Z_REG_L_M  0x49
#define OFFSET_Z_REG_H_M  0x4A


#define INTERFACE_IMU_INT_MAG 19

typedef int16_t vector3D[3];


/////////////////////////////////////////////////////////////
/// errCode
/// \brief Defines the error codes returned by the functions.    
/////////////////////////////////////////////////////////////
typedef enum 
{

  NO_ERROR, /**< The operation was succesful. */

  WL_ERROR_DATA_TOO_LONG, /**< Wire.h library error code. It states that the data to 
                              transmit was too long to fit in the library buffer
                              (32 bytes). */ 

  WL_ERROR_RECV_NACK_ADDRESS, /**< Wire.h library error code. Received NACK on transmit of address. */

  WL_ERROR_RECV_NACK_DATA,  /**< Wire.h library error code. Received NACK on transmit of data. */

  WL_ERROR_UNDEFINED,  /**< Wire.h library error code. Other error */

  R_ERROR_LENGTH  /**< The number of bytes readed doesn't match with the expected number of bytes readed **/

} errCode;



///
class imu
{
  public:
    
    imu();
  
    //////////////// LOW LEVEL COMMUNICATION FUNCTIONS /////////////////////
    errCode getRegister(uint8_t I2CAddress, uint8_t regAddress, uint8_t *data);
    errCode setRegister(uint8_t I2CAddress, uint8_t regAddress, uint8_t data);
  
    errCode getDataChunk(uint8_t I2CAddress, uint8_t beginAddress, uint8_t data[], uint8_t numberOfBytesToGet); 
  
  
    /////////////// CONFIGURATION FUNCTIONS ////////////////////
    errCode setAccelerometerPowerMode(uint8_t mode);
    errCode setAccelerometerDataRate(uint8_t ODR);
    errCode setAccelerometerAxisPower(boolean x, boolean y, boolean z);
    errCode setAccelerometerScale(uint8_t scale);
    
    errCode configAccelerometerHPFilter(uint8_t filterMode, uint8_t HPC);
    //errCode setAccelerometer
    
    errCode setFIFOmode(uint8_t mode);
  

    
    errCode setMagnetometerPowerMode(uint8_t mode);
    errCode setMagnetometerDataRate(uint8_t rate);
    errCode setMagnetometerCaptureMode(uint8_t mode);
    errCode setMagnetometerFilter(boolean filter);
    errCode setMagnetometerDRDYInterrupt(boolean enable);
    errCode setMagnetometerOffsetCompSingleMode(boolean enable);
    errCode setMagnetometerOffsetCompContinuousMode(boolean enable);
    errCode setMagnetometerOffsetPulseSetFreq(uint8_t freq);
    void attachMagnetometerDRDYInterruptISR(void* ISR);
    void detachMagnetometerDRDYInterruptISR(void* ISR);
  
    errCode resetMagnetometer();
    boolean magnetometerSelfTest();

    //Este test requiere rotar el magnetormetro en el plano XY para obtener una nube de puntos con la que calibrar
    //Se compensará la nube de puntos para que este centrada en el origen y tenga forma circular, en vez de elipsoidal.
    errCode magnetometerCalibrationXY(uint32_t numberOfSamples, int16_t* axis_x_hard_iron, int16_t* axis_y_hard_iron, float* axis_x_scale, float* axis_y_scale );

    errCode magnetometerCalibrationXYwithErrorFilter(uint16_t numberOfSamples, int16_t* axis_x_hard_iron, int16_t* axis_y_hard_iron, float* axis_x_scale, float* axis_y_scale, float maxStdDev );

    
    errCode setMagnetometerHardIronCompensation(int16_t axis_x, int16_t axis_y, int16_t axis_z);
  
  
    ////////////// DATA RETRIEVE FUNCTIONS ///////////////////
  
    errCode getAccelerationVector(vector3D aVector);
    errCode getMagneticVector(vector3D mVector);
    
    int16_t getAValue_X_Axis();
    int16_t getAValue_Y_Axis();
    int16_t getAValue_Z_Axis();
    
  
    int16_t getMValue_X_Axis();
    int16_t getMValue_Y_Axis();
    int16_t getMValue_Z_Axis();

  
};





#endif
