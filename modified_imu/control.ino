#include <Adafruit_PWMServoDriver.h>
#include "configs.h"

//globals
//global variables, objects and structs    
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
Attitude desired_raw, desired, measured, pid_result;
Throttle throttle;
PidError eroll, epitch, eyaw, eheight;
PidConstants kroll,kpitch,kyaw, kheight;




/////////////////////////////////////////////////
//ALL (THE BIG HITTERS)

void idle_control(void){
    set_all_motors(motor_min);   
}

void throttle_control(void){
    set_all_motors(motor_min);  //TODO 
}


void flight_control(void){
  get_measured();
  get_all_errors();
  get_all_pid();
  apply_all_errors();
  send_throttle_to_motors();    
}

void get_all_pid(void){
  pid_result.roll=get_pid_result(eroll,kroll);
  pid_result.pitch=get_pid_result(epitch,kpitch);  
  pid_result.yaw=get_pid_result(eyaw,kyaw);
  
}

//gets errors for Attitudes
void get_all_errors(void){
  get_single_error(eroll, normalise(measured.roll - desired.roll));
  get_single_error(epitch, normalise(measured.pitch - desired.pitch));  
  get_single_error(eyaw, normalise(measured.yaw - desired.yaw));
  
  //get_height_error();
}


float normalise(float e){//d, float m){

  //float e=m-d;

  if (e>180.0)e=e-360;
  else if (e<-180.0)e=e+360;
  return e;

}



//gets (kp*p+ki*i+kd*d) and applies corrections to all throttle values
void apply_all_errors(){
    throttle.fl=desired.throttle;//base the throttle from desired
    throttle.fr=desired.throttle;
    throttle.bl=desired.throttle;
    throttle.br=desired.throttle;
    
    apply_roll( );
    apply_pitch( );
    apply_yaw( );
    //apply_height( );
   
    limit_throttle();//dont get out of hand now children...
}





void pwm_t(void){

  if (not on){
    int channel=0;
    int val=4096;
    for (channel=0; channel<16;channel++){    
      pwm.setPWM(channel, val, 0);  
      Serial.print("setting_motor_to all on:"); Serial.print(val);
      Serial.println();
    }   
  }
  else if (on){
    int channel=0;
    int val=4096;
    for (channel=0; channel<16;channel++){    
      pwm.setPWM(channel, 0,val);  
      Serial.print("setting_motor_to all off:"); Serial.print(val);
      Serial.println();
    }   
  }
}

///////////////////////////////////////////////////////
//ERROR
void get_single_error(PidError & err, float temp ){
  
  //if(desired-measured>180.0)desied-=180.0;//normalise desired to get the closest path to stability
  //else if(measured-desired>180.0)desied+=180.0;  
  //float temp=measure-desire;
  //err.d=(temp-err.p)/LOOP_PERIOD;
  
  err.p=temp;
  
  //err.i+=temp*LOOP_PERIOD;
  if (err.i>=i_error_max)err.i=i_error_max;
  else if (err.i<=-i_error_max)err.i=-i_error_max;

}




//////////////////////////////////////////////////////////////////////////
//PID
//calculates kp*p+ki*i+kd*d and returns result after max and min is applied
float get_pid_result(PidError err, PidConstants constant){
  float result=0.0;      
 
  result+=constant.kp*err.p;//P
  result+=constant.ki*err.i;//I
  result+=constant.kd*err.d; //D

  return result;   
}
    

    
    
    
/////////////////////////////////////////////////
//sides
void apply_roll(){
//positive roll error means we are rolloed too far right, increase right throttle
  throttle.fl-=pid_result.roll;
  throttle.bl-=pid_result.roll;
  
  throttle.fr+=pid_result.roll;
  throttle.br+=pid_result.roll;

    
}

