#include <Adafruit_PWMServoDriver.h>
#include "configs.h"

void send_throttle_to_motors(void){
    if(flight_mode==flight&& motor_arm==true){
      if (ENABLE_PWM){
        pwm.setPWM(FL, 0, (int)throttle.fl);   
        pwm.setPWM(FR, 0, (int)throttle.fr);   
        pwm.setPWM(BL, 0, (int)throttle.bl);   
        pwm.setPWM(BR, 0, (int)throttle.br);  
      } 
    }
    else{
      set_all_motors(motor_min);
    }

}



void set_all_motors(int value){

  
  if (ENABLE_PWM==true){
    //Preadjust value!!!!!!
    if  (motor_arm==false) value=motor_min;
    else if (value<=motor_min) value=motor_min;
    else if (value>=motor_max) value=motor_max;   

    pwm.setPWM(FL, 0, value);   
    pwm.setPWM(FR, 0, value);   
    pwm.setPWM(BL, 0, value);   
    pwm.setPWM(BR, 0, value);  
      
    
  }

  
}

void setup_motors(void){
  if(ENABLE_PWM){//probably want to just send to low signal to disable esc!!
    pwm.begin();
    pwm.setPWMFreq(pwm_frequency);  // This is the maximum PWM frequency
    set_all_motors(motor_min);
    
  }
  else{
    Serial.println("No PWM in setup");
  }

  
  delay(10);
  
}

