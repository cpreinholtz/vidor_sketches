
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

/*
struct pid_error  //type
{
   float kp;
   float ki;
   float kd;
   //...
};
typedef struct pid_error PidError;*/

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

struct fcartesian  //type
{
   float x;
   float y;
   float z;
};

typedef struct fcartesian Fcart; 
   
struct icartesian  //type
{
   int x;
   int y;
   int z;
};

typedef struct icartesian Icart;   

#endif
