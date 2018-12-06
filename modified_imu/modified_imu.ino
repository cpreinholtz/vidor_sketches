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




float LOOP_PERIOD=0.02;
bool UPSIDEDOWN=false;
int epoch=0;


///////////////////////////////////////////////////////////////////////////////
void loop() {
  float ax, ay, gx, gy, gz, h;
  float cx=0.0, cy=0.0, cz=0.0, px=0.0, py=0.0, pz=0.0;

  while(1){
  unsigned long startTime = millis();
  

  poll_sensor(ax, ay, gx, gy, gz, h);
  //px=get_gyro_pos(px,gx);//gyro only
  px=get_gyro_pos(cx,gx);//according to contols theory, feedback the filtered measurment
  py=get_gyro_pos(cy,gy);
  pz=get_gyro_pos(cz,gz);
  
  
  cf(cx, ax, px);
  cf(cy, ay, py);
  cf(cz, h,  pz);


  print_conditional(ax,ay,gx,gy,gz,h, cx, cy, px, py, pz);
  regulate_time(startTime);
  }//while (1)
 

}






///////////////////////////////////////////////////////////////////////////////
void setup() {
         // join i2c bus (address optional for master)

  Serial.begin(115200);  // start serial for output  

  detectIMU();
  enableIMU();  
  pinMode(LED_BUILTIN, OUTPUT);
}


