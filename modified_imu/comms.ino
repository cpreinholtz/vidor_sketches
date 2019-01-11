#include <SPI.h>
#include <WiFiNINA.h>
#include "secrets.h"


#include "configs.h"
WiFiClient client;
/*
//Add YOUR Network configs in "secrets.h":
#define SECRET_SSID "netwrk"
#define SECRET_PASS "pass"
#define SECRET_PORT 1234
#define SECRET_SERVER_IP "ip"
 */





void get_flight_desired(void){

  //probably time to rethink yaw... new controller
  desired.yaw=desired.yaw+desired_raw.yaw;//offset already applied in yaw
  if (desired.yaw>360.0)desired.yaw-=360;
  else if(desired.yaw<0.0)desired.yaw+=360;    

  
  if ( ENABLE_RC_RECIEVER ==true and controller_connected==true){
    //desired.throttle=map(throttle_in.high_time,  throttle_in_min, throttle_in_max,  motor_min, motor_max);
    desired.throttle=map(throttle_in.high_time,  mid_stick.throttle, throttle_in_max,  motor_min, motor_max);
    desired.roll=float(map(roll_in.high_time,  mid_stick.roll-stick_travel, mid_stick.roll+stick_travel,  craft_angle_min, craft_angle_max))+offset.roll;
    desired.pitch=float(map(pitch_in.high_time,  mid_stick.pitch-stick_travel, mid_stick.pitch+stick_travel,  craft_angle_min, craft_angle_max))+offset.pitch;
    
  }
  else{
    desired.throttle=desired.throttle+desired_raw.throttle;//capped later, leave raw for now  
    desired.roll=desired_raw.roll+offset.roll;  
    desired.pitch=desired_raw.pitch+offset.pitch;
  }
  
  
}









////////////////////////////////////////////
//SERIAL
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
      
      get_desired_serial();
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

      get_desired_serial();
      //get_numeric_command();
    }
  }  


}


static void get_desired_serial(void){
  desired_raw.roll=0.0;
  desired_raw.pitch=0.0;
  desired_raw.yaw=0.0;
  desired_raw.yaw=0.0;
  desired_raw.throttle=0.0;
   String infloat="";
  
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
      case '5':
        //get_idle_desired();
      break;

    case '0': case'k'://kill / idle
      set_all_motors(motor_min);
      flight_mode=idle;
      desired.throttle=motor_min;      
      Serial.println("Entering IDLE Mode, state chage disabled without K");
      aux_statechange_enable=false;
    break;
    case 'K':
      set_all_motors(motor_min);
      flight_mode=idle;
      desired.throttle=motor_min; 
      if (aux_in.high_time<aux_in_idle && controller_connected){
        aux_statechange_enable=true;     
        Serial.println("Allowing aux state change enable");
      }
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
        //aux_statechange_enable=true;
      }    
    break;

    case 'm'://diect throttle (no PID corrections)
      if (flight_mode==idle){
        flight_mode=motor_direct;  
        Serial.println("Entering motor direct throttle test Mode");      
      }    
    break;


    case 'f'://fly
      if (flight_mode==orientation_mode){
        flight_mode=flight;   
        Serial.println("Entering flight Mode");     
      }    
    break;


    
    
    case 'P'://fly
      Serial.print("Adjusting p:");    
      delay(100);
      infloat="";
      while (Serial.available()>0){
        char inchar=Serial.read( );
        if  (inchar != '\n') infloat+=inchar;
      }
      kroll.kp=infloat.toFloat();
      kpitch.kp=infloat.toFloat();
      //print_PidConstants(kroll);  
      //print_PidConstants(kpitch);
    break;
    
    case 'I'://fly
      Serial.print("Adjusting i:");     
      delay(100);
      infloat="";
      while (Serial.available()>0){
        char inchar=Serial.read( );
        if  (inchar != '\n') infloat+=inchar;
      }
      kroll.ki=infloat.toFloat();
      kpitch.ki=infloat.toFloat();
      //print_PidConstants(kroll); 
      //print_PidConstants(kpitch); 
    break;
    
    case 'D'://fly
      Serial.print("Adjusting d:");   
      delay(100);
      infloat="";
      while (Serial.available()>0){
        char inchar=Serial.read( );
        if  (inchar != '\n') infloat+=inchar;
      }
      kroll.kd=infloat.toFloat();
      kpitch.kd=infloat.toFloat();  
      //print_PidConstants(kpitch); 

    break;


    default:
      if (flight_mode==idle){
        flight_mode=general_debug;        
        Serial.println("Entering debug test Mode");  
        aux_statechange_enable=false;    
      }
    break;

      

    }
}








/////////////////////////////////////////////////////////////////////////////////
//WIFI


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


void kill_client(void){
  Serial.println("Killing Client");
  client.stop();
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
