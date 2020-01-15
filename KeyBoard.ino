/**
 * \brief This file aims at providing efficient code in order to create music thanks to MIDI standart associated with analog signal.
 * \author Brice Croix
 * \date june 25 of 2019
 */

 // Enables the debug functionalities
//#define DEBUG


// Define the number of keys, between 12 or 48
#define KEYS_NUMBER 36

// Enables the tremolo and vibrato potentiometers
#define ENABLE_TREMOLO_VIBRATO

#include <avr/io.h>
#include <avr/interrupt.h>


// The no-operation function as defined in assembly language
#define nop() __asm__("nop\n\t")

// Number of notes per octave
#define OCTAVE 12

// Number of available notes
#define NOTE_NUMBER 108

// Initial value of pitch_0
#define DEFAULT_PITCH_0 48;

// Max value of the pitch_0 variable, depending on the number of keys
#define MAX_PITCH_0 (NOTE_NUMBER - KEYS_NUMBER)

// Masks to check keys level in a key group
#define KEY_0_MSK 0x04
#define KEY_1_MSK 0x08
#define KEY_2_MSK 0x10
#define KEY_3_MSK 0x20
#define KEY_4_MSK 0x40
#define KEY_5_MSK 0x80
#define KEYS_RELEVANT_MSK 0xFC

/**
 * \brief Macro function to send a byte, waits for buffer to be empty
 */
#define USART_SEND(byte) while(!(UCSR0A & 0x20)); UDR0 = byte

/**
 * \brief Stores state of keys 0 to 5, same goes for key_6, key_12 etc. Only the 6 MSB are relevant on these variables. [k5,k4,k3,k2,k1,k0,xx,xx]
 */
volatile uint8_t keys_0 = 0;

/**
 * \brief stores the state of the keys at last sample
 */
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

/**
 * \brief index of the lowest key, 0 for C0, 2 for D0, 12 for C1...
 */
volatile int8_t pitch_0 = DEFAULT_PITCH_0;


#ifdef ENABLE_TREMOLO_VIBRATO
/**
 * \brief Stores the value of the vibrato potentiometer
 */
volatile uint8_t ADC_vibrato = 64;

/**
 * \brief A flag to indicate that the ADC vibrato value has been updated
 */
volatile uint8_t ADC_vibrato_flag = 0;

/**
 * \brief Stores the value of the tremolo potentiometer
 */
volatile uint8_t ADC_tremolo = 64;

/**
 * \brief A flag to indicate that the ADC tremolo value has been updated
 */
volatile uint8_t ADC_tremolo_flag = 0;
#endif


/**
 * \brief Sets pins PD7:2 as pulled-up inputs and pins PB5:0, PC3:0 as outputs
 */
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


#ifdef ENABLE_TREMOLO_VIBRATO
/**
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
#endif

/**
 * \brief update the values of all keys and option buttons
 */
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


#ifdef ENABLE_TREMOLO_VIBRATO
/**
 * \brief update, if available, the value of potentiometers
 */
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
#endif


/**
 * \brief Handles the settings buttons, without updating buttons_settings_last
 */
void process_settings(){
  if(buttons_settings & KEYS_RELEVANT_MSK){
    // If at least one button is pressed
    if((buttons_settings & KEY_0_MSK) && !(buttons_settings_last & KEY_0_MSK)){
      // If first button was just pressed, minus an octave
      pitch_0 -= OCTAVE;
      // Verify that pitch do not goes sub 0
      if(pitch_0 < 0) pitch_0 = 0;
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
      if(pitch_0 < 0) pitch_0 = 0;
    }
    if((buttons_settings & KEY_3_MSK) && !(buttons_settings_last & KEY_3_MSK)){
      // If fourth button was just pressed, add a semitone
      pitch_0 += 1;
      // Verify that pitch do not exceed max value
      if(pitch_0 > MAX_PITCH_0) pitch_0 = MAX_PITCH_0;
    }
    if((buttons_settings & KEY_4_MSK) && !(buttons_settings_last & KEY_4_MSK)){
      // If fifth button was just pressed, reset pitch_0 to closest C
      pitch_0 -= pitch_0 % OCTAVE;
    }
  }
}


/**
 * \brief main function containing the choice between analog and MIDI mode
 */
int main(){
  // Disable interrupts while initializing, cf p11 doc
  SREG &= ~0x80;
  init_pins();

  #ifdef ENABLE_TREMOLO_VIBRATO
  init_adc();
  #endif

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
