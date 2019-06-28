/**
 * \file KeyBoard.ino
 * \brief This file aims at providing efficient code in order to create music thanks to MIDI standart associated with analog signal.
 * \author Brice Croix
 * \date june 25 of 2019
 */

#include <avr/io.h>
#include "binary.h"

//Sample frequency in Hz
#define SAMPLE_FREQUENCY 62500
//Sample time in micro second
#define SAMPLE_TIME 16
//The following macros takes the PIND register as bVVVVVVXX and returns b00VVVVVV since there are pull-ups most bits will be 1 by except with pressed keys which will be 0
#define PIND_RELEVANT (PIND>>2)
//The no-operation function as defined in assembly language
#define nop() __asm__("nop\n\t")
//The analog amplitude for one note, defining how many notes can be played at once, here 8 notes because 8*32 = 256
#define NOTE_AMP 32
//Number of notes per octave
#define OCTAVE 12

//Each period in us from C0 to B8
const uint16_t PERIODS[] = {
  61156, 57724, 54484, 51426, 48540, 45815, 43244, 40817, 38526, 36364, 34323, 32396, //C0 to B0
  30578, 28862, 27242, 25713, 24270, 22908, 21622, 20408, 19263, 18182, 17161, 16198, //C1 to B1
  15289, 14431, 13621, 12856, 12135, 11454, 10811, 10204, 9631,  9091,  8581,  8099,  //C2 to B2
  7645,  7215,  6810,  6428,  6067,  5727,  5405,  5102,  4816,  4545,  4290,  4050,  //C3 to B3
  3822,  3608,  3405,  3214,  3034,  2863,  2703,  2551,  2408,  2273,  2145,  2025,  //C4 to B4
  1911,  1804,  1703,  1607,  1517,  1432,  1351,  1276,  1204,  1136,  1073,  1012,  //C5 to B5
  956,   902,   851,   804,   758,   716,   676,   638,   602,   568,   536,   506,   //C6 to B6
  478,   451,   426,   402,   379,   358,   338,   319,   301,   284,   268,   253,   //C7 to B7
  239,   225,   213,   201,   190,   179,   169,   159,   150,   142,   134,   127    //C8 to B8
};



/**
 * \var key_0
 * \brief Stores state of keys 0 to 5, same goes for key_6, key_12 etc. Only the 6 LSB are relevant on these variables.
 */
uint8_t keys_0;
uint8_t keys_6;
uint8_t keys_12;
uint8_t keys_18;
uint8_t keys_24;
uint8_t keys_30;
uint8_t buttons_settings_1;
uint8_t buttons_settings_2;

/**
 * \var pitch
 * \brief index of the lowest key, 0 for C0, 2 for D0, 12 for C1...
 */
uint8_t pitch=24;

/**
 * \var t
 * \brief time in micro second
 */
uint64_t t=0;

/**
 * \var analog_out
 * \brief the analog value to write on the analog output
 */
uint8_t analog_out=0;

/**
 * \fn void init_timer_1()
 * \brief Sets timer 1 in condition to generate PWM signal
 */
void init_timer_1(){
  //Enable timer1 with no prescaler, cf doc p110
  TCCR1B &= ~0x07; //Clears bits CS10:12 that handle timer activation
  TCCR1B |= 0x01; //The timer is supposed to count at 16 Mhz, an external precision clock could be used if necessary

  //Disable noise canceling for input
  TCCR1B &= ~0x07;

  //Reset counter value
  TCNT1 = 0;

  //Enable only OVF interrupts, cf doc p112
  TIMSK1 = 0;
  TIMSK1 |= 0x01;

  //Timer 1 in fast PWM mode, cf p108, WGM13:10 = 1110
  TCCR1A &= ~0x03; //Clear bits WGM10:11
  TCCR1A |= 0x02; //Set WGM11 to 1
  TCCR1B |= 0x18; //Set WGM13:12 to 1
  //In this mode the Auto-Reload value (TOP, or ARR for a STM32) is in the Input Capture Register
  ICR1 = 0x00FF; //Sampling Frequency = 62745 Hz

  //Connect OC1A to pin PB1 in non-inverting PWM mode, cf p108
  TCCR1A &= ~0xF0;
  TCCR1A |= 0x80;

  //Initial duty cycle of 0%
  OCR1A = 0x0000;

  //N.B. OC1A (channel A) output is on pin 9 and OC1B on pin 10
}

/**
 * \fn void init_pins()
 * \brief Sets pinsXXX to input and pinsXXX to output
 */
void init_pins(){
  //Enabling pull-ups for all ports
  MCUCR &= ~0x10;

  //PortD 2:7 as input for all buttons, this will allow to get all values in one instruction
  DDRD &= ~0xFC;
  //Put pins PD2:7 to pull-up
  PORTD |= 0xFC;

  //PWM output OCA1, which is on PB1, aka Pin9
  //PortB 0 and 2:5 as output for buttons from Keys 0 to 30
  //Built-in led also on PB5 as output
  DDRB |= 0x3F;
  //Turning on all outputs for now
  PORTB |= 0x3D;

  //PortC 0:2 as output for keys 31 to 36 and buttons settings 1 and 2
  DDRC |= 0x07;
  //Turning on all outputs for now
  PORTC |= 0x07;
}

