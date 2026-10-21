///////////////////////////////////////////////
//
//  BQ34110 - Battery Fuel Gauge Library
//
//////////////////////////////////////////////
// ARGOS - SWARM ROBOTICS PLATFORM
//////////////////////////////////////////////
// by Carlos Berbell Sánchez


#include "fuelGauge.h"

#define I2C_ADDRESS_BQ34110PW 0x55 //1010101

/////////////////// CONSTRUCTOR /////////////////////////
fuelGauge::fuelGauge()
{
  
  //Init I2C bus
  Wire.begin();

  //Config the pin (PD3/D18) connected to Alert1 as input
  pinMode(18, INPUT);
  
}



//////////// INTERRUPT CONFIGURATION FUNCTIONS /////////

void fuelGauge::attachISR(void (*ISR)())
{

  attachInterrupt(digitalPinToInterrupt(18), (*ISR), HIGH);
  
}

void fuelGauge::dettachISR()
{

  detachInterrupt(digitalPinToInterrupt(18));
  
}


/////////////// COMMAND RELATED FUNCTIONS ///////////////

fuelGauge::errCode fuelGauge::getVoltage(uint16_t *retVoltage)
{
  uint8_t buff[2];
  uint16_t voltageReadReturned = 0;
  
  fuelGauge::errCode retError = readFromFunction(0x08, buff, 2);  //Reading two bytes from address 0x08

  if(retError == NO_ERROR)
  {

    voltageReadReturned = ((buff[1]<<8) | buff[0]);

    *retVoltage = voltageReadReturned;
   
  }

  return retError;
  
}


fuelGauge::errCode fuelGauge::getCurrent(int16_t *retCurrent)
{
  uint8_t buff[2];
  int16_t currentReadReturned = 0;
  fuelGauge::errCode retError;

  retError = readFromFunction(0x0C, buff, 2);  //Reading two bytes from address 0x08

  if(retError == NO_ERROR)
  {
    
    currentReadReturned = ((buff[1]<<8) | buff[0]);

    *retCurrent = currentReadReturned;
    
  }

  return retError;
  
}


fuelGauge::errCode fuelGauge::getAvgCurrent(uint8_t numberOfSamples, float *retAvgCurrent)
{
  
  int32_t rawDataSum = 0;
  uint8_t loopCount = 0;
  uint8_t counterNow;
  uint8_t counterPrev;

  uint8_t buff[2];

  int16_t current;

  fuelGauge::errCode retError;
  
  //Reading counterNow
  readFromFunction(0x79, buff, 1);
  
  counterNow = buff[0];
  counterPrev = counterNow;

  for(loopCount ; loopCount < numberOfSamples ; loopCount++)
  {

    do
    {
  
      //Wait 200ms
      delay(200);
  
        //Reading counterNow
        retError = readFromFunction(0x79, buff, 1);

        if(retError != NO_ERROR)
          return retError;
    
        counterNow = buff[0];
        
    }
    while(counterNow == counterPrev);

    retError = getCurrent(&current);

    if(retError != NO_ERROR)
      return retError;

    rawDataSum += current;

    counterPrev = counterNow;

  }

  //Return the average current
  *retAvgCurrent = ((float)rawDataSum/(float)numberOfSamples);

  //Returning the error code
  return  retError;
  
}


//Return the temperature in Kelvins
fuelGauge::errCode fuelGauge::getTemperature(float *retTemperature)
{
  uint16_t temp;

  fuelGauge::errCode retError = readWordFromFunction(0x06, &temp);

  *retTemperature = (float)(temp*0.1f);

  return retError;
  
}


fuelGauge::errCode fuelGauge::getRemainingCapacity(uint16_t *remainingCapacity)
{

  return readWordFromFunction(0x10, remainingCapacity);
  
}


fuelGauge::errCode fuelGauge::getFullChargeCapacity(uint16_t *fullChargeCapacity)
{

  return readWordFromFunction(0x12, fullChargeCapacity);
  
}


fuelGauge::errCode fuelGauge::getRelativeStateOfCharge(uint16_t *relativeStateOfCharge)
{

  return readWordFromFunction(0x2C, relativeStateOfCharge);
  
}


fuelGauge::errCode fuelGauge::getAccumulatedCharge(int16_t *accumulatedCharge)
{

  return readWordFromFunction(0x1A, accumulatedCharge);

}


fuelGauge::errCode fuelGauge::getAccumulatedTime(uint16_t *accumulatedTime)
{

  return readWordFromFunction(0x1C, accumulatedTime);
  
}


fuelGauge::errCode fuelGauge::acummReset()
{

  return writeMACOperation(ACCUM_RESET, NULL, 0);
  
}


fuelGauge::errCode fuelGauge::updateInterruptionFlags()
{

  fuelGauge::errCode retError;

  //Return all the Alert1 flags
  retError = readMACOperation(ALERT1STATUS, alertStatusFlags, 7);

  if(retError != NO_ERROR)
    return retError;

  //Return all the Alert2 flags
  retError = readMACOperation(ALERT2STATUS, (alertStatusFlags+7), 7);

  return retError;
  
}


uint8_t fuelGauge::getAlertStatusFlags(uint8_t AlertStatusRegister)
{

  return alertStatusFlags[AlertStatusRegister];
  
}


