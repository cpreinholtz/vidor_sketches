


void regulate_time(unsigned long startTime){  

  
  //DUMMY ITEMS  
  if (epoch%20==0) digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level);
  else digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  const int slow_loop= 0;//ms additional delay
  delay(slow_loop);


  
  epoch = epoch +1;
  //Each loop should be  20ms.
  if(millis() - startTime > (LOOP_PERIOD*1000) ){
    Serial.print("TOO SLOW!!!!: loop time (ms)= ");
    Serial.print(millis() - startTime);
  }
  
  while(millis() - startTime < (LOOP_PERIOD*1000))
  {
    delay(1);
  }
      
}



void regulate_utime(unsigned long startTime){

  epoch = epoch +1;

  //Each loop should be at least 20ms.
  while(micros() - startTime < (LOOP_PERIOD*1000000))
  {
    delayMicroseconds(1);
  }
        
}
