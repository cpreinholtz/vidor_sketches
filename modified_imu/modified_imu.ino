/*
  This program  reads the angles and heading from the accelerometer, gyroscope
   and compass on a BerryIMU connected to an Arduino.
  
    Both the BerryIMUv1 and BerryIMUv2 are supported.
    Feel free to do whatever you like with this code
    Distributed as-is; no warranty is given.
    http://ozzmaker.com/
*/
/*
  Modified by cpreinholtz in accordance to control theory
*/
#include "configs.h"





//pre flight checklist
bool oriented=false;
bool liftoff=false;
int flight_mode=idle;


//int command_mode=idle;
int epoch=0;
char command='k';


///////////////////////////////////////////////////////////////////////////////
void loop() {

  unsigned long start_loop ;

  while(1){
    start_loop = millis(); 
          
    dostuff();
    get_command();//this covers desired
    

    regulate_time(start_loop);  

    
  }//while (1)
}




void dostuff(void){
  switch(flight_mode){
    
//Deliverable modes    
    case idle:
      //no measured      
      idle_control();      
    break;
        
    case esc_calibration:
      calibrate_esc();
      flight_mode=idle;  
    break;
        
    case orientation_mode: 
      idle_control();  
      calc_offsets(); 
      print_offset(); 
    break;

    case flight:
      flight_control();
      print_x();
    break;


//////////////////////////////
//tests
    case sensor_test:
      flight_control();
      print_measured();
    break;
    
    case desired_test:
      flight_control();
      print_desired();
      //print_desired_raw();
    break;
    
    case error_test:
      flight_control();
      print_eroll();
    break;

    
    case throttle_test:
      flight_control();
      print_throttle();
    break;

//////////////////////////////

 
    default:
      Serial.println("unsuporrted FLIGHT_MODE");
      flight_mode=idle;
    break;
  }


  
}




///////////////////////////////////////////////////////////////////////////////
void setup() {
         // join i2c bus (address optional for master) 
  flight_mode=idle; 
  Serial.begin(115200);  // start serial for output  
  delay(1000);
  pinMode(LED_BUILTIN, OUTPUT);
  
  setup_wifi();
  setup_imu();
  setup_bmp();
  setup_pid();
  setup_motors();

  

 
  
}







