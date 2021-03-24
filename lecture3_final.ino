#define FOSC 16000000L // Clock Speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1
#include <stdio.h>
 
uint32_t ticks = 0;  
uint8_t car;
char str[20];
 
ISR(TIMER1_COMPA_vect){
  // code for ISR
  PORTD ^= 1<<PD6; // blink LED on PD6

  //increment uptime ticks
  ticks++;
}
 
void initTimer2(){
  //fast PWM, clear OC2B - non inverting mode
  TCCR2A |= (1<<COM2B1) | (1<<WGM21) | (1<<WGM20);
  // clk/128 prescaler, clear timer on compare mode
  TCCR2B = (1<<CS20)|(1<<CS22); 
  
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
 
 
void initUsart(uint16_t ubrr){
  UBRR0H = ubrr >> 8;
  UBRR0L = ubrr;
  // Enable receiver and transmitter
  UCSR0B = (1<<RXEN0) | (1<<TXEN0);
  // Set frame format: 8data, 2stop bit
  UCSR0C = (1<<UCSZ00) | (1<<UCSZ01);
}
 
void usartPutchar(uint8_t data){
  // Wait for empty transmit buffer
  while( !( UCSR0A & (1<<UDRE0)));
  // Put data into buffer, sends the data
  UDR0 = data;
}
 
uint8_t usartGetchar(){
  // Wait for data to be received
  while( !(UCSR0A & (1<<RXC0)));
  // Get and return received data from buffer
  return UDR0;
}
 
void initIO(){
  DDRD |= (1<<PD6) | (1<<PD3); //PD6 as output
}
 
void ledOn(){
  //PORTD |= 1 << PD3;
  OCR2B = 255;
}
 
void ledOff(){
  //PORTD &= ~ (1 << PD3);
  OCR2B = 0;
}
 
void setup(){
    initTimer1();
    initTimer2();
    initIO();
    initUsart(MYUBRR);
  
    serialPrintln("LED Commander 1.0");
    serialPrintln("usage: 0 - ledOff, 1 - ledOn");
}
 
void serialPrintln(char *msg){
  usartPutchar('\n');
  
  while(*msg != '\0'){
      usartPutchar(*msg++);
  }
    
}

void loop(){
    serialPrintln("> ");
  
    car = usartGetchar();
    usartPutchar(car);
  
    switch(car){
      case '0': ledOff();
                break;
      case '1': ledOn();
                break;
      case 't': sprintf(str, "Uptime: %d", ticks);
                serialPrintln(str);
                break;
      default : serialPrintln("usage: 0 - ledON, 1 - ledOFF");
                break;
    }
}
