#include <SPI.h>
#include <WiFiNINA.h>
#include "secrets.h"

/*
//Add YOUR Network configs in "secrets.h":
#define SECRET_SSID "netwrk"
#define SECRET_PASS "pass"
#define SECRET_PORT 1234
#define SECRET_SERVER_IP "ip"
 */
#include "configs.h"


WiFiClient client;
int status = WL_IDLE_STATUS;

char ssid[]=SECRET_SSID;
char pass[]=SECRET_PASS;
unsigned int localPort = SECRET_PORT;      // local port to listen on
char servername[]=SECRET_SERVER_IP;  // remote server we will connect to


char start='{';
char last='}';
char comma=',';

char* id[]={
  "\"ms\":", //time
  "\"te\":", //telem
  "\"cx\":", //measured x
  "\"cy\":", //measured y
  "\"cz\":", //measured x
  "\"dx\":", //desired x
  "\"dy\":", //desired y
  "\"dz\":", //desired z
  "\"dt\":", //desired throttle
};


///////////////////////////////////////////////////////
//WIFI
void get_command(void){

  if(ENABLE_WIFI==true){
    if (client.available()) {
      Serial.print("Got Command: ");
      command = client.read();
      
      Serial.print(command);
      Serial.print(" at Epoch: ");
      Serial.print(epoch);
      Serial.println("");
      client.flush();
      
      get_desired();
      //get_numeric_command();
    }
  }
  else{
    if (Serial.available() > 0) {
      command = Serial.read();
      Serial.print("Got Command: ");
      Serial.print(command);
      Serial.print(" at Epoch: ");
      Serial.print(epoch);
      Serial.println("");

      get_desired();
      //get_numeric_command();
    }
  }  


}



void get_desired(void){
  get_desired_raw();
  get_flight_desired();
  //if (flight_mode==flight)get_flight_desired();
  //else if (flight_mode!=flight)get_idle_desired();
     
  
}


void get_desired_raw(void){
  desired_raw.roll=0.0;
  desired_raw.pitch=0.0;
  desired_raw.yaw=0.0;
  desired_raw.yaw=0.0;
  desired_raw.throttle=0.0;
  
  float diff=5.0;
//  flaot tdiff=
    switch(command){
      case '8'://pitch forward
        desired_raw.pitch=-diff; 
        break;
       case '2'://pitch back
        desired_raw.pitch=diff; 
        break;
       case '4'://roll left
        desired_raw.roll=-diff; 
        break;
       case '6'://roll right
        desired_raw.roll=+diff; 
        break;
       case '7'://yaw ccw
        desired_raw.yaw=-diff;        
        break;
       case '9'://yaw cw
        desired_raw.yaw=diff;        
        break;
      case '-'://throttle down
        desired_raw.throttle=-diff;        
        break;

      case '+'://throttle up
        desired_raw.throttle=diff;        
        break;
      case '5':default:
        //get_idle_desired();
      break;

    case '0': case'k'://kill / idle
      set_all_motors(motor_min);
      flight_mode=idle;
      desired.throttle=motor_min;      
      Serial.println("Entering IDLE Mode");
    break;
    case 'c'://calibration
      if (flight_mode==idle){
        flight_mode=esc_calibration;  
        Serial.println("Entering Calibration Mode");      
      }   
    break;




        //switches based on key pressed to get the command mode
    case 'o'://orientation_mode
      if (flight_mode==idle){
        flight_mode=orientation_mode;     
        Serial.println("Entering orientation_mode Mode");
      }    
    break;

    case 'm'://diect throttle (no PID corrections)
      if (flight_mode==idle){
        flight_mode=motor_direct;  
        Serial.println("Entering motor direct throttle test Mode");      
      }    
    break;

    case 'f'://fly
      if (flight_mode==orientation_mode||flight_mode==hover){
        flight_mode=flight;   
        Serial.println("Entering flight Mode");     
      }    
    break;

    /*
    case 'O'://orientation_mode
      if (flight_mode==idle){
        flight_mode=test_orientation_mode;     
        Serial.println("Entering orientation_mode Mode");
      }    
    break;
    
    
    
    
    case 'C'://calibration
      if (flight_mode==idle){
        flight_mode=idle;  
        Serial.println("Entering Calibration Mode"); 
        //high_calibration();
             
      }   
    break;

    case 't':
      if (flight_mode==idle){
        flight_mode=throttle_test;  
        Serial.println("Entering throttle test Mode");      
      }    
    break;
    


    case 'q':
      if (flight_mode==idle){
        flight_mode=transform_test;  
        Serial.println("Entering transform test Mode");      
      }    
    break;
    
    case 's':
      if (flight_mode==idle){
        flight_mode=sensor_test;  
        Serial.println("Entering sensor test Mode");      
      }    
    break;

    case 'r':
      if (flight_mode==idle){
        flight_mode=error_roll_test;  
        Serial.println("Entering error roll test Mode");      
      }    
    break;

    case 'p':
      if (flight_mode==idle){
        flight_mode=error_pitch_test; 
        Serial.println("Entering error pitch test Mode");      
      }    
    break;
    case 'y':
      if (flight_mode==idle){
        flight_mode=error_yaw_test;  
        Serial.println("Entering error yaw test Mode");      
      }    
    break;
    case 'g':
      if (flight_mode==idle){
        flight_mode=general_test;  
        Serial.println("Entering General test Mode");      
      }    
      
      
      pwm_t();on=not on;
    break;

    case 'd':
      if (flight_mode==idle){
        flight_mode=desired_test;  
        Serial.println("Entering desired test Mode");      
      }    
    break;




*/

/*
    case 'h'://hover
      if (flight_mode==hover){//TODO
        flight_mode=hover;   
        Serial.println("Entering hover Mode");     
      }    
    break;*/
      

    }
}



