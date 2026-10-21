////////////////////////////////////////////
//        RB_sensor.cpp
////////////////////////////////////////////
// This file contains the implementation of all 
// the functions related with Range and Bearing sensor

#include <Arduino.h>
#include "RB_sensor.h"

LUT RBLookUpTable;

compensationCoefficientsArray RBCompCoeff;

//Variables for nonBlocking sensor read
uint8_t sensorIndex = 0;
long sensorTimeStamp;
typedef enum {waiting, stage_1, stage_2, stage_3, stage_4} RBSensorReadingStates;

RBSensorReadingStates RBSensorReadingState = stage_1;


void init_R_B_sensor_rx()
{
  
  pinMode(RB_SENSOR_RX_ENABLE, OUTPUT);

  pinMode(RB_SENSOR_RX_RESET, OUTPUT);
  pinMode(RB_SENSOR_RX_MUX_E, OUTPUT);

  pinMode(RB_SENSOR_RX_ADC_1, INPUT);
  pinMode(RB_SENSOR_RX_ADC_2, INPUT);
  pinMode(RB_SENSOR_RX_ADC_3, INPUT);

  pinMode(RB_SENSOR_RX_A0, OUTPUT);  
  pinMode(RB_SENSOR_RX_A1, OUTPUT);  
  pinMode(RB_SENSOR_RX_A2, OUTPUT);  

  digitalWrite(RB_SENSOR_RX_RESET, LOW);
  
  digitalWrite(RB_SENSOR_RX_MUX_E, HIGH); 

  //Configuramos la referencia del ADC como externa
  analogReference(EXTERNAL);
  
}


void setPowerRB_sensor_rx(boolean power)
{
  if(power)
    digitalWrite(RB_SENSOR_RX_ENABLE, HIGH);
  else
    digitalWrite(RB_SENSOR_RX_ENABLE, LOW);
}


//Selector fotodiodos; 1, 2, 3, 4, 5, 6, 7, 8
void selectPhotodiode(unsigned char PDn)
{

  PDn--;

  //Configuramos las salidas del decodificador para activar los sectores
  if(PDn & 0x01)
    digitalWrite(RB_SENSOR_RX_A0, HIGH);
  else
    digitalWrite(RB_SENSOR_RX_A0, LOW);

  if(PDn & 0x02)
    digitalWrite(RB_SENSOR_RX_A1, HIGH);
  else
    digitalWrite(RB_SENSOR_RX_A1, LOW);

  if(PDn & 0x04)
    digitalWrite(RB_SENSOR_RX_A2, HIGH);
  else
    digitalWrite(RB_SENSOR_RX_A2, LOW);

}


void readSensorsADCValues(sensorsValuesMatrix ADCValues)
{

  for(uint8_t i = 1 ; i <= 8; i++)
  {

    selectPhotodiode(i);
    
    delayMicroseconds(100);

    digitalWrite(RB_SENSOR_RX_RESET, HIGH);
    delayMicroseconds(500);
    digitalWrite(RB_SENSOR_RX_RESET, LOW);
    
    //Esperamos a que el detector de pico alcance el valor máximo de la señal
    delay(2);

    ADCValues[i-1][0] = (uint16_t) analogRead(RB_SENSOR_RX_ADC_1);
    ADCValues[i-1][1] = (uint16_t) analogRead(RB_SENSOR_RX_ADC_2);
    ADCValues[i-1][2] = (uint16_t) analogRead(RB_SENSOR_RX_ADC_3);
    
  }
  
}


