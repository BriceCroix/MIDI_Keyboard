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
uint8_t buttons_settings;

/**
 * \var pitch
 * \brief index of the lowest key, 0 for C0, 2 for D0, 12 for C1...
 */
int8_t pitch=2;

/**
 * \var t
 * \brief time in micro second
 */
uint64_t t=0;

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

  //Initial duty cycle of 50%
  OCR1A = 0x008F;

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
  //Let us update time
  t += SAMPLE_TIME;

  //Let us compute the analog output value by checking each key, this will be the new duty-cycle OCR1A
  analog_out = 0;

  //Write MIDI messages to serial port
  //TODO
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
    //Checking keys 0:5 by setting PB0 to 0
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
    PORTB |= 0x01;

    //Checking buttons_settings_1 by setting PC1 to 0
    PORTB &= ~0x02;
    nop();
    buttons_settings = ~(PIND>>2);
    PORTB |= 0x02;

    //Checking buttons_settings_2 by setting PC2 to 0
    PORTB &= ~0x04;
    nop();
    buttons_settings = ~(PIND>>2);
    PORTB |= 0x04;

    //TODO : set output signal to right value according to all pressed keys

    //TOREMOVE : sets PC5 to high by putting PD2 to GND and to low by putting PD3 to GND
    if(keys_0 & 0x01) PORTC |= 0x20;
    if(keys_0 & 0x02) PORTC &= ~0x20;
  }
  //Program won't actually go outside this loop
  return EXIT_SUCCESS;
}
