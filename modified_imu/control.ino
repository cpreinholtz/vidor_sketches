#include <Adafruit_PWMServoDriver.h>
#include "configs.h"

//globals
//global variables, objects and structs    
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
Attitude desired, measured;
Throttle throttle;
PidError eroll, epitch, eyaw, eheight;
PidConstants kroll,kpitch,kyaw, kheight;



/////////////////////////////////////////////////
//ALL (THE BIG HITTERS)


void flight_control(void){
    get_all_errors();
    apply_all_errors();
    send_all_to_motors();    
}


//gets errors for Attitudes
void get_all_errors(void){
    get_single_error(eroll,desired.roll,measured.roll);
    get_single_error(epitch,desired.pitch,measured.pitch);
    get_yaw_error(eyaw,desired.yaw,measured.yaw);
}

//gets (kp*p+ki*i+kd*d) and applies corrections to all throttle values
void apply_all_errors(){
    apply_roll( throttle, get_pid_result(eroll,kroll) );
    apply_pitch( throttle, get_pid_result(epitch,kpitch) );
    apply_yaw( throttle, get_pid_result(eyaw,kyaw) );
    apply_height( throttle, get_pid_result(eheight,kheight) );
    limit_throttle();
}


void send_all_to_motors(void){
    if(mode==flight||mode==takeoff){
      pwm.setPWM(FL, 0, (int)throttle.fl);   
      pwm.setPWM(FR, 0, (int)throttle.fr);   
      pwm.setPWM(BL, 0, (int)throttle.bl);   
      pwm.setPWM(BR, 0, (int)throttle.br);  
      

    }  

}



    

    

///////////////////////////////////////////////////////
//ERROR AND PID
void get_single_error(PidError & err, float desire, float measure){
  
  //if(desired-measured>180.0)desied-=180.0;//normalise desired to get the closest path to stability
  //else if(measured-desired>180.0)desied+=180.0;
  
  float temp=desire-measure;
  err.d=temp-err.d;
  
  err.p=temp;
  if(mode!=flight){
    err.i=0.0;
  }else{
    err.i+=temp;
    if (err.i>=i_error_max)err.i=i_error_max;
    else if (err.i<=-i_error_max)err.i=-i_error_max;
  }
}


//accounts for the 0-360 range near north
void get_yaw_error(PidError & err, float desire, float measure){
  if(desire-measure>180.0)desire-=180.0;//normalise desired to get the closest path to stability
  else if(measure-desire>180.0)desire+=180.0;
  
  float temp=desire-measure;
  err.d=temp-err.d;
  err.i+=temp;
  err.p=temp;
  if(mode!=flight){
    err.i=0.0;
  }
}

//calculates kp*p+ki*i+kd*d and returns result after max and min is applied
float get_pid_result(PidError err, PidConstants constant){
  float result=0.0;      
 
  result+=constant.kp*err.p;//P
  result+=constant.ki*err.i;//I
  result+=constant.kd*err.d; //D
  
  if (result>=constant.max){
    result=constant.max;
    Serial.println("PID MAX HIT!");
  }
  else if(result<=constant.min){
    result=constant.min;
    Serial.println("PID MIN HIT!");
  }
  return result;   
}
    

    
    
    
/////////////////////////////////////////////////
//sides
void apply_roll(Throttle &throt, float res){
  if (res>0.0){   //roll right for + desired roll
    throt.fl+=res;
    throt.bl+=res;
  }
  else{
    throt.fr+=res;
    throt.br+=res;
  }            
}

//front/back 
void apply_pitch(Throttle &throt, float res){
  if (res>0.0){   //pitch forward up for + desired pitch
    throt.fl+=res;
    throt.fr+=res;
  }
  else{
    throt.bl+=res;
    throt.br+=res;
  }            
}

//opposites
void apply_yaw(Throttle &throt, float res){
  if (res>0.0){   // yaw cw for + desired heading
    throt.fr+=res;//fr spins ccw,
    throt.bl+=res;//bl spins ccw,
  }
  else{
    throt.fl+=res; //fl spins cw, 
    throt.br+=res; //br spins cw, 
  }            
}

