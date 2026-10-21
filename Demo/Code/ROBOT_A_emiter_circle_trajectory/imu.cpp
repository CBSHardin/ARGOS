///////////////////////////////////////////////
//
//    LSM303 - Inertial Measurement Unit
//
//////////////////////////////////////////////
//      ARGOS - SWARM ROBOTICS PLATFORM
//////////////////////////////////////////////
// by Carlos Berbell Sánchez
//////////////////////////////////////////////

#include <Arduino.h>
#include <Wire.h>

#include "imu.h"


imu::imu()
{

  //Init I2C bus
  Wire.begin();

  //Config the interruption pin as inputs
  //INTERFACE_IMU_INT_MAG
  pinMode(INTERFACE_IMU_INT_MAG, INPUT);

  //INTERFACE_IMU_INT_1_XL
  DDRJ &= 0xBF;   //Configure PJ6 as input
  PORTJ &= 0xBF;  //Dissable pull-up resistors

};


errCode imu::getRegister(uint8_t I2CAddress, uint8_t regAddress, uint8_t *data)
{
  errCode err;

  Wire.beginTransmission(I2CAddress);
  Wire.write(regAddress);

  err = Wire.endTransmission(false);

  if (err != NO_ERROR)
  {
    Wire.endTransmission(true);

    return err;
  }

  if (Wire.requestFrom(I2CAddress, 1) != 1)
    return R_ERROR_LENGTH;

  *data =  Wire.read();

  return NO_ERROR;
}


errCode imu::setRegister(uint8_t I2CAddress, uint8_t regAddress, uint8_t data)
{

  Wire.beginTransmission(I2CAddress);
  Wire.write(regAddress);
  Wire.write(data);

  return Wire.endTransmission(true);

}


errCode imu::getDataChunk(uint8_t I2CAddress, uint8_t beginAddress, uint8_t data[], uint8_t numberOfBytesToGet)
{

  errCode err;
  uint8_t i;

  Wire.beginTransmission(I2CAddress);
  Wire.write(beginAddress);

  err = Wire.endTransmission(false);

  if (err != NO_ERROR)
  {
    Wire.endTransmission(true);

    return err;
  }

  if (Wire.requestFrom(I2CAddress, numberOfBytesToGet) != numberOfBytesToGet)
    return R_ERROR_LENGTH;

  for (i = 0 ; i < numberOfBytesToGet; i++)
    data[i] = Wire.read();

  return NO_ERROR;

}

errCode imu::setAccelerometerPowerMode(uint8_t mode)
{

  //Modificamos el registro CRTL_REG1_A
  uint8_t reg;

  errCode err = getRegister(I2CaccAddress, CRTL_REG1_A, &reg);

  if (err != NO_ERROR)
    return err;

  //LPen bit
  err = setRegister(I2CaccAddress, CRTL_REG1_A, (reg & (0x08 ^ 0xff)) | (mode << 2) );

  if (err != NO_ERROR)
    return err;

  err = getRegister(I2CaccAddress, CRTL_REG4_A, &reg);

  if (err != NO_ERROR)
    return err;

  //HR bit
  return setRegister(I2CaccAddress, CRTL_REG4_A, (reg & (0x08 ^ 0xff)) | (mode << 3) );

}


errCode imu::setFIFOmode(uint8_t mode)
{

  uint8_t reg;

  errCode err = getRegister(I2CaccAddress, FIFO_CTRL_REG_A, &reg);

  if (err != NO_ERROR)
    return err;

  return setRegister(I2CaccAddress, FIFO_CTRL_REG_A, (reg & (0xC0 ^ 0xff)) | (mode << 6));

}


errCode imu::setAccelerometerAxisPower(boolean x = true, boolean y = true, boolean z = true)
{

  uint8_t reg;

  errCode err = getRegister(I2CaccAddress, CRTL_REG1_A, &reg);

  if (err != NO_ERROR)
    return err;

  reg = (reg & (0x07 ^ 0xff)) | (z << 2) | (y << 1) | x;

  return setRegister(I2CaccAddress, CRTL_REG1_A, reg);

}


errCode imu::setAccelerometerDataRate(uint8_t ODR)
{

  uint8_t reg;

  errCode err = getRegister(I2CaccAddress, CRTL_REG1_A, &reg);

  if (err != NO_ERROR)
    return err;

  reg = (reg & 0x0f ) | (ODR << 5);

  return setRegister(I2CaccAddress, CRTL_REG1_A, reg);

}


