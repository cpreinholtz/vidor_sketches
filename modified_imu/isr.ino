#include "configs.h"


/*
 * Configuration for Futiba:
 * 
 * Channel 1 = Rudder (Broken, cant turn one direction)
 * Channel 2 = Throttle (heep throttle exp to 0 and high trim to max on the transmitter) (range ~ 1190-1970uS with stated trim)
 * Channel 3 = Aux (range 900-2000)
 * Channel 4 = Multiplexed??
 * 
 * Trims:
 * Keep controller in "warmup"
 * Brake trim = min(left)
 * Throttle max trim = max(right)
 * trottle exp = min (left)
 * 
 * Hardware:  
 * Looking at the recieverfrom the front, far left pin is signal, middle is 5v, right is gnd
 * Bottom to top is BAT, CH 1 2 3 4 
 * Connect 3v3 from vidor to the BATT gnd and 3v3,
 * Connect the Channel 2 RX signal to pin 4 on the Vidor for throttle.
 * 
 */

#define THROTTLE_IN_ENABLE   true // 4 
#define AUX_IN_ENABLE      true // 5 


#define THROTTLE_PIN    4 
#define AUX_PIN         5




volatile interruptTimer throttle_in={0,0,0,0};
volatile interruptTimer roll_in={0,0,0,0};
volatile interruptTimer aux_in={0,0,0,0};
volatile interruptTimer switch_in={0,0,0,0};





 //SETUP
void interrupt_setup(void) {

  pinMode(THROTTLE_PIN, INPUT);
  pinMode(AUX_PIN, INPUT);
  
  if (THROTTLE_IN_ENABLE) attachInterrupt(THROTTLE_PIN, throttle_rising_isr, RISING);
  if (AUX_IN_ENABLE) attachInterrupt(digitalPinToInterrupt(AUX_PIN), aux_rising_isr, RISING);


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




