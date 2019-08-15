/**
 * \file KeyBoard.ino
 * \brief This file aims at providing efficient code in order to create music thanks to MIDI standart associated with analog signal.
 * \author Brice Croix
 * \date june 25 of 2019
 */
#define DEBUG


// Define this if you want your keyboard to have 42 keys, 36 else
//#define KEY_NUMBER_42

#include <avr/io.h>
#include "binary.h"


// The no-operation function as defined in assembly language
#define nop() __asm__("nop\n\t")
// Number of notes per octave
#define OCTAVE 12
// Masks to check keys level in a key group
#define KEY_0_MSK 0x04
#define KEY_1_MSK 0x08
#define KEY_2_MSK 0x10
#define KEY_3_MSK 0x20
#define KEY_4_MSK 0x40
#define KEY_5_MSK 0x80


/**
 * \var keys_0
 * \brief Stores state of keys 0 to 5, same goes for key_6, key_12 etc. Only the 6 MSB are relevant on these variables. [k5,k4,k3,k2,k1,k0,xx,xx]
 */
volatile uint8_t keys_0 = 0;
volatile uint8_t keys_6 = 0;
volatile uint8_t keys_12 = 0;
volatile uint8_t keys_18 = 0;
volatile uint8_t keys_24 = 0;
volatile uint8_t keys_30 = 0;
#ifdef KEY_NUMBER_42
volatile uint8_t keys_36 = 0;
volatile uint8_t keys_42 = 0;
#endif
volatile uint8_t buttons_settings = 0;


/**
 * \var keys_0_last
 * \brief stores the state of the keys at last sample
 */
volatile uint8_t keys_0_last = 0;
volatile uint8_t keys_6_last = 0;
volatile uint8_t keys_12_last = 0;
volatile uint8_t keys_18_last = 0;
volatile uint8_t keys_24_last = 0;
volatile uint8_t keys_30_last = 0;
#ifdef KEY_NUMBER_42
volatile uint8_t keys_36_last = 0;
volatile uint8_t keys_42_last = 0;
#endif
volatile uint8_t buttons_settings_last = 0;


/**
 * \var pitch_0
 * \brief index of the lowest key, 0 for C0, 2 for D0, 12 for C1...
 */
volatile uint8_t pitch_0 = 36;


/**
 * \var ADC_vibrato
 * \brief Stores the value of the vibrato potentiometer
 */
volatile uint8_t ADC_vibrato = 127;


/**
 * \var ADC_tremolo
 * \brief Stores the value of the tremolo potentiometer
 */
volatile uint8_t ADC_tremolo = 127;


/**
 * \fn void init_pins()
 * \brief Sets pins PD7:2 as pulled-up inputs and pins PB5:0, PC3:0 as outputs
 */
void init_pins(){
  // Enabling pull-ups for all ports
  MCUCR &= ~0x10;

  // PortD 2:7 as input for all buttons, this will allow to get all values in one instruction
  DDRD &= ~(1<<DDD2 | 1<<DDD3 | 1<<DDD4 | 1<<DDD5 | 1<<DDD6 | 1<<DDD7);
  // Put pins PD2:7 to pull-up
  PORTD |= B11111100;

  // PWM output OCA1, which is on PB1, aka Pin9
  // PortB 0 and 2:5 as output for buttons from Keys 0 to 23
  // Built-in led also on PB5 as output
  DDRB |= (1<<DDB0 | 1<<DDB1 | 1<<DDB2 | 1<<DDB3 | 1<<DDB4 | 1<<DDB5);
  // Turning on all outputs for now, except LED
  PORTB |= B00011101;

  // PortC 0:3 as output for keys 24 to 42 , PC4 for settings buttons and PC5 for LED
  DDRC |= (1<<DDC0 | 1<<DDC1 | 1<<DDC2 | 1<<DDC3 | 1<<DDC4 | 1<<DDC5 );
  // Turning on all outputs for now, except LED
  PORTC |= B00011111;
}


/**
 * \fn void init_adc()
 * \brief Enables the ADC with no ADC clock prescaler
 */
 void init_adc(){
   // Reference is AVcc = 5V
   // Result is left aligned
   ADMUX = (1<<REFS0) | (1<<ADLAR);

   // ADC Enable
   ADCSRA |= (1<<ADEN);
   // Prescaler of 128, ADC_CLK = 125kHz
   ADCSRA &= ~((1<<ADPS0) | (1<<ADPS1) | (1<<ADPS2) );
   ADCSRA |= (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);

   // For now ADC connected to pin A7
   ADMUX |= (1<<MUX2)|(1<<MUX1)|(1<<MUX0);

   // start single conversion by writing ’1′ to ADSC
   //Useful since first conversion is longer
   //This bit will be ON untill conversion is done
   ADCSRA |= (1<<ADSC);
   while(ADCSRA & (1<<ADSC) );

   //Result will be readable in ADCH
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
 * \fn void read_buttons()
 * \brief update the values of the 36 keys and of the 12 buttons
 */
void read_buttons(){
  // Checking keys 0:5 by setting PB0 to 0, a no_operation is required for sync, see datasheet p60
  PORTB &= ~0x01;
  nop();
  keys_0 = ~(PIND);
  PORTB |= 0x01;
  // Checking keys 6:11 by setting PB2 to 0
  PORTB &= ~0x04;
  nop();
  keys_6 = ~(PIND);
  PORTB |= 0x04;
  // Checking keys 12:17 by setting PB3 to 0
  PORTB &= ~0x08;
  nop();
  keys_12 = ~(PIND);
  PORTB |= 0x08;
  // Checking keys 18:23 by setting PB4 to 0
  PORTB &= ~0x10;
  nop();
  keys_18 = ~(PIND);
  PORTB |= 0x10;
  // Checking keys 24:29 by setting PC0 to 0
  PORTC &= ~0x01;
  nop();
  keys_24 = ~(PIND);
  PORTC |= 0x01;
  // Checking keys 30:35 by setting PC1 to 0
  PORTC &= ~0x02;
  nop();
  keys_30 = ~(PIND);
  PORTC |= 0x02;

#ifdef KEY_NUMBER_42
  // Checking keys 36:41 by setting PC2 to 0
  PORTC &= ~0x04;
  nop();
  keys_36 = ~(PIND);
  PORTC |= 0x04;
  // Checking keys 42:47 by setting PC3 to 0
  PORTC &= ~0x08;
  nop();
  keys_42 = ~(PIND);
  PORTC |= 0x08;
#endif

  // Checking buttons_settings by setting PC4 to 0
  PORTC &= ~0x10;
  nop();
  buttons_settings = ~(PIND);
  PORTC |= 0x10;
}


/**
 * \fn void main()
 * \brief main function to loop over and over endlessly
 */
int main(){
  // Disable interrupts while initializing, cf p11
  SREG &= ~0x80;
  init_pins();
  init_adc();

  //Turn ON analog and midi LEDs
  PORTB |= (1<<DDB5);
  PORTC |= (1<<DDC5);

  // Wait until user press key 0 or key 1
  while( ((keys_0 & KEY_0_MSK) == 0) && ((keys_0 & KEY_1_MSK) == 0)){
    read_buttons();
  }

  // Switch between analog and MIDI depending on the pressed key
  if(keys_0 & KEY_0_MSK){
    analog_behaviour();
  }else if(keys_0 & KEY_1_MSK){
    midi_behaviour();
  }

  // Program won't actually go here
  return EXIT_FAILURE;
}