fuelGauge::errCode fuelGauge::updateInterruptionMasks()
{
  
  uint8_t returnedData[14];
  
  fuelGauge::errCode retError = writeMACOperation(ALERT1_0_CONFIG, alertStatusMasks, 14);

  if(retError != NO_ERROR)
    return retError;

  //Now, we test if the mask was corrected writted in the memory
  retError = readMACOperation(ALERT1_0_CONFIG, returnedData, 14);

  if(retError != NO_ERROR)
    return retError;

  //Test, byte per byte if the information was wroten correctly
  if(memcmp(alertStatusMasks, returnedData, 14)!=0)
    return WR_ERROR_FLASH_MEMORY;

  return NO_ERROR;
}


void fuelGauge::setAlertStatusMasks(uint8_t AlertStatusRegister, uint8_t mask)
{

  alertStatusMasks[AlertStatusRegister] = mask;
  
}


fuelGauge::errCode fuelGauge::reset()
{

  return writeMACOperation(RESET, NULL, 0);
  
}


//////////////// CONFIGURATION FUNCTIONS ////////////////

fuelGauge::errCode fuelGauge::setNumberOfCells(uint8_t numberOfCells)
{
  
  return writeByteToFlash(NUMBER_OF_SERIES_CELLS, numberOfCells, 0);
  
}


fuelGauge::errCode fuelGauge::getNumberOfCells(uint8_t *numberOfCells)
{

  return readByteFromFlash(NUMBER_OF_SERIES_CELLS, numberOfCells);
  
}


fuelGauge::errCode fuelGauge::setVoltageDivisor(uint16_t voltageDivisor)
{

  return writeWordToFlash(VOLTAGE_DIVIDER, voltageDivisor, 0);
  
}


fuelGauge::errCode fuelGauge::getVoltageDivisor(uint16_t* voltageDivisor)
{

  return readWordFromFlash(VOLTAGE_DIVIDER, voltageDivisor);
  
}


fuelGauge::errCode fuelGauge::setExternalDivisor()
{

  //First, we get the value of the Pin Control Register
  uint8_t pinControlReg;
  
  fuelGauge::errCode retError = readByteFromFlash(PIN_CONTROL_CONFIG, &pinControlReg);

  if(retError != NO_ERROR)
    return retError;

  //Then, we write the new value to the register
  return writeByteToFlash(PIN_CONTROL_CONFIG, (pinControlReg | 0x10), 0 );
  
}


fuelGauge::errCode fuelGauge::getExternalDivisor(bool *isEnabled)
{
  uint8_t data;

  fuelGauge::errCode retError = readByteFromFlash(PIN_CONTROL_CONFIG, &data);

  if(retError != NO_ERROR)
    return retError;
    
  if(data & 0x10)
    *isEnabled = true;
  else
    *isEnabled = false;

  return NO_ERROR;

}


fuelGauge::errCode fuelGauge::setFlashOKVoltage(uint16_t okVoltage)
{

  return writeWordToFlash(FLASH_UPDATE_OK_VOLTAGE, okVoltage, 0);

}


fuelGauge::errCode fuelGauge::getFlashOKVoltage(uint16_t *okVoltage)
{

  return readWordFromFlash(FLASH_UPDATE_OK_VOLTAGE, okVoltage);
  
}


fuelGauge::errCode fuelGauge::setCEDVConfiguration(uint16_t CEDVReg)
{

  return writeWordToFlash(CEDV_GAUGING_CONFIGURATION, CEDVReg, 0);
  
}

    
fuelGauge::errCode fuelGauge::getCEDVConfiguration(uint16_t *CEDVReg)
{

  return readWordFromFlash(CEDV_GAUGING_CONFIGURATION, CEDVReg);
  
}


fuelGauge::errCode fuelGauge::setDesignCapacity(int16_t designCapacity)
{

  return writeWordToFlash(DESIGN_CAPACITY_mAh, designCapacity, 75);
  
}


fuelGauge::errCode fuelGauge::getDesignCapacity(int16_t *designCapacity)
{

  return readWordFromFlash(DESIGN_CAPACITY_mAh, designCapacity);
  
}


fuelGauge::errCode fuelGauge::setFullChargeCapacity(int16_t fullChargeCapacity)
{

  writeWordToFlash(LEARNED_FULL_CHARGE_CAPACITY, fullChargeCapacity, 75);
  
}


fuelGauge::errCode fuelGauge::getFullChargeCapacityFromFlash(int16_t *fullChargeCapacity)
{

  return readWordFromFlash(LEARNED_FULL_CHARGE_CAPACITY, fullChargeCapacity);
  
}


fuelGauge::errCode fuelGauge::setCEDVProfile(int16_t profile[])
{

  uint8_t data[22];

  uint8_t retValue[22];
  fuelGauge::errCode retError;
  
  //The profile array has 11 elements, we will iterate over them to create the array
  //that will send to be programmed into the flash memory

  uint8_t i;
  for(i = 0 ; i < 22 ; i+=2)
  {
    data[i] = (uint8_t)(profile[i] >> 8);     //Big endian order (MSB first in memory)
    data[i + 1] = (uint8_t)(profile[i] & 0x00ff);  
  }

  //We will load the data, beginning with the first "Voltage 0% DOD"
  retError = writeMACOperation(VOLTAGE_0_DOD, data, 22);

  if(retError != NO_ERROR)
    return retError;

  //Now, we test if the mask was corrected writted in the memory
  retError = readMACOperation(VOLTAGE_0_DOD, retValue, 22);

  if(retError != NO_ERROR)
    return retError;

  //Test, byte per byte if the information was wroten correctly
  if(memcmp(data, retValue, 14)!=0)
    return WR_ERROR_FLASH_MEMORY;

  return NO_ERROR;

}