/**
 * \fn disable_ide_stuff()
 * \brief Disables some default functionnalities provided by Arduino IDE
 */
void disable_ide_stuff(){
  //TODO
  //Disable timer_0, which interrupts to provide delay and millis functions
}

/**
 * \fn ISR(TIMER1_OVF_vect)
 * \brief interruption code when an ovf occurs on timer 1
 */
ISR(TIMER1_OVF_vect){
  //Let us update the analog pin output value, computed at last interruption
  OCR1A = analog_out;
  //Let us update time
  t += SAMPLE_TIME;
  //Let us update analog_out for next interruption, indeed next sample
  setAnalogOut();
  //Write MIDI messages to serial port
  //TODO
}

/**
 * \fn void setAnalogOut()
 * \brief Sets the analog_out value according to the keys state. WARNING : Output should not be written while this function is computing
 */
void setAnalogOut(){
  //In order not to access memory multiple times
  uint8_t current_pitch = pitch;
  //Variable to store current considered period
  uint16_t T;
  //Resetting analog_out
  analog_out=0;

  if(keys_0 & 0x01){
    T = PERIODS[current_pitch]; //We don't want to access this array two times
    if(t%T < ( T >> 1) ){
      analog_out += NOTE_AMP;
    }
  }
  if(keys_0 & 0x02){
    T = PERIODS[current_pitch+1];
    if(t%T < ( T >> 1) ){
      analog_out += NOTE_AMP;
    }
  }
  if(keys_0 & 0x04){
    T = PERIODS[current_pitch+2];
    if(t%T < ( T >> 1) ){
      analog_out += NOTE_AMP;
    }
  }
  if(keys_0 & 0x08){
    T = PERIODS[current_pitch+3];
    if(t%T < ( T >> 1) ){
      analog_out += NOTE_AMP;
    }
  }
  if(keys_0 & 0x10){
    T = PERIODS[current_pitch+4];
    if(t%T < ( T >> 1) ){
      analog_out += NOTE_AMP;
    }
  }
  if(keys_0 & 0x20){
    T = PERIODS[current_pitch+5];
    if(t%T < ( T >> 1) ){
      analog_out += NOTE_AMP;
    }
  }
  if(keys_6 & 0x01){
    T = PERIODS[current_pitch+6];
    if(t%T < ( T >> 1) ){
      analog_out += NOTE_AMP;
    }
  }
  if(keys_6 & 0x02){
    T = PERIODS[current_pitch+7];
    if(t%T < ( T >> 1) ){
      analog_out += NOTE_AMP;
    }
  }
  if(keys_6 & 0x04){
    T = PERIODS[current_pitch+8];
    if(t%T < ( T >> 1) ){
      analog_out += NOTE_AMP;
    }
  }
  if(keys_6 & 0x08){
    T = PERIODS[current_pitch+9];
    if(t%T < ( T >> 1) ){
      analog_out += NOTE_AMP;
    }
  }
  if(keys_6 & 0x10){
    T = PERIODS[current_pitch+10];
    if(t%T < ( T >> 1) ){
      analog_out += NOTE_AMP;
    }
  }
  if(keys_6 & 0x20){
    T = PERIODS[current_pitch+11];
    if(t%T < ( T >> 1) ){
      analog_out += NOTE_AMP;
    }
  }
  if(keys_12 & 0x01){
    T = PERIODS[current_pitch+12];
    if(t%T < ( T >> 1) ){
      analog_out += NOTE_AMP;
    }
  }
  if(keys_12 & 0x02){
    T = PERIODS[current_pitch+13];
    if(t%T < ( T >> 1) ){
      analog_out += NOTE_AMP;
    }
  }
  if(keys_12 & 0x04){
    T = PERIODS[current_pitch+14];
    if(t%T < ( T >> 1) ){
      analog_out += NOTE_AMP;
    }
  }
  if(keys_12 & 0x08){
    T = PERIODS[current_pitch+15];
    if(t%T < ( T >> 1) ){
      analog_out += NOTE_AMP;
    }
  }
  if(keys_12 & 0x10){
    T = PERIODS[current_pitch+16];
    if(t%T < ( T >> 1) ){
      analog_out += NOTE_AMP;
    }
  }
  if(keys_12 & 0x20){
    T = PERIODS[current_pitch+17];
    if(t%T < ( T >> 1) ){
      analog_out += NOTE_AMP;
    }
  }
  if(keys_18 & 0x01){
    T = PERIODS[current_pitch+18];
    if(t%T < ( T >> 1) ){
      analog_out += NOTE_AMP;
    }
  }
  if(keys_18 & 0x02){
    T = PERIODS[current_pitch+19];
    if(t%T < ( T >> 1) ){
      analog_out += NOTE_AMP;
    }
  }
  if(keys_18 & 0x04){
    T = PERIODS[current_pitch+20];
    if(t%T < ( T >> 1) ){
      analog_out += NOTE_AMP;
    }
  }
  if(keys_18 & 0x08){
    T = PERIODS[current_pitch+21];
    if(t%T < ( T >> 1) ){
      analog_out += NOTE_AMP;
    }
  }
  if(keys_18 & 0x10){
    T = PERIODS[current_pitch+22];
    if(t%T < ( T >> 1) ){
      analog_out += NOTE_AMP;
    }
  }
  if(keys_18 & 0x20){
    T = PERIODS[current_pitch+23];
    if(t%T < ( T >> 1) ){
      analog_out += NOTE_AMP;
    }
  }
  if(keys_24 & 0x01){
    T = PERIODS[current_pitch+24];
    if(t%T < ( T >> 1) ){
      analog_out += NOTE_AMP;
    }
  }
  if(keys_24 & 0x02){
    T = PERIODS[current_pitch+25];
    if(t%T < ( T >> 1) ){
      analog_out += NOTE_AMP;
    }
  }
  if(keys_24 & 0x04){
    T = PERIODS[current_pitch+26];
    if(t%T < ( T >> 1) ){
      analog_out += NOTE_AMP;
    }
  }
  if(keys_24 & 0x08){
    T = PERIODS[current_pitch+27];
    if(t%T < ( T >> 1) ){
      analog_out += NOTE_AMP;
    }
  }
  if(keys_24 & 0x10){
    T = PERIODS[current_pitch+28];
    if(t%T < ( T >> 1) ){
      analog_out += NOTE_AMP;
    }
  }
  if(keys_24 & 0x20){
    T = PERIODS[current_pitch+29];
    if(t%T < ( T >> 1) ){
      analog_out += NOTE_AMP;
    }
  }
  if(keys_30 & 0x01){
    T = PERIODS[current_pitch+30];
    if(t%T < ( T >> 1) ){
      analog_out += NOTE_AMP;
    }
  }
  if(keys_30 & 0x02){
    T = PERIODS[current_pitch+31];
    if(t%T < ( T >> 1) ){
      analog_out += NOTE_AMP;
    }
  }
  if(keys_30 & 0x04){
    T = PERIODS[current_pitch+32];
    if(t%T < ( T >> 1) ){
      analog_out += NOTE_AMP;
    }
  }
  if(keys_30 & 0x08){
    T = PERIODS[current_pitch+33];
    if(t%T < ( T >> 1) ){
      analog_out += NOTE_AMP;
    }
  }
  if(keys_30 & 0x10){
    T = PERIODS[current_pitch+34];
    if(t%T < ( T >> 1) ){
      analog_out += NOTE_AMP;
    }
  }
  if(keys_30 & 0x20){
    T = PERIODS[current_pitch+35];
    if(t%T < ( T >> 1) ){
      analog_out += NOTE_AMP;
    }
  }
}