//front/back 
void apply_pitch(){

//positive pitch error means we are pitched too far forward(los is up and the craft is moving backward), increase back throttle
  throttle.fl-=pid_result.pitch;  
  throttle.fr-=pid_result.pitch;  
  
  throttle.bl+=pid_result.pitch;
  throttle.br+=pid_result.pitch;

  
}

//opposites
void apply_yaw(){

  //positive yaw error means we are to far cw, yaw the craft ccw by increasing throttle to cw motors(fl, and br)
  //cw motor spin
  throttle.fl+=pid_result.yaw;  
  throttle.br+=pid_result.yaw;

  //ccw motor spin
  throttle.fr-=pid_result.yaw;    
  throttle.bl-=pid_result.yaw;
  

  
  
}






//////////////////////////////////////////////////
//cap at max and min
void limit_throttle(void){
  if (throttle.fl>=motor_max)throttle.fl=motor_max;
  else if (throttle.fl<=motor_min)throttle.fl=motor_min;
  
  if (throttle.fr>=motor_max)throttle.fr=motor_max;
  else if (throttle.fr<=motor_min)throttle.fr=motor_min;
  
  if (throttle.bl>=motor_max)throttle.bl=motor_max;
  else if (throttle.bl<=motor_min)throttle.bl=motor_min;
  
  if (throttle.br>=motor_max)throttle.br=motor_max;
  else if (throttle.br<=motor_min)throttle.br=motor_min;
}



    
   
    
    
    
    
/////////////////////////////////////////////////
//CALIBRATION    
void calibrate_esc(void){
  Serial.flush();    
  set_all_motors(motor_max);

  Serial.println("plug in ESCs, beeps will increase in pitch, followed by a beep matching the pitch, press any key to continue...");
  while(!Serial.available());
  Serial.flush();
  delay(100);  while(Serial.available()) Serial.read(); delay(100);
  
  int i;
  for(i=motor_max; i>=motor_min; i=i-1){
    set_all_motors(i);
    Serial.println(i);
    delay(1);
  }
  set_all_motors(motor_min);
  
  Serial.println("Throtled down to the low end, two beeps of the same pitch should be emitted, press any key to cont");      
  while(!Serial.available());  
  //if (Serial.read()=='n') return;
  Serial.flush();
  delay(100);  while(Serial.available()) Serial.read(); delay(100);

  Serial.println("Done With Calibration");  
  
}









///////////////////////////////////////////////
//SETUP
void setup_pid(void){
  //just set these to 0 for startup!    
  eroll.p=0.0;
  eroll.i=0.0;
  eroll.d=0.0;        
  epitch.p=0.0;
  epitch.i=0.0;
  epitch.d=0.0;        
  eyaw.p=0.0;
  eyaw.i=0.0;
  eyaw.d=0.0;             
  measured.roll=0.0;
  measured.pitch=0.0;
  measured.yaw=0.0;
  
  //Tune-able Parameters!!! 
  float kp=0.5;
  float ki=0.0;
  float kd=0.0;
  
  kroll.kp=kp;
  kroll.ki=ki;
  kroll.kd=kd;    
  //kroll.min= motor_max-motor_start *  -0.1 ;
  //kroll.max= motor_max-motor_start *   0.1 ;

  kpitch.kp=kp;
  kpitch.ki=ki;
  kpitch.kd=kd;
  //kpitch.min= motor_max-motor_start * -0.1 ;
  //kpitch.max= motor_max-motor_start *  0.1 ;

  //kyaw.kp=0.05;
  kyaw.kp=0.00;
  kyaw.ki=0.0;
  kyaw.kd=0.0;
  //kyaw.min= motor_max-motor_start  *  -0.1 ;
  //kyaw.max= motor_max-motor_start *  0.1 ;

  kheight.kp=0.0;
  kheight.ki=0.0;
  kheight.kd=0.0;    
  //kheight.min= motor_max-motor_start *  -0.1 ;
  //kheight.max= motor_max-motor_start *   0.1 ;

}