fuelGauge::errCode fuelGauge::getCEDVProfile(int16_t profile[])
{
  
  uint8_t data[11*2];
  fuelGauge::errCode retError;

  //Retrieve all the data in big endian order
  retError = readMACOperation(VOLTAGE_0_DOD, data, 22);

  if(retError != NO_ERROR)
    return retError;

  uint8_t i;
  for(i = 0 ; i < 11 ; i++)
    profile[i] = ((uint16_t)data[i*2] << 8) + (uint16_t) data[i*2 + 1];

  return NO_ERROR;
  
}


fuelGauge::errCode fuelGauge::setOverloadCurrent(int16_t overloadCurrent)
{

  return writeWordToFlash(OVERLOAD_CURRENT, overloadCurrent, 0);
  
}


fuelGauge::errCode fuelGauge::getOverloadCurrent(int16_t *overloadCurrent)
{

  return readWordFromFlash(OVERLOAD_CURRENT, overloadCurrent);
  
}


fuelGauge::errCode fuelGauge::setEDV(int16_t EDV[])
{

  fuelGauge::errCode retError;

  //Writing the EDV voltages:
  //EDV0
  retError = writeWordToFlash(FIXED_EDV0, EDV[0], 0);

  if(retError != NO_ERROR)
    return retError;

  //EDV1
  retError = writeWordToFlash(FIXED_EDV1, EDV[1], 0);

  if(retError != NO_ERROR)
    return retError;

  //EDV2
  retError = writeWordToFlash(FIXED_EDV2, EDV[2], 0);

  if(retError != NO_ERROR)
    return retError;

  return NO_ERROR;
  
}


fuelGauge::errCode fuelGauge::getEDV(int16_t EDV[])
{

  fuelGauge::errCode retError;

  //Read EDV voltages
  //EDV0
  retError = readWordFromFlash(FIXED_EDV0, &EDV[0]);

  if(retError != NO_ERROR)
    return retError;
  
  //EDV1
  retError = readWordFromFlash(FIXED_EDV1, &EDV[1]);

  if(retError != NO_ERROR)
    return retError;

  //EDV2
  retError = readWordFromFlash(FIXED_EDV2, &EDV[2]);

  if(retError != NO_ERROR)
    return retError;

  return NO_ERROR;
  
}


fuelGauge::errCode fuelGauge::setPinControlConfig(uint8_t reg)
{

  return writeByteToFlash(PIN_CONTROL_CONFIG, reg, 0);
  
}


fuelGauge::errCode fuelGauge::getPinControlConfig(uint8_t *reg)
{

  return readByteFromFlash(PIN_CONTROL_CONFIG, reg);
  
}


fuelGauge::errCode fuelGauge::setAccumChargePossitiveThreshold(int16_t chargeThreshold)
{

  return writeWordToFlash(ACCUM_CHARGE_POSSITVE_THRESHOLD, chargeThreshold, 75);
  
}


fuelGauge::errCode fuelGauge::getAccumChargePossitiveThreshold(int16_t *chargeThreshold)
{

  return readWordFromFlash(ACCUM_CHARGE_POSSITVE_THRESHOLD, chargeThreshold);
  
}


fuelGauge::errCode fuelGauge::setManufacturingStatusInit(uint16_t reg)
{

  return writeWordToFlash(MFG_STATUS_INIT, reg, 75);
  
}


fuelGauge::errCode fuelGauge::getManufacturingStatusInit(uint16_t *reg)
{

  return readWordFromFlash(MFG_STATUS_INIT, reg);
  
}


fuelGauge::errCode fuelGauge::setBATLOWConfig(int16_t lowSetThreshold, uint8_t lowTime, int16_t lowClearThreshold)
{

    fuelGauge::errCode retError = writeWordToFlash(BATTERY_LOW_SET_THRESHOLD, lowSetThreshold, 0);

    if(retError != NO_ERROR)
      return retError;

    retError = writeByteToFlash(BATTERY_LOW_TIME, lowTime, 0);

    if(retError != NO_ERROR)
      return retError;

    return writeWordToFlash(BATTERY_LOW_CLEAR_THRESHOLD, lowSetThreshold, 0);
    
}


fuelGauge::errCode fuelGauge::getBATLOWConfig(int16_t *lowSetThreshold, uint8_t *lowTime, int16_t *lowClearThreshold)
{

    fuelGauge::errCode retError = readWordFromFlash(BATTERY_LOW_SET_THRESHOLD, lowSetThreshold);

    if(retError != NO_ERROR)
      return retError;

    retError = readByteFromFlash(BATTERY_LOW_TIME, lowTime);

    if(retError != NO_ERROR)
      return retError;

    return readWordFromFlash(BATTERY_LOW_CLEAR_THRESHOLD, lowClearThreshold);
  
}



///////////// CALIBRATION RELATED FUNCTIONS /////////////

//Return true if it was possible to enter into the calibration mode
//and return false, if there was some error, and threfore, it was
//not possible to enter into the calibration mode.
bool fuelGauge::enterCalibrationMode()
{

  if(toggleCalibrationMode() == 1)
    return true;
  else
    return false;

}


