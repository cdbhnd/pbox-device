# pbox-device
### setup
- https://docs.labs.mediatek.com/resource/linkit-one/en/getting-started

### documentation
- Arduino: https://www.arduino.cc/en/Reference/HomePage
- Linkit: http://labs.mediatek.com/api/linkit-one/frames.html?frmname=topic&frmfile=index.html
- All Things Talk: http://www.allthingstalk.com/docs

### libraries
- ATT: https://github.com/allthingstalk/arduino-client/archive/master.zip
- Temperature & Humidity: https://github.com/Seeed-Studio/Grove_Starter_Kit_For_LinkIt/tree/master/libraries/Humidity_Temperature_Sensor
- Accelerometer: http://www.seeedstudio.com/wiki/File:DigitalAccelerometer_ADXL345.zip
- LED: https://github.com/Seeed-Studio/Grove_LED_Bar.git

### note:
> before sketch is uploaded to device, make sure that you uncommented `deviceId` and type of connecting to internet `initializeWIFI()` or `initializeGPRS`.