#include "types.h"





//control mode constants
#define idle 1
#define orientation_mode 2
#define sensor_test 4
#define flight 8
#define esc_calibration 16
#define throttle_test 32
#define sensor_test 64
#define error_roll_test 128
#define desired_test 256
#define error_pitch_test 512
#define error_yaw_test 1024
#define motor_direct 2048
#define test_orientation_mode 5096
#define general_test 5097
#define transform_test 5098
#define hover 0 //unsupported right now


//motors
//#define pwm_frequency 60  //16mS total, 4.06uS per step (4096 max)
#define pwm_frequency 1000
#define motor_min 1000
//#define motor_start 1000 
#define motor_max 2000


#define FL  8
#define FR  9
#define BL  10
#define BR  11




//enablers
#define ENABLE_WIFI false
#define ENABLE_IMU true
#define ENABLE_PWM true
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


//Dont Touch
#define LOOP_PERIOD 0.02
#define G_GAIN 0.070    // [deg/s/LSB]  //DONT CHANGE UNLESS CHANGING THE GYRO SCALE
//PID
//const float i_error_max=100* LOOP_PERIOD;   //100 degrees of integrated error maximun (factoring in that int error is not scaled by dt)
#define i_error_max (100* LOOP_PERIOD)