//Return true if it was possible to exit of the calibration mode
//and return false, if there was some error, and threfore, it was
//not possible to exit of the calibration mode.
bool fuelGauge::exitCalibrationMode()
{

  if(toggleCalibrationMode() == 0)
    return true;
  else
    return false;
    
}


fuelGauge::errCode fuelGauge::boardOffsetCalibration()
{

  uint8_t buff[2];
  fuelGauge::errCode retError;

  do
  {

    retError = writeWordToFunction(0x00, 0x0009);

    if(retError != NO_ERROR)
      return retError;

    delay(100);
        
    retError = writeWordToFunction(0x00, 0x0009);

    if(retError != NO_ERROR)
      return retError;

    delay(100);

    retError = readFromFunction(0x00, buff, 2);

    if(retError != NO_ERROR)
      return retError;

    delay(100);
   
  }while((buff[0] & 0x30) != 0x30);


  //Waiting for BCA == 0 which means that the calibration process has finished
  while((buff[0] & 0x10) == 0x10)
  {

      do
      {

        delay(500);
        
        retError = readFromFunction(0x00, buff, 2);

        if(retError != NO_ERROR)
            return retError;
        
      }while((buff[0] & 0x20) == 0x20);
    
  }

  return NO_ERROR;
  
}


//Vapplied in mV
fuelGauge::errCode fuelGauge::voltageCalibration(uint16_t Vapplied)
{

  uint16_t oldDivider;
  uint16_t newDivider;
  float avgVoltage;
  
  fuelGauge::errCode retError;
  
  retError = getAvgRawVoltage(100, &avgVoltage);

  if(retError != NO_ERROR)
    return retError;

  delay(1);
  
  //Reading the voltage divider:
  retError = getVoltageDivisor(&oldDivider);

  if(retError != NO_ERROR)
    return retError;

  newDivider = round((float)Vapplied*oldDivider/(float)avgVoltage);
  
  delay(1);
  
  //Saving the new divider
  return setVoltageDivisor(newDivider);
  
}


fuelGauge::errCode fuelGauge::currentCalibration(float forcedCurrent)
{

  float ccGain;
  float ccDelta;
  
  float avgCurrent;

  uint8_t data[4];
  uint8_t readedData[4];

  fuelGauge::errCode retError;
  
  //Reading the boardOffset and CC0ffset from the flash memory.
  int16_t ccOffset;
  int8_t boardOffset;
  
  retError = readWordFromFlash(0x4008, &ccOffset);

  if(retError != NO_ERROR)
    return retError;

  retError = readByteFromFlash(0x400C, &boardOffset);

  if(retError != NO_ERROR)
    return retError;

  //Getting the measured rawCurrent
  retError = getAvgRawCurrent(100, &avgCurrent);

  if(retError != NO_ERROR)
    return retError;

  //Computing ccGain and ccDelta
  ccGain = forcedCurrent/(avgCurrent - ((float)ccOffset + (float)boardOffset/(1000.0f))/16.0f);

  ccDelta = ccGain *1193046.0f;

  //Now, we will write ccDelta and ccGain to the flash memory
  convertFPtoRawBytes(ccGain, data);
  retError = writeMACOperation(0x4000, data, 4);

  if(retError != NO_ERROR)
    return retError;

  delay(75);

  retError = readMACOperation(0x4000, readedData, 4);

  if(retError != NO_ERROR)
    return retError;

  //Test if the bytes was correctly wroten
   if(memcmp(readedData, data, 4)!=0)
    return WR_ERROR_FLASH_MEMORY;

  delay(1);

  convertFPtoRawBytes(ccDelta, data);
  retError = writeMACOperation(0x4004, data, 4);

  if(retError != NO_ERROR)
    return retError;
    
  delay(75);
  
  retError = readMACOperation(0x4004, readedData, 4);

  if(retError != NO_ERROR)
    return retError;

  //Test if the bytes was correctly wroten
  if(memcmp(readedData, data, 4)!=0)
    return WR_ERROR_FLASH_MEMORY;
  
  //Done!
  return NO_ERROR;
}


fuelGauge::errCode fuelGauge::getAvgRawVoltage(uint8_t samplesToAvg, float *avgRawVoltage)
{

  uint32_t rawDataSum = 0;
  uint8_t loopCount = 0;
  uint8_t counterNow;
  uint8_t counterPrev;

  uint8_t buff[2];

  fuelGauge::errCode retError;

  //Reading counterNow
  retError = readFromFunction(0x79, buff, 1);

  if(retError != NO_ERROR)
    return NO_ERROR;
  
  counterNow = buff[0];
  counterPrev = counterNow;

  for(loopCount ; loopCount < samplesToAvg ; loopCount++)
  {

    do
    {
  
      //Wait 200ms
      delay(200);
  
  
        //Reading counterNow
        retError = readFromFunction(0x79, buff, 1);
        
        if(retError != NO_ERROR)
          return NO_ERROR;
    
        counterNow = buff[0];
        
    }while(counterNow == counterPrev);


    //Reading the voltage
    retError = readFromFunction(0x7C, buff, 2);

    if(retError != NO_ERROR)
      return NO_ERROR;

    rawDataSum += (buff[1] << 8) + buff[0];


    counterPrev = counterNow;

  }

  //Computing the average:
  *avgRawVoltage = ((float)rawDataSum/(float)samplesToAvg);

  return NO_ERROR;

  
}


