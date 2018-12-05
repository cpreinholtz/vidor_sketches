/*
  This program  reads the angles and heading from the accelerometer, gyroscope
   and compass on a BerryIMU connected to an Arduino.
  
    Both the BerryIMUv1 and BerryIMUv2 are supported.
    Feel free to do whatever you like with this code
    Distributed as-is; no warranty is given.
    http://ozzmaker.com/
*/
/*
  Modified by cpreinholtz in accordance to control theory
*/
#include "IMU.h"



int epoch=0;
float loop_period=0.02;
bool UPSIDEDOWN=false;


///////////////////////////////////////////////////////////////////////////////
void loop() {
 
  unsigned long startTime = millis();
  
  float ax, ay, gx, gy, gz, h;
  if(UPSIDEDOWN){
    get_acc_upsidedown(ax, ay);
    get_gyro_rate_upsidedown(gx, gy, gz);
    get_mag_upsidedown(h);
  }
  else{
    get_acc(ax, ay);
    get_gyro_rate(gx, gy, gz);
    get_mag(h);
  }



  print_conditional(ax,ay,gx,gy,gz,h);
  regulate_time(startTime);
}






///////////////////////////////////////////////////////////////////////////////
void setup() {
         // join i2c bus (address optional for master)

  Serial.begin(115200);  // start serial for output  

  detectIMU();
  enableIMU();  
  pinMode(LED_BUILTIN, OUTPUT);
}


