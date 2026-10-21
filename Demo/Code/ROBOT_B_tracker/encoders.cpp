////////////////////////////////////////////
//        encoders.cpp
////////////////////////////////////////////
// This file contains the implementation of all the functions related
// with the encoders control. This functions includes
// get RPM and get time beteewn encoder ticks. 

#include <Arduino.h>
#include <util/atomic.h> 
#include "encoders.h"

volatile uint8_t lastStatusPCINT0;

volatile uint8_t detectedSpinDir = 0; //This variable will contain the spin direction of all the encoders.
                                      //0 will be forward, 1 will be backward

volatile uint16_t timeBetweenEncTicksM1 = 0;  //Time between encoders ticks in miliseconds. This gives a maximum 
volatile uint16_t timeBetweenEncTicksM2 = 0;  //measurable speed of 3750rpm. Our motors can reach has maximum, 800rpm.
volatile uint16_t timeBetweenEncTicksM3 = 0;
volatile uint16_t timeBetweenEncTicksM4 = 0;

volatile uint32_t lastTimeStampM1;
volatile uint32_t lastTimeStampM2;
volatile uint32_t lastTimeStampM3;
volatile uint32_t lastTimeStampM4;

volatile int16_t encoderCountM1 = 0;  //This variable will count the ticks of the encoder if it goes forward
volatile int16_t encoderCountM2 = 0;  //and will discount the ticks if it goes backward.
volatile int16_t encoderCountM3 = 0;
volatile int16_t encoderCountM4 = 0;


volatile boolean M1watchdog = false;
volatile boolean M2watchdog = false;
volatile boolean M3watchdog = false;
volatile boolean M4watchdog = false;


void initEncoders()
{

  uint32_t currentTimeStamp;

  //Configuramos todos los pines de los encoders como entradas
  pinMode(M1_ENC_CH1, INPUT);
  pinMode(M1_ENC_CH2, INPUT);

  pinMode(M2_ENC_CH1, INPUT);
  pinMode(M2_ENC_CH2, INPUT);

  pinMode(M3_ENC_CH1, INPUT);
  pinMode(M3_ENC_CH2, INPUT);

  pinMode(M4_ENC_CH1, INPUT);
  pinMode(M4_ENC_CH2, INPUT);

  lastStatusPCINT0 =  (unsigned char) digitalRead(M3_ENC_CH1) | (unsigned char)digitalRead(M1_ENC_CH1) << 1 | (unsigned char)digitalRead(M2_ENC_CH1) << 2 | (unsigned char)digitalRead(M4_ENC_CH1) << 3;

  currentTimeStamp = millis();

  lastTimeStampM1 = currentTimeStamp;
  lastTimeStampM2 = currentTimeStamp;
  lastTimeStampM3 = currentTimeStamp;
  lastTimeStampM4 = currentTimeStamp;

  //Habilitamos las interrupciones de los pines PCINT4, PCINT5, PCINT6 y PCINT7 
  PCMSK0 = 0xf0;
  PCICR = 0x01;
  
}

double getRPMM1()
{
  
  //Critical section
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
  {
  /*  if(M1watchdog == false)
      return 0;
*/
    
    if(timeBetweenEncTicksM1 == 0)
      return 0;

    return (1000.0*60.0/(double)encoderResolution)/((double)timeBetweenEncTicksM1);
   
  }

}


double getRPMM2()
{
  
  //Critical section
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
  {
/*
   if(M2watchdog == false)
     return 0;
*/
   if(timeBetweenEncTicksM2 == 0)
      return 0;

    return (1000.0*60/(double)encoderResolution)/((double)timeBetweenEncTicksM2);
   
  }
  
}

double getRPMM3()
{
  
  //Critical section
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
  {
/*
    if(M3watchdog == false)
      return 0;
*/
    if(timeBetweenEncTicksM3 == 0)
      return 0;

    return (1000.0*60/(double)encoderResolution)/((double)timeBetweenEncTicksM3);
   
  }
 
}

double getRPMM4()
{
  
  //Critical section
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
  {
/*
     if(M4watchdog == false)
      return 0;
      */
    if(timeBetweenEncTicksM4 == 0)
      return 0;
      
    return (1000.0*60/(double)encoderResolution)/((double)timeBetweenEncTicksM4);
   
  }
 
}