errCode imu::setAccelerometerScale(uint8_t scale)
{

  uint8_t reg;

  errCode err = getRegister(I2CaccAddress, CRTL_REG4_A, &reg);

  reg = (reg & (0x30 ^ 0xff)) | (scale << 4);

  return setRegister(I2CaccAddress, CRTL_REG4_A, reg);

}


errCode imu::setMagnetometerPowerMode(uint8_t mode)
{

  uint8_t reg;

  errCode err = getRegister(I2CmagAddress, CFG_REG_A_M, &reg);

  if (err != NO_ERROR)
    return err;

  reg = (reg & 0xEF) | (mode << 4);

  return setRegister(I2CmagAddress, CFG_REG_A_M, reg);

}


errCode imu::setMagnetometerDataRate(uint8_t rate)
{

  uint8_t reg;

  errCode err = getRegister(I2CmagAddress, CFG_REG_A_M, &reg);

  if (err != NO_ERROR)
    return err;

  reg = (reg & 0xF3) | (rate << 2);

  return setRegister(I2CmagAddress, CFG_REG_A_M, reg);

}


errCode imu::setMagnetometerCaptureMode(uint8_t mode)
{

  uint8_t reg;

  errCode err = getRegister(I2CmagAddress, CFG_REG_A_M, &reg);

  if (err != NO_ERROR)
    return err;

  reg = (reg & (0x03 ^ 0xff)) | mode;

  return setRegister(I2CmagAddress, CFG_REG_A_M, reg);

}


errCode imu::setMagnetometerFilter(boolean filter)
{

  uint8_t reg;

  errCode err = getRegister(I2CmagAddress, CFG_REG_B_M, &reg);

  if (err != NO_ERROR)
    return err;

  reg = (reg & 0xFE) | filter;

  return setRegister(I2CmagAddress, CFG_REG_B_M, reg);

}


errCode imu::setMagnetometerDRDYInterrupt(boolean enable)
{
  uint8_t reg;

  errCode err = getRegister(I2CmagAddress, CFG_REG_C_M, &reg);

  if (err != NO_ERROR)
    return err;

  if (enable)
    reg = (reg | 0x11);
  else
    reg = (reg & 0xf7);

  return setRegister(I2CmagAddress, CFG_REG_C_M, reg);

}


errCode imu::setMagnetometerOffsetCompSingleMode(boolean enable)
{
  uint8_t reg;

  errCode err = getRegister(I2CmagAddress, CFG_REG_B_M, &reg);

  if (err != NO_ERROR)
    return err;

  if (enable)
    reg = (reg | 0x10);
  else
    reg = (reg & 0x7f);

  return setRegister(I2CmagAddress, CFG_REG_B_M, reg);

}

errCode imu::setMagnetometerOffsetCompContinuousMode(boolean enable)
{
  uint8_t reg;

  errCode err = getRegister(I2CmagAddress, CFG_REG_B_M, &reg);

  if (err != NO_ERROR)
    return err;

  if (enable)
    reg = (reg | 0x02);
  else
    reg = (reg & 0xfD);

  return setRegister(I2CmagAddress, CFG_REG_B_M, reg);

}


errCode imu::setMagnetometerOffsetPulseSetFreq(uint8_t freq)
{
  uint8_t reg;

  errCode err = getRegister(I2CmagAddress, CFG_REG_B_M, &reg);

  if (err != NO_ERROR)
    return err;

  reg = ( (reg & (0xff ^ 0x04)) | (freq << 3) );

  return setRegister(I2CmagAddress, CFG_REG_B_M, reg);



}


void imu::attachMagnetometerDRDYInterruptISR(void* ISR)
{

  attachInterrupt(digitalPinToInterrupt(INTERFACE_IMU_INT_MAG), ISR, RISING);

}

void detachMagnetometerDRDYInterruptISR(void* ISR)
{

  detachInterrupt(digitalPinToInterrupt(INTERFACE_IMU_INT_MAG));

}

errCode imu::resetMagnetometer()
{

  errCode err = setRegister(I2CmagAddress, CFG_REG_A_M, 0x40);

  delay(50);

  return err;

}


