/*#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085_U.h>

//BMP SHIET
Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified(10085);
float pressure;

void displaySensorDetails(void)
{
  sensor_t sensor;
  bmp.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" hPa");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" hPa");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" hPa");  
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);
}


void setup_bmp(void) 
{
  if(ENABLE_BMP){
    Serial.println("Pressure Sensor Test"); Serial.println("");
    
 
    if(!bmp.begin())
    {

      Serial.print("Ooops, no BMP085 detected ... Check your wiring or I2C ADDR!");
      while(1);
    }

    displaySensorDetails();
  }
  else{
    Serial.println("No BMP!");
  }
}

void get_altitude(void){
  
  sensors_event_t event;
  bmp.getEvent(&event);

  if (event.pressure)
  {

    pressure=event.pressure;

    //Serial.print("Pressure:    ");
    //Serial.print(event.pressure);
    //Serial.println(" hPa");

    //float temperature;
    //bmp.getTemperature(&temperature);



    //float seaLevelPressure = SENSORS_PRESSURE_SEALEVELHPA;
    //Serial.print(bmp.pressureToAltitude(seaLevelPressure,  event.pressure)); 

  }
  else
  {
    Serial.println("BMP Sensor error");
  }
  
}*/
void setup_bmp(void) {
  
}
