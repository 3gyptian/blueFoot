/**
 * This example turns the ESP32 into a Bluetooth LE keyboard that writes the words, presses Enter, presses a media key and then Ctrl+Alt+Delete
 */

#include <Arduino.h>
#include <BleKeyboard.h>
#include <FastLED.h>

// MPU6050
#define MPU_VCC 21
#define MPU_GND 19
#define SCL 18 
#define SDA 5  
#include "I2Cdev.h"
#include "MPU6050.h"

#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
  #include <Wire.h>
#endif

#define OUTPUT_READABLE_ACCELGYRO

MPU6050 accelgyro;
//MPU6050 accelgyro(0x69); // <-- use for AD0 high

int16_t ax, ay, az;
int16_t gx, gy, gz;
float mpuTemp;

int32_t gTrig=6000;



BleKeyboard bleKeyboard;

void setup() {
  Serial.begin(115200);
  //pinMode(LED_BUILTIN,OUTPUT);

  // Power OFF MPU6050 
  Serial.println("Hard power off MPU6050..");
  digitalWrite(MPU_VCC,LOW);
  digitalWrite(MPU_GND,LOW);
  delay(10);
  pinMode(MPU_VCC,OUTPUT);
  pinMode(MPU_GND,OUTPUT);
  delay(1000);

  
  //Serial.flush();
  delay(100);
  //while(//Serial.available()==0){
  //  //Serial.println("Hit any key to begin..");
  //  delay(1000);
  //}

  // Power ON MPU6050 
  Serial.println("Powering up MPU6050..");
  digitalWrite(MPU_VCC,HIGH);
  digitalWrite(MPU_GND,LOW);
  delay(10);
  pinMode(MPU_VCC,OUTPUT);
  pinMode(MPU_GND,OUTPUT);
  delay(500);

 // join I2C bus (I2Cdev library doesn't do this automatically)
  #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    //Wire.begin();
    Wire.begin(SDA, SCL, 400000);
  #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
      Fastwire::setup(400, true);
  #endif

  // initialize MPU6050 device
  Serial.println("Initializing MPU6050 ...");
  accelgyro.initialize();
  accelgyro.setFullScaleGyroRange(MPU6050_GYRO_FS_2000);

  // verify connection
  Serial.println("Testing MPU6050 connection ...");
  Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");


  Serial.println("Starting BLE work!");
  bleKeyboard.begin();
}

void loop() {

  while (!bleKeyboard.isConnected()){
    Serial.println("Waiting for bleKeyboard to connect...");
    delay(1000);
  };



  EVERY_N_MILLIS(50){
    // read raw accel/gyro measurements from device
    //accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    accelgyro.getRotation(&gx, &gy, &gz);
    uint32_t gTotal=abs(gx)+abs(gy)+abs(gz); 

    if (gTotal > gTrig){
      //Serial.print(gTotal);Serial.print("\t");
      //Serial.print(gx); Serial.print("\t");
      //Serial.print(gy); Serial.print("\t");
      //Serial.println(gz);
  
      if (abs(gx) > gTrig/2 && abs(gx) > abs(gz)) {
        bleKeyboard.write(KEY_MEDIA_PLAY_PAUSE);
        Serial.print(" gx: ");Serial.print(gx);Serial.println(" Play/Pause");
      }
      else if (gz > gTrig/2 && abs(gz) > abs(gx) ) {
        bleKeyboard.write(KEY_RIGHT_ARROW);
        Serial.print("gz: ");Serial.print(gz);Serial.println(" Right Arrow");
      }
      else if (gz < -gTrig/2 && abs(gz) > abs(gx) ) {
        bleKeyboard.write(KEY_LEFT_ARROW);
        Serial.print("gz: ");Serial.print(gz);Serial.println(" Left Arrow");
      }

      delay(500); // prevent footbounce
    
    }

    // Show MPU chip temperature in Celcius
    /*EVERY_N_MILLIS(1000){
      mpuTemp=accelgyro.getTemperature();
      mpuTemp=mpuTemp/340 + 36.53;
      Serial.print("mpuTemp: ");Serial.println(mpuTemp);
    } */
  
  }

}
