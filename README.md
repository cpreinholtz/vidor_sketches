# vidor_sketches

BetaPhase...

# TODO:
  Watchdog for commands
  Presure Soensor
  experiment with udp telem?

# Vidor Pinout



# ESC Calibration:
1
Set the radio throttle or servo tester to the highest position, then connect power to the ESC. The 
motor should produce a series of initialization beeps increasing in pitch, followed by another beep 
matching the pitch of the last initialization beep. This indicates that the calibration mode has been 
entered, and the pulse length has been learned.
2
Move the stick or knob to the lowest position. Two beeps of the same pitch should be emitted. This
indicates that the low pulse length has been learned. 
3
If the RC Car-style reversible mode has been 
enabled (RC_PULS_NEUTRAL), move the stick or knob to the center, and wait for three beeps. This 
indicates that the neutral (center) pulse length has been learned

# References:

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

General Arduino:
https://www.arduino.cc/reference/en/

Arduino WifiNINA(Vidor):
https://www.arduino.cc/en/Reference/WiFiNINA

WIFI
https://www.arduino.cc/en/Reference/WiFi

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

Ozzmaker Repo... Arduino w/ IMU:
https://github.com/ozzmaker/BerryIMU/tree/master/arduino-BerryIMU

IMU datasheet:
https://cdn-shop.adafruit.com/datasheets/LSM9DS0.pdf

IMU Type (Twos comp, left justified):
https://stackoverflow.com/questions/49467573/convert-twos-complement-left-justified-integer-into-regular-binary
(consider trying just 8bit precision??),  also try the shift operations rather than odd typecasting??

Presure sensor:
https://cdn-shop.adafruit.com/datasheets/BST-BMP180-DS000-09.pdf

Presure sensor repo:
https://github.com/adafruit/Adafruit_BMP085_Unified

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!



AfroESC user manual:
https://arduino.ua/docs/AfroESC30A.pdf

Servo w/ adafruit:
https://github.com/adafruit/Adafruit-PWM-Servo-Driver-Library


!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

Complimentary filter explanation:
https://www.youtube.com/watch?v=whSw42XddsU

more complimentary filter (time constants):
https://robotics.stackexchange.com/questions/1717/how-to-determine-the-parameter-of-a-complementary-filter

Python+msql??
https://moderndata.plot.ly/graph-data-from-mysql-database-in-python/

Thinking in C++:
https://www.micc.unifi.it/bertini/download/programmazione/TICPP-2nd-ed-Vol-one-printed.pdf
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