boolean imu::magnetometerSelfTest()
{

  uint8_t reg;

  setRegister(I2CmagAddress, CFG_REG_A_M, 0x8C);
  setRegister(I2CmagAddress, CFG_REG_B_M, 0x02);
  setRegister(I2CmagAddress, CFG_REG_C_M, 0x10);
  delay(50);

  //Esperamos a que haya algun dato disponible
  do
  {
    getRegister(I2CmagAddress, STATUS_REG_M, &reg);
  }
  while (!(reg & 0x08));

  //Cuando lo haya, leemos
  getMValue_X_Axis();
  getMValue_Y_Axis();
  getMValue_Z_Axis();

  long OUTX_NOST = 0;
  long OUTY_NOST = 0;
  long OUTZ_NOST = 0;

  for (uint8_t i = 0; i < 50; i++)
  {

    //Esperamos a que haya algun dato disponible
    do
    {
      getRegister(I2CmagAddress, STATUS_REG_M, &reg);
    }
    while (!(reg & 0x08));

    //Cuando lo haya, leemos y almacenamos el resultado
    OUTX_NOST = OUTX_NOST + getMValue_X_Axis();
    OUTY_NOST = OUTY_NOST + getMValue_Y_Axis();
    OUTZ_NOST = OUTZ_NOST + getMValue_Z_Axis();

  }

  //Sacamos la media de los datos
  OUTX_NOST = OUTX_NOST / 50;
  OUTY_NOST = OUTY_NOST / 50;
  OUTZ_NOST = OUTZ_NOST / 50;

  //Habilitamos el self test
  setRegister(I2CmagAddress, CFG_REG_C_M, 0x12);

  //Leemos el primer valor y lo descartamos
  do
  {
    getRegister(I2CmagAddress, STATUS_REG_M, &reg);
  }
  while (!(reg & 0x08));

  //Cuando lo haya, leemos
  getMValue_X_Axis();
  getMValue_Y_Axis();
  getMValue_Z_Axis();

  //Leemos 50 valores y sacamos la media
  long OUTX_ST = 0;
  long OUTY_ST = 0;
  long OUTZ_ST = 0;

  for (uint8_t i = 0; i < 50; i++)
  {

    //Esperamos a que haya algun dato disponible
    do
    {
      getRegister(I2CmagAddress, STATUS_REG_M, &reg);
    }
    while (!(reg & 0x08));

    //Cuando lo haya, leemos y almacenamos el resultado
    OUTX_ST = OUTX_ST + getMValue_X_Axis();
    OUTY_ST = OUTY_ST + getMValue_Y_Axis();
    OUTZ_ST = OUTZ_ST + getMValue_Z_Axis();

  }

  //Sacamos la media de los datos
  OUTX_ST = OUTX_ST / 50;
  OUTY_ST = OUTY_ST / 50;
  OUTZ_ST = OUTZ_ST / 50;

  boolean res = ( (15 <= abs(OUTX_ST - OUTX_NOST)) && (abs(OUTX_ST - OUTX_NOST) <= 500) ) && ( (15 <= abs(OUTY_ST - OUTY_NOST)) && (abs(OUTY_ST - OUTY_NOST) <= 500) ) && ( (15 <= abs(OUTZ_ST - OUTZ_NOST)) && (abs(OUTZ_ST - OUTZ_NOST) <= 500) );

  //Deshabilitamos el self-test
  setRegister(I2CmagAddress, CFG_REG_C_M, 0x10);

  //Modo reposo
  setRegister(I2CmagAddress, CFG_REG_A_M, 0x83);

  return res;

}

