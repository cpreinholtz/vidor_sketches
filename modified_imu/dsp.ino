
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




const float GA = 0.97;         // complementary filter constant
const float AA = (1-GA);        // complementary filter constant



void compute_compliment(float &val, float a, float g){
    //Complementary filter used to combine the accelerometer and gyro values.  
  /////////////////////////////////////////////////////////////////////
  //OZZMAKER
  //CFangleX=( GA*( CFangleX+(rate_gyr_x *DT) ) ) + ((AA) * AccXangle);
  //CFangleY=( GA*( CFangleY+(rate_gyr_y *DT) ) ) + ((AA) * AccYangle);
  //CPR
  //CFangleX=( GA* ( ( CFangleX+rate_gyr_x ) *DT ) ) + ( ( AA ) * AccXangle );
  //CFangleY=( GA* ( ( CFangleY+rate_gyr_y ) *DT ) ) + ( ( AA ) * AccYangle );
  /////////////////////////////////////////////////////////////////////


 
}
