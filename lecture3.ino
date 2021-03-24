 ISR(TIMER1_COMPA_vect){
  // code for ISR
  PORTD ^= 1<<PD6; // blink LED on PD6
}

void initTimer1(){
  TCCR1A = 0x00;
  // clk/1024 prescaler, clear timer on compare mode
  TCCR1B = (1<<CS10)|(1<<CS12)|(1<<WGM12); 
  //16000000/1024/65536 = 0.238Hz ~ 4.2s
  //16000000/1024/15625 = 1Hz
  OCR1A = 15625; // 1 second delay
  TIMSK1 = 1<<OCIE1A; // enable output compare interrupt
}

void initIO(){
  DDRD |= (1<<PD6); //PD6 & PD3 as output
}

void setup(){
  initTimer1();
  initIO();
}

void loop(){
 
}
