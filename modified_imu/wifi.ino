#include <SPI.h>
#include <WiFiNINA.h>
#include <WiFiUdp.h>



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


/////////////////////////////////////
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


void kill_client(void){
  Serial.println("Killing Client");
  client.stop();
}


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
