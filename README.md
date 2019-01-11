# vidor_sketches

BetaPhase...

# Dependancies:
  
Native:
  
  Wifi nina

  Vidor periphrials

  adafruit servo blaster

  optional: Adafruin BMP085-BMP180

Install yourself:

  Berry IMU (included here... see readme in that folder)





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

Quaternion: 
https://github.com/Tyrint/Razor-IMU

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


//@code

class PulseTimer{
private:
    int rising_edge;    //us
    int falling_edge;   //us
    int low_time;      //us
    int high_time;      //us
    bool flag; //signals that at least low->high->low has been captured, gets set at every falling edge
    enum State{
        low=0,
        high=1
    }state;//signals if the current state is high or low. 
        //used primarily tp prevent the first falling ege from causing issues if it comes before a rising, 
        //although this shouldn't happent if the isr is only attached to rising in setup...
    
public:

    //////////////////
    //Ctor
    PulseTimer(void):
        pin(attachToPin),
        rising_edge(0),
        falling_edge(0),
        high_time(0),
        low_time(0),
        state(low)
        flag(false)    
    {}; //Ctor noop for now (besides initialization)
    
    
    //////////////////
    //Rise and fall (called from ISR, etc)
    void rise(void){
        //if (state==low){
            rising_edge = micros();
            low_time = rising_edge - falling_edge;  
            //state=high;
        //}
    };
    void fall(void){
        //if (state==high){
            falling_edge = micros();  
            high_time = falling_edge - rising_edge;
            flag=true;
           // state=low;
        //}
    }  
    
    
    //////////////////
    //getters
    int high_time(void)const{
        return high_time;
    }
    
    bool flag(void)const{
        return flag;
    }
    
    //////////////////
    //setters
    void clear_flag(void){
        flag=false;
    }

}


PulseTimer auxPulseTimer, throttlePulseTimer;


//AUX ISRs
void aux_rising_isr(void) {
  attachInterrupt(AUX_PIN, aux_falling_isr, FALLING);
  auxPulseTimer.rise();
} 
void aux_falling_isr(void) {
  attachInterrupt(AUX_PIN, aux_rising_isr, RISING);
  auxPulseTimer.fall(); 
}




//timing and printing...
void handle_intrupt_flags(void){
  
  if (throttlePulseTimer.flag()){
    throttlePulseTimer.clear_flag();
    controller_connected=true;
    watchdog= millis();
  }
}
//@endcode

