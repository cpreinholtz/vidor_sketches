#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085_U.h>
#include "IMU.h"
#include "configs.h"

Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified(10085);


Attitude offset;


float G = 0.70;         // complementary filter constant
float A = (1-G);        // complementary filter constant


float ax, ay;//accelerometer postitions
float gx, gy, gz;//gyro rates
float px=0.0, py=0.0, pz=0.0;//gyro positions

float h;//heading
float pressure;

float cx=0.0, cy=0.0, cz=0.0;//results of complimentary filter




void get_measured(void){

  get_comp_filter();
  
  measured.roll=cx;
  measured.pitch=cy;
  measured.yaw=cz; 
      

}


void get_comp_filter(void){
  
  //read acc, gyro, mag, height
  poll_sensors();  //poll_sensor(ax, ay, gx, gy, gz, h);

  px=get_gyro_pos(cx,gx);//according to contols theory, feedback the filtered measurment when getting the gyro position
  py=get_gyro_pos(cy,gy);
  pz=get_gyro_pos(cz,gz);
      
      
  cf(cx, ax, px);//comlimentary filter
  cf(cy, ay, py);
  cf(cz, h,  pz);

}






void poll_sensors(){  //(float &ax, float &ay, float &gx, float &gy, float &gz, float &h){
  
  if(UPSIDEDOWN){
    //get_acc_upsidedown(ax, ay);
    //get_gyro_rate_upsidedown(gx, gy, gz);
    //get_mag_upsidedown(h);
    Serial.println("UPSIDE DOWN UNSUPPORTED!");
  }
  else{
    get_acc(ax, ay);
    get_gyro_rate(gx, gy, gz);
    get_mag(h);
  }
  //if(ENABLE_BMP){get_altitude();}
  
}




void calc_offsets(void){

  get_measured();
  
  float mult=0.9;
  offset.roll=offset.roll*mult+cx*(1-mult);
  offset.pitch=offset.pitch*mult+cy*(1-mult);
  offset.yaw=offset.yaw*mult+cz*(1-mult);

  desired.roll=offset.roll;
  desired.pitch=offset.pitch;
  desired.yaw=offset.yaw;
  idle_control(); 

  
}

void get_altitude(void){
    /* Get a new sensor event */ 
  sensors_event_t event;
  bmp.getEvent(&event);
 
  /* Display the results (barometric pressure is measure in hPa) */
  if (event.pressure)
  {

    pressure=event.pressure;
    /* Display atmospheric pressue in hPa */
    //Serial.print("Pressure:    ");
    //Serial.print(event.pressure);
    //Serial.println(" hPa");

    //float temperature;
    //bmp.getTemperature(&temperature);


    /* Then convert the atmospheric pressure, and SLP to altitude         */
    /* Update this next line with the current SLP for better results      */
    //float seaLevelPressure = SENSORS_PRESSURE_SEALEVELHPA;
    //Serial.print(bmp.pressureToAltitude(seaLevelPressure,  event.pressure)); 

  }
  else
  {
    Serial.println("BMP Sensor error");
  }
  
}





void setup_imu(void) {
  if(ENABLE_IMU){
    detectIMU();
    enableIMU(); 
  }
  else{
    Serial.println("No IMU in setup");
  }
  offset.yaw=0.0;
  offset.roll=0.0;
  offset.pitch=0.0;
}

/////////////////////////////////////////////
//ACC

//Read the measurements from  sensors and convert to degrees
void get_acc(float & x, float & y){
  byte buff[6];  
  readACC(buff);
    
  int rawx=bytes_to_int(buff[0] , buff[1]) ;
  int rawy=bytes_to_int(buff[2] , buff[3]) ;
  int rawz=bytes_to_int(buff[4] , buff[5]) ;  

    
  x =- ( (float) (atan2(rawx,rawz))  ) *RAD_TO_DEG;
  y =- ( (float) (atan2(rawy,rawz))  ) *RAD_TO_DEG;
}








///////////////////////////////////////////////////////////
//GYRO

void get_gyro_rate(float & x, float & y,float & z ){
  byte buff[6]; 
  readGYR(buff);
  
  //Convert Gyro raw to degrees per second

  //flip x and y so it agrees with the ACC
  x =  bytes_to_int(buff[2] , buff[3] ) * G_GAIN;   
  y = - bytes_to_int(buff[0] , buff[1] ) * G_GAIN; 
  z = -bytes_to_int(buff[4] , buff[5] ) * G_GAIN; 

}







float get_gyro_pos(float last, float rate ){
  
  float delta =LOOP_PERIOD * rate;  
  float pos= ( delta  + last);
   
  return pos;
}


///////////////////////////////////////////////////////////
//MAG
void get_mag(float &head){
  
  byte buff[6]; 
  readMAG(buff);
  int mx = bytes_to_int(buff[0] , buff[1] );   
  int my = bytes_to_int(buff[2] , buff[3] );
  int mz = bytes_to_int(buff[4] , buff[5] );

  //Compute heading  
  //head = atan2(my,mx) *RAD_TO_DEG;
  head = -atan2(mx,my) *RAD_TO_DEG;
  
  //Convert heading to 0 - 360
  if(head < 0) head += 360;            

}






///////////////////////////////////////////////////////////
//DSP

int bytes_to_int(byte ls,byte ms){

  if (ms&0x80){ //negative check
    int val=0xFFFF0000;//only works with 32 bit architechture & 16 bits of data!
    val=val | (int(ms)<<8) ;//should be all ones at this point, so anding is prob what we need
    val=val| ((int)ls) ;
    return val;
  }
  else{
    int val=int(ms);
    val=val << 8;
    val=val| ((int)ls) ;
    return val;
  }

  
}







void cf(float &val, float a, float g){
  
  A= 1-G;

  val= g*G + a*A;

 
}



/**************************************************************************/
/*
    Displays some basic information on this sensor from the unified
    sensor API sensor_t type (see Adafruit_Sensor for more information)
*/
/**************************************************************************/
void displaySensorDetails(void)
{
  sensor_t sensor;
  bmp.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" hPa");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" hPa");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" hPa");  
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);
}

/**************************************************************************/
/*
    Arduino setup function (automatically called at startup)
*/
/**************************************************************************/
void setup_bmp(void) 
{
  if(ENABLE_BMP){
    Serial.println("Pressure Sensor Test"); Serial.println("");
    
    /* Initialise the sensor */
    if(!bmp.begin())
    {
      /* There was a problem detecting the BMP085 ... check your connections */
      Serial.print("Ooops, no BMP085 detected ... Check your wiring or I2C ADDR!");
      while(1);
    }
    
    /* Display some basic information on this sensor */
    displaySensorDetails();
  }
  else{
    Serial.println("No BMP!");
  }
}