boolean readSensorsADCValuesNonBlocking(sensorsValuesMatrix ADCValues)
{
  switch(RBSensorReadingState)
  {

    //Selecionamos el fotodiodo correspondiente, y fijamos la primera timestamp
    case waiting:
      sensorIndex = 1;
      selectPhotodiode(sensorIndex);

      RBSensorReadingState = stage_1;

      sensorTimeStamp = micros();

      break;

    //Esperamos a que hallan pasado 100us
    case stage_1:
      if(micros() - sensorTimeStamp >= 100)
      {
        digitalWrite(RB_SENSOR_RX_RESET, HIGH);

        RBSensorReadingState = stage_2;

        sensorTimeStamp = micros();
      }
      
      break;


    //Una vez hayan pasado 500 micros, bajamos la señal de reset
    //y pasamos a espera 2ms
      
    case stage_2:
      if(micros() - sensorTimeStamp >= 500)
      {
        digitalWrite(RB_SENSOR_RX_RESET, LOW);
      
        RBSensorReadingState = stage_3;
      
        sensorTimeStamp = micros();
      }
      
      break;

    //Leemos los valores y si el proceso a terminado lo indicamos, sino
    //pasamos al estado 2
    case stage_3:
      if(micros() - sensorTimeStamp >= 2000)
      {
        
        ADCValues[sensorIndex-1][0] = (uint16_t) analogRead(RB_SENSOR_RX_ADC_1);
        ADCValues[sensorIndex-1][1] = (uint16_t) analogRead(RB_SENSOR_RX_ADC_2);
        ADCValues[sensorIndex-1][2] = (uint16_t) analogRead(RB_SENSOR_RX_ADC_3);

        if(sensorIndex == 8)
        {
          
          RBSensorReadingState = waiting;
          
          return true;
        }

        sensorIndex++;

        selectPhotodiode(sensorIndex);

        RBSensorReadingState = stage_1;
        
        sensorTimeStamp = micros();
      }
      
      break;

      return false;
    
  }
  
}



void readOneSensorADCValues(uint8_t sensor, uint16_t ADCValues[3])
{

  selectPhotodiode(sensor);
  
  delayMicroseconds(100);
  
  digitalWrite(RB_SENSOR_RX_RESET, HIGH);
  delayMicroseconds(500);
  digitalWrite(RB_SENSOR_RX_RESET, LOW);
  
  //Esperamos a que el detector de pico alcance el valor máximo de la señal
  delay(2);
  
  ADCValues[0] = (uint16_t) analogRead(RB_SENSOR_RX_ADC_1);
  ADCValues[1] = (uint16_t) analogRead(RB_SENSOR_RX_ADC_2);
  ADCValues[2] = (uint16_t) analogRead(RB_SENSOR_RX_ADC_3);
  
}


void computeOneSensorMeasure(uint16_t ADCValues[3], uint32_t *lastRangeEquivalentValue)
{

  if(ADCValues[0] >= 50)
    *lastRangeEquivalentValue =  (uint32_t)long( ((float)ADCValues[0])*G2*G3);
    
  else if(ADCValues[1] >= 50)
    *lastRangeEquivalentValue = (uint32_t)long( ((float)ADCValues[1])*G3);

  else
    *lastRangeEquivalentValue = (uint32_t)ADCValues[2];
  
}


void computeSensorMeasures(sensorsValuesMatrix ADCValues, uint32_t lastRangeEquivalentValues[8])
{

  for(uint8_t i = 0; i < 8 ; i++)
    computeOneSensorMeasure(ADCValues[i], &lastRangeEquivalentValues[i]);

}


boolean computeDistanceAndBearing(int32_t sensorMeasures[8], float *distance_in_m, float *angle_in_rad)
{

  uint8_t PDnMax = 1;

  //Primero, buscamos que recibe mayor intensidad
  for(uint8_t i = 1; i<=8 ; i++)
  {
    if(sensorMeasures[(i-1)] > sensorMeasures[(PDnMax-1)] )
      PDnMax = i;
  }

  //Obtenemos los valores de r0, r1, r-1 del algoritmo
  int32_t rZero = sensorMeasures[PDnMax-1];

  int32_t rMinusOne;
  
  if(PDnMax == 1)
    rMinusOne =  sensorMeasures[7];
  else
    rMinusOne =  sensorMeasures[PDnMax-2];

  int32_t rOne;
  
  if(PDnMax == 8)
    rOne = sensorMeasures[0];
  else
    rOne = sensorMeasures[PDnMax];


  //Calculamos las variables a y b
  float a = (rOne + rMinusOne + 2*rZero)/(2*cos(PI/4)+2);
  float b = (rOne - rMinusOne)/(2*sin(PI/4));

  *angle_in_rad = atan(b/a) + PI/4*(PDnMax-1);

  float r = sqrt(pow(a, 2) + pow(b, 2) );

  #ifdef debug
    Serial.println("r: "+String(r));
  #endif

  float r0, r1;
  int16_t d0, d1;

  //Buscamos en la LUT
  if(LUTlocateInterpolationInterval(r, &r0, &d0, &r1, &d1))
  {

    *distance_in_m = ((r-r0)*(d1-d0)/(r1-r0)+d0)/1000;  //Interpolación lineal
    
    return true;
    
  }
  else
    return false; //No esta en la LUT, por lo que esta fuera de rango
  
}



