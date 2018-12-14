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
//bool oriented=false;
//bool liftoff=false;
int flight_mode=idle;
bool motor_arm=false;

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


bool on=false;

void dostuff(void){
  switch(flight_mode){

////////////////////////////////////////////
//Deliverable modes    
    case idle:
      motor_arm=false;
      desired.throttle=motor_min;     
      set_all_motors(motor_min);    
    break;   
     
    case orientation_mode: 
      motor_arm=false;
      set_all_motors(motor_min);       
      desired.throttle=(motor_min);
      calc_offsets(); 
      print_offset(); 
    break;    
    
    case flight:
      motor_arm=true;
      flight_control();
      //print_x();
    break;  
    
////////////////////////////////////

    case esc_calibration:
      motor_arm=true;
      calibrate_esc();
      flight_mode=idle;  
    break;   
     
    case motor_direct:
      motor_arm=true;
      set_all_motors(desired.throttle);      
      print_desired();
    break;    

//////////////////////////////
//tests
    
    case general_debug:
      motor_arm=false;
      flight_control();      
      switch(command){
        //already used characters: pid kof mc
        //used here: xyz tT ds
        
        case'x':
          print_eroll(); break;          
        case'y':
          print_epitch(); break;
        case'z':
          print_eyaw(); break;
          
        case't':
          print_throttle(); break;
        case'T':
          print_throttle_in(); break;
          
        case 'd':
          print_desired();break;
        case 's':
          print_measured();break;
          
        
        default:
          Serial.println("Unrecogniosed command"); break;
      }    
    break;

//////////////////////////////

 
    default:
      motor_arm=false;
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
  //setup_bmp();
  setup_pid();
  setup_motors();
  interrupt_setup();

  

 
  
}
