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

#define DT  0.02          // Loop time 20ms
#define G_GAIN 0.070    // [deg/s/LSB]




///////////////////////////////////////////////////////////
//CPR
#define VERBOSE 2
#define NORMAL 1
const int PRINT_ENABLE = VERBOSE;
const float GA = 0.97;         // complementary filter constant
const float AA = (1-GA);        // complementary filter constant

const int slow_loop= 0;//ms additional delay

///////////////////////////////////////////////////////////



byte buff[6];
int accRaw[3];
int magRaw[3];
int gyrRaw[3];

float rate_gyr_y = 0.0;   // [deg/s]
float rate_gyr_x = 0.0;    // [deg/s]
float rate_gyr_z = 0.0;     // [deg/s]

float gyroXangle = 0.0;
float gyroYangle = 0.0;
float gyroZangle = 0.0;

float AccYangle = 0.0;
float AccXangle = 0.0;

float CFangleX = 0.0;
float CFangleY = 0.0;

float heading = 0.0;

int epoch=0;


unsigned long startTime;

void setup() {
         // join i2c bus (address optional for master)
  if (PRINT_ENABLE){
    Serial.begin(115200);  // start serial for output
  }

  detectIMU();
  enableIMU();

  
  pinMode(LED_BUILTIN, OUTPUT);


}



void loop() {
 
  startTime = millis();

  test_atan();
  //get_acc(AccXangle, AccYangle);
  //get_gyro_rate(rate_gyr_x, rate_gyr_y, rate_gyr_z);
  //get_mag(heading);
  
  //compute_compliment();

  regulate_time();
}

void test_atan(void){

  int y;
  int x;
  int m=63000;

  float ma=1000.0;
  float mi=-1000.0;

  for (y=-63000;y<63000;y=y+10000){
    for(x=-63000;x<63000;x=x+10000){
      float ans= atan2(y,x);
      Serial.print(" x ");Serial.print(x);
      Serial.print(" y ");Serial.print(y);
      Serial.print(" a ");Serial.print(ans);
      Serial.println();
      
    }
  }

  while(1);
  
  
}





void get_acc(float & x, float & y){
  //Read the measurements from  sensors
  readACC(buff);

  
  accRaw[0] = (int)(buff[0] | (buff[1] << 8));   
  accRaw[1] = (int)(buff[2] | (buff[3] << 8));
  accRaw[2] = (int)(buff[4] | (buff[5] << 8));



  //Convert Accelerometer values to degrees
  ///////////////////////////////////////////////////////////////////
  //original
  //AccXangle = (float) (atan2(accRaw[1],accRaw[2])+M_PI)*RAD_TO_DEG;
  //AccYangle = (float) (atan2(accRaw[2],accRaw[0])+M_PI)*RAD_TO_DEG;

  ////////////////////////////////////////////////////////////////////


  
  AccXangle = (float) (atan2(accRaw[2],accRaw[1])+M_PI)*RAD_TO_DEG;
  AccYangle = (float) (atan2(accRaw[0],accRaw[2])+M_PI)*RAD_TO_DEG;


  
  x= AccXangle;
  y= AccYangle;    

  
  if (PRINT_ENABLE & VERBOSE){

    Serial.print(" buff0: ");Serial.print(buff[0]);
    Serial.print(" buff1: ");Serial.print(buff[1]);
    Serial.print(" buff2: ");Serial.print(buff[2]);
    Serial.print(" buff3: ");Serial.print(buff[3]);
    Serial.print(" buff4: ");Serial.print(buff[4]);
    Serial.print(" buff5: ");Serial.print(buff[5]);

    
    Serial.print(" accraw0: ");Serial.print(accRaw[0]);
    Serial.print(" accraw1: ");Serial.print(accRaw[1]);
    Serial.print(" accraw2: ");Serial.print(accRaw[2]);

    
    Serial.print(" #AccX:  ");
    Serial.print(AccXangle);
    Serial.print(" #AccY:  ");
    Serial.print(AccYangle);
    
    Serial.println();
  }

}



void get_gyro_rate(float & x, float & y,float & z ){
  
  readGYR(buff);
  gyrRaw[0] = (int)(buff[0] | (buff[1] << 8));   
  gyrRaw[1] = (int)(buff[2] | (buff[3] << 8));
  gyrRaw[2] = (int)(buff[4] | (buff[5] << 8));

  
  //Convert Gyro raw to degrees per second
  x = (float) gyrRaw[0] * G_GAIN;
  y = (float) gyrRaw[1]  * G_GAIN;
  z = (float) gyrRaw[2]  * G_GAIN;

}

void get_gyro_pos(float & x, float & y,float & z ){
  
  readGYR(buff);
  gyrRaw[0] = (int)(buff[0] | (buff[1] << 8));   
  gyrRaw[1] = (int)(buff[2] | (buff[3] << 8));
  gyrRaw[2] = (int)(buff[4] | (buff[5] << 8));

  
  //Convert Gyro raw to degrees per second
  x = (float) gyrRaw[0] * G_GAIN;
  y = (float) gyrRaw[1]  * G_GAIN;
  z = (float) gyrRaw[2]  * G_GAIN;

}



void compute_compliment(void){
    //Complementary filter used to combine the accelerometer and gyro values.  
  /////////////////////////////////////////////////////////////////////
  //OZZMAKER
  //CFangleX=( GA*( CFangleX+(rate_gyr_x *DT) ) ) + ((AA) * AccXangle);
  //CFangleY=( GA*( CFangleY+(rate_gyr_y *DT) ) ) + ((AA) * AccYangle);
  //CPR
  CFangleX=( GA* ( ( CFangleX+rate_gyr_x ) *DT ) ) + ( ( AA ) * AccXangle );
  CFangleY=( GA* ( ( CFangleY+rate_gyr_y ) *DT ) ) + ( ( AA ) * AccYangle );
  /////////////////////////////////////////////////////////////////////


 
}


void get_mag(float &head){
  
  readMAG(buff);
  magRaw[0] = (int)(buff[0] | (buff[1] << 8));   
  magRaw[1] = (int)(buff[2] | (buff[3] << 8));
  magRaw[2] = (int)(buff[4] | (buff[5] << 8));

  //Compute heading  
  head = 180 * atan2(magRaw[1],magRaw[0])/M_PI;
  
  //Convert heading to 0 - 360
  if(head < 0) head += 360;
            

}



void regulate_time(void){
    
  //Each loop should be at least 20ms.
  while(millis() - startTime < (DT*1000))
  {
    delay(1);
  }
        
  epoch = epoch +1;


  
  if (epoch%2==0) digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level);
  else digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  //if (epoch >=100) while(1);

  delay(slow_loop);


  
}

void print_all(void){
    if(PRINT_ENABLE){
      Serial.print("#AccX  ");
      Serial.print(AccXangle);
      Serial.print("  ###AccY  ");
      Serial.print(AccYangle);
  
  
      Serial.print("  ###GyrX");
      Serial.print( gyroXangle);
      Serial.print("  ###GyrY  ");
      Serial.print(gyroYangle);
      Serial.print("   ###GyrZ");
      Serial.print(gyroZangle);
  
      Serial.print("  ######CFangleX  ");
      Serial.print( CFangleX);
      Serial.print("  ######CFangleY  ");
      Serial.print(CFangleY);
  
      Serial.print("  ######  HEADING ");
      Serial.print( heading);
  }
}


