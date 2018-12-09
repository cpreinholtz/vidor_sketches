#include "types.h"





//control mode constants
#define idle 1
#define orientation_mode 2
#define sensor_test 4
#define flight 8
#define esc_calibration 16
#define throttle_test 32
#define sensor_test 64
#define error_test 128
#define desired_test 256
#define hover 0 //unsupported right now


//motors
#define pwm_frequency 60  //16mS total, 4.06uS per step (4096 max)
#define motor_min 900
#define motor_start 1000 //~4ms?
#define motor_max 2000  //~8ms?



//enablers
#define ENABLE_WIFI false
#define ENABLE_IMU true
#define ENABLE_PWM false
#define UPSIDEDOWN false
#define ENABLE_BMP true
#define FLIGHT_MODE true// (disables motor writes)













//global variables
extern float G;//definition located at the top of "sensors"
extern float A;
extern Attitude desired, measured, desired_raw, offset;
extern Throttle throttle;
extern PidError eroll, epitch, eyaw, eheight;
extern PidConstants kroll,kpitch,kyaw, kheight;


//Dont Touch
#define LOOP_PERIOD 0.02
#define G_GAIN 0.070    // [deg/s/LSB]  //DONT CHANGE UNLESS CHANGING THE GYRO SCALE