double getSignedRPMM1()
{
  if(getSpinDirM1() == eforward)
    return getRPMM1();
    
  else
    return (getRPMM1()*-1);
}


double getSignedRPMM2()
{
  if(getSpinDirM2() == eforward)
    return getRPMM2();
    
  else
    return (getRPMM2()*-1);
}


double getSignedRPMM3()
{
  if(getSpinDirM3() == eforward)
    return getRPMM3();
    
  else
    return (getRPMM3()*-1);
}


double getSignedRPMM4()
{
  if(getSpinDirM4() == eforward)
    return getRPMM4();
    
  else
    return (getRPMM4()*-1);
}



uint32_t getTimeBetweenEncTicksM1()
{

  //Critical section
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
  {
    return timeBetweenEncTicksM1;
  }
  
}


uint32_t getTimeBetweenEncTicksM2()
{

  //Critical section
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
  {
    return timeBetweenEncTicksM2;
  }
  
}


uint32_t getTimeBetweenEncTicksM3()
{
  
  //Critical section
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
  {
    return timeBetweenEncTicksM3;
  }
  
}

uint32_t getTimeBetweenEncTicksM4()
{

  //Critical section
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
  {
    return timeBetweenEncTicksM4;
  }
  
}


encoderSpinDir getSpinDirM1()
{
  //Critical section
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
  {
 
    if(detectedSpinDir & 0x01)
      return ebackward;
      
    else
      return eforward;
      
  }
}


encoderSpinDir getSpinDirM2()
{
  //Critical section
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
  {
 
    if(detectedSpinDir & 0x02)
      return ebackward;
      
    else
      return eforward;
      
  }
}


encoderSpinDir getSpinDirM3()
{
  //Critical section
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
  {
 
    if(detectedSpinDir & 0x04)
      return ebackward;
      
    else
      return eforward;
      
  }
}

encoderSpinDir getSpinDirM4()
{
  //Critical section
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
  {
 
    if(detectedSpinDir & 0x08)
      return ebackward;
      
    else
      return eforward;
      
  }
}



void clearEncoderCountM1()
{
  //Critical section
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
  {

    encoderCountM1 = 0;
  
  }
}


void clearEncoderCountM2()
{
  //Critical section
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
  {

    encoderCountM2 = 0;
  
  }
}


void clearEncoderCountM3()
{
  //Critical section
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
  {

    encoderCountM3 = 0;
  
  }
}


void clearEncoderCountM4()
{
  //Critical section
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
  {

    encoderCountM4 = 0;
  
  }
}


int16_t getEncoderCountM1()
{
  //Critical section
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
  {

    return encoderCountM1;
  
  }
}


int16_t getEncoderCountM2()
{
  //Critical section
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
  {

    return encoderCountM2;
  
  }
}


int16_t getEncoderCountM3()
{
  //Critical section
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
  {

    return encoderCountM3;
  
  }
}


int16_t getEncoderCountM4()
{
  //Critical section
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
  {

    return encoderCountM4;
  
  }
}


//ISR del pin M1_ENC_CH1
void ISR_PCINT5()
{

  uint32_t currentTimeStamp;

  //If the CH1 of the encoder is in an open slot of the encoder:
  if(digitalRead(M1_ENC_CH1) == LOW)
  {
    //First, we measured the time 
    currentTimeStamp = millis();

    //Detect the spining dir
    if(digitalRead(M1_ENC_CH2) == HIGH)   //If the encoder is spining forward
    {
      
      detectedSpinDir &= 0xfE;
      
      encoderCountM1++;
      
    }
    else
    {
    
      detectedSpinDir |= 0x01 ;

      encoderCountM1--;

    }
    
    //Compute the time between ticks
    timeBetweenEncTicksM1 = currentTimeStamp - lastTimeStampM1;

    lastTimeStampM1 = currentTimeStamp;

    M1watchdog = true;
    
  }

}


