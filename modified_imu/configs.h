#include "types.h"

//enablers
#define ENABLE_WIFI false
#define ENABLE_IMU true
#define UPSIDEDOWN false


//motors
#define pwm_frequency 60  //16mS total, 4.06uS per step (4096 max)
#define motor_min 900
#define motor_start 1000 //~4ms?
#define motor_max 2000  //~8ms?

#define SERVOMIN  150 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  600 // this is the 'maximum' pulse length count (out of 4096)


#define FL  1
#define FR  2
#define BL  3
#define BR  4


//PID
//const float i_error_max=100* LOOP_PERIOD;   //100 degrees of integrated error maximun (factoring in that int error is not scaled by dt)
#define i_error_max (100* LOOP_PERIOD)


//mode constants
#define idle 1
#define orientation 2
#define takeoff 4
#define flight 8

#define calibration 128
#define test 256



//global variables
extern float G;//definition located at the top of "sensors"
extern float A;


extern Attitude desired, measured;
extern Throttle throttle;
extern PidError eroll, epitch, eyaw, eheight;
extern PidConstants kroll,kpitch,kyaw, kheight;









//Dont Touch
#define LOOP_PERIOD 0.02
#define G_GAIN 0.070    // [deg/s/LSB]  //DONT CHANGE UNLESS CHANGING THE GYRO SCALE





