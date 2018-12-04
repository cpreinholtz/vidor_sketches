# vidor_sketches

BetaPhase...


# TODO
  See TODO.txt
  
  
          if (Serial.available() > 0) {
                incomingByte = Serial.read();
          }
          
          switch (incomingByte){
            case '1':
              break;
            case '2':
              break;
          }
          


# References:

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
General Arduino:
https://www.arduino.cc/reference/en/

Arduino Wifi:
https://www.arduino.cc/en/Reference/WiFi

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
Ozzmaker Repo... Arduino w/ IMU:
https://github.com/ozzmaker/BerryIMU/tree/master/arduino-BerryIMU

IMU datasheet:
https://cdn-shop.adafruit.com/datasheets/LSM9DS0.pdf

IMU Type (Twos comp, left justified):
https://stackoverflow.com/questions/49467573/convert-twos-complement-left-justified-integer-into-regular-binary
(consider trying just 8bit precision??),  also try the shift operations rather than odd typecasting??

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
AfroESC user manual:
https://arduino.ua/docs/AfroESC30A.pdf

Servo, ESC control:
https://www.arduino.cc/en/Reference/Servo

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
Complimentary filter explanation:
https://www.youtube.com/watch?v=whSw42XddsU

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

