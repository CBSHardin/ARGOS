////////////////////////////////////////////
//        encoders.h
////////////////////////////////////////////
// This file contains all the functions related
// with the encoders control. This functions includes
// get RPM and get time beteewn encoder ticks. 

#ifndef __ENCODERS_H_
#define __ENCODERS_H_

//Encoders pins
#define M1_ENC_CH1        11  //PCINT5
#define M1_ENC_CH2        26 

#define M2_ENC_CH1        12  //PCINT6
#define M2_ENC_CH2        27

#define M3_ENC_CH1        10  //PCINT4
#define M3_ENC_CH2        28

#define M4_ENC_CH1        13  //PCINT7
#define M4_ENC_CH2        29


//Encoder parameters
#define encoderResolution 25 //Number of slots per rotation



typedef enum {eforward, ebackward} encoderSpinDir;

void initEncoders();

double getRPMM1();

double getRPMM2();

double getRPMM3();

double getRPMM4();


double getSignedRPMM1();

double getSignedRPMM2();

double getSignedRPMM3();

double getSignedRPMM4();


uint32_t getTimeBetweenEncTicksM1();

uint32_t getTimeBetweenEncTicksM2();

uint32_t getTimeBetweenEncTicksM3();

uint32_t getTimeBetweenEncTicksM4();


encoderSpinDir getSpinDirM1();

encoderSpinDir getSpinDirM2();

encoderSpinDir getSpinDirM3();

encoderSpinDir getSpinDirM4();


void clearEncoderCountM1();

void clearEncoderCountM2();

void clearEncoderCountM3();

void clearEncoderCountM4();


int16_t getEncoderCountM1();

int16_t getEncoderCountM2();

int16_t getEncoderCountM3();

int16_t getEncoderCountM4();


void encodersWatchdog();

#endif