fuelGauge::errCode fuelGauge::getAvgRawCurrent(uint8_t samplesToAvg, float *avgRawCurrent)
{

  int32_t rawDataSum = 0;
  uint8_t loopCount = 0;
  uint8_t counterNow;
  uint8_t counterPrev;

  uint8_t buff[2];

  fuelGauge::errCode retError;

  //Reading counterNow
  retError = readFromFunction(0x79, buff, 1);

  if(retError != NO_ERROR)
    return NO_ERROR;
  
  counterNow = buff[0];
  counterPrev = counterNow;

  for(loopCount ; loopCount < samplesToAvg ; loopCount++)
  {

    do
    {
  
      //Wait 200ms
      delay(200);
  
  
        //Reading counterNow
        retError = readFromFunction(0x79, buff, 1);

        if(retError != NO_ERROR)
          return NO_ERROR;
    
        counterNow = buff[0];
        
    }while(counterNow == counterPrev);


    //Reading the voltage
    retError = readFromFunction(0x7A, buff, 2);

    if(retError != NO_ERROR)
      return NO_ERROR;

    //Serial.println("Raw current sample: Low byte: 0x"+String(buff[0], HEX)+" High byte: 0x"+String(buff[1], HEX));

    rawDataSum += (buff[1] << 8) + buff[0];


    counterPrev = counterNow;

  }


  //Computing the average:
  *avgRawCurrent = ((float)rawDataSum/(float)samplesToAvg);

  return NO_ERROR;
  
}


void fuelGauge::convertFPtoRawBytes(float val, uint8_t dataForFlash[])
{

  int exp = 0;
  float mod_val;
  float tmpVal = 0;

  uint8_t byte2, byte1, byte0;
  
  
  if(val < 0)
    mod_val = val * (-1);
  else
    mod_val = val;


  tmpVal = mod_val;

  tmpVal = tmpVal * (1+pow(2, -25));


  if(tmpVal < 0.5)
  {
  
    while(tmpVal < 0.5)
    {
      tmpVal*=2.0f;
      exp--;
      
    }
    
  }
  else if(tmpVal >= 1.0)
  {

    while(tmpVal >= 1.0)
    {
      tmpVal = tmpVal/2.0f;
      exp++;
    }

  }


  if(exp > 127)
  
    exp = 127;
    
  else if(exp < -128)
  
    exp = -128;
    
  
  //Conversión a bytes
  tmpVal = pow(2, (8 - exp))*mod_val - 128;

  byte2 = tmpVal;

  tmpVal = pow(2, 8)*(tmpVal - byte2);

  byte1 = tmpVal;

  tmpVal = pow(2, 8)*(tmpVal - byte1);

  byte0 = tmpVal;


  if(val < 0)
    byte2 |= 0x80;


  //Cargamos los bytes en el array que enviaremos a la memoria.
  dataForFlash[0] = exp + 128;
  
  dataForFlash[1] = byte2;

  dataForFlash[2] = byte1;

  dataForFlash[3] = byte0;
  
}


///////////// SECURITY RELATED FUNCTIONS /////////////

fuelGauge::errCode fuelGauge::sendKeys(uint32_t key)
{ 
  fuelGauge::errCode retError;

  //The keys will be send to the control register in little endian order
  Wire.beginTransmission(I2C_ADDRESS_BQ34110PW);
  Wire.write(0x00);

  Wire.write( (uint8_t)(key & 0x00ff));
  Wire.write( (uint8_t)(key >> 8));

  retError = Wire.endTransmission();

  delay(10);

  if(retError != NO_ERROR)
    return retError;

  Wire.beginTransmission(I2C_ADDRESS_BQ34110PW);
  Wire.write(0x00);

  Wire.write( (uint8_t)(key >> 16)); 
  Wire.write( (uint8_t)(key >> 24));

  retError = Wire.endTransmission();

  delay(1);

  return retError;

}


fuelGauge::errCode fuelGauge::readKeys(uint32_t *unsealedKey, uint32_t *fullAccess_key)
{

  uint8_t data[8];

  //Reading the access keys
  fuelGauge::errCode retError = readMACOperation(0x0035, data, 8);

  if(retError != NO_ERROR)
    return retError;

  //The datasheet says that they have to be readed in big-endian, but then, it doesn't work when you try
  //to send the keys to unseal the device. So, we will read it in little endian (which works)
  *unsealedKey = ( ((uint32_t)data[3] << 24) | ((uint32_t)data[2] << 16) | ((uint32_t)data[1] << 8) | (uint32_t)data[0]);

  *fullAccess_key = ( ((uint32_t)data[7] << 24) | ((uint32_t)data[6] << 16) | ((uint32_t)data[5] << 8) | (uint32_t)data[4]);

  return NO_ERROR;
  
}



