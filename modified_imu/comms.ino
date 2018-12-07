#include <SPI.h>
#include <WiFiNINA.h>
#include "secrets.h"



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
void get_command(){

  if(ENABLE_WIFI==true){
    if (client.available()) {
      Serial.print("Got Command: ");
      command = client.read();
      
      Serial.print(command);
      Serial.print(" at Epoch: ");
      Serial.print(epoch);
      Serial.println("");
      client.flush();
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
    }
  }  

  
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



}



void setup_wifi(void) {
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