boolean computePossition(int32_t sensorMeasures[8], float *x, float *y)
{

  float distance_in_m, angle_in_rad;

  if(computeDistanceAndBearing(sensorMeasures, &distance_in_m, &angle_in_rad))
  {

    *x = distance_in_m*cos(angle_in_rad);
  
    *y = distance_in_m*sin(angle_in_rad);

    return true;

  }
  else
    return  false;
    
}


boolean LUTlocateInterpolationInterval(float r, float *r0, uint16_t *d0, float *r1, uint16_t *d1)
{

  int16_t indexIntervalLowValue = -1;
  
  int16_t indexIntervalHighValue = -1;
  

  for(uint16_t i = 0; i < RBLookUpTable.occupancy ; i++)
  {

    if( RBLookUpTable.entries[i].r <=  r )
    {
      if(indexIntervalLowValue == -1)
        indexIntervalLowValue = i;
      else if(RBLookUpTable.entries[i].r > RBLookUpTable.entries[indexIntervalLowValue].r)
        indexIntervalLowValue = i;
      
    }
    else
    {
       if(indexIntervalHighValue == -1)
        indexIntervalHighValue = i;
        
       else if(RBLookUpTable.entries[i].r < RBLookUpTable.entries[indexIntervalHighValue].r)
        indexIntervalHighValue = i;
      
    }
    
  }

  #ifdef debug
    Serial.println("indexIntervalLowValue: "+String(indexIntervalLowValue));
    Serial.println("indexIntervalHighValue: "+String(indexIntervalHighValue));
  #endif
  
  if(indexIntervalLowValue == -1 || indexIntervalHighValue == -1)
    return false;

  *r0 = RBLookUpTable.entries[indexIntervalLowValue].r;

  *r1 = RBLookUpTable.entries[indexIntervalHighValue].r;

  *d0 = RBLookUpTable.entries[indexIntervalLowValue].distance_in_mm;

  *d1 = RBLookUpTable.entries[indexIntervalHighValue].distance_in_mm;

  return true;
  
}


