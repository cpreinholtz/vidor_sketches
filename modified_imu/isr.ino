

#define THROTTLE_IN_ENABLE   true // 4 
//#define ROLL_IN_ENABLE      false // 5 
//#define AUX_IN_ENABLE       false // 6 
//#define SWITCH_IN_ENABLE     false // 7

#define THROTTLE_PIN    4 
//#define ROLL_PIN        3// 5 
//#define AUX_PIN         4// 6 
//#define SWITCH_PIN      5// 7


volatile interruptTimer throttle_in={0,0,0,0};
volatile interruptTimer roll_in={0,0,0,0};
volatile interruptTimer aux_in={0,0,0,0};
volatile interruptTimer switch_in={0,0,0,0};





 //SETUP
void interrupt_setup(void) {

  pinMode(THROTTLE_PIN, INPUT);
  //pinMode(ROLL_PIN, INPUT);
  //pinMode(AUX_PIN, INPUT);
  //pinMode(SWITCH_PIN, INPUT);
  
  if (THROTTLE_IN_ENABLE) attachInterrupt(THROTTLE_PIN, throttle_rising_isr, RISING);
  //if (ROLL_IN_ENABLE) attachInterrupt(digitalPinToInterrupt(ROLL_PIN), roll_rising_isr, RISING);
  //if (AUX_IN_ENABLE) attachInterrupt(digitalPinToInterrupt(AUX_PIN), aux_rising_isr, RISING);
  //if (SWITCH_IN_ENABLE) attachInterrupt(digitalPinToInterrupt(SWITCH_PIN), switch_rising_isr, RISING);

  interrupts();
} 










//THROTTLE
void throttle_rising_isr(void) {
  //detachInterrupt(
  attachInterrupt(THROTTLE_PIN, throttle_falling_isr, FALLING);
  throttle_in.rising_edge = micros();
  throttle_in.low_time = throttle_in.rising_edge - throttle_in.falling_edge;
  //Serial.println("ISR1");
} 
void throttle_falling_isr(void) {
  attachInterrupt(THROTTLE_PIN, throttle_rising_isr, RISING);
  throttle_in.falling_edge = micros();  
  throttle_in.high_time = throttle_in.falling_edge - throttle_in.rising_edge;
  //Serial.println("ISR2");
}