errCode imu::magnetometerCalibrationXY(uint32_t numberOfSamples, int16_t* axis_x_hard_iron, int16_t* axis_y_hard_iron, float* axis_x_scale, float* axis_y_scale )
{

  int16_t axisX_max = 0;
  int16_t axisX_min = 0;

  int16_t axisY_max = 0;
  int16_t axisY_min = 0;

  vector3D data;
  errCode err;
  /*
    //Primero, obtenemos cada cuanto se muestrea según la configuración
    uint8_t reg;

    errCode err = getRegister(I2CmagAddress, CFG_REG_A_M, &reg);

    if(err != NO_ERROR)
      return err;

    uint8_t sample_time_ms = 0;

    switch(reg)
    {
      case 0x00:
        sample_time_ms = 102;
        break;

      case 0x01:
        sample_time_ms = 52;
        break;

      case 0x02:
        sample_time_ms = 22;
        break;

      case 0x03:
        sample_time_ms = 12;
        break;
    }
  */

  err = setMagnetometerDRDYInterrupt(true);

  if (err != NO_ERROR)
    return err;

  err = setMagnetometerCaptureMode(CONTINUOUS_MODE);

  //delay(10*sample_time_ms);

  if (err != NO_ERROR)
    return err;

  //delay(sample_time_ms);

  while (digitalRead(INTERFACE_IMU_INT_MAG) == LOW);

  err = getMagneticVector(data);


  //Muestreamos
  for (uint32_t i = 0 ; i < numberOfSamples ; i++)
  {

    while (digitalRead(INTERFACE_IMU_INT_MAG) == LOW);

    // Serial.println("n: "+String(i));

    err = getMagneticVector(data);

    if (err != NO_ERROR)
      return err;

    //Obtenemos el máximo y el mínimo para el eje x
    if (data[0] >= axisX_max)
      axisX_max = data[0];
    else if (data[0] < axisX_min)
      axisX_min = data[0];


    //Obtenemos el máximo y el mínimo para el eje y
    if (data[1] >= axisY_max)
      axisY_max = data[1];
    else if (data[1] < axisY_min)
      axisY_min = data[1];


    //Esperamos para obtener la siguiente muestra
    // delay(sample_time_ms);

  }

  //Paramos
  err = setMagnetometerCaptureMode(IDLE_MODE);

  if (err != NO_ERROR)
    return err;

  //Cálculamos los coeficientes de corrección
  *axis_x_hard_iron = (axisX_max + axisX_min) / 2;
  *axis_y_hard_iron = (axisY_max + axisY_min) / 2;

  float avgDeltaX = (axisX_max - axisX_min) / 2;
  float avgDeltaY = (axisY_max - axisY_min) / 2;
  float avgDelta = (avgDeltaX + avgDeltaY) / 2;

  *axis_x_scale = avgDelta / avgDeltaX;
  *axis_y_scale = avgDelta / avgDeltaY;

  return err;

}



errCode imu::magnetometerCalibrationXYwithErrorFilter(uint16_t numberOfSamples, int16_t* axis_x_hard_iron, int16_t* axis_y_hard_iron, float* axis_x_scale, float* axis_y_scale, float maxStdDev )
{

  int16_t axisX_max = 0;
  int16_t axisX_min = 0;

  int16_t axisY_max = 0;
  int16_t axisY_min = 0;

  vector3D data;

  //Primero, obtenemos cada cuanto se muestrea según la configuración
  uint8_t reg;

  errCode err = getRegister(I2CmagAddress, CFG_REG_A_M, &reg);

  if (err != NO_ERROR)
    return err;

  uint8_t sample_time_ms = 0;

  switch (reg)
  {
    case 0x00:
      sample_time_ms = 102;
      break;

    case 0x01:
      sample_time_ms = 52;
      break;

    case 0x02:
      sample_time_ms = 22;
      break;

    case 0x03:
      sample_time_ms = 12;
      break;
  }

  err = setMagnetometerCaptureMode(CONTINUOUS_MODE);

  delay(10 * sample_time_ms);

  if (err != NO_ERROR)
    return err;

  delay(sample_time_ms);

  //Muestreamos
  for (uint16_t i = 0 ; i < numberOfSamples ; i++)
  {

    err = getMagneticVector(data);

    if (err != NO_ERROR)
      return err;

    //Obtenemos el máximo y el mínimo para el eje x
    if (data[0] >= axisX_max)
      axisX_max = data[0];
    else if (data[0] < axisX_min)
      axisX_min = data[0];


    //Obtenemos el máximo y el mínimo para el eje y
    if (data[1] >= axisY_max)
      axisY_max = data[1];
    else if (data[1] < axisY_min)
      axisY_min = data[1];


    //Esperamos para obtener la siguiente muestra
    delay(sample_time_ms);

  }

  //Paramos
  err = setMagnetometerCaptureMode(IDLE_MODE);

  if (err != NO_ERROR)
    return err;

  //Cálculamos los coeficientes de corrección
  *axis_x_hard_iron = (axisX_max + axisX_min) / 2;
  *axis_y_hard_iron = (axisY_max + axisY_min) / 2;

  float avgDeltaX = (axisX_max - axisX_min) / 2;
  float avgDeltaY = (axisY_max - axisY_min) / 2;
  float avgDelta = (avgDeltaX + avgDeltaY) / 2;

  *axis_x_scale = avgDelta / avgDeltaX;
  *axis_y_scale = avgDelta / avgDeltaY;

  return err;

}


