# Dosimeter
Arduino &amp; RadSens &amp; Nextion simple dosimeter

### Example device
![Dosimeter-ed](https://github.com/matlire/Dosimeter/assets/137503385/6336a57b-5e8d-480d-b218-d731626ee2ce)


### About
It's a simple dosimeter to meausure radiation level in uR/h or uSv/h. Also some functions as real-time clock or battery power may not work properly.

### Construct
You need:
- Arduino uno
- Buzzer
- Button
- RadSens sensor (Universal dosimeter module based on SMD-20)
- Nextion display (NX_4024K032_011)

### Connection
Module -> Arduino

Buzzer -> Pin 9

Button -> Pin 10

Nextion RX -> TX

Nextion TX -> RX

RadSens SDA -> A4

RadSens SCL -> A5

All GND -> GND

All VCC -> 5V

### WARNING!
1) If you construct device incorrectly, we we are not responsible
2) Don't touch any metal parts of the device as voltage of RadSens is near 300V!

### How to use
First you need to install these libraries from arduino library manager:
- ClimateGuard RadSens
- Easy Nextion Library
- GyverButton


Disconnect Nextion TX & RX before uploading code! Then select needed board in arduino ide and upload code to board. Then turn the power off, connect Nextion TX & RX and now device is ready! You can turn it on!
