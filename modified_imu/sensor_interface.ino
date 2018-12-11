
#include "IMU.h"
#include "configs.h"




Attitude offset={0.0,0.0,0.0};

Icart gyro_raw, acc_raw, mag_raw;








//float cx=0.0, cy=0.0, cz=0.0;//results of complimentary filter


/////////////////////////////////////////////
//ACC



Icart get_acc_vector(){
  Icart a;
  byte buff[6];  
  readACC(buff);  
  a.x=  bytes_to_int(buff[0] , buff[1]) ;//right is -x dir
  a.y=- bytes_to_int(buff[2] , buff[3]) ; //front is -y dir
  a.z=  bytes_to_int(buff[4] , buff[5]) ;  
  /*DONT TOUCH
  a.x=  bytes_to_int(buff[0] , buff[1]) ;//right is -x dir
  a.y=- bytes_to_int(buff[2] , buff[3]) ; //front is -y dir
  a.z=  bytes_to_int(buff[4] , buff[5]) ;  */
  return a;
}





///////////////////////////////////////////////////////////
//GYRO
Icart get_gyro_rate_vector(){
  Icart raw;
  byte buff[6]; 
  readGYR(buff);
  raw.x = - bytes_to_int(buff[2] , buff[3] );   
  raw.y = - bytes_to_int(buff[0] , buff[1] );
  raw.z = - bytes_to_int(buff[4] , buff[5] );
  /*DONT TOUCH
    raw.x = - bytes_to_int(buff[2] , buff[3] );   
  raw.y = - bytes_to_int(buff[0] , buff[1] );
  raw.z = - bytes_to_int(buff[4] , buff[5] );*/
  return raw;
}




///////////////////////////////////////////////////////////
//MAG
Icart get_magnetic_vector(){
  Icart m;
  byte buff[6]; 
  readMAG(buff);
  m.x = bytes_to_int(buff[0] , buff[1] );   
  m.y = bytes_to_int(buff[2] , buff[3] );
  m.z = bytes_to_int(buff[4] , buff[5] );            
  return m;
}




///////////////////////////////////////////////////////////
//SETUP
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








