/*
void print_conditional(float ax,float ay,float gx,float gy,float gz,float h,float cx,float cy, float px, float py, float pz){

  //float frequency =1/LOOPPERIOD;

  if (epoch%2==0){


    
    //if (Serial.available() > 0) {
      //incomingByte = Serial.read();
    //}
    switch (command){
    case 'x'://print x gyro and acc unfiltered
      Serial.print(" ax ");Serial.print(ax);
      Serial.print("\t\t");
      Serial.print(" gx ");Serial.print(gx);    
      Serial.println();
      break;
     
    case 'y':
      Serial.print(" ay ");Serial.print(ay);
      Serial.print("\t\t");
      Serial.print(" gy ");Serial.print(gy);    
      Serial.println();
  
      break;
  
     case 'h':
      Serial.print(" heading ");Serial.print(h); 
      Serial.print("\t\t");
      Serial.print(" gz ");Serial.print(gz);  
      Serial.println();
  
      break;
  
      case 'c':
      Serial.print(" cx ");Serial.print(cx); 
      Serial.print("\t\t");
      Serial.print(" ax ");Serial.print(ax);
      Serial.print("\t\t");
      Serial.print(" gx ");Serial.print(gx); 
      Serial.println();
      break;
      
      case 'C':
      Serial.print(" cy ");Serial.print(cy); 
      Serial.print("\t\t");
      Serial.print(" ay ");Serial.print(ay);
      Serial.print("\t\t");
      Serial.print(" gy ");Serial.print(gy); 
      Serial.println();
      break;
  
      case '-':
        if(G>0.02)G=G-0.01;
        else G = 0.01;
        command ='g';
      break;
      
      case '+':
        if(G<0.98)G=G+0.01;
        else G=0.99;
        command ='g';
      break;
      
      case 'g':
      Serial.print(" cx ");Serial.print(cx); 
      Serial.print("\t\t");
      Serial.print(" ax ");Serial.print(ax);
      Serial.print("\t\t");
      Serial.print(" gx ");Serial.print(gx);
      Serial.print("\t\t");
      Serial.print(" px ");Serial.print(px);
      Serial.print("\t\t");  
      Serial.print(" G_GAIN ");Serial.print(G);
      Serial.println();
      break;

      case 'G':
      Serial.print(" cy ");Serial.print(cy); 
      Serial.print("\t\t");
      Serial.print(" ay ");Serial.print(ay);
      Serial.print("\t\t");
      Serial.print(" gy ");Serial.print(gy);
      Serial.print("\t\t");
      Serial.print(" py ");Serial.print(py);
      Serial.print("\t\t");  
      Serial.print(" G_GAIN ");Serial.print(G);
      Serial.println();
      break;
      
      case 'p':
      Serial.print(" gx ");Serial.print(gx);
      Serial.print("\t\t");
      Serial.print(" px ");Serial.print(px);
      Serial.println();
      break;
      
    default:
  
      break;
     
    }
  
  }
  
}*/


/*
int create_json(void){
      String str ="";
      str.concat(start);
      
      str.concat(id[0]);
      str.concat(data.time);
      str.concat(comma);
      
      str.concat(id[1]);
      str.concat(data.x);
      str.concat(comma);
      //...
      str.concat(end);
}*/


/*
void test_wifi(void) {
  if(ENABLE_WIFI){
    
      Serial.println("Assessing connection");
    
      if (client.connected()){
        Serial.println("attempting to send message");
        client.println("hello, server");
      }
      else{
        Serial.println("No Connection");
      }
    
    Serial.println("Reading connection");
    while (client.available()) {
      char c = client.read();
      Serial.print(c);
    }
    Serial.println("");
  }



}
*/



/*
  Serial.print(" g ");Serial.print(g);
  Serial.print("\t\t");
  Serial.print(" G ");Serial.print(G);
  Serial.print("\t\t");
  Serial.print(" a ");Serial.print(a);
  Serial.print("\t\t");
  Serial.print(" A ");Serial.print(A);
  Serial.print("\t\t");
  Serial.print(" val ");Serial.print(val);
  Serial.println();
  */
  


/*

void print_acc(void){
    if (PRINT_ENABLE & VERBOSE){

    Serial.print(" buff0: ");Serial.print(buff[0]);
    Serial.print(" buff1: ");Serial.print(buff[1]);
    Serial.print(" buff2: ");Serial.print(buff[2]);
    Serial.print(" buff3: ");Serial.print(buff[3]);
    Serial.print(" buff4: ");Serial.print(buff[4]);
    Serial.print(" buff5: ");Serial.print(buff[5]);

    
    Serial.print(" accraw0: ");Serial.print(accRaw[0]);
    Serial.print(" accraw1: ");Serial.print(accRaw[1]);
    Serial.print(" accraw2: ");Serial.print(accRaw[2]);

    
    Serial.print(" #AccX:  ");
    Serial.print(AccXangle);
    Serial.print(" #AccY:  ");
    Serial.print(AccYangle);
    
    Serial.println();
  }
}





void print_all(void){
    if(PRINT_ENABLE){
      Serial.print("#AccX  ");
      Serial.print(AccXangle);
      Serial.print("  ###AccY  ");
      Serial.print(AccYangle);
  
  
      Serial.print("  ###GyrX");
      Serial.print( gyroXangle);
      Serial.print("  ###GyrY  ");
      Serial.print(gyroYangle);
      Serial.print("   ###GyrZ");
      Serial.print(gyroZangle);
  
      Serial.print("  ######CFangleX  ");
      Serial.print( CFangleX);
      Serial.print("  ######CFangleY  ");
      Serial.print(CFangleY);
  
      Serial.print("  ######  HEADING ");
      Serial.print( heading);
  }
}

*/
/*
  if (ax != accRaw[0] ){
    Serial.print(" ax:  ");Serial.print(ax,HEX);
    Serial.print(" accX:  ");Serial.print(accRaw[0],HEX);
    Serial.print("   <--- mismatch!!!!!!");
    Serial.println();

    Serial.print(" ax:  ");Serial.print(ax);
    Serial.print(" accX:  ");Serial.print(accRaw[0]);
    Serial.print("   <--- mismatch!!!!!!");
    Serial.println();
    delay(500);
  }
*/


/*
void get_idle_desired(void){
  //desired.roll=0.0;
  //desired.pitch=0.0;
  //desired.yaw=0.0;
  desired.roll=0.0+offset.roll;
  desired.pitch=0.0+offset.pitch;
  desired.yaw=desired_raw.yaw;
}*/


/*
float normalise(float throt){//takes a throtle uS value and normalises from 0-99

    if (throt<=motor_min) throt=0;
    else if (throt>=motor_max) throt=0;
}*/



/*
void regulate_utime(unsigned long startTime){

  epoch = epoch +1;

  //Each loop should be at least 20ms.
  while(micros() - startTime < (LOOP_PERIOD*1000000))
  {
    delayMicroseconds(1);
  }

}*/
