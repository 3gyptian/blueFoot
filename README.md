# blueFoot
Using an ESP32 and a MPU6050 board send keyboard commands via Bluetooth by the shake of your foot


Easy soldering of standard GY-521 makes for a very quick/easy assembly of gesture based device that in this instance is attached to foot to allow for control of Youtube video as an example allowing for handsfree play/pause, rewind 5 seconds and fast forward 6 seconds.  


Assembly:

The MPU6050 power consumption is minimal can be powered directly from ESP32 GPIO pins. This allows for quick and easy direct soldering to rear of ESP32 as shown:


![Image description]https://github.com/3gyptian/blueFoot/blob/master/IMG_8743.JPG





Using DOIT ESP32 board as described here:
https://github.com/playelek/pinout-doit-32devkitv1


```
GY-521   GPIO PIN
VCC  ==>  21 (OUTPUT set to HIGH)
GND  ==>  19 (OUTPUT set to LOW)
SCL  ==>  18 (#define SCL 18)
SDA  ==>  5  (#define SDA 5)
XDA  ==>  17 (not used - INPUT)
XCL  ==>  16 (not used - INPUT)
ADD  ==>  4  (not used - INPUT)
INT  ==>  2  (not used - INPUT)
```




