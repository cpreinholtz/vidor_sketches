#include <Adafruit_PWMServoDriver.h>
#include "configs.h"

void send_throttle_to_motors(void){
    if(flight_mode==flight){
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
  if (ENABLE_PWM){
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

