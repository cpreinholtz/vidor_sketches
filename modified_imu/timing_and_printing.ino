#include "configs.h"




const float watchdog_timeout=80.0;//ms

float watchdog=0.0;
bool controller_connected=false;
bool throttle_isr_flag=false, aux_isr_flag=false, roll_isr_flag=false, pitch_isr_flag=false, yaw_isr_flag=false;
bool aux_statechange_enable = true;


////////////////////////////////////////////////////////////
void regulate_time(void){  
  
  blink_mode();
  
  handle_intrupt_flags();
  
  watchdog_timer();

  poll_till_timeout();

      
}








void handle_intrupt_flags(void){
  
  if (throttle_isr_flag){
    throttle_isr_flag=false;
    controller_connected=true;
    watchdog= millis();
  }
  
  static float aux_in_avg =0.0;
  if (aux_isr_flag){
    
    aux_isr_flag=false;      
    aux_in_avg= aux_in_avg*0.8+ float(aux_in.high_time)*0.2;

    if (aux_statechange_enable==true){
      if ( aux_in_avg<aux_in_idle){
        if (  flight_mode == flight || flight_mode == orientation_mode){
          flight_mode=idle; 
          Serial.print(aux_in_avg); Serial.println(" Aux to idle!");   
        }  
      }
      else if (aux_in_avg < aux_in_orientation && aux_in_avg>=aux_in_idle){
        if(flight_mode == idle ) {
          flight_mode=orientation_mode; 
          Serial.print(aux_in_avg); Serial.println(" Aux to orientation!");
        }
      }
      else if ( aux_in_avg >= aux_in_orientation){
        if (flight_mode == orientation_mode && desired.throttle<motor_start) {
          flight_mode=flight;       
          Serial.print(aux_in_avg); Serial.println(" Aux to flight!");
        }
      }
    
    }    
  }  
}

void poll_till_timeout(void){
  //Regulate Time
  epoch = epoch +1;
  //Each loop should be  20ms. (ideally)
  if(millis() - start_loop > (LOOP_PERIOD*1000) ){
    Serial.print("TOO SLOW!!!!: loop time (ms)= ");
    Serial.println(millis() - start_loop);
  }  
  while(millis() - start_loop < (LOOP_PERIOD*1000))    delayMicroseconds(1);
}

void watchdog_timer(void){
  //WATCHDOG FOR CONTROLLER
  if (controller_connected==true && millis()-watchdog >watchdog_timeout){
    controller_connected=false;
    Serial.println("Controller Timeout! Switching to keyboard only control");
    if(ENABLE_WATCHDOG) {
      desired.throttle=motor_min; 
      set_all_motors(motor_min);
      flight_mode=idle;
    }
  }
}




void blink_mode(void){
  //LED Blinking
  if (flight_mode==idle) digitalWrite(LED_BUILTIN, HIGH);
  else if (flight_mode==orientation_mode ) epoch_blink(4);  
  else if (flight_mode==flight) digitalWrite(LED_BUILTIN, LOW);
  else epoch_blink(100);  
}


void epoch_blink(int rate){
  static bool on=false;
  if (epoch%rate==0) {
    if (on) {digitalWrite(LED_BUILTIN, HIGH); on=!on; }
    else {digitalWrite(LED_BUILTIN, LOW); on=!on; }
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

void print_aux_in(){
      Serial.print("high_time_uS: ");Serial.print(aux_in.high_time);Serial.print("\t\t");
      Serial.print("low_time_uS: ");Serial.print(aux_in.low_time);Serial.print("\t\t");
      Serial.print("rising_edge_uS: ");Serial.print(aux_in.rising_edge);Serial.print("\t\t");
      Serial.print("falling_edge_uS: ");Serial.println(aux_in.falling_edge);
}

void print_roll_in(){
      Serial.print("high_time_uS: ");Serial.print(roll_in.high_time);Serial.print("\t\t");
      Serial.print("low_time_uS: ");Serial.print(roll_in.low_time);Serial.print("\t\t");
      Serial.print("rising_edge_uS: ");Serial.print(roll_in.rising_edge);Serial.print("\t\t");
      Serial.print("falling_edge_uS: ");Serial.println(roll_in.falling_edge);
}

void print_pitch_in(){
      Serial.print("high_time_uS: ");Serial.print(pitch_in.high_time);Serial.print("\t\t");
      Serial.print("low_time_uS: ");Serial.print(pitch_in.low_time);Serial.print("\t\t");
      Serial.print("rising_edge_uS: ");Serial.print(pitch_in.rising_edge);Serial.print("\t\t");
      Serial.print("falling_edge_uS: ");Serial.println(pitch_in.falling_edge);
}
void print_yaw_in(){
      Serial.print("high_time_uS: ");Serial.print(yaw_in.high_time);Serial.print("\t\t");
      Serial.print("low_time_uS: ");Serial.print(yaw_in.low_time);Serial.print("\t\t");
      Serial.print("rising_edge_uS: ");Serial.print(yaw_in.rising_edge);Serial.print("\t\t");
      Serial.print("falling_edge_uS: ");Serial.println(yaw_in.falling_edge);
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




