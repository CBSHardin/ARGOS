
#include <argosAPI.h>
#include <delegate.h>


void onInterruptTriggeredCallback()
{

  Serial.println("M1 Interrupt trigered!!");

}

void intM2()
{

  Serial.println("M2 Interrupt trigered!!");

}

void intM3()
{

  Serial.println("M3 Interrupt trigered!!");

}

void intM4()
{

  Serial.println("M4 Interrupt trigered!!");

}


void setup()
{

  Serial.begin(115200);
  delay(200);

  interrupts();

  Serial.println("Test external interrupts ");

  pinMode(11, INPUT);
  pinMode(12, INPUT);
  pinMode(10, INPUT);
  pinMode(13, INPUT);


  delegate<void> pcint5CallbackDelegate;

  delegate<void> pcint6CallbackDelegate;

  delegate<void> pcint7CallbackDelegate;

  delegate<void> pcint4CallbackDelegate;

  pcint5CallbackDelegate.setCallback<onInterruptTriggeredCallback>();

  pcint6CallbackDelegate.setCallback<intM2>();

  pcint4CallbackDelegate.setCallback<intM3>();

  pcint7CallbackDelegate.setCallback<intM4>();
/*
  Argos::GpioInterrupts.enable(Argos::GpioInterrupts.pcint5, pcint5CallbackDelegate, Argos::GpioInterrupts.ANY_EDGE);
  Argos::GpioInterrupts.enable(Argos::GpioInterrupts.pcint6, pcint6CallbackDelegate, Argos::GpioInterrupts.ANY_EDGE);
  Argos::GpioInterrupts.enable(Argos::GpioInterrupts.pcint7, pcint7CallbackDelegate, Argos::GpioInterrupts.ANY_EDGE);
  Argos::GpioInterrupts.enable(Argos::GpioInterrupts.pcint4, pcint4CallbackDelegate, Argos::GpioInterrupts.ANY_EDGE);
*/

  Argos::GpioInterrupts.enable(GpioPinToIntSig(11), pcint5CallbackDelegate, Argos::GpioInterrupts.ANY_EDGE);
  Argos::GpioInterrupts.enable(GpioPinToIntSig(12), pcint6CallbackDelegate, Argos::GpioInterrupts.ANY_EDGE);
  Argos::GpioInterrupts.enable(GpioPinToIntSig(13), pcint7CallbackDelegate, Argos::GpioInterrupts.ANY_EDGE);
  Argos::GpioInterrupts.enable(GpioPinToIntSig(10), pcint4CallbackDelegate, Argos::GpioInterrupts.ANY_EDGE);



  //Active encoders
  pinMode(38, OUTPUT);

  digitalWrite(38, LOW);

  //Activar motor 1
  Argos::PowerTrain.begin();

  Argos::PowerTrain.setDutyCycle(1, 130);



}



void loop()
{

}
