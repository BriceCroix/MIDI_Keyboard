/**
 * \brief This file aims at providing efficient code in order to create music thanks to MIDI standart associated with analog signal.
 * \author Brice Croix
 * \date june 25 of 2019
 */

#include "keyboard.h"

#include <avr/io.h>
#include <avr/interrupt.h>

volatile uint8_t keys_0 = 0;
volatile uint8_t keys_0_last = 0;
volatile uint8_t keys_6 = 0;
volatile uint8_t keys_6_last = 0;
#if KEYS_NUMBER >= 12
volatile uint8_t keys_12 = 0;
volatile uint8_t keys_12_last = 0;
#endif
#if KEYS_NUMBER >= 18
volatile uint8_t keys_18 = 0;
volatile uint8_t keys_18_last = 0;
#endif
#if KEYS_NUMBER >= 24
volatile uint8_t keys_24 = 0;
volatile uint8_t keys_24_last = 0;
#endif
#if KEYS_NUMBER >= 30
volatile uint8_t keys_30 = 0;
volatile uint8_t keys_30_last = 0;
#endif
#if KEYS_NUMBER >= 36
volatile uint8_t keys_36 = 0;
volatile uint8_t keys_36_last = 0;
#endif
#if KEYS_NUMBER >= 42
volatile uint8_t keys_42 = 0;
volatile uint8_t keys_42_last = 0;
#endif
volatile uint8_t buttons_settings = 0;
volatile uint8_t buttons_settings_last = 0;

volatile int8_t pitch_0 = DEFAULT_PITCH_0;

#ifdef ENABLE_VIBRATO

volatile uint8_t ADC_vibrato = 64;
volatile uint8_t ADC_vibrato_flag = 0;

#endif

#ifdef ENABLE_TREMOLO

volatile uint8_t ADC_tremolo = 64;
volatile uint8_t ADC_tremolo_flag = 0;

#endif


void init_pins(){
  // Enabling pull-ups for all ports
  MCUCR &= ~0x10;

  // PortD 2:7 as input for all buttons, this will allow to get all values in one instruction
  DDRD &= ~(1<<DDD2 | 1<<DDD3 | 1<<DDD4 | 1<<DDD5 | 1<<DDD6 | 1<<DDD7);
  // Put pins PD2:7 to pull-up
  PORTD |= 0xFC;

  // PWM output OCA1, which is on PB1, aka Pin9
  // PortB 0 and 2:5 as output for buttons from Keys 0 to 23
  // Built-in led also on PB5 as output
  DDRB |= (1<<DDB0 | 1<<DDB1 | 1<<DDB2 | 1<<DDB5);
  #if KEYS_NUMBER >= 12
  DDRB |= 1<<DDB3;
  #endif
  #if KEYS_NUMBER >= 18
  DDRB |= 1<<DDB3;
  #endif
  // Turning on all outputs for now, except LED
  PORTB |= 0x1D;

  // PortC 0:3 as output for keys 24 to 42
  #if KEYS_NUMBER >= 24
  DDRC |= 1<<DDC0;
  #endif
  #if KEYS_NUMBER >= 30
  DDRC |= 1<<DDC1;
  #endif
  #if KEYS_NUMBER >= 36
  DDRC |= 1<<DDC2;
  #endif
  #if KEYS_NUMBER >= 42
  DDRC |= 1<<DDC3;
  #endif
  // PC4 for settings buttons and PC5 for LED as outputs
  DDRC |= (1<<DDC4 | 1<<DDC5 );
  // Turning on all outputs for now, except LED
  PORTC |= 0x1F;
}


#if defined ENABLE_VIBRATO || defined ENABLE_TREMOLO

void init_adc(){
// Reference is AVcc = 5V
// Result is left aligned
ADMUX = (1<<REFS0) | (1<<ADLAR);

// ADC Enable
ADCSRA |= (1<<ADEN);

// Clear Prescaler
ADCSRA &= ~((1<<ADPS0) | (1<<ADPS1) | (1<<ADPS2) );
#if defined ENABLE_VIBRATO && defined ENABLE_TREMOLO
// Prescaler of 128, ADC_CLK = 125kHz
ADCSRA |= (1<<ADPS0)|(1<<ADPS1)|(1<<ADPS2);
#else
// Prescaler of 64, ADC_CLK = 250kHz
ADCSRA |= (1<<ADPS1)|(1<<ADPS2);
#endif

// For now ADC connected to pin A6
ADMUX |= (1<<MUX2)|(1<<MUX1);
#ifdef ENABLE_VIBRATO
// Connect to A7
ADMUX |= (1<<MUX0);
#endif

// start single conversion by writing ’1′ to ADSC
// Useful since first conversion is longer
// This bit will be ON untill conversion is done
ADCSRA |= (1<<ADSC);
while(ADCSRA & (1<<ADSC) );

//Result will be readable in ADCH
}
#endif


