
byte incomingByte ='0';






void print_conditional(float ax,float ay,float gx,float gy,float gz,float h){
  if (Serial.available() > 0) {
    incomingByte = Serial.read();
  }
  switch (incomingByte){
  case 'x'://print x gyro and acc unfiltered
    Serial.print(" ax ");Serial.print(ax);
    Serial.print("\t\tgx ");Serial.print(gx);    
    Serial.println();
    break;
   
  case 'y':
    Serial.print(" ay ");Serial.print(ay);
    Serial.print("\t\tgy ");Serial.print(gy);    
    Serial.println();

    break;

   case 'h':
    Serial.print(" heading ");Serial.print(h); 
    Serial.print("\t\tgz ");Serial.print(gz);  
    Serial.println();

    break;
    
  default:

    break;
   
  }
  
}
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