/*

float getDistance(sensorsValuesMatrix ADCValues)
{

  uint8_t PDn = 1;

  //Primero, buscamos que recibe mayor intensidad
  for(uint8_t i = 1; i<=8 ; i++)
  {
    if(ADCValues[(i-1)][1] > ADCValues[(PDn-1)][1])
      PDn = i;
  }

  //Calculamos la distancia
  return computeDistance(ADCValues[PDn--][1]);
 
}


float getAngle(sensorsValuesMatrix ADCValues)
{

  uint8_t PDn = 1;

  //Primero, buscamos que recibe mayor intensidad
  for(uint8_t i = 1; i<=8 ; i++)
  {
    if(ADCValues[(i-1)][1] > ADCValues[(PDn-1)][1])
      PDn = i;
  }

  return 45*(PDn-1);
  
}


//Computa la distancia y la devuelve en metros
float computeDistance(uint16_t adcCount)
{

  return sqrt((Ie*k*G0*G1*G2*H0)/(((float)adcCount)*LSB));
  
}


void computePositionFromTwoSensors(vector2D s1Pos, vector2D s2Pos, float r1, float r2, float *x, float *y)
{

  float a = (r1*r1 - r2*r2 + s2Pos[0]*s2Pos[0] - s1Pos[0]*s1Pos[0] + s2Pos[1]*s2Pos[1] - s1Pos[1]*s1Pos[1])/(2*(s2Pos[1]-s1Pos[1]));
  float b = (s2Pos[0]-s1Pos[0])/(s2Pos[1]-s1Pos[1]);

  float d = (1 + b*b);
  float f = 2*(s1Pos[0] +b*(-s1Pos[1]+a));
  float g = s1Pos[0]*s1Pos[0] + s1Pos[1]*s1Pos[1] + a*a -r1*r1-2*a*s1Pos[1];

  *x = (f + sqrt(f*f - 4*d*g))/(2*d);
  *y = a -b*(*x);

}


void computePositionFromOneSensors(vector2D s1Pos, float r1, float *x, float *y)
{

  float m = s1Pos[1]/s1Pos[0];

  *x = s1Pos[0] + r1/sqrt(1+m*m);

  *y = (*x)*m;
  
}

boolean getPosition(sensorsValuesMatrix values, float *x, float *y)
{

  uint8_t PDnMax = 0;
  uint8_t PDnSecond = 0;

  uint16_t PDnMaxValue = 0;
  uint16_t PDnSecondValue = 0;

  float r1, r2;

  //Primero, buscamos que recibe mayor intensidad
  for(uint8_t i = 0; i<8 ; i++)
  {
    if(values[i][1] >= PDnMaxValue)
    {
      PDnSecondValue = PDnMaxValue;
      PDnMaxValue = values[i][1];
      PDnMax = i;
    }
    else if(values[i][1] >= PDnSecondValue )
    {
      PDnSecondValue = values[i][1];
      PDnSecond = i;
    }

  }

  
  //Si el valor máximo leido por el ADC es menor que el threshold de 50, pasamos
  if(PDnMaxValue < 50)
    return false;

  r1 = computeDistance(values[PDnMax][1]);

  r2 = computeDistance(values[PDnSecond][1]);

  //Calculamos la distancia utilizando dos sensores
  computePositionFromTwoSensors(PDcoordinates[PDnMax], PDcoordinates[PDnSecond], r1, r2,  x, y);

  if(*x != NAN)
    return true;

  //Si la posición no se puede computar, probablemente alguno de los sensores esta parcialmente cubierto
  //Computamos la posición usando un solo sensor:
   computePositionFromOneSensors(PDcoordinates[PDnMax], r1, x, y);

  return true;
}

*/

/*
#define f_seg 0.067158f
#define PD_r 0.0482f
#define ro 45*PI/180

//Calculo alternativo de las coordenadas usando trigonometria
float getPositionPolarCoords(sensorsValuesMatrix values, float *r, float *angle)
{

  float r1 = computeDistance(values[0][1]);
  float r2 = computeDistance(values[1][1]);

  float epsilon = acos((r1*r1+f_seg*f_seg-r2*r2)/(2*r1*f_seg));

  *r = sqrt(r1*r1+PD_r*PD_r-2*r1*PD_r*cos(ro+epsilon));

  float aux = ( (r1*sin(epsilon+ro))/ *r) ;

  *angle = asin(aux);

}
*/

void init_R_B_sensor_tx()
{
  
  pinMode(RB_SENSOR_TX_ENABLE, OUTPUT);

  pinMode(RB_SENSOR_TX_A0, OUTPUT);
  pinMode(RB_SENSOR_TX_A1, OUTPUT);
  pinMode(RB_SENSOR_TX_A2, OUTPUT);
  pinMode(RB_SENSOR_TX_SINGLE_CRTL_ENABLE, OUTPUT);
  
  pinMode(RB_SENSOR_TX_ALL, OUTPUT);

  //Configuramos el TIMER3 para generar una señal cuadrada de 100kHz
  pinMode(RB_SENSOR_TX_EXC_INPUT, OUTPUT);

  unsigned char PRR1_tmp = PRR1;

  PRR1 = PRR1_tmp & (0x08 ^ 0xff);

  //Configuramos el TIMER3 para operar en modo CTC
  TCCR3A = 0x40;
  TCCR3B = 0x08;
  
  OCR3AH = (799 & 0xff00) >> 8;
  OCR3AL = 799 & 0x00ff;
  
}



