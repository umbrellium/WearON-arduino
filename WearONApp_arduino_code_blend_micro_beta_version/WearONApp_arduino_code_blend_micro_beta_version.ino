#include <SPI.h>
#include <boards.h>
#include <RBL_nRF8001.h>

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

int digitalPin12 = 12;
int trackerFromApp = 0;
int x =1;

//to check whether all analog pins are enabled 
//(there is an issue with BLE write: board can send up to 3 analog pin data to phone per time, 
//but if it sends more than 4 then it stall the last pin data
boolean send_data_in_three = false;
boolean four_pins_on = false;

void setup(){
  // Default pins set to 9 and 8 for REQN and RDYN
  // Set your REQN and RDYN here before ble_begin() if you need
  //ble_set_pins(3, 2);

  // Set your BLE Shield name here, max. length 10
  //ble_set_name("My Name");

  // Init. and start BLE library.
  ble_begin();

  // Enable serial debug
  Serial.begin(57600);
  pinMode(digitalPin9, OUTPUT);
  pinMode(digitalPin10, OUTPUT);
  pinMode(digitalPin12, INPUT);

  // Default to internally pull high, change it if you need
  digitalWrite(digitalPin12, HIGH);

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
  while(ble_available())
  {
    // read out command and data
    byte data0 = ble_read();  // read which pin is targeted for controlling  
    byte data1 = ble_read();  // enable analog reading 
    byte data2 = ble_read();  // control all digital putput pins


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
      analog_enabled_A2 = false;
      analog_enabled_A3 = false;
      analog_enabled_A4 = false;
      analog_enabled_A5 = false;
      digitalWrite(digitalPin9, LOW);
      digitalWrite(digitalPin10, LOW);
    }

  }


  if (analog_enabled_A0)  // if analog reading enabled
  {
    if(send_data_in_three == true){ //send_data_in_two = true (for A0, A1, A2)
      // Read and send out
      uint16_t valueA0 = analogRead(analogPin0);
      tracker();
      ble_write(0x0F);  // binary: 00001011 (data 0) 
      ble_write(valueA0 >> 8); 
      ble_write(valueA0);
    }
  } 

  if (analog_enabled_A1)  // if analog reading enabled
  {
    if(send_data_in_three == true){ //send_data_in_two = true (for A0, A1, A2)
      // Read and send out
      uint16_t valueA1 = analogRead(analogPin1);
      tracker();
      ble_write(0x0E);  // binary: 00001011 (data 0) 
      ble_write(valueA1 >> 8); 
      ble_write(valueA1);
    }
  } 

  if (analog_enabled_A2)  // if analog reading enabled
  {
    if(send_data_in_three == true){ //send_data_in_two = true (for A0, A1, A2)
      // Read and send out
      uint16_t valueA2 = analogRead(analogPin2);
      tracker();
      ble_write(0x0D);  // binary: 00001011 (data 0) 
      ble_write(valueA2 >> 8); 
      ble_write(valueA2);
    }
  } 


  if (analog_enabled_A3)  // if analog reading enabled
  {
    if(send_data_in_three == false){//send_data_in_two = false ( for A3, A4 and A5)
      // Read and send out
      uint16_t valueA3 = analogRead(analogPin3);
      tracker();
      ble_write(0x0C);  // binary: 00001011 (data 0) 
      ble_write(valueA3 >> 8); 
      ble_write(valueA3);
    } 

  } 

  if (analog_enabled_A4)  // if analog reading enabled
  {
    if(send_data_in_three == false){//send_data_in_two = false ( for A3, A4 and A5)
      // Read and send out
      uint16_t valueA4 = analogRead(analogPin4);
      tracker();
      ble_write(0x0A);  // binary: 00001011 (data 0) 
      ble_write(valueA4 >> 8); 
      ble_write(valueA4);
    }
  } 


  if (analog_enabled_A5)  // if analog reading enabled
  {

    if(send_data_in_three == false){//send_data_in_two = false ( for A3, A4 and A5)
      // Read and send out
      uint16_t valueA5 = analogRead(analogPin5);
      tracker();
      ble_write(0x0B);  // binary: 00001011 (data 0) 
      ble_write(valueA5 >> 8); 
      ble_write(valueA5);
    }
  } 


  if(send_data_in_three == false){
    send_data_in_three = true;
  }
  else{
    send_data_in_three = false;
  }


  if (!ble_connected())  // if ble is not connected, set everything to off
  {
    analog_enabled_A2 = false;
    analog_enabled_A3 = false;
    analog_enabled_A4 = false;
    analog_enabled_A5 = false;
    digitalWrite(digitalPin9, LOW);
    digitalWrite(digitalPin10, LOW); 
    trackerFromApp = 0;WearONApp_arduino_code_blend_micro_beta_version
    x =0;
  } 
  else{
    tracker();
  }
  // Allow BLE Shield to send/receive data
  ble_do_events();

} 

void tracker(){

  //this part of tracking function is to track whether when device gets disconnected, 
  //whether phone still think its receiving something from device
  int connectionTracker= 1;
  Serial.println(connectionTracker);
  uint16_t trackerState = connectionTracker;
  ble_write(0xB0);  
  ble_write(trackerState >> 8); 
  ble_write(trackerState); 
};








































