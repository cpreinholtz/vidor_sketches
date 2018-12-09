
#ifndef TYPES
#define TYPES 1

//////////////////////////////////////////////////////
//Types and Structs
struct pid_constants  //type
{
   float kp;
   float ki;
   float kd;
   float min;
   float max;
   //...
};
typedef struct pid_constants PidConstants;


struct pid_error  //type
{
   float p;
   float i;
   float d;
   //...
};
typedef struct pid_error PidError;

struct throttle_container  //type
{
   int fl;
   int fr;
   int bl;
   int br;
};
typedef struct throttle_container Throttle;


struct attitude  //type
{
   float roll;
   float pitch;
   float yaw;
   float throttle;
};
typedef struct attitude Attitude;    



#endif
