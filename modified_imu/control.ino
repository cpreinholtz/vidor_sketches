#include <Adafruit_PWMServoDriver.h>
#include "configs.h"

//globals
//constants
/////////////////////////////////////////////////////////////////
  const float kp =1.0;   
  //0.5 feels too weak? no oscillation though 
  //kp =0.55;  very slow oscilation, probably the best so far
  //0.7 feels good, oscilation, present
  //0.6 feels nice with slight oscilation
  
  const float ki =0.00;//this is multiplied by LOOP_PERIOD
  //0.35 seems ok, maybe too high, I am getting oscilations
  //.05 feels too weak? too slow?
  //.15 feels good with kp = 1.0 and kd = 0.02.  think i need a bit more ki and less kd???

  
  const float kd =0.00;//this is divided by LOOP_PERIOD
  //better with 0.01
  //felt ok with 0.02
//////////////////////////////////////////////////////////

//global variables, objects and structs    
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

Attitude measured, pid_result;
Attitude offset={0.0,0.0,0.0};

AttitudeAndPower desired_raw, desired;
AttitudeAndPower mid_stick={0.0,0.0,0.0,0.0};

Throttle throttle;
PidError eroll, epitch, eyaw, eheight;
PidConstants kroll, kpitch, kyaw, kheight;




/////////////////////////////////////////////////
//ALL (THE BIG HITTERS)





void flight_control(void){  
  get_measured();//handled in sensor.ino
  get_flight_desired();//handled in comms.ino
  get_all_errors();
  get_all_pid();
  apply_all_errors();
  send_throttle_to_motors();//handled in motor.ino
     
}



//gets errors for Attitudes
void get_all_errors(void){
  get_single_error(eroll, normalise(measured.roll - desired.roll));
  get_single_error(epitch, normalise(measured.pitch - desired.pitch));  
  get_single_error(eyaw, normalise(measured.yaw - desired.yaw));
  
  //get_height_error();
}



void get_all_pid(void){
  pid_result.roll=get_pid_result(eroll,kroll);
  pid_result.pitch=get_pid_result(epitch,kpitch);  
  pid_result.yaw=get_pid_result(eyaw,kyaw);  
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




//////////////////////////////////////////
//Subroutines
//////////////////////////////////////////


///////////////////////////////////////////////////////
//ERROR AND PID
void get_single_error(PidError & err, float temp ){
  
  //if(desired-measured>180.0)desied-=180.0;//normalise desired to get the closest path to stability
  //else if(measured-desired>180.0)desied+=180.0;
  
  //float temp=measure-desire;
  err.d=(temp-err.p)/LOOP_PERIOD;
  
  err.p=temp;
  //if(!liftoff){
   // err.i=0.0;
  //}else{
    err.i+=temp*LOOP_PERIOD;
    if (err.i>=i_error_max)err.i=i_error_max;
    else if (err.i<=-i_error_max)err.i=-i_error_max;
  //}
}

float normalise(float e){//d, float m){

  //float e=m-d;

  if (e>180.0)e=e-360;
  else if (e<-180.0)e=e+360;
  return e;

}

/*
//accounts for the 0-360 range near north
void get_yaw_error(PidError & err, float desire, float measure){

  
  float temp=measure-desire;
  err.d=temp-err.d;
  err.i+=temp;
  err.p=temp;
  if(!liftoff){
    err.i=0.0;
  }
}*/

//calculates kp*p+ki*i+kd*d and returns result after max and min is applied
float get_pid_result(PidError err, PidConstants constant){
  float result=0.0;      
 
  result+=constant.kp*err.p;//P
  result+=constant.ki*err.i;//I
  result+=constant.kd*err.d; //D
  
  /*if (result>=constant.max){
    result=constant.max;
    Serial.println("PID MAX HIT!");
  }
  else if(result<=constant.min){
    result=constant.min;
    Serial.println("PID MIN HIT!");
  }*/
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

/*
//all4
void apply_height(Throttle &throt, float res){

  throt.fr+=res;//fr spins ccw,
  throt.bl+=res;//bl spins ccw,
  throt.fl+=res; //fl spins cw, 
  throt.br+=res; //br spins cw,
        
}*/

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



    
///////////////////////////////////////////////////////
//DEBUG ONLY
    
void throttle_control(void){
  set_all_motors(motor_min);  //TODO 
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

  kyaw.kp=0.00;
  kyaw.ki=0.0 * LOOP_PERIOD;
  kyaw.kd=0.0 / LOOP_PERIOD;
  //kyaw.min= motor_max-motor_start  *  -0.1 ;
  //kyaw.max= motor_max-motor_start *  0.1 ;

  kheight.kp=0.0;
  kheight.ki=0.0 * LOOP_PERIOD;
  kheight.kd=0.0 / LOOP_PERIOD;    
  //kheight.min= motor_max-motor_start *  -0.1 ;
  //kheight.max= motor_max-motor_start *   0.1 ;

}