/**
 * \fn void main()
 * \brief main function to loop over and over endlessly
 */
int main(){
  //Disable interrupts while initializing, cf p11
  SREG &= ~0x80;
  //TOREMOVE : sets pin A5 to output, low level
  DDRC |= 0x20;
  PORTC &= ~0x20;
  init_timer_1();
  init_pins();
  //Enable interrupts
  SREG |= 0x80;

  while(1){
    //Checking keys 0:5 by setting PB0 to 0, a no_operation is required for sync, see datasheet p60
    PORTB &= ~0x01;
    nop();
    keys_0 = ~(PIND>>2);
    PORTB |= 0x01;
    //Checking keys 6:11 by setting PB2 to 0
    PORTB &= ~0x04;
    nop();
    keys_6 = ~(PIND>>2);
    PORTB |= 0x04;
    //Checking keys 12:17 by setting PB3 to 0
    PORTB &= ~0x08;
    nop();
    keys_12 = ~(PIND>>2);
    PORTB |= 0x08;
    //Checking keys 18:23 by setting PB4 to 0
    PORTB &= ~0x10;
    nop();
    keys_18 = ~(PIND>>2);
    PORTB |= 0x10;
    //Checking keys 24:29 by setting PB5 to 0
    PORTB &= ~0x20;
    nop();
    keys_24 = ~(PIND>>2);
    PORTB |= 0x20;
    //Checking keys 30:35 by setting PC0 to 0
    PORTC &= ~0x01;
    nop();
    keys_30 = ~(PIND>>2);
    PORTC |= 0x01;
    //Checking buttons_settings_1 by setting PC1 to 0
    PORTC &= ~0x02;
    nop();
    buttons_settings_1 = ~(PIND>>2);
    PORTC |= 0x02;
    //Checking buttons_settings_2 by setting PC2 to 0
    PORTC &= ~0x04;
    nop();
    buttons_settings_2 = ~(PIND>>2);
    PORTC |= 0x04;

    //TOREMOVE : sets PC5 to high by putting PD2 to GND and to low by putting PD3 to GND
    if(keys_0 & 0x01) PORTC |= 0x20;
    if(keys_0 & 0x02) PORTC &= ~0x20;
  }
  //Program won't actually go outside this loop

  return EXIT_FAILURE;
}
