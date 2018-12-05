


#define G_GAIN 0.070    // [deg/s/LSB]

//Read the measurements from  sensors and convert to degrees
void get_acc(float & x, float & y){
  byte buff[6];  
  readACC(buff);
    
  int rawx=bytes_to_int(buff[0] , buff[1]) ;
  int rawy=bytes_to_int(buff[2] , buff[3]) ;
  int rawz=bytes_to_int(buff[4] , buff[5]) ;  
  
  x = ( (float) (atan2(rawz,rawx))  ) *RAD_TO_DEG;
  y = ( (float) (atan2(rawz,rawy))  ) *RAD_TO_DEG;
    
  x = ( (float) (atan2(rawx,rawz))  ) *RAD_TO_DEG;
  y = ( (float) (atan2(rawy,rawz))  ) *RAD_TO_DEG;
}




void get_gyro_rate(float & x, float & y,float & z ){
  byte buff[6]; 
  readGYR(buff);
  
  //Convert Gyro raw to degrees per second

  //flip x and y so it agrees with the ACC
  y =  bytes_to_int(buff[0] , buff[1] ) * G_GAIN; 
  x =  bytes_to_int(buff[2] , buff[3] ) * G_GAIN; 
  z = -bytes_to_int(buff[4] , buff[5] ) * G_GAIN; 

}






void get_gyro_pos(float & x, float & y,float & z ){

}






void get_acc_upsidedown(float & x, float & y){
  byte buff[6];  
  readACC(buff);
    
  int rawx= -bytes_to_int(buff[0] , buff[1]) ;
  int rawy= bytes_to_int(buff[2] , buff[3]) ;
  int rawz= -bytes_to_int(buff[4] , buff[5]) ;  
  
  x = ( (float) (atan2(rawz,rawx))  ) *RAD_TO_DEG;
  y = ( (float) (atan2(rawz,rawy))  ) *RAD_TO_DEG;
    
  x = ( (float) (atan2(rawx,rawz))  ) *RAD_TO_DEG;
  y = ( (float) (atan2(rawy,rawz))  ) *RAD_TO_DEG;
}






void get_gyro_rate_upsidedown(float & x, float & y,float & z ){
  byte buff[6]; 
  readGYR(buff);
  
  //Convert Gyro raw to degrees per second

  //flip x and y so it agrees with the ACC
  y = -bytes_to_int(buff[0] , buff[1] ) * G_GAIN; 
  x = -bytes_to_int(buff[2] , buff[3] ) * G_GAIN; 
  z =  bytes_to_int(buff[4] , buff[5] ) * G_GAIN; 

}



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


void get_mag_upsidedown(float &head){
  
  byte buff[6]; 
  readMAG(buff);
  int mx = bytes_to_int(buff[0] , buff[1] );   
  int my = bytes_to_int(buff[2] , buff[3] );
  int mz = bytes_to_int(buff[4] , buff[5] );

  //Compute heading  
  //head = atan2(my,mx) *RAD_TO_DEG;
  head = atan2(mx,my) *RAD_TO_DEG;
  
  //Convert heading to 0 - 360
  if(head < 0) head += 360;            

}


