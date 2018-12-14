#include "configs.h"

float watchdog_timeout=200.0;//ms
float watchdog=0.0;



void regulate_time(unsigned long startTime){  

  
  //DUMMY ITEMS  
  if (epoch%20==0) digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level);
  else digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  const int slow_loop= 0;//ms additional delay
  delay(slow_loop);

  if (throttle_isr_flag){
    throttle_isr_flag=false;
    watchdog= millis();
  }

  if (millis()-watchdog >watchdog_timeout){
    Serial.println("Controller Timeout!");
    if(ENABLE_WATCHDOG) {
      desired.throttle=motor_min; 
      send_all_to_motors(motor_min)
      flight_mode=idle;
    }
  }
  
  epoch = epoch +1;
  //Each loop should be  20ms. (ideally)
  if(millis() - startTime > (LOOP_PERIOD*1000) ){
    Serial.print("TOO SLOW!!!!: loop time (ms)= ");
    Serial.println(millis() - startTime);
  }
  
  while(millis() - startTime < (LOOP_PERIOD*1000))
  {
    delayMicroseconds(1);
  }
      
}







///////////////////////////////////////////////////
//General  prints
void print_Icart(Icart f){
  Serial.print("x: ");Serial.print(f.x);Serial.print("\t\t");  
  Serial.print("y: ");Serial.print(f.y);Serial.print("\t\t");  
  Serial.print("z: ");Serial.print(f.z);Serial.print("\t\t");  
  Serial.println("");
}


void print_Fcart(Fcart f){
  Serial.print("x: ");Serial.print(f.x);Serial.print("\t\t");  
  Serial.print("y: ");Serial.print(f.y);Serial.print("\t\t");  
  Serial.print("z: ");Serial.print(f.z);Serial.print("\t\t");  
  Serial.println("");
}


void print_Throttle(Throttle t){
  Serial.print("fl: ");Serial.print(t.fl);Serial.print("\t\t");  
  Serial.print("fr: ");Serial.print(t.fr);Serial.print("\t\t");  
  Serial.print("bl: ");Serial.print(t.bl);Serial.print("\t\t");  
  Serial.print("br: ");Serial.print(t.br);Serial.print("\t\t");
  Serial.println("");
}


void print_attitude(Attitude a){
  
  Serial.print("roll: ");Serial.print(a.roll);Serial.print("\t\t");
  Serial.print("pitch: ");Serial.print(a.pitch);Serial.print("\t\t");
  Serial.print("yaw: ");Serial.print(a.yaw);Serial.print("\t\t");
  Serial.print("throt: ");Serial.print(a.throttle);Serial.print("\t\t");
  Serial.println("");

}

void print_error(PidError e){
  Serial.print("ep: ");Serial.print(e.p);Serial.print("\t\t");
  Serial.print("ei: ");Serial.print(e.i);Serial.print("\t\t");
  Serial.print("ed: ");Serial.print(e.d);Serial.print("\t\t");
  Serial.println("");
}


///////////////////////////////////////////
//Dedicated prints
void print_measured(void){
  Serial.print("measured ");
  print_attitude(measured);
}
void print_offset(void){
  Serial.print("offset ");
  print_attitude(offset);
}
void print_desired(void){
  Serial.print("desired ");
  print_attitude(desired);
}

void print_desired_raw(void){
  Serial.print("desired_raw ");
  print_attitude(desired_raw);
}



void print_throttle(void){
  Serial.print("throttle ");
  print_Throttle(throttle);
}



void print_eroll(void){
  Serial.print("eroll ");
  print_error(eroll);
}
void print_epitch(void){
  Serial.print("epitch ");
  print_error(epitch);
}
void print_eyaw(void){
  Serial.print("eyaw ");
  print_error(eyaw);
}





void print_throttle_in(){
      Serial.print("high_time_uS: ");Serial.print(throttle_in.high_time);Serial.print("\t\t");
      Serial.print("low_time_uS: ");Serial.print(throttle_in.low_time);Serial.print("\t\t");
      Serial.print("rising_edge_uS: ");Serial.print(throttle_in.rising_edge);Serial.print("\t\t");
      Serial.print("falling_edge_uS: ");Serial.println(throttle_in.falling_edge);
}




/////////////////////////////////////
//XYZ
void print_x(void){
  Serial.print("gx: ");Serial.print(gyro_rate.x);Serial.print("\t\t");
  Serial.print("ax: ");Serial.print(acc_pos.x);Serial.print("\t\t");
  Serial.print("px: ");Serial.print(gyro_pos.x);Serial.print("\t\t");
  Serial.print("cx: ");Serial.print(cfilter.x);Serial.print("\t\t");
  Serial.println("");
}

void print_y(void){
  Serial.print("gy: ");Serial.print(gyro_rate.y);Serial.print("\t\t");
  Serial.print("ay: ");Serial.print(acc_pos.y);Serial.print("\t\t");
  Serial.print("py: ");Serial.print(gyro_pos.y);Serial.print("\t\t");
  Serial.print("cy: ");Serial.print(cfilter.y);Serial.print("\t\t");
  Serial.println("");
}

void print_z(void){
  Serial.print("gz: ");Serial.print(gyro_rate.z);Serial.print("\t\t");
  Serial.print("head: ");Serial.print(heading);Serial.print("\t\t");
  Serial.print("pz: ");Serial.print(gyro_pos.z);Serial.print("\t\t");
  Serial.print("cz: ");Serial.print(cfilter.z);Serial.print("\t\t");
  Serial.println("");
}




