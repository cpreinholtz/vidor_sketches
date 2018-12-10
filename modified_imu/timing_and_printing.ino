#include "configs.h"


void regulate_time(unsigned long startTime){  

  
  //DUMMY ITEMS  
  if (epoch%20==0) digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level);
  else digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  const int slow_loop= 0;//ms additional delay
  delay(slow_loop);


  
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























void print_x(void){
  Serial.print("gx: ");Serial.print(gx);Serial.print("\t\t");
  Serial.print("ax: ");Serial.print(ax);Serial.print("\t\t");
  Serial.print("px: ");Serial.print(px);Serial.print("\t\t");
  Serial.print("cx: ");Serial.print(cx);Serial.print("\t\t");
  Serial.println("");
}

void print_y(void){
  Serial.print("gy: ");Serial.print(gy);Serial.print("\t\t");
  Serial.print("ay: ");Serial.print(ay);Serial.print("\t\t");
  Serial.print("py: ");Serial.print(py);Serial.print("\t\t");
  Serial.print("cy: ");Serial.print(cy);Serial.print("\t\t");
  Serial.println("");
}

void print_z(void){
  Serial.print("gz: ");Serial.print(gz);Serial.print("\t\t");
  Serial.print("mh: ");Serial.print(h);Serial.print("\t\t");
  Serial.print("pz: ");Serial.print(pz);Serial.print("\t\t");
  Serial.print("cz: ");Serial.print(cz);Serial.print("\t\t");
  Serial.println("");
}




