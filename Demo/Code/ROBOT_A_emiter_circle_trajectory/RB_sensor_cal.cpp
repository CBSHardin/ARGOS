////////////////////////////////////////////
//        RB_sensor.h
////////////////////////////////////////////
// This file contains the implementation of all 
// the functions related with the calibration of
// Range and Bearing sensor

#include <Arduino.h>
#include "RB_sensor.h"
#include <EEPROM.h>


extern LUT RBLookUpTable;

uint16_t LUTIndex;

extern compensationCoefficientsArray RBCompCoeff;

uint32_t sensorPD1RefMeasure;



void beginLookUpTableComputation()
{
  
   //Reset lookUpTable index
   LUTIndex = 0;
   RBLookUpTable.occupancy = 0;
   
}


void takeMeasuresAndComputeAverageFromNSamples(uint8_t numberOfSamples, uint32_t avgMeasures[8])
{

  uint64_t avgAcumulator = 0;
  uint16_t ADCValues[3];
  uint32_t measure;
  

  //Leemos los sensores en orden
  for(uint8_t i = 0; i < 8 ; i++)
  {

    for(uint8_t j = 0 ; j < numberOfSamples ; j++)
    {
      readOneSensorADCValues(i+1, ADCValues);

      computeOneSensorMeasure(ADCValues, &measure); //Combina las medidas y devuelve una medida equivalente

      avgAcumulator += measure;
    }      

    avgMeasures[i] = (uint32_t)(avgAcumulator / (float)numberOfSamples);

    avgAcumulator = 0;
             
  }

}


void takeMeasureAndComputeAverageFromNSamples(uint8_t numberOfSamples, uint8_t PDn, uint32_t *avgMeasure)
{

  uint64_t avgAcumulator = 0;
  uint16_t ADCValues[3];
  uint32_t measure;

  for(uint8_t j = 0 ; j < numberOfSamples ; j++)
  {
    readOneSensorADCValues(PDn, ADCValues);

    computeOneSensorMeasure(ADCValues, &measure); //Combina las medidas y devuelve una medida equivalente

    avgAcumulator += measure;
  }      

  *avgMeasure = (uint32_t)(avgAcumulator / (float)numberOfSamples);
  
}


void addSampleToLookUpTable(int32_t measures[8], float distance_in_m)
{

  //Calculamos las variables a y b
  float a = (measures[1] + measures[7] + 2*measures[0])/(2*cos(PI/4)+2);
  float b = (measures[1] - measures[7])/(2*sin(PI/4));

  float r = sqrt(pow(a, 2) + pow(b, 2) );
  

  RBLookUpTable.entries[LUTIndex].r = r;

  RBLookUpTable.entries[LUTIndex].distance_in_mm = (uint16_t)(distance_in_m*1000);

  if(LUTIndex < LOOK_UP_TABLE_MAX_SAMPLES)
  {
    LUTIndex++;
    RBLookUpTable.occupancy++;

  }
    
}


boolean isLookUpTableComplete()
{
  
  return LUTIndex == (LOOK_UP_TABLE_MAX_SAMPLES - 1);
  
}


void printLookUpTable()
{

  Serial.print("const lookUpTable[] = {");

  for(uint16_t i = 0; i < RBLookUpTable.occupancy ; i++)
  {
    Serial.print("\n {"+String(RBLookUpTable.entries[i].r)+", "+String(RBLookUpTable.entries[i].distance_in_mm)+"}");

    if(i != LUTIndex)
      Serial.print(", ");
  }

  Serial.print("\n};");
    
}


void saveLookUpTableInEEPROM(uint16_t address)
{

  RBLookUpTable.magicNumber = 0xCC;

  EEPROM.put(address, RBLookUpTable);
  
}


void loadLookUpTableFromEEPROM(uint16_t address)
{

  EEPROM.get(address, RBLookUpTable);
  
}


boolean isLookUpTablePresentInEEPROM(uint16_t address)
{

  uint8_t var;
  
  var = EEPROM.read(address);
  
  return var == 0xCC;
  
}


void addSampleForSensitivityCompensation(uint16_t measure, uint8_t PDn)
{

  if(PDn == 1)
    sensorPD1RefMeasure = measure;
    
  else
    RBCompCoeff.coeff[PDn-2] = (float)measure / (float)sensorPD1RefMeasure;
   
}


void printSensitivityCompensationCoef()
{

  Serial.print("const compCoeff[] = {");

  for(uint16_t i = 0; i <= 8 ; i++)
  {
    Serial.print(String(RBCompCoeff.coeff[i]));

    if(i != 7)
      Serial.print(", ");
  }

  Serial.print("};");
  
}




void compensateMeasure(uint32_t* measure, uint8_t PDn)
{
  
  if( PDn != 1)
    *measure = (uint32_t) (RBCompCoeff.coeff[PDn-2] * (*measure) );
      
}     
      
      
void compensateMeasures(uint32_t measures[])
{
   
  for(uint8_t i = 0; i < 7 ; i++)
    measures[i] = (uint32_t) (RBCompCoeff.coeff[i]*measures[i]);

}




void saveSensitivityCompensationCoefToEEPROM(uint16_t address)
{
  
  RBLookUpTable.magicNumber = 0xCA;
  
  EEPROM.put(address, RBCompCoeff);
  
}

void loadSensitivityCompensationCoefFromEEPROM(uint16_t address)
{

  EEPROM.get(address, RBCompCoeff);
 
}


boolean isCompensationCoeffArrayInEEPROM(uint16_t address)
{
  
  uint8_t var;
  
  var = EEPROM.read(address);
  
  return var == 0xCA;
  
}