fuelGauge::errCode fuelGauge::writeKeys(uint32_t unsealedKey, uint32_t fullAccessKey )
{

  uint8_t data[8];

  //The key would be write in little endian order
  data[0] = (uint8_t)unsealedKey;
  data[1] = (uint8_t)(unsealedKey >> 8);
  data[2] = (uint8_t)(unsealedKey >> 16);
  data[3] = (uint8_t)(unsealedKey >> 24);

  data[4] = (uint8_t)fullAccessKey;
  data[5] = (uint8_t)(fullAccessKey >> 8);
  data[6] = (uint8_t)(fullAccessKey >> 16);
  data[7] = (uint8_t)(fullAccessKey >> 24);

  //Writing to SECURITY_KEY "control" register
  return writeMACOperation(0x0035, data, 8);
  
}


fuelGauge::errCode fuelGauge::sealDevice()
{

  //Execute device sealing
  return writeMACOperation(0x0030, NULL, 0);
  
}


fuelGauge::errCode fuelGauge::getAccessStatus(accessStatus *accStatus)
{

  uint8_t reg;

  //We get the low byte of the operation: OperationStatus()
  fuelGauge::errCode retError = readFromFunction(0x3A, &reg, 1);

  *accStatus = ((reg >> 1) & 0x03);

  return retError;

}



//////////// LOW LEVEL INTERFACE ACCESS FUNCTIONS ////////////

fuelGauge::errCode fuelGauge::readFromFunction(uint8_t functionAddress, uint8_t data[], uint8_t len)
{

   fuelGauge::errCode retError;

   Wire.beginTransmission(I2C_ADDRESS_BQ34110PW);
   Wire.write(functionAddress);
   retError = Wire.endTransmission(false);

   if(retError != NO_ERROR)
    return retError;

   if(Wire.requestFrom(I2C_ADDRESS_BQ34110PW, len) != len)
    return R_ERROR_LENGTH;
   
   uint8_t i;

   for(i = 0; i < len ; i++)
    data[i] = Wire.read();

   delayMicroseconds(600); 

   return NO_ERROR;
  
}


fuelGauge::errCode fuelGauge::writeToFunction(uint8_t functionAddress, uint8_t data[], uint8_t len)
{

   fuelGauge::errCode retError;

   Wire.beginTransmission(I2C_ADDRESS_BQ34110PW);
   
   Wire.write(functionAddress);
   Wire.write(data, len);
   
   retError = Wire.endTransmission(true);

   delayMicroseconds(600); 

   return retError;
  
}


fuelGauge::errCode fuelGauge::readWordFromFunction(uint8_t functionAddress, uint16_t *wordReaded)
{

  uint8_t data[2];

  fuelGauge::errCode retError;

  retError = readFromFunction(functionAddress, data, 2);

  *wordReaded = ( data[0] | ((uint16_t) data[1] << 8) );

  return retError;
  
}


fuelGauge::errCode fuelGauge::writeWordToFunction(uint8_t functionAddress, uint16_t data)
{

  uint8_t buff[] = {data & 0x00ff, data >> 8};

  return writeToFunction(functionAddress, buff, 2);
  
}


fuelGauge::errCode fuelGauge::readMACOperation(uint16_t MACsubcommand, uint8_t data[], uint8_t dataLen)
{

  uint8_t buff[36];

  uint8_t err;
  
  //Sending the subcommand
  Wire.beginTransmission(I2C_ADDRESS_BQ34110PW);
  
  Wire.write(0x3E);
  Wire.write(MACsubcommand & 0x00ff);
  Wire.write(MACsubcommand >> 8);
  
  err = Wire.endTransmission();

  if(err)
    return err;


  delay(1);


  //Reading from 0x3E (MAC subcommand)
  Wire.beginTransmission(I2C_ADDRESS_BQ34110PW);
  
  Wire.write(0x3E);
  
  err = Wire.endTransmission(false);

  if(err)
    return err;
    

  //Loading the data into the buffer
  if(Wire.requestFrom(I2C_ADDRESS_BQ34110PW, 32, false) != 32)   //Reading the first 32 bytes
    return R_ERROR_LENGTH;

  uint8_t i;
  for(i = 0; i < 32 ; i++)
    buff[i] = Wire.read();
    

  if(Wire.requestFrom(I2C_ADDRESS_BQ34110PW, 4) != 4)           //Reading the last 4 bytes
    return R_ERROR_LENGTH;
  
  for(i = 0; i < 4 ; i++)
    buff[i+32] = Wire.read();


  //Checking the subcommand
  if((buff[0] | ((uint16_t) buff[1] << 8) )!= MACsubcommand)
    return R_ERROR_MACSUBCOMMAND;    //Subcommand tranmission fail


  //Checking the checksum
  if(buff[34] == computeMACDataSum(MACsubcommand, (buff+2), 32))
  {
    //Coping the data in the buffer to the user array
     memcpy(data, (buff+2), dataLen);
    
    return NO_ERROR;
  }
  else
    return R_ERROR_CHECKSUM; //Checksum fail


  delay(1);
  
}


