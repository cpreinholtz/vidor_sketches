#include <SPI.h>
#include <WiFiNINA.h>
#include "secrets.h"
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
      
      switch_flight_mode();
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

      switch_flight_mode();
      //get_numeric_command();
    }
  }  


}





void switch_flight_mode(void){

  if (command>='0'&& command <='9'){
    get_desired();
  }

  //switches based on key pressed to get the command mode
  switch(command){
    case 'o'://orientation_mode
      if (flight_mode==idle){
        flight_mode=orientation_mode;     
        Serial.println("Entering orientation_mode Mode");
      }    
    break;
    
    case 'k'://kill / idle
      flight_mode=idle;
      Serial.println("Entering IDLE Mode");
    break;
    
    case 'c'://calibration
      if (flight_mode==idle){
        flight_mode=esc_calibration;  
        Serial.println("Entering Calibration Mode");      
      }   
    break;

    case 't'://throttle (no PID corrections)
      if (flight_mode==idle){
        flight_mode=throttle_test;  
        Serial.println("Entering throttle test Mode");      
      }    
    break;
    
    case 's'://throttle (no PID corrections)
      if (flight_mode==idle){
        flight_mode=sensor_test;  
        Serial.println("Entering sensor test Mode");      
      }    
    break;

    case 'e'://throttle (no PID corrections)
      if (flight_mode==idle){
        flight_mode=error_test;  
        Serial.println("Entering error test Mode");      
      }    
    break;

    case 'd'://throttle (no PID corrections)
      if (flight_mode==idle){
        flight_mode=desired_test;  
        Serial.println("Entering desired test Mode");      
      }    
    break;

    case 'f'://fly
      if (flight_mode==orientation_mode||flight_mode==hover){
        flight_mode=flight;   
        Serial.println("Entering flight Mode");     
      }    
    break;


    case 'h'://hover
      if (flight_mode==hover){
        flight_mode=hover;   
        Serial.println("Entering hover Mode");     
      }    
    break;


    

    default:
    break;
    
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
  float diff=5.0;
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
      case '5':default:
        //get_idle_desired();
      break;
      

    }
}

/*
void get_idle_desired(void){
  //desired.roll=0.0;
  //desired.pitch=0.0;
  //desired.yaw=0.0;
  desired.roll=0.0+offset.roll;
  desired.pitch=0.0+offset.pitch;
  desired.yaw=desired_raw.yaw;
}*/

void get_flight_desired(void){

  desired.roll=desired_raw.roll+offset.roll;
  desired.pitch=desired_raw.pitch+offset.roll;
  desired.yaw=desired.yaw+desired_raw.yaw;//offset already applied in yaw
  if (desired.yaw>360.0)desired.yaw-=360;
  else if(desired.yaw<0.0)desired.yaw+=360;
}





void print_desired(void){
  Serial.print("droll: ");Serial.print(desired.roll);Serial.print("\t");
  Serial.print("dpitch: ");Serial.print(desired.pitch);Serial.print("\t");
  Serial.print("dyaw: ");Serial.print(desired.yaw);Serial.print("\t");
  Serial.println("");
}

void print_desired_raw(void){
  Serial.print("droll_raw: ");Serial.print(desired_raw.roll);Serial.print("\t");
  Serial.print("dpitch_raw: ");Serial.print(desired_raw.pitch);Serial.print("\t");
  Serial.print("dyaw_raw: ");Serial.print(desired_raw.yaw);Serial.print("\t");
  Serial.println("");
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
  
}
















/////////////////////////////////////////////////////////////////////////////



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












///////////////////////////////////////////////////////
//OLD


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

