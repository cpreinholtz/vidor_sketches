#include "types.h"





//control mode constants
#define idle 1
#define orientation_mode 2
#define flight 4

#define general_debug 8
#define esc_calibration 16
#define motor_direct 32





//motors
//#define pwm_frequency 60  //16mS total, 4.06uS per step (4096 max)
#define pwm_frequency 1600
#define motor_min 680//#define motor_start 1000
#define motor_start 821 
#define motor_max 1400
#define motor_start 821

#define FL  0
#define FR  1
#define BL  2
#define BR  3




//enablers
#define ENABLE_WIFI false
#define ENABLE_IMU true
#define ENABLE_PWM true
#define ENABLE_WATCHDOG true
#define ENABLE_RC_RECIEVER true
#define UPSIDEDOWN false

#define ENABLE_BMP false

//#define ENABLE_DEBUG_OUTPUT true //adafruit
//#define FLIGHT_MODE true// (disables motor writes)













//global variables
extern const float G,A;
extern float heading;
extern Attitude desired, measured, desired_raw, offset;
extern Throttle throttle;
extern PidError eroll, epitch, eyaw, eheight;
extern PidConstants kroll,kpitch,kyaw, kheight;
extern Fcart acc_pos, gyro_rate, gyro_pos, cfilter; 
extern volatile  interruptTimer throttle_in;

extern bool motor_arm;


//Dont Touch
#define LOOP_PERIOD 0.02
#define G_GAIN 0.070    // [deg/s/LSB]  //DONT CHANGE UNLESS CHANGING THE GYRO SCALE
//PID
//const float i_error_max=100* LOOP_PERIOD;   //100 degrees of integrated error maximun (factoring in that int error is not scaled by dt)
#define i_error_max (100* LOOP_PERIOD)