void setPowerRB_sensor_tx(boolean power)
{

  digitalWrite(RB_SENSOR_TX_ENABLE, power);
   
}

void startExcitation()
{

  //Arrancamos el timer
  TCCR3B |= 0x01;

}

void stopExcitation()
{
  
  //Detenemos el TIMER 3
  TCCR3B &= 0xFE;
  
}


void setEmiterSector(unsigned char sector)
{

  //Los sectores de se numeran empezando por el 1
  sector--;
  
  //Configuramos las salidas del decodificador para activar los sectores
  if(sector & 0x01)
    digitalWrite(RB_SENSOR_TX_A0, HIGH);
  else
    digitalWrite(RB_SENSOR_TX_A0, LOW);

  if(sector & 0x02)
    digitalWrite(RB_SENSOR_TX_A1, HIGH);
  else
    digitalWrite(RB_SENSOR_TX_A1, LOW);

  if(sector & 0x04)
    digitalWrite(RB_SENSOR_TX_A2, HIGH);
  else
    digitalWrite(RB_SENSOR_TX_A2, LOW);

}


void turnOnSector()
{

  digitalWrite(RB_SENSOR_TX_SINGLE_CRTL_ENABLE, HIGH);
  
}


void turnOffSector()
{

  digitalWrite(RB_SENSOR_TX_SINGLE_CRTL_ENABLE, LOW);
  
}


void turnOnAllEmiters()
{

  digitalWrite(RB_SENSOR_TX_ALL, HIGH);
  
}


void turnOffAllEmiters()
{

  digitalWrite(RB_SENSOR_TX_ALL, LOW);
  
}


void scanOneSectorLIDAR(uint16_t ADCCounts[3], uint8_t sector)
{

  uint16_t value;

  setEmiterSector(sector);

  selectPhotodiode(sector);

  turnOnSector();

  delayMicroseconds(100);

  digitalWrite(RB_SENSOR_RX_RESET, HIGH);
  delayMicroseconds(500);
  digitalWrite(RB_SENSOR_RX_RESET, LOW);
  
  //Esperamos a que el detector de pico alcance el valor máximo de la señal
  delay(2);

  ADCCounts[0] = (uint16_t) analogRead(RB_SENSOR_RX_ADC_1);
  ADCCounts[1] = (uint16_t) analogRead(RB_SENSOR_RX_ADC_2);
  ADCCounts[2] = (uint16_t) analogRead(RB_SENSOR_RX_ADC_3);

  turnOffSector();

}


void scanAllSectorsLIDAR(sensorsValuesMatrix ADCValues)
{
  
  for(uint8_t i = 1; i<=8 ; i++)
    scanOneSectorLIDAR(ADCValues[i-1], i);
  
}


float getDistanceToObstacleOneSectorsLIDAR(uint16_t ADCCounts[3], float reflectionCoef)
{

    return sqrt((Ie*reflectionCoef*k*G0*G1*G2*H0)/(((float)ADCCounts[1])*LSB));

}


void getDistanceToObstacleAllSectorsLIDAR(sensorsValuesMatrix ADCValues, float *distances, float reflectionCoef)
{

  for(uint8_t i = 1 ; i <= 8 ; i++)
    distances[i-1] = getDistanceToObstacleOneSectorsLIDAR(ADCValues[i-1], reflectionCoef);
  
}


float calibrateReflectionCoefficient(uint16_t ADCvalue, float calibrationDistance)
{

  return ( pow(calibrationDistance, 2)*(float)ADCvalue*LSB/(Ie*k*G0*G1*G2*H0) );
}
