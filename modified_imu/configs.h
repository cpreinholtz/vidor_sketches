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


//flight mode setting (disables motor writes)
#define FLIGHT_MODE true


//enablers
#define ENABLE_WIFI false
#define ENABLE_IMU true
#define ENABLE_PWM false
#define UPSIDEDOWN false
#define ENABLE_BMP true






extern Attitude desired, measured, desired_raw, offset;
extern Throttle throttle;
extern PidError eroll, epitch, eyaw, eheight;
extern PidConstants kroll,kpitch,kyaw, kheight;








//global variables
extern float G;//definition located at the top of "sensors"
extern float A;

//Dont Touch
#define LOOP_PERIOD 0.02
#define G_GAIN 0.070    // [deg/s/LSB]  //DONT CHANGE UNLESS CHANGING THE GYRO SCALE





