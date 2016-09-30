## WearON  
[WearON](http://umbrellium.co.uk/initiatives/wearon/) is a prototyping platform for wearable designers to connect their devices quickly and simply to a smartphone, to the web and to each other.

## About WearON arduino code

The arduino code was developed separately using
* [Arduino Uno](https://www.arduino.cc/en/Main/ArduinoBoardUno) and Red Bear Lab [BLE Mini](http://redbearlab.com/blemini/)
* Red Bear Lab [Blend Micro](http://redbearlab.com/blendmicro/)

##Note
* The arduino code was developed on Arduino version `1.0.5`. Go to [Arduino](https://www.arduino.cc/en/Main/OldSoftwareReleases#previous) to download the previous version

##For `WearONApp_arduino_code_ble_mini`
  * Install `lib`->`RBL_BLEMini.zip` into your Arduino library
  * Follow the instruction on [Arduino](https://www.arduino.cc/en/Guide/Libraries) to install the libraries in `lib` into your arduino program
  * Open the arduino file `WearONApp_arduino_code_ble_mini`
  * Note: When uploading the code onto Arduino Uno, make sure that you disconnect the wires that connects BLE Mini to arduino pin 0 and 1. After you have uploaded the code onto the controller board, reconnect back the wires, restart the board and you are good to go!

##For `WearONApp_arduino_code_blend_micro`
  * Follow the set of instructions on how to set up the Arduino IDE setup for the first time in order to use board `Blend Micro 3.3V/8MHz`
  *Install the following libraries into your Arduino library folder:
      *`lib`->`ble-sdk-arduino-0.9.0.beta.zip` 
      *`lib`->`RBL_nRF8001.zip` 
      * Follow the instruction on [Arduino](https://www.arduino.cc/en/Guide/Libraries) to install the libraries in `lib` into your arduino program
    *Install the hardware folder from `lib`->`Blend_Add-On.zip` into your Arduino Sketchbook folder
    *Replace the main.cpp in your arduino folder with the one in `lib`->`Blend_Add-On.zip` (this is a very important step, if you didnt do this step, your Blend Micro board might lost its firmware, hence disabling the usb connection)
  *Alternatively, you can also check out our [Wiki](https://github.com/umbrellium/WearON-arduino/wiki) for instructions on setting up Blend Micro board
  * Open the arduino file `WearONApp_arduino_code_blend_micro`
  * Note: When uploading the code, use board `Blend Micro 3.3V/8MHz` 


## About
WearON is an open source initiative created by [Umbrellium](http://umbrellium.co.uk/) built for the community and for us to quickly prototype connected wearables and iot devices. Join us in improving the project by contributing through github or simply drop us a message if there is any question or issue with using the platform!
