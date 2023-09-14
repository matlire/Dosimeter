/*
Dozimeter arduino project

First you need to install these libraries from arduino library manager:
- ClimateGuard RadSens
- Easy Nextion Library
- GyverButton
*/

// Include needed libraries
#include <Arduino.h>
#include <Wire.h>
#include "CG_RadSens.h" 
#include "EasyNextionLibrary.h"
#include "GyverButton.h"
#include <SPI.h>

// Define buzzer pin, radSens, nextion display and button
#define BUZZ_PIN 9

CG_RadSens radSens(RS_DEFAULT_I2C_ADDRESS); 
EasyNex nextion(Serial);
GButton btn(10);

// One-time code
void setup() {
  // Begin serial & i2c
  Serial.begin(115200);
  Wire.begin();
  delay(1000);

  // Init radSens
  while(!radSens.init())
  {
    Serial.println("Sensor wiring error!");
    delay(1000);
  }

  uint8_t sensorChipId = radSens.getChipId();

  Serial.print("Chip id: 0x");
  Serial.println(sensorChipId, HEX);

  uint8_t firmWareVer = radSens.getFirmwareVersion();

  Serial.print("Firmware  r version: ");
  Serial.println(firmWareVer);

  Serial.println("-------------------------------------");
  Serial.println("Set Sensitivity example:\n");

  uint16_t sensitivity = radSens.getSensitivity();

  Serial.print("\t getSensitivity(): ");
  Serial.println(sensitivity);
  Serial.println("\t setSensitivity(55)... ");

  radSens.setSensitivity(55);

  sensitivity = radSens.getSensitivity();
  Serial.print("\t getSensitivity(): ");
  Serial.println(sensitivity);
  Serial.println("\t setSensitivity(105)... ");

  radSens.setSensitivity(105);

  Serial.print("\t getSensitivity(): ");
  Serial.println(radSens.getSensitivity());
  Serial.println("-------------------------------------");
  Serial.println("HW generator example:\n");

  bool hvGeneratorState = radSens.getHVGeneratorState();

  Serial.print("\n\t HV generator state: ");
  Serial.println(hvGeneratorState);
  Serial.println("\t setHVGeneratorState(false)... ");

  radSens.setHVGeneratorState(false);

  hvGeneratorState = radSens.getHVGeneratorState();
  Serial.print("\t HV generator state: ");
  Serial.println(hvGeneratorState);
  Serial.println("\t setHVGeneratorState(true)... ");

  radSens.setHVGeneratorState(true);

  hvGeneratorState = radSens.getHVGeneratorState();
  Serial.print("\t HV generator state: ");
  Serial.println(hvGeneratorState);
  Serial.println("-------------------------------------");
  Serial.println("LED indication control example:\n");

  bool ledState = radSens.getLedState();

  Serial.print("\n\t LED indication state: ");
  Serial.println(ledState);
  Serial.println("\t turn off LED indication... ");

  radSens.setLedState(false);
  ledState = radSens.getLedState();
  Serial.print("\t LED indication state: ");
  Serial.println(ledState);
  Serial.println("\t turn on led indication... ");

  radSens.setLedState(true);

  ledState = radSens.getLedState();
  Serial.print("\t LED indication state: ");
  Serial.print(ledState);
  Serial.println("\n-------------------------------------");

  Serial.end();
  nextion.begin(9600); // Start communication with nextion display

  // Load completed actions
  pinMode(BUZZ_PIN, OUTPUT);
  tone(BUZZ_PIN, 1000);
  delay(200);
  tone(BUZZ_PIN, 500);
  delay(200);
  tone(BUZZ_PIN, 200);
  delay(200);
  nextion.writeStr("page 1");
  delay(200);
  nextion.writeStr("page 1");
  tone(BUZZ_PIN, 1000);
  delay(200);
  noTone(BUZZ_PIN);
}

// Define needed global vars
long unsigned int out_timer = millis();
unsigned int new_pulses = 0;
unsigned int old_pulses = 0;
int out_delay = 1000;
byte battery_perc = 100;

int dynamic_intensivity = 0;
int delta_pulses = 0;
int sound_on = 1;

// Buzzer sound on a particle passes through Geiger tube.
void sound_pulse(unsigned int intensivity) {
  for (unsigned int i = 0; i <= intensivity; i++) {
    tone(BUZZ_PIN, 200);
    delay(10);
    noTone(BUZZ_PIN);
    delay(10);
  }
}

// Loop code
void loop()
{
  btn.tick();
  nextion.writeStr("battery_perc.val", String(battery_perc));
  new_pulses = radSens.getNumberOfPulses(); // New amount of passed particles
  delta_pulses = new_pulses - old_pulses;
  if (delta_pulses > 0 and sound_on == 1) { sound_pulse(delta_pulses); }
  if (out_delay < millis() - out_timer) { // Update screen
    out_timer = millis();
    dynamic_intensivity = radSens.getRadIntensyDynamic();
    nextion.writeStr("rad_dyn_r.txt", String(dynamic_intensivity));
    nextion.writeStr("rad_dyn_sv.txt", String(dynamic_intensivity * 0.01));
    nextion.writeStr("add 5,0," + String(dynamic_intensivity));
  }
  if (btn.isClick()) { sound_on = !sound_on; } // Turn sound off/on
  if (sound_on) { nextion.writeStr("sound_txt.txt", "Sound is On"); }
  else { nextion.writeStr("sound_txt.txt", "Sound is Off"); }
  old_pulses = new_pulses; // Update old_pulses
  delay(10);
}