//ISR del pin M2_ENC_CH1
void ISR_PCINT6()
{

  uint32_t currentTimeStamp;

  //If the CH1 of the encoder is in an open slot of the encoder:
  if(digitalRead(M2_ENC_CH1) == LOW)
  {
    //First, we measured the time 
    currentTimeStamp = millis();

    //Detect the spining dir
    if(digitalRead(M2_ENC_CH2) == LOW)   //If the encoder is spining forward
    {
      
      detectedSpinDir &= 0xfD;
      
      encoderCountM2++;
      
    }
    else
    {
    
      detectedSpinDir |= 0x02;

      encoderCountM2--;

    }
    
    //Compute the time between ticks
    timeBetweenEncTicksM2 = currentTimeStamp - lastTimeStampM2;

    lastTimeStampM2 = currentTimeStamp;
    
    M2watchdog = true;
    
  }
  
}


//ISR del pin M3_ENC_CH1
void ISR_PCINT4()
{
  uint32_t currentTimeStamp;

  //If the CH1 of the encoder is in an open slot of the encoder:
  if(digitalRead(M3_ENC_CH1) == LOW)
  {
    //First, we measured the time 
    currentTimeStamp = millis();

    //Detect the spining dir
    if(digitalRead(M3_ENC_CH2) == HIGH)   //If the encoder is spining forward
    {
      
      detectedSpinDir &= 0xfB;
      
      encoderCountM3++;
      
    }
    else
    {
    
      detectedSpinDir |= 0x04;

      encoderCountM3--;

    }
    
    //Compute the time between ticks
    timeBetweenEncTicksM3 = currentTimeStamp - lastTimeStampM3;

    lastTimeStampM3 = currentTimeStamp;

    M3watchdog = true;
    
  }
  
}


//ISR del pin M4_ENC_CH1
void ISR_PCINT7()
{

  uint32_t currentTimeStamp;

  //If the CH1 of the encoder is in an open slot of the encoder:
  if(digitalRead(M4_ENC_CH1) == LOW)
  {
    //First, we measured the time 
    currentTimeStamp = millis();

    //Detect the spining dir
    if(digitalRead(M4_ENC_CH2) == LOW)   //If the encoder is spining forward
    {
      
      detectedSpinDir &= 0xf7;
      
      encoderCountM4++;
      
    }
    else
    {
    
      detectedSpinDir |= 0x08;

      encoderCountM4--;

    }
    
    //Compute the time between ticks
    timeBetweenEncTicksM4 = currentTimeStamp - lastTimeStampM4;

    lastTimeStampM4 = currentTimeStamp;

    M4watchdog = true;
    
  }
  
}


//Macro para atenteder la interrupción de los puertos PCINT 0-7
ISR(PCINT0_vect)
{

  //Comprobamos que puerto produjo la interrupción
  unsigned char currentStatusPCINT0 = (unsigned char) digitalRead(M3_ENC_CH1) | (unsigned char)digitalRead(M1_ENC_CH1) << 1 | (unsigned char)digitalRead(M2_ENC_CH1) << 2 | (unsigned char)digitalRead(M4_ENC_CH1) << 3;

  unsigned char res = currentStatusPCINT0 ^ lastStatusPCINT0;

  //Serial.println("Interrupción PCINT generada, RESULTADO:");
  //Serial.println(res);

  //Atendemos la interrupcion generada por PCINT4 - M3_ENC_CH1
  if(res & 0x01)
    ISR_PCINT4();
    

  //Atendemos la interrupcion generada por PCINT5 - M1_ENC_CH1 
  if(res & 0x02)
    ISR_PCINT5();

  //Atendemos la interrupcion generada por PCINT6 - M2_ENC_CH1
  if(res & 0x04)
    ISR_PCINT6();

  //Atendemos la interrupcion generada por PCINT7 - M4_ENC_CH1  
  if(res & 0x08)
    ISR_PCINT7();

  //Guardamos el estado actual para detectar el pin que produjo que pin produjo el
  //cambio de estado
  lastStatusPCINT0 = currentStatusPCINT0;
  
}


void encodersWatchdog()
{
  //Critical section
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
  {

    if(M1watchdog == false)
      timeBetweenEncTicksM1 = 0;

    M1watchdog = false;


    if(M2watchdog == false)
      timeBetweenEncTicksM2 = 0;

    M2watchdog = false;

    if(M3watchdog == false)
      timeBetweenEncTicksM3 = 0;

    M3watchdog = false;
    
    if(M4watchdog == false)
      timeBetweenEncTicksM4 = 0;

    M4watchdog = false;
    
  }
  
}
