/*
   SimpleSender.cpp

    Demonstrates sending IR codes in standard format with address and command
    An extended example for sending can be found as SendDemo.

    Copyright (C) 2020-2021  Armin Joachimsmeyer
    armin.joachimsmeyer@gmail.com

    This file is part of Arduino-IRremote https://github.com/Arduino-IRremote/Arduino-IRremote.

    MIT License
*/
#include <Arduino.h>

/*
   Define macros for input and output pin etc.
*/
#include "PinDefinitionsAndMore2.h"

//#define SEND_PWM_BY_TIMER
//#define USE_NO_SEND_PWM

#include <IRremote.h>

float duration, distance;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, INPUT);
  Serial.begin(115200);

  // Just to know which program is running on my Arduino
  Serial.println(F("START " __FILE__ " from " __DATE__ "\r\nUsing library version " VERSION_IRREMOTE));

  /*
     The IR library setup. That's all!
  */
  IrSender.begin(2, ENABLE_LED_FEEDBACK); // Specify send pin and enable feedback LED at default feedback LED pin

  Serial.print(F("Ready to send IR signals at pin "));
  Serial.println(2);
}

/*
   Set up the data to be sent.
   For most protocols, the data is build up with a constant 8 (or 16 byte) address
   and a variable 8 bit command.
   There are exceptions like Sony and Denon, which have 5 bit address.
*/
uint16_t sAddress = 0x0102;
uint8_t sCommand = 0x34;
uint8_t sRepeats = 0;

int commandChanges = 0;
int commandState = 0;

void loop() {

  int commandState = commandChanges % 2;

  if (commandState == 0) {
    sCommand = 0x34;
  } else if (commandState == 1) {
    sCommand = 0x43;
  }

  Serial.println();
  Serial.print(F("Send now: address=0x"));
  Serial.print(sAddress, HEX);
  Serial.print(F(" command1=0x"));
  Serial.print(sCommand, HEX);
  Serial.print(F(" repeats="));
  Serial.print(sRepeats);
  Serial.println();

  Serial.println(F("Send NEC with 16 bit address"));
  Serial.flush();

  // Results for the first loop to: Protocol=NEC Address=0x102 Command=0x34 Raw-Data=0xCB340102 (32 bits)
  IrSender.sendNEC(sAddress, sCommand, sRepeats);

  /*
     If you cannot avoid to send a raw value directly like e.g. 0xCB340102 you must use sendNECRaw()
  */
  //    Serial.println(F("Send NECRaw 0xCB340102"));
  //    IrSender.sendNECRaw(0xCB340102, sRepeats);

  sRepeats++;
  // clip repeats at 4
  if (sRepeats > 4) {
    sRepeats = 4;
  }

  delay(10);  // delay must be greater than 5 ms (RECORD_GAP_MICROS), otherwise the receiver sees it as one long signal

  // Ultrasonic Sensor: Change in distance (i.e. holding hand in front of sensor) switches transmitting command.

  digitalWrite(8, LOW);
  delayMicroseconds(2);
  digitalWrite(8, HIGH);
  delayMicroseconds(10);
  digitalWrite(8, LOW);

  duration = pulseIn(9, HIGH);
  distance = (duration * .0343) / 2;
  Serial.print("Distance: ");
  Serial.println(distance);
  delay(50);

  Serial.println(commandChanges);
  Serial.println(commandState);

  if (round(distance) < 50) {
    commandChanges++;
    delay(1000);
  }

}
