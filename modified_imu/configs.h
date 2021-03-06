#include "types.h"

//////////////////////////////////////////
//control mode constants
#define idle 1
#define orientation_mode 2
#define flight 4
#define general_debug 8
#define esc_calibration 16
#define motor_direct 32

/////////////////////////////////////////


//motors
#define motor_min 680//
#define motor_start 821
#define motor_max 1400




#define craft_angle_max 10 //max roll or pitch angle in degrees  //  roll and pitch get mapped to this
#define craft_angle_min (-craft_angle_max)

//#define stick_min 1000
//#define stick_max 2000
#define stick_travel 500

#define throttle_in_min 1500 //midpoint
#define throttle_in_max 2000

//#define aux_in_min 950
#define aux_in_idle 1250 //idle<
#define aux_in_orientation 1750 //orientation<   fly>
//#define aux_in_max 2000





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
extern Attitude desired, measured, desired_raw, offset, mid_stick;
extern Throttle throttle;
extern PidError eroll, epitch, eyaw, eheight;
extern PidConstants kroll,kpitch,kyaw, kheight;
extern Fcart acc_pos, gyro_rate, gyro_pos, cfilter; 
extern volatile  interruptTimer throttle_in, roll_in, pitch_in, yaw_in, aux_in;

extern bool motor_arm;//controlls if motors get power or not
extern bool controller_connected;//set in time handler after isr, cleared if wtchdog timeout. controls throttle in source
extern bool throttle_isr_flag, aux_isr_flag, aux_statechange_enable;
//Dont Touch
#define LOOP_PERIOD 0.02
#define G_GAIN 0.070    // [deg/s/LSB]  //DONT CHANGE UNLESS CHANGING THE GYRO SCALE
//PID
//const float i_error_max=100* LOOP_PERIOD;   //100 degrees of integrated error maximun (factoring in that int error is not scaled by dt)
#define i_error_max 100.0