//all4
void apply_height(Throttle &throt, float res){

  throt.fr+=res;//fr spins ccw,
  throt.bl+=res;//bl spins ccw,
  throt.fl+=res; //fl spins cw, 
  throt.br+=res; //br spins cw,
        
}

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
  if (mode==calibration){      
    set_all_motors(motor_max);
  }
  Serial.println("plug in ESCs, beeps will increase in pitch, followed by a beep matching the pitch, press any key to continue...");
  while(!Serial.available());
  Serial.flush();
  
  int i;
  for(i=motor_max; i>=motor_min; i--){
    set_all_motors(i);
    delay(1);
  }
  
  Serial.println("Throtled down to the low end, two beeps of the same pitch should be emitted, press any key to cont");      
  Serial.println("type \"n\" to skip the last step (sets throttle up to motor_start)");
  while(!Serial.available());
  
  if (Serial.read()=='n') return;
  Serial.flush();
  
  for(i=motor_min; i<=motor_start; i++){
    set_all_motors(i);
    delay(1);
  }
  
  Serial.println("Throtled up to motor_start, three beeps should be emitted, press any key to cont");
  while(!Serial.available());
  Serial.flush();

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
  desired.roll=0.0;
  desired.pitch=0.0;
  desired.yaw=0.0;        
  measured.roll=0.0;
  measured.pitch=0.0;
  measured.yaw=0.0;
  
  //Tune-able Parameters!!! 
  kroll.kp=1.0;
  kroll.ki=0.0 * LOOP_PERIOD;
  kroll.kd=0.0 / LOOP_PERIOD;    
  kroll.min= motor_max-motor_start *  -0.1 ;
  kroll.max= motor_max-motor_start *   0.1 ;

  kpitch.kp=1.0;
  kpitch.ki=0.0 * LOOP_PERIOD;
  kpitch.kd=0.0 / LOOP_PERIOD;
  kpitch.min= motor_max-motor_start * -0.1 ;
  kpitch.max= motor_max-motor_start *  0.1 ;

  kyaw.kp=0.2;
  kyaw.ki=0.0 * LOOP_PERIOD;
  kyaw.kd=0.0 / LOOP_PERIOD;
  kyaw.min= motor_max-motor_start  *  -0.1 ;
  kyaw.max= motor_max-motor_start *  0.1 ;

  kheight.kp=1.0;
  kheight.ki=0.0 * LOOP_PERIOD;
  kheight.kd=0.0 / LOOP_PERIOD;    
  kheight.min= motor_max-motor_start *  -0.1 ;
  kheight.max= motor_max-motor_start *   0.1 ;

}


void setup_motors(void){
  if(mode!=test){
    pwm.begin();
    pwm.setPWMFreq(pwm_frequency);  // This is the maximum PWM frequency

    
    if (mode!=calibration){  
      set_all_motors(motor_min);
    }
  }
  delay(10);
  
}

void set_all_motors(int value){

  if (value<=motor_min){
    pwm.setPWM(FL, 0, motor_min);   
    pwm.setPWM(FR, 0, motor_min);   
    pwm.setPWM(BR, 0, motor_min);   
    pwm.setPWM(BL, 0, motor_min);       
  }
  else if( value>=motor_max){
    pwm.setPWM(FL, 0, motor_max);   
    pwm.setPWM(FR, 0, motor_max);   
    pwm.setPWM(BL, 0, motor_max);   
    pwm.setPWM(BR, 0, motor_max);    
  }
  else{
    pwm.setPWM(FL, 0, value);   
    pwm.setPWM(FR, 0, value);   
    pwm.setPWM(BL, 0, value);   
    pwm.setPWM(BR, 0, value);  
  }
  
}


/*
float normalise(float throt){//takes a throtle uS value and normalises from 0-99

    if (throt<=motor_min) throt=0;
    else if (throt>=motor_max) throt=0;
}*/