void get_flight_desired(void){

  desired.roll=desired_raw.roll+offset.roll;  
  desired.pitch=desired_raw.pitch+offset.pitch;
  
  desired.yaw=desired.yaw+desired_raw.yaw;//offset already applied in yaw
  if (desired.yaw>360.0)desired.yaw-=360;
  else if(desired.yaw<0.0)desired.yaw+=360;    

  
  desired.throttle=desired.throttle+desired_raw.throttle;//capped later, leave raw for now
  
  
}





void send_telem(float ms){
  if(ENABLE_WIFI){
    String str ="";
    
    str.concat(start);  
    str.concat(id[0]);
    str.concat(ms);
    str.concat(last);  
    //client.println(str);
    //Serial.println(str);
  
    
    if (((float)epoch*LOOP_PERIOD)>=0.5){
      kill_client();
      while(1);
    }
  }
  
}

void kill_client(void){
  Serial.println("Killing Client");
  client.stop();
}




////////////////////////////////////////////
//SERIAL












/////////////////////////////////////////////////////////////////////////////








/////////////////////////////////////////////////////////////////////////////////
//WIFI SETUP
void say_hello(void) {
  if(ENABLE_WIFI){
    
    Serial.println("Assessing connection");
  
    if (client.connected()){
      Serial.println("attempting to send message");
      client.println("{\"hello\":\"Vidor1\"}");
    }
    else{
      Serial.println("No Connection");
    }
    Serial.println("Waiting for reply.");
    delay(1000);
    Serial.println("Reading connection");
    while (client.available()) {
      char c = client.read();
      Serial.print(c);
    }
    Serial.println("");
  }

  else{
    Serial.println("No WIFI in setup");
  }



}



void setup_wifi(void) {
  desired_raw.roll=0.0;
  desired_raw.pitch=0.0;
  desired_raw.yaw=0.0;
  desired_raw.throttle=0.0;

  desired.roll=0.0;
  desired.pitch=0.0;
  desired.yaw=0.0;  
  desired.throttle=motor_min;
  
  if(ENABLE_WIFI){
    // check for the WiFi module:
    if (WiFi.status() == WL_NO_MODULE) {
      Serial.println("Communication with WiFi module failed!");
      // don't continue
      while (true);
    }
  
    String fv = WiFi.firmwareVersion();
    if (fv < "1.0.0") {
      Serial.println("Please upgrade the firmware");
    }
  
    // attempt to connect to Wifi network:
    while (status != WL_CONNECTED) {
      Serial.print("Attempting to connect to SSID: ");
      Serial.println(ssid);
      // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
      status = WiFi.begin(ssid, pass);
  
      // wait 10 seconds for connection:
      delay(10000);
    }
    Serial.println("Connected to wifi");
  
    
  
    Serial.println("\nStarting connection to server...");
    if (client.connect(servername, localPort)) {
        Serial.println("connected");
        // Make a HTTP request:
        //client.println("GET /search?q=arduino HTTP/1.0");
        //client.println();
    }
    else{
      Serial.println("Error: Not connected!!");
    }
  
    say_hello();
      
  }
}
