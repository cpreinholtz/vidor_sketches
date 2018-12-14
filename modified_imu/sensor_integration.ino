#include "configs.h"

const float G = 0.7;         // complementary filter constant
const float A = (1-G);        // complementary filter constant


float heading;//heading
Fcart acc_pos;
Fcart gyro_rate;
Fcart gyro_pos={0.0,0.0,0.0};
Fcart cfilter={0.0,0.0,0.0};



//gets all and sets measured
void get_measured(void){

  poll_sensors();
  //transform here?!?!
  
  measured=sensor_to_craft(cfilter);

}

//////////////////////////////////////////////////
//gets all sensor readings
void poll_sensors(){  
  
  acc_pos=get_acc_pos();
  gyro_rate=get_gyro_rate();
  heading=get_mag();
  gyro_pos=get_gyro_pos();
  
  cf(cfilter.x, acc_pos.x, gyro_pos.x);//comlimentary filter
  cf(cfilter.y, acc_pos.y, gyro_pos.y);
  cf(cfilter.z, heading,  gyro_pos.z);    
  
}







//////////////////////////////////////////////////
//COMP FILTER
void cf(float &val, float a, float g){
  
  //A= 1-G;
  val= g*G + a*A;
 
}


//////////////////////////////////////////////////
//OFFSET CALIRATION 
void calc_offsets(void){

  get_measured();
  
  float mult=0.9;
  offset.roll=offset.roll*mult+cfilter.x*(1-mult);
  offset.pitch=offset.pitch*mult+cfilter.y*(1-mult);
  offset.yaw=offset.yaw*mult+cfilter.z*(1-mult);

  desired.roll=offset.roll;
  desired.pitch=offset.pitch;
  desired.yaw=offset.yaw;
   

  
}
