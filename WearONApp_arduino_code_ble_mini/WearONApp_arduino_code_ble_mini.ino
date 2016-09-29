#include <ble_mini.h>

/* WearON arduino code for BLE Mini board 
 * WearON arduino code enables communication between WearON mobile phone app
 * and pin 9, 10, analogue input A4, A5 via bluetooth. The code is a basic framework
 * set up to enable bluetooth communication between the microcontroller and the mobile 
 * phone app. 
 */

/*
  Copyright (c) 2015 Umbrellium Ltd
 
   Permission is hereby granted, free of charge, to any person obtaining a copy of 
   this software and associated documentation files (the "Software"), to deal in the 
   Software without restriction, including without limitation the rights to use, copy, 
   modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, 
   and to permit persons to whom the Software is furnished to do so, subject to the 
   following conditions:
   
   The above copyright notice and this permission notice shall be included in all copies 
   or substantial portions of the Software.
   
   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
   INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR 
   PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE 
   FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR 
   OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
   DEALINGS IN THE SOFTWARE.
 */

// digital output pins
int digitalPin9 = 9;
int digitalPin10 = 10;

// analog input pins
const int analogPin4 = A4; 
const int analogPin5 = A5;

void setup(){

  BLEMini_begin(57600);
  pinMode(digitalPin9, OUTPUT);
  pinMode(digitalPin10, OUTPUT);

}

void loop(){

  static boolean analog_enabled_A4 = false; //enable analog reading on analog pin A4
  static boolean analog_enabled_A5 = false; //enable analog reading on analog pin A4

  static byte old_state = LOW;

  // If data is ready
  while (BLEMini_available())
  {
    // read out command and data
    byte data0 = BLEMini_read();
    byte data1 = BLEMini_read();
    byte data2 = BLEMini_read();

    if (data0 == 0xA0) // Command is to enable analog in reading - binary: 10100000 (data 0) 
    {

      if (data1 == 0x01){  // binary:  00000001 (data 1) 
        analog_enabled_A4 = true;
      }
      else if (data1 == 0x02){  // binary: 00000010 (data 1)
        analog_enabled_A4 = false;
      }
      else if (data1 == 0x03){  // binary: 00000011 (data 1)
        analog_enabled_A5 = true;
      }
      else if (data1 == 0x04){  // binary: ? (data 1)
        analog_enabled_A5 = false;
      }

    }

    else if (data0 == 0x01)  // Command is to control digital out pin - binary: 00000001 (data 0)
    {
      if (data2 == 0x01) {
        digitalWrite(digitalPin9, HIGH); // binary: 00000001 (data 2) 
      }  
      else if (data2 == 0x02){
        digitalWrite(digitalPin9, LOW); // binary: 00000010 (data 2)
      }
      else if (data2 == 0x03) {
        digitalWrite(digitalPin10, HIGH); // binary: 00000011 (data 2)
      }  
      else if (data2 == 0x04){
        digitalWrite(digitalPin10, LOW); // binary: 00000100 (data 2)
      }          
    }

    else if (data0 == 0x04) //command to change everything to default state (off mode) - binary: 00000100 (data 0)
    {
      analog_enabled_A4 = false;
      analog_enabled_A5 = false;
      digitalWrite(digitalPin9, LOW);
      digitalWrite(digitalPin10, LOW);
    }

    else if (data0 == 0x03) //command to change everything to default state (off mode) - binary: 00000100 (data 0)
    {
      tracker();
    }

  }

  if (analog_enabled_A4)  // if analog reading enabled
  {
    // Read and send out
    uint16_t valueA4 = analogRead(analogPin4);
    BLEMini_write(0x0A);  // binary: 00001011 (data 0) 
    BLEMini_write(valueA4 >> 8); 
    BLEMini_write(valueA4);
  }

  if (analog_enabled_A5)  // if analog reading enabled
  {
    // Read and send out
    uint16_t valueA5 = analogRead(analogPin5);
    BLEMini_write(0x0B);  // binary: 00001011 (data 0) 
    BLEMini_write(valueA5 >> 8); 
    BLEMini_write(valueA5);
  }

  // when delay val goes lower the app becomes slower, have to balance it out
  delay(50); // pretty annoying bug!

}

void tracker(){
  //this part of tracking function is to track whether when device gets disconnected, 
  //whether phone still thought its receiving something from device
  int connectionTracker= 1;
  uint16_t trackerState = connectionTracker;
  BLEMini_write(0x0D);  
  BLEMini_write(trackerState >> 8); 
  BLEMini_write(trackerState); 
};


