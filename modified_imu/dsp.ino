
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




float G = 0.70;         // complementary filter constant
float A = (1-G);        // complementary filter constant



void cf(float &val, float a, float g){
  
  A= 1-G;

  val= g*G + a*A;

 
}