void read_buttons(){
  // Checking keys 0:5 by setting PB0 to 0, a no_operation is required for sync, see datasheet p60
  PORTB &= ~0x01;
  nop();
  keys_0 = ~(PIND);
  keys_0 &= KEYS_RELEVANT_MSK;
  PORTB |= 0x01;
  // Checking keys 6:11 by setting PB2 to 0
  PORTB &= ~0x04;
  nop();
  keys_6 = ~(PIND);
  keys_6 &= KEYS_RELEVANT_MSK;
  PORTB |= 0x04;
  #if KEYS_NUMBER >= 12
  // Checking keys 12:17 by setting PB3 to 0
  PORTB &= ~0x08;
  nop();
  keys_12 = ~(PIND);
  keys_12 &= KEYS_RELEVANT_MSK;
  PORTB |= 0x08;
  #endif
  #if KEYS_NUMBER >= 18
  // Checking keys 18:23 by setting PB4 to 0
  PORTB &= ~0x10;
  nop();
  keys_18 = ~(PIND);
  keys_18 &= KEYS_RELEVANT_MSK;
  PORTB |= 0x10;
  #endif
  #if KEYS_NUMBER >= 24
  // Checking keys 24:29 by setting PC0 to 0
  PORTC &= ~0x01;
  nop();
  keys_24 = ~(PIND);
  keys_24 &= KEYS_RELEVANT_MSK;
  PORTC |= 0x01;
  #endif
  #if KEYS_NUMBER >= 30
  // Checking keys 30:35 by setting PC1 to 0
  PORTC &= ~0x02;
  nop();
  keys_30 = ~(PIND);
  keys_30 &= KEYS_RELEVANT_MSK;
  PORTC |= 0x02;
  #endif
  #if KEYS_NUMBER >= 36
  // Checking keys 36:41 by setting PC2 to 0
  PORTC &= ~0x04;
  nop();
  keys_36 = ~(PIND);
  keys_36 &= KEYS_RELEVANT_MSK;
  PORTC |= 0x04;
  #endif
  #if KEYS_NUMBER >= 42
  // Checking keys 42:47 by setting PC3 to 0
  PORTC &= ~0x08;
  nop();
  keys_42 = ~(PIND);
  keys_42 &= KEYS_RELEVANT_MSK;
  PORTC |= 0x08;
  #endif
  // Checking buttons_settings by setting PC4 to 0
  PORTC &= ~0x10;
  nop();
  buttons_settings = ~(PIND);
  buttons_settings &= KEYS_RELEVANT_MSK;
  PORTC |= 0x10;
}


// switch read_pots implementation
#if defined ENABLE_VIBRATO && defined ENABLE_TREMOLO
void read_pots(){
  // If no ADC conversion is running
  if( !(ADCSRA & (1<<ADSC)) ){
    // What was the selected channel ?
    if(ADMUX & (1<<MUX0)){
      // Channel 7
      // Only act when there is a change
      if(ADC_vibrato != (ADCH >> 1)){
        // Precision is 7 bits
        ADC_vibrato = ADCH >> 1;
        ADC_vibrato_flag = 1;
      }
    }else{
      // Channel 6
      // Only act when there is a change
      if(ADC_tremolo != (ADCH >> 1)){
        // Precision is 7 bits
        ADC_tremolo = ADCH >> 1;
        ADC_tremolo_flag = 1;
      }
    }
    // Change channel
    ADMUX = ADMUX ^ (1<<MUX0);

    // Start new conversion
    ADCSRA |= (1<<ADSC);
  }
}

#elif defined ENABLE_VIBRATO && not defined ENABLE_TREMOLO
void read_pots(){
  // If no ADC conversion is running
  if( !(ADCSRA & (1<<ADSC)) ){
      // Channel 7
      // Only act when there is a change
      if(ADC_vibrato != (ADCH >> 1)){
        // Precision is 7 bits
        ADC_vibrato = ADCH >> 1;
        ADC_vibrato_flag = 1;
      }
    // Start new conversion
    ADCSRA |= (1<<ADSC);
  }
}

#elif not defined ENABLE_VIBRATO && defined ENABLE_TREMOLO
void read_pots(){
  // If no ADC conversion is running
  if( !(ADCSRA & (1<<ADSC)) ){
      // Channel 6
      // Only act when there is a change
      if(ADC_tremolo != (ADCH >> 1)){
        // Precision is 7 bits
        ADC_tremolo = ADCH >> 1;
        ADC_tremolo_flag = 1;
      }
    // Start new conversion
    ADCSRA |= (1<<ADSC);
  }
}
#endif


void process_settings(){
  if(buttons_settings & KEYS_RELEVANT_MSK){
    // If at least one button is pressed
    if((buttons_settings & KEY_0_MSK) && !(buttons_settings_last & KEY_0_MSK)){
      // If first button was just pressed, minus an octave
      pitch_0 -= OCTAVE;
      // Verify that pitch do not goes sub 0
      if(pitch_0 < 0) pitch_0 = FIRST_KEY;
    }
    if((buttons_settings & KEY_1_MSK) && !(buttons_settings_last & KEY_1_MSK)){
      // If second button was just pressed, add an octave
      pitch_0 += OCTAVE;
      // Verify that pitch do not exceed max value
      if(pitch_0 > MAX_PITCH_0) pitch_0 = MAX_PITCH_0;
    }
    if((buttons_settings & KEY_2_MSK) && !(buttons_settings_last & KEY_2_MSK)){
      // If third button was just pressed, minus a semitone
      pitch_0 -= 1;
      // Verify that pitch do not goes sub 0
      if(pitch_0 < 0) pitch_0 = FIRST_KEY;
    }
    if((buttons_settings & KEY_3_MSK) && !(buttons_settings_last & KEY_3_MSK)){
      // If fourth button was just pressed, add a semitone
      pitch_0 += 1;
      // Verify that pitch do not exceed max value
      if(pitch_0 > MAX_PITCH_0) pitch_0 = MAX_PITCH_0;
    }
    if((buttons_settings & KEY_4_MSK) && !(buttons_settings_last & KEY_4_MSK)){
      // If fifth button was just pressed, reset pitch_0
      pitch_0 = DEFAULT_PITCH_0;
    }
  }
}
