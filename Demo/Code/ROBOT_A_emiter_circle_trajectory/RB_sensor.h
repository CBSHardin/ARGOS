////////////////////////////////////////////
//        RB_sensor.h
////////////////////////////////////////////
// This file contains all the functions related
// with Range and Bearing sensor
// It principles are based on the algorithm 
// developed by Pugh et al. More info in the
// project book.

#ifndef __RB_SENSOR_H_
#define __RB_SENSOR_H_

//Range and Bearing sensor pins

//TX module:
#define RB_SENSOR_TX_ENABLE     35

#define RB_SENSOR_TX_SINGLE_CRTL_ENABLE 30
#define RB_SENSOR_TX_A0         33
#define RB_SENSOR_TX_A1         32
#define RB_SENSOR_TX_A2         31

#define RB_SENSOR_TX_ALL        44

#define RB_SENSOR_TX_EXC_INPUT  5


//RX module:
#define RB_SENSOR_RX_ENABLE     34 

#define RB_SENSOR_RX_MUX_E      46
#define RB_SENSOR_RX_A0         49
#define RB_SENSOR_RX_A1         48
#define RB_SENSOR_RX_A2         47

#define RB_SENSOR_RX_RESET      45

#define RB_SENSOR_RX_ADC_1      A0
#define RB_SENSOR_RX_ADC_2      A1
#define RB_SENSOR_RX_ADC_3      A2

////////////// RX module params ////////////
//Ganancias
#define G0 421.19f
#define G1 42.93f
#define G2 20.0f
#define G3 G2
#define H0 1.07976f

//Parametros de los sensores
#define Ie 0.05F
#define k 0.000008f

//ADC
#define LSB 0.002f
#define Vref = 2.039f

#define radiusOffset 0.088f

typedef float vector2D[2];
typedef uint16_t sensorsValuesMatrix[8][3];

//Posiciones de los sensores
const vector2D PDcoordinates[]= {{0, 0.0482f},  {0.034082546f, 0.034082546f}, {0.0482f, 0}, {0.034082546f, -0.034082546f},
                                         {0, -0.0482f}, {-0.034082546f, -0.034082546f}, {-0.0482f, 0}, {-0.034082546f, 0.034082546f}};

#define LOOK_UP_TABLE_MAX_SAMPLES 400

typedef struct
{
  float r;
  uint16_t distance_in_mm;
}
lookUpTableEntry;

typedef struct
{
  uint8_t magicNumber;
  
  uint16_t occupancy;
  
  lookUpTableEntry entries[LOOK_UP_TABLE_MAX_SAMPLES];
}
LUT;


typedef struct
{
  uint8_t magicNumber;
  
  float coeff[8];
}
compensationCoefficientsArray;


////////////// RX module functions ///////////////////
void init_R_B_sensor_rx();

void setPowerRB_sensor_rx(boolean power);

void selectPhotodiode(unsigned char PDn);

void readSensorsADCValues(sensorsValuesMatrix ADCValues);

boolean readSensorsADCValuesNonBlocking(sensorsValuesMatrix ADCValues);   //Devuelve si la medida ha sido realizada

void readOneSensorADCValues(uint8_t sensor, uint16_t ADCValues[3]);

void computeOneSensorMeasure(uint16_t ADCValues[3], uint32_t *lastRangeEquivalentValue); //Combina las medidas y devuelve una medida equivalente
                                                                                  //en el rango 3
void computeSensorMeasures(sensorsValuesMatrix ADCValues, uint32_t lastRangeEquivalentValues[8]);


boolean computeDistanceAndBearing(int32_t sensorMeasures[8], float *distance_in_m, float *angle_in_rad);

boolean computePossition(int32_t sensorMeasures[8], float *x, float *y);


boolean LUTlocateInterpolationInterval(float r, float *r0, uint16_t *d0, float *r1, uint16_t *d1);


//float computeDistance(uint16_t adcCount);

//void computePositionFromTwoSensors(vector2D s1Pos, vector2D s2Pos, float r1, float r2, float *x, float *y);

//void computePositionFromOneSensors(vector2D s1Pos, float r1, float *x, float *y);

//boolean getPosition(sensorsValuesMatrix values, float *x, float *y);



//// Calibration functions
void beginLookUpTableComputation();

void takeMeasuresAndComputeAverageFromNSamples(uint8_t numberOfSamples, uint32_t avgMeasures[8]);

void takeMeasureAndComputeAverageFromNSamples(uint8_t numberOfSamples, uint8_t PDn, uint32_t *avgMeasure);

void addSampleToLookUpTable(int32_t measures[8], float distance_in_m);

boolean isLookUpTableComplete();

void printLookUpTable();

void saveLookUpTableInEEPROM(uint16_t address);

void loadLookUpTableFromEEPROM(uint16_t address);

//void loadLookUpTableFromRAM();

boolean isLookUpTablePresentInEEPROM(uint16_t address);



void addSampleForSensitivityCompensation(uint32_t measure, uint8_t PDn);

void printSensitivityCompensationCoef();

void compensateMeasure(uint32_t measure, uint8_t PDn);

void compensateMeasures(uint32_t measure[8]);

void saveSensitivityCompensationCoefToEEPROM(uint16_t address);

void loadSensitivityCompensationCoefFromEEPROM(uint16_t address);

boolean isCompensationCoeffArrayInEEPROM(uint16_t address);


////////////// TX module functions ///////////////////

void init_R_B_sensor_tx();

void setPowerRB_sensor_tx(boolean power);

void startExcitation();
void stopExcitation();

void setEmiterSector(unsigned char sector);
void turnOnSector();
void turnOffSector();

void turnOnAllEmiters();
void turnOffAllEmiters();


/////////////////////// LIDAR functions /////////////////////

void scanAllSectorsLIDAR(sensorsValuesMatrix ADCValues);
void scanOneSectorLIDAR(uint16_t ADCCounts[3], uint8_t sector);

void getDistanceToObstacleAllSectorsLIDAR(sensorsValuesMatrix ADCValues, float *distances, float reflectivityCoef);
float getDistanceToObstacleOneSectorsLIDAR(uint16_t ADCCounts[3], float reflectionCoef);

float calibrateReflectionCoefficient(uint16_t ADCvalue, float calibrationDistance);

#endif
