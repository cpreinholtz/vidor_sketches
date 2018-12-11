#include "configs.h"

float q_trans[3][3]={
  {1,0,0},
  {0,1,0},
  {0,0,1}
};

///////////////////////////////////////////////////////////
//sensor to craft transform
Attitude sensor_to_craft(Fcart sensor){
  Attitude res;
  
  res.roll= q_trans[0][0]*sensor.x+ q_trans[0][1]*sensor.y+ q_trans[0][2]*sensor.z;
  res.pitch= q_trans[1][0]*sensor.x+ q_trans[1][1]*sensor.y+ q_trans[1][2]*sensor.z;
  res.yaw= q_trans[2][0]*sensor.x+ q_trans[2][1]*sensor.y+ q_trans[2][2]*sensor.z;
  return res;
}



///////////////////////////////////////////////////////////
//ACC convert to degrees
Fcart get_acc_pos(void){
  Icart a=get_acc_vector();
  Fcart a_trans;
    
  a_trans.x = ( (float) (atan2(a.x,a.z))  ) *RAD_TO_DEG;
  a_trans.y = ( (float) (atan2(a.y,a.z))  ) *RAD_TO_DEG;
  return a_trans;
}


///////////////////////////////////////////////////////////
//Gyro position

Fcart get_gyro_rate(){
  Fcart g;
  gyro_raw=get_gyro_rate_vector();
  
  //Convert Gyro raw to degrees per second

  g.x = ((float)gyro_raw.x) * G_GAIN;   
  g.y = ((float)gyro_raw.y) * G_GAIN; 
  g.z = ((float)gyro_raw.z) * G_GAIN; 
  

  return g;
}



float get_single_gyro_pos(float last, float rate ){
  
  float delta =LOOP_PERIOD * rate;  
  float pos= ( delta  + last);
   
  return pos;
}


Fcart get_gyro_pos(void){
  Fcart g_pos;
  g_pos.x=get_single_gyro_pos(cfilter.x,gyro_rate.x);//according to contols theory, feedback the filtered measurment when getting the gyro position
  g_pos.y=get_single_gyro_pos(cfilter.y,gyro_rate.y);
  g_pos.z=get_single_gyro_pos(cfilter.z,gyro_rate.z);
  return g_pos;
}




///////////////////////////////////////////////////////////
//MAG (x,y vector to heading)
float get_mag (void){
  Icart m=get_magnetic_vector();
  float head;


  head = -atan2(m.x,m.y) *RAD_TO_DEG;
  //print_Icart(m);



  //float magXcomp = m.x*cos(cfilter.x)-m.z*sin(cfilter.y);
  //Serial.print(magXcomp); Serial.print ("\t\t");
  //float magYcomp = m.x*sin(cfilter.x)*sin(cfilter.y)+m.y*cos(cfilter.x)-m.z*sin(cfilter.x)*cos(cfilter.y); // LSM9DS0
  //Serial.print(magYcomp); Serial.println ("\t\t");
  
  if(head < 0) head += 360; 
  return head;
}




///////////////////////////////////////////////////////////
//conver 2 8bit bytes into int with signed extension

int bytes_to_int(byte ls, byte ms){

  if (ms&0x80){ //negative check
    int val;
    val =-1;
    val =val xor 0xFFFF;
    //int val=0xFFFF0000;//only works with 32 bit architechture & 16 bits of data!
    
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


