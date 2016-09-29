## WearON  
[WearON](http://umbrellium.co.uk/initiatives/wearon/) is a prototyping platform for wearable designers to connect their devices quickly and simply to a smartphone, to the web and to each other.

## About WearON arduino code
g
The arduino codes were developed separately using
* [Arduino Uno](https://www.arduino.cc/en/Main/ArduinoBoardUno) and Red Bear Lab [BLE Mini](http://redbearlab.com/blemini/)
* Red Bear Lab [Blend Micro](http://redbearlab.com/blendmicro/)

##Note
* For `WearONApp_arduino_code_ble_mini`
    * When uploading the code onto Arduino Uno, make sure that you disconnect the wires that connects BLE Mini to arduino pin 0 and 1. After you have uploaded the code onto the controller board, reconnect back the wires, restart the board and you are good to go!

* For `WearONApp_arduino_code_blend_micro`
    * When uploading the code, use board `Blend Micro 3.3V/8MHz` 

* On installing the RBL libraries
    * Follow the instruction on [Arduino](https://www.arduino.cc/en/Guide/Libraries) to install the libraries in `lib` into your arduino program
    * you can also log onto Red Bear Lab's website to download their latest Blend Micro and BLE Mini libraries. The ones in `lib` folder are the libraries that were used to develop WearON. 

## About
WearON is an open source initiative created by [Umbrellium](http://umbrellium.co.uk/) built for the community and us to quickly prototype connected wearables and iot devices. Join us in improving the project by contributing through github or simply drop us a message if there is any question or issue with using the app!
