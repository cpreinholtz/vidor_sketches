#include "configs.h"



/*
 * Configuration for FlySky:
 * 
 * Channel 1 = Roll (right stick)
 * Channel 2 = Pitch (roight stick) *Reversed*
 * Channel 3 = Throttle (range 900-2000)
 * Channel 4 = N/A
 * Channel 5 = SWC (high sw = low pwm; low sw = high pwm)
 * Channel 6 = VRB ( right = +heading ) (use for yaw)
 * 
 * ch1 = pin4 roll
 * ch2 = pin5 pitch
 * ch3 = pin6 throttle
 * ch5 = pin7 aux
 * ch6 = pin8 yaw
 * 
 * Power
 * Connect 3v3 from vidor to the BATT gnd and 3v3,
 * 
 */
#define THROTTLE_IN_ENABLE   true 
#define YAW_IN_ENABLE      true 
#define PITCH_IN_ENABLE      true
#define ROLL_IN_ENABLE      true
#define AUX_IN_ENABLE      true

#define ROLL_PIN        4
#define PITCH_PIN       5
#define THROTTLE_PIN    6 
#define AUX_PIN         7
#define YAW_PIN         8




volatile interruptTimer throttle_in={0,0,0,0};
volatile interruptTimer roll_in={0,0,0,0};
volatile interruptTimer pitch_in={0,0,0,0};
volatile interruptTimer aux_in={0,0,0,0};
volatile interruptTimer yaw_in={0,0,0,0};





 //SETUP
void interrupt_setup(void) {

  pinMode(THROTTLE_PIN, INPUT);
  pinMode(AUX_PIN, INPUT);
  pinMode(ROLL_PIN, INPUT);
  pinMode(PITCH_PIN, INPUT);
  pinMode(YAW_PIN, INPUT);
  
  if (THROTTLE_IN_ENABLE) attachInterrupt(THROTTLE_PIN, throttle_rising_isr, RISING);
  if (AUX_IN_ENABLE) attachInterrupt(AUX_PIN, aux_rising_isr, RISING);
  if (ROLL_IN_ENABLE) attachInterrupt(ROLL_PIN, roll_rising_isr, RISING);
  if (PITCH_IN_ENABLE) attachInterrupt(PITCH_PIN, pitch_rising_isr, RISING);
  if (YAW_IN_ENABLE) attachInterrupt(YAW_PIN, yaw_rising_isr, RISING);


  interrupts();
} 










//THROTTLE
void throttle_rising_isr(void) {
  //detachInterrupt(
  attachInterrupt(THROTTLE_PIN, throttle_falling_isr, FALLING);
  throttle_in.rising_edge = micros();
  throttle_in.low_time = throttle_in.rising_edge - throttle_in.falling_edge;
} 
void throttle_falling_isr(void) {
  attachInterrupt(THROTTLE_PIN, throttle_rising_isr, RISING);
  throttle_in.falling_edge = micros();  
  throttle_in.high_time = throttle_in.falling_edge - throttle_in.rising_edge;
  throttle_isr_flag=true;
}




//AUX
void aux_rising_isr(void) {
  attachInterrupt(AUX_PIN, aux_falling_isr, FALLING);
  aux_in.rising_edge = micros();
  aux_in.low_time = aux_in.rising_edge - aux_in.falling_edge;

} 
void aux_falling_isr(void) {
  attachInterrupt(AUX_PIN, aux_rising_isr, RISING);
  aux_in.falling_edge = micros();  
  aux_in.high_time = aux_in.falling_edge - aux_in.rising_edge;
  aux_isr_flag=true;
}





//ROLL
void roll_rising_isr(void) {
  attachInterrupt(ROLL_PIN, roll_falling_isr, FALLING);
  roll_in.rising_edge = micros();
  roll_in.low_time = roll_in.rising_edge - roll_in.falling_edge;

} 
void roll_falling_isr(void) {
  attachInterrupt(ROLL_PIN, roll_rising_isr, RISING);
  roll_in.falling_edge = micros();  
  roll_in.high_time = roll_in.falling_edge - roll_in.rising_edge;
  //roll_isr_flag=true;
}







//PITCH
void pitch_rising_isr(void) {
  attachInterrupt(PITCH_PIN, pitch_falling_isr, FALLING);
  pitch_in.rising_edge = micros();
  pitch_in.low_time = pitch_in.rising_edge - pitch_in.falling_edge;

} 
void pitch_falling_isr(void) {
  attachInterrupt(PITCH_PIN, pitch_rising_isr, RISING);
  pitch_in.falling_edge = micros();  
  pitch_in.high_time = pitch_in.falling_edge - pitch_in.rising_edge;
  //pitch_isr_flag=true;
}






//YAW
void yaw_rising_isr(void) {
  attachInterrupt(YAW_PIN, yaw_falling_isr, FALLING);
  yaw_in.rising_edge = micros();
  yaw_in.low_time = yaw_in.rising_edge - yaw_in.falling_edge;

} 
void yaw_falling_isr(void) {
  attachInterrupt(YAW_PIN, yaw_rising_isr, RISING);
  yaw_in.falling_edge = micros();  
  yaw_in.high_time = yaw_in.falling_edge - yaw_in.rising_edge;
  //yaw_isr_flag=true;
}