//Este comando realizará una transaccion completa, escribiendo en una sola rafaga
//todos los registros relacionados con las operaciones de un determinado subcomando
fuelGauge::errCode fuelGauge::writeMACOperation(uint16_t MACsubcommand, uint8_t data[], uint8_t dataLen)
{

  uint8_t buff[37]; //37 bytes (2 for the subcommand, 32 for the data, 1 for the datasum, 1 for the length and 1 for the address)
  uint8_t err;

  //Writing the subcommand to the buff
  buff[0] = 0x3E;
  buff[1] = (uint8_t)(MACsubcommand & 0x00ff);
  buff[2] = (uint8_t)((MACsubcommand & 0xff00) >> 8);

  //Writing the data
  uint8_t i;
  for(i = 0 ; i < dataLen ; i++)
    buff[i+3] = data[i];

  //Writing 0 in the empty data
  for(i = 3 + dataLen ; i < 3 + 32 ; i++)
    buff[i] = 0;

  //Writing the datasum
  buff[35] = computeMACDataSum(MACsubcommand, data, dataLen);

  //Writing the datalen:
  buff[36] = (dataLen+4);   //The datasheet says you have to add 4, only the Maker knows why...


  //Sending the bytes, the Wire library has a buffer of 32 bytes, so we should send only 32 bytes per data frame
  Wire.beginTransmission(I2C_ADDRESS_BQ34110PW);
  
  Wire.write(buff, 32);
  
  err = Wire.endTransmission();
  if(err)
    return err;

  Wire.beginTransmission(I2C_ADDRESS_BQ34110PW);
  
  Wire.write((0x3E) + 31);
  Wire.write((buff+32), 5);
  
  err = Wire.endTransmission();
  if(err)
    return err;

  delay(1);

  return NO_ERROR;
  
}


fuelGauge::errCode fuelGauge::readByteFromFlash(uint16_t flashAddress, uint8_t* readedByte)
{

  return readMACOperation(flashAddress, readedByte, 1);
  
}


fuelGauge::errCode fuelGauge::readWordFromFlash(uint16_t flashAddress, uint16_t* readedWord)
{
  uint8_t retData[2];
  
  fuelGauge::errCode retError = readMACOperation(flashAddress, retData, 2);
  
  //The bytes of the flash memory are ordered in Big-Endian.
  *readedWord = ((retData[0] << 8) | retData[1]);

  return retError;
  
}


fuelGauge::errCode fuelGauge::writeByteToFlash(uint16_t flashAddress, uint8_t data, uint16_t delayBetweenWR)
{

  uint8_t buff = data;
  uint8_t retValue;

  fuelGauge::errCode retError = writeMACOperation(flashAddress, &buff, 1);

  if(retError != NO_ERROR)
    return retError;

  delay(delayBetweenWR);

  retError = readByteFromFlash(flashAddress, &retValue);

  if(retError != NO_ERROR)
    return retError;

  if(retValue != data)
    return WR_ERROR_FLASH_MEMORY;

  return NO_ERROR;

}


fuelGauge::fuelGauge::errCode fuelGauge::writeWordToFlash(uint16_t flashAddress, uint16_t data, uint16_t delayBetweenWR)
{
  
  uint8_t buff[2];
  
  uint16_t retValue;
  fuelGauge::errCode retError;

  buff[1] = (data & 0x00ff);

  buff[0] = (data >>8);

  retError =  writeMACOperation(flashAddress, buff, 2);
  
  if(retError != NO_ERROR)
    return retError;

  delay(delayBetweenWR);

  retError = readWordFromFlash(flashAddress, &retValue);

  if(retError != NO_ERROR)
    return retError;

  if(retValue != data)
    return WR_ERROR_FLASH_MEMORY;

  return NO_ERROR;

}



///////////////////// DEBUG FUNCTIONS /////////////////////

void fuelGauge::printInternalTempCoef()
{

  uint8_t buff[2];

  Serial.println("INTERNAL TEMPERATURE MEASURE MODEL");
  

  readMACOperation(0x41C1, buff, 2);

  Serial.println("Int Coeff 1: " +String( (buff[0] << 8) + buff[1]));

  delay(1);


  readMACOperation(0x41C3, buff, 2);

  Serial.println("Int Coeff 2: " +String( (buff[0] << 8) + buff[1]));

  delay(1);


  readMACOperation(0x41C5, buff, 2);

  Serial.println("Int Coeff 3: " +String( (buff[0] << 8) + buff[1]));

  delay(1);


  readMACOperation(0x41C7, buff, 2);

  Serial.println("Int Coeff 4: " +String( (buff[0] << 8) + buff[1]));

  delay(1); 


  readMACOperation(0x41C9, buff, 2);

  Serial.println("Int Min AD: " +String( (buff[0] << 8) + buff[1]));

  delay(1); 


  readMACOperation(0x41CB, buff, 2);

  Serial.println("Int Max Temp: " +String( (buff[0] << 8) + buff[1]));

  delay(1); 

}


void fuelGauge::printInternalVoltCompCoef()
{

  uint8_t buff[2];

  Serial.println("INTERNAL REFERENCE TEMPERATURE COMPENSATION MODEL");
  

  readMACOperation(0x41D7, buff, 2);

  Serial.println("Vcomp Coeff 1: " +String( (buff[0] << 8) + buff[1]));

  delay(1);


  readMACOperation(0x41D9, buff, 2);

  Serial.println("Vcomp Coeff 2: " +String( (buff[0] << 8) + buff[1]));

  delay(1);


  readMACOperation(0x41DB, buff, 2);

  Serial.println("Vcomp Coeff 3: " +String( (buff[0] << 8) + buff[1]));

  delay(1);


  readMACOperation(0x41DD, buff, 2);

  Serial.println("Vcomp Coeff 4: " +String( (buff[0] << 8) + buff[1]));

  delay(1); 


  readMACOperation(0x41DF, buff, 2);

  Serial.println("Vcomp Input Multiplier: " +String( (buff[0] << 8) + buff[1]));

  delay(1); 


  readMACOperation(0x41E0, buff, 2);

  Serial.println("Vcomp Output Divisor: " +String( (buff[0] << 8) + buff[1]));

  delay(1); 

 
}