errCode imu::setMagnetometerHardIronCompensation(int16_t axis_x, int16_t axis_y, int16_t axis_z)
{

  errCode err = setRegister(I2CmagAddress, OFFSET_X_REG_L_M, axis_x & 0x00ff);
  if (err != NO_ERROR)
    return err;

  err = setRegister(I2CmagAddress, OFFSET_X_REG_H_M, (axis_x & 0xff00) >> 8);
  if (err != NO_ERROR)
    return err;

  err = setRegister(I2CmagAddress, OFFSET_Y_REG_L_M, axis_y & 0x00ff);
  if (err != NO_ERROR)
    return err;

  err = setRegister(I2CmagAddress, OFFSET_Y_REG_H_M, (axis_y & 0xff00) >> 8);
  if (err != NO_ERROR)
    return err;

  err = setRegister(I2CmagAddress, OFFSET_Z_REG_L_M, axis_z & 0x00ff);
  if (err != NO_ERROR)
    return err;

  err = setRegister(I2CmagAddress, OFFSET_Z_REG_H_M, (axis_z & 0xff00) >> 8);
  if (err != NO_ERROR)
    return err;

}


////////////////// DATA RETRIEVE FUNCTIONS ///////////////////
errCode imu::getAccelerationVector(vector3D aVector)
{

  uint8_t i;
  uint8_t j;

  uint8_t data[6];

  errCode err = getDataChunk(I2CaccAddress, OUT_X_L_A, data, 6);

  if (err != NO_ERROR)
    return err;

  j = 0;
  for (i = 0 ; i < 6 ; i  = i + 2)
  {
    aVector[j] = data[i] | ((uint16_t) data[i + 1] << 8);

    j++;
  }

  return NO_ERROR;

}



errCode imu::getMagneticVector(vector3D mVector)
{

  uint8_t i;
  uint8_t j;

  uint8_t data[6];

  errCode err = getDataChunk(I2CmagAddress, OUTX_L_REG_M, data, 6);

  if (err != NO_ERROR)
    return err;

  j = 0;
  for (i = 0 ; i < 6 ; i  = i + 2)
  {
    mVector[j] = data[i] | ((uint16_t) data[i + 1] << 8);

    j++;
  }

  return NO_ERROR;

}


int16_t imu::getAValue_X_Axis()
{

  uint8_t regL, regH;

  getRegister(I2CaccAddress, OUT_X_L_A, &regL);

  getRegister(I2CaccAddress, OUT_X_H_A, &regH);

  return regL | (regH << 8);

}


int16_t imu::getAValue_Y_Axis()
{

  uint8_t regL, regH;

  getRegister(I2CaccAddress, OUT_Y_L_A, &regL);

  getRegister(I2CaccAddress, OUT_Y_H_A, &regH);

  return regL | (regH << 8);

}


int16_t imu::getAValue_Z_Axis()
{

  uint8_t regL, regH;

  getRegister(I2CaccAddress, OUT_Z_L_A, &regL);

  getRegister(I2CaccAddress, OUT_Z_H_A, &regH);

  return regL | (regH << 8);

}


int16_t imu::getMValue_X_Axis()
{

  uint8_t regL, regH;

  getRegister(I2CmagAddress, OUTX_L_REG_M, &regL);

  getRegister(I2CmagAddress, OUTX_H_REG_M, &regH);

  return regL | (regH << 8);

}


int16_t imu::getMValue_Y_Axis()
{

  uint8_t regL, regH;

  getRegister(I2CmagAddress, OUTY_L_REG_M, &regL);

  getRegister(I2CmagAddress, OUTY_H_REG_M, &regH);

  return regL | (regH << 8);

}


int16_t imu::getMValue_Z_Axis()
{

  uint8_t regL, regH;

  getRegister(I2CmagAddress, OUTZ_L_REG_M, &regL);

  getRegister(I2CmagAddress, OUTZ_H_REG_M, &regH);

  return regL | (regH << 8);

}
