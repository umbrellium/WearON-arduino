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
const int analogPin0 = A0;
const int analogPin1 = A1; 
const int analogPin2 = A2;
const int analogPin3 = A3; 
const int analogPin4 = A4; 
const int analogPin5 = A5;

//to check whether all analog pins are enabled 
//(there is an issue with BLE write: board can send up to 3 analog pin data to phone per time, 
//but if it sends more than 4 then it stall the last pin data
boolean send_data_in_three = false;
boolean all_pins_on = false;

void setup(){

  BLEMini_begin(57600);
  pinMode(digitalPin9, OUTPUT);
  pinMode(digitalPin10, OUTPUT);

}

void loop(){
  static boolean analog_enabled_A0 = false; //enable analog reading on analog pin A0
  static boolean analog_enabled_A1 = false; //enable analog reading on analog pin A1
  static boolean analog_enabled_A2 = false; //enable analog reading on analog pin A2
  static boolean analog_enabled_A3 = false; //enable analog reading on analog pin A3
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
      else if (data1 == 0x05){  // binary: 00000011 (data 1)
        analog_enabled_A3 = true;
      }
      else if (data1 == 0x06){  // binary: ? (data 1)
        analog_enabled_A3 = false;
      }
      else if (data1 == 0x07){  // binary: 00000011 (data 1)
        analog_enabled_A2 = true;
      }
      else if (data1 == 0x08){  // binary: ? (data 1)
        analog_enabled_A2 = false;
      }
      else if (data1 == 0x09){  // binary: 00000011 (data 1)
        analog_enabled_A1 = true;
      }
      else if (data1 == 0x10){  // binary: ? (data 1)
        analog_enabled_A1 = false;
      }
      else if (data1 == 0x11){  // binary: 00000011 (data 1)
        analog_enabled_A0 = true;
      }
      else if (data1 == 0x12){  // binary: ? (data 1)
        analog_enabled_A0 = false;
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

  if (analog_enabled_A0)  // if analog reading enabled
  {
    if(all_pins_on==false){
      // Read and send out
      uint16_t valueA0 = analogRead(analogPin0);
      tracker();
      BLEMini_write(0x0F);  // binary: 00001011 (data 0) 
      BLEMini_write(valueA0 >> 8); 
      BLEMini_write(valueA0);
    }
    else{
      if(send_data_in_three == true){ //send_data_in_two = true (for A0, A1, A2)
        // Read and send out
        uint16_t valueA0 = analogRead(analogPin0);
        tracker();
        BLEMini_write(0x0F);  // binary: 00001011 (data 0) 
        BLEMini_write(valueA0 >> 8); 
        BLEMini_write(valueA0);
      }
    }
  } 

  if (analog_enabled_A1)  // if analog reading enabled
  {
    if(all_pins_on==false){
      // Read and send out
      uint16_t valueA1 = analogRead(analogPin1);
      tracker();
      BLEMini_write(0x0E);  // binary: 00001011 (data 0) 
      BLEMini_write(valueA1 >> 8); 
      BLEMini_write(valueA1);
    }
    else{
      if(send_data_in_three == true){ //send_data_in_two = true (for A0, A1, A2)
        // Read and send out
        uint16_t valueA1 = analogRead(analogPin1);
        tracker();
        BLEMini_write(0x0E);  // binary: 00001011 (data 0) 
        BLEMini_write(valueA1 >> 8); 
        BLEMini_write(valueA1);
      }
    }
  } 

  if (analog_enabled_A2)  // if analog reading enabled
  {
    if(all_pins_on==false){
      // Read and send out
      uint16_t valueA2 = analogRead(analogPin2);
      tracker();
      BLEMini_write(0x0D);  // binary: 00001011 (data 0) 
      BLEMini_write(valueA2 >> 8); 
      BLEMini_write(valueA2);
    }
    else{
      if(send_data_in_three == true){ //send_data_in_two = true (for A0, A1, A2)
        // Read and send out
        uint16_t valueA2 = analogRead(analogPin2);
        tracker();
        BLEMini_write(0x0D);  // binary: 00001011 (data 0) 
        BLEMini_write(valueA2 >> 8); 
        BLEMini_write(valueA2);
      }
    }
  } 


  if (analog_enabled_A3)  // if analog reading enabled
  {
    if(all_pins_on==false){
      // Read and send out
      uint16_t valueA3 = analogRead(analogPin3);
      tracker();
      BLEMini_write(0x0C);  // binary: 00001011 (data 0) 
      BLEMini_write(valueA3 >> 8); 
      BLEMini_write(valueA3);
    }
    else{
      if(send_data_in_three == false){//send_data_in_two = false ( for A3, A4 and A5)
        // Read and send out
        uint16_t valueA3 = analogRead(analogPin3);
        tracker();
        BLEMini_write(0x0C);  // binary: 00001011 (data 0) 
        BLEMini_write(valueA3 >> 8); 
        BLEMini_write(valueA3);
      } 
    }
  } 

  if (analog_enabled_A4)  // if analog reading enabled
  {
    if(all_pins_on==false){
      // Read and send out
      uint16_t valueA4 = analogRead(analogPin4);
      tracker();
      BLEMini_write(0x0A);  // binary: 00001011 (data 0) 
      BLEMini_write(valueA4 >> 8); 
      BLEMini_write(valueA4);
    }
    else{
      if(send_data_in_three == false){//send_data_in_two = false ( for A3, A4 and A5)
        // Read and send out
        uint16_t valueA4 = analogRead(analogPin4);
        tracker();
        BLEMini_write(0x0A);  // binary: 00001011 (data 0) 
        BLEMini_write(valueA4 >> 8); 
        BLEMini_write(valueA4);
      }
    }
  } 


  if (analog_enabled_A5)  // if analog reading enabled
  {
    if(all_pins_on==false){
      // Read and send out
      uint16_t valueA5 = analogRead(analogPin5);
      tracker();
      BLEMini_write(0x0B);  // binary: 00001011 (data 0) 
      BLEMini_write(valueA5 >> 8); 
      BLEMini_write(valueA5);
    }
    else{
      if(send_data_in_three == false){//send_data_in_two = false ( for A3, A4 and A5)
        // Read and send out
        uint16_t valueA5 = analogRead(analogPin5);
        tracker();
        BLEMini_write(0x0B);  // binary: 00001011 (data 0) 
        BLEMini_write(valueA5 >> 8); 
        BLEMini_write(valueA5);
      }
    }
  } 

  if((analog_enabled_A0 == true) && (analog_enabled_A1==true) && (analog_enabled_A2 == true) && (analog_enabled_A3==true) && (analog_enabled_A4==true) && (analog_enabled_A5==true)){
    all_pins_on = true;
    if(send_data_in_three == false){
      send_data_in_three = true;
    }
    else{
      send_data_in_three = false;
    }
  }
  else{
    all_pins_on = false;
    send_data_in_three = false;
  }

  // when delay val goes lower the app becomes slower, have to balance it out
  delay(50); // pretty annoying bug!

}

void tracker(){
  //this part of tracking function is to track whether when device gets disconnected, 
  //whether phone still thought its receiving something from device
  int connectionTracker= 1;
  uint16_t trackerState = connectionTracker;
  BLEMini_write(0xB0);  
  BLEMini_write(trackerState >> 8); 
  BLEMini_write(trackerState); 
};