void fuelGauge::printTempOffset()
{
  uint8_t buff[2];

  Serial.println("TEMPERATURE OFFSET");
  

  readMACOperation(0x400D, buff, 1);

  Serial.println("Int Temp Offse: " +String( (int8_t) buff[0]));

  delay(1);


  readMACOperation(0x400E, buff, 1);

  Serial.println("Ext Temp Offse: " +String( (int8_t) buff[0]));

  delay(1);


    readMACOperation(0x400F, buff, 1);

  Serial.println("Pack V Offset: " +String( (int8_t) buff[0]));

  delay(1);


    readMACOperation(0x41A7, buff, 2);

  Serial.println("Default temperature: " +String( (buff[0] << 8) + buff[1]));

  delay(1); 



  //Seting the defualt temperature
    writeWordToFlash(0x41A7, 2982, 0);


  delay(1);

  

  readMACOperation(0x41A7, buff, 2);

  Serial.println("Default temperature: " +String( (buff[0] << 8) + buff[1]));

  delay(1); 
  
}


void fuelGauge::setToDefaultInternalTempCoef()
{

  Serial.println("Set internal temperature coef to deafult");

  
  writeWordToFlash(0x41C1, 0, 0);


  delay(1);


  writeWordToFlash(0x41C3, 0, 0);


  delay(1);


  writeWordToFlash(0x41C5, -12324, 0);


  delay(1);


  writeWordToFlash(0x41C7, 6131, 0);


  delay(1);

  
  writeWordToFlash(0x41C9, 0, 0);


  delay(1);


  writeWordToFlash(0x41CB, 6131, 0);


  delay(1);

}


void fuelGauge::setToDefaultInternalVoltCompCoef()
{

  Serial.println("Set internal volt comp coef to deafult");

  
  writeWordToFlash(0x41D7, 0, 0);


  delay(1);


  writeWordToFlash(0x41D9, 14902, 0);
  

  delay(1);


  writeWordToFlash(0x41DB, -623, 0);


  delay(1);


  writeWordToFlash(0x41DD, 37, 0);


  delay(1); 


  writeWordToFlash(0x41DF, 48, 0);



  delay(1); 


  writeWordToFlash(0x41E0, 256, 0);

  delay(1); 
 
}


void fuelGauge::setccGainccDeltaToDefault()
{
  uint8_t data[4];
  
  //Now, we will write ccDelta and ccGain to the flash memory
  convertFPtoRawBytes(0.47095f, data);
  writeMACOperation(0x4000, data, 4);

  delay(1);

  convertFPtoRawBytes(5.677446e5f, data);
  writeMACOperation(0x4004, data, 4);
  
}


void fuelGauge::printAccessStatus()
{

  accessStatus accStatus;

  getAccessStatus(&accStatus);

  switch(accStatus)
  {

    case INVALID:
      Serial.println("Access status: INVALID");
      break;

    case FULL_ACCESS:
      Serial.println("Access status: FULL_ACCESS");
      break;

    case UNSEALED:
      Serial.println("Access status: UNSEALED");
      break;

    case SEALED:
      Serial.println("Access status: SEALED");
      break;

    default:
      break;

  }

}



////////////////// PRIVATE FUNCTIONS //////////////////

//Return the value of the bit CAL as 0 or 1, or -1 if there was some problem
int8_t fuelGauge::toggleCalibrationMode()
{

  fuelGauge::errCode retError;

  Wire.beginTransmission(I2C_ADDRESS_BQ34110PW);
  Wire.write(0x00);
  Wire.write(0x2D); //Little endian address: 0x002D
  Wire.write(0x00);
  retError = Wire.endTransmission();

  delay(100);

  if(retError != NO_ERROR)
    return -1;

  Wire.beginTransmission(I2C_ADDRESS_BQ34110PW);
  Wire.write(0x00);
  Wire.write(0x57); //Little endian address: 0x0057
  Wire.write(0x00);
  retError = Wire.endTransmission();  

  delay(100);

  if(retError != NO_ERROR)
    return -1;

  Wire.beginTransmission(I2C_ADDRESS_BQ34110PW);
  Wire.write(0x40);
  retError = Wire.endTransmission(false);

  delay(10);

  if(retError != NO_ERROR)
    return -1;

  Wire.requestFrom(I2C_ADDRESS_BQ34110PW, 2);
  
  if(Wire.available() != 2)
    return -1;

  delay(100);

  Wire.read();  //Reading and discard the low byte
  
  if(Wire.read() & 0x80)  //If CAL bit == 1 -> Success enterting calibration mode
    return 1;
  else
    return 0;
}


uint8_t fuelGauge::computeMACDataSum(uint16_t MACsubcommand, uint8_t dataBytes[], uint8_t dataLen)
{

  uint8_t checksum = 0;

  checksum = ((uint8_t)((MACsubcommand & 0xff00) >> 8)) + ((uint8_t)(MACsubcommand & 0x00ff));

  uint8_t i;
  for(i = 0; i < dataLen ; i++)
    checksum += dataBytes[i];

  //Calculamos el complemento a 1:
  checksum = ~checksum;
  
  return checksum;
}
