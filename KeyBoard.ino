/**
 * \file KeyBoard.ino
 * \brief This file aims at providing efficient code in order to create music thanks to MIDI standart associated with analog signal.
 * \author Brice Croix
 * \date june 25 of 2019
 */
#define DEBUG

#include <avr/io.h>
#include "binary.h"

//Sample frequency in Hz
#define SAMPLE_FREQUENCY 31250
//Sample time in micro second
#define SAMPLE_TIME 32
//The no-operation function as defined in assembly language
#define nop() __asm__("nop\n\t")
//The analog amplitude for one note, defining how many notes can be played at once, here 8 notes because 8*64 = 512
#define NOTE_AMP 64
//Number of notes per octave
#define OCTAVE 12
//Minimum duty value for analog output, notice NOTE_AMP/2
#define PWM_MIN 32
//Masks to check keys level in a key group
#define KEY_0_MSK 0x04
#define KEY_1_MSK 0x08
#define KEY_2_MSK 0x10
#define KEY_3_MSK 0x20
#define KEY_4_MSK 0x40
#define KEY_5_MSK 0x80

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

//MIDI code for each note
const uint8_t MIDI_NOTES[] = {
  0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13, //C0 to B0
  0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, //C1 to B1
  0x1c, 0x1d, 0x1e, 0x1f, 0x20, 0x21, 0x22, 0x23, //C2 to B2
  0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b, //C3 to B3
  0x2c, 0x2d, 0x2e, 0x2f, 0x30, 0x31, 0x32, 0x33, //C4 to B4
  0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b, //C5 to B5
  0x3c, 0x3d, 0x3e, 0x3f, 0x40, 0x41, 0x42, 0x43, //C6 to B6
  0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a, 0x4b, //C7 to B7
  0x4c, 0x4d, 0x4e, 0x4f, 0x50, 0x51, 0x52, 0x53, //C8 to B8
};



/**
 * \var keys_0
 * \brief Stores state of keys 0 to 5, same goes for key_6, key_12 etc. Only the 6 MSB are relevant on these variables. [k5,k4,k3,k2,k1,k0,xx,xx]
 */
volatile uint8_t keys_0;
volatile uint8_t keys_6;
volatile uint8_t keys_12;
volatile uint8_t keys_18;
volatile uint8_t keys_24;
volatile uint8_t keys_30;
uint8_t buttons_settings_1;
uint8_t buttons_settings_2;

/**
 * \var keys_0_last
 * \brief stores the state of the keys at last sample
 */
volatile uint8_t keys_0_last;
volatile uint8_t keys_6_last;
volatile uint8_t keys_12_last;
volatile uint8_t keys_18_last;
volatile uint8_t keys_24_last;
volatile uint8_t keys_30_last;

/**
 * \var pitch_0
 * \brief index of the lowest key, 0 for C0, 2 for D0, 12 for C1...
 */
uint8_t pitch_0 = 24;

/**
 * \var t
 * \brief time in micro second
 */
volatile uint64_t t = 0;

/**
 * \var analog_out
 * \brief the analog value to write on the analog output
 */
volatile uint16_t analog_out = 0;

/**
 * \var flag_request_update
 * \brief a flag to indicate that the analog output value needs to be updated
 */
volatile uint8_t flag_request_update = 0;

/**
 * \fn void init_timer_1()
 * \brief Sets timer 1 in condition to generate PWM signal
 */
void init_timer_1(){
  //Enable timer1 with no prescaler, cf doc p110
  TCCR1B &= ~0x07; //Clears bits CS10:12 that handle timer activation
  TCCR1B |= (1<<CS10); //The timer is supposed to count at 16 Mhz, an external precision clock could be used if necessary

  //Reset counter value
  TCNT1 = 0;

  //Enable only OVF interrupts, cf doc p112
  TIMSK1 = 0;
  TIMSK1 |= 0x01;

  //Timer 1 in fast PWM mode, cf p108, WGM13:10 = 1110
  TCCR1A &= ~0x03; //Clear bits WGM10:11
  TCCR1A |= 1<<WGM11; //Set WGM11 to 1
  TCCR1B |= 1<<WGM13 | 1<<WGM12; //Set WGM13:12 to 1
  //In this mode the Auto-Reload value (TOP, or ARR for a STM32) is in the Input Capture Register
  ICR1 = 0x01FF; //Sampling Frequency = 31250 Hz with 9 bits resolution

  //Connect OC1A to pin PB1 in non-inverting PWM mode, cf p108
  TCCR1A &= ~0xF0;
  TCCR1A |= 1<<COM1A1;

  /*arbitrary min value, 512 as ARR authorize 8 notes of 64 as velocity
   let us use 32 as min value and 480 as max, this means that it will
   saturate above 7 simultaneous notes*/
  OCR1A = PWM_MIN;

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
  DDRD &= ~(1<<DDD2 | 1<<DDD3 | 1<<DDD4 | 1<<DDD5 | 1<<DDD6 | 1<<DDD7);
  //Put pins PD2:7 to pull-up
  PORTD |= B11111100;

  //PWM output OCA1, which is on PB1, aka Pin9
  //PortB 0 and 2:5 as output for buttons from Keys 0 to 23
  //Built-in led also on PB5 as output
  DDRB |= (1<<DDB0 | 1<<DDB1 | 1<<DDB2 | 1<<DDB3 | 1<<DDB4 | 1<<DDB5);
  //Turning on all outputs for now
  PORTB |= B00011101;

  //PortC 0:3 as output for keys 24 to 35 and buttons settings 1 and 2
  DDRC |= (1<<DDC0 | 1<<DDC1 | 1<<DDC2 | 1<<DDC3);
  //Turning on all outputs for now
  PORTC |= B00001111;
}

/**
 * \fn init_serial()
 * \brief initialize the serial peripheral interface
 */
void init_serial(){
  //Enable SPI
  SPCR |= 0x40;
  //Disable SPI interrupts
  SPCR &= ~0x80;
  //LSB first
  SPCR |= 0x20;
  //Master mode
  SPCR |= 0x10;
  //Baud rate
  Serial.begin(9600);
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
  //Let us update the analog pin output value
  OCR1A = analog_out;
  //Request an output update
  flag_request_update = 1;
}

uint8_t getSquareWave(uint64_t t, uint16_t period){
  if(t%period < (period>>1)){
    //High
    return NOTE_AMP;
  }else{
    //Low
    return 0;
  }
}

/**
 * \fn void setAnalogOut()
 * \brief Sets the analog_out value according to the keys state. WARNING : Output should not be written while this function is computing
 */
void setAnalogOut(){
  //In order not to access memory multiple times
  uint8_t current_pitch_0 = pitch_0;
  //Variable to store current considered period
  uint16_t T;
  //Variable to temporarily store wave value at given time
  uint8_t value_current = 0;
  //Variable to store wave value at last sample
  uint8_t value_last = 0;

  // Is first key pressed ?
  if(keys_0 & KEY_0_MSK){
    // If first key pressed, was it pressed last sample ?
    if(keys_0_last & KEY_0_MSK){
      // If was pressed
      T = PERIODS[current_pitch_0];
      value_current = getSquareWave(t, T);
      value_last = getSquareWave(t-SAMPLE_TIME, T);
      analog_out += (value_current - value_last);
    }else{
      // Was not pressed last sample, it is a change
      T = PERIODS[current_pitch_0];
      value_current = getSquareWave(t,T);
      analog_out += value_current;
    }
  }else{
    // The first key is not pressed, was it pressed last sample ?
    if(keys_0_last & KEY_0_MSK){
      // If was pressed, it is a change
      T = PERIODS[current_pitch_0];
      value_current = getSquareWave(t, T);
      analog_out -= value_current;
    }
    // Nothing to do if key not pressed and was not pressed
  }
  // Same thing for all 36 keys :
  if(keys_0 & KEY_1_MSK){
  T = PERIODS[current_pitch_0 + 1];
  if(keys_0_last & KEY_1_MSK){
    value_current = getSquareWave(t, T);
    value_last = getSquareWave(t-SAMPLE_TIME, T);
    analog_out += (value_current - value_last);
  }else{
    value_current = getSquareWave(t,T);
    analog_out += value_current;
  }
}else{
  if(keys_0_last & KEY_1_MSK){
    T = PERIODS[current_pitch_0 + 1];
    value_current = getSquareWave(t, T);
    analog_out -= value_current;
  }
}

if(keys_0 & KEY_2_MSK){
  T = PERIODS[current_pitch_0 + 2];
  if(keys_0_last & KEY_2_MSK){
    value_current = getSquareWave(t, T);
    value_last = getSquareWave(t-SAMPLE_TIME, T);
    analog_out += (value_current - value_last);
  }else{
    value_current = getSquareWave(t,T);
    analog_out += value_current;
  }
}else{
  if(keys_0_last & KEY_2_MSK){
    T = PERIODS[current_pitch_0 + 2];
    value_current = getSquareWave(t, T);
    analog_out -= value_current;
  }
}

if(keys_0 & KEY_3_MSK){
  T = PERIODS[current_pitch_0 + 3];
  if(keys_0_last & KEY_3_MSK){
    value_current = getSquareWave(t, T);
    value_last = getSquareWave(t-SAMPLE_TIME, T);
    analog_out += (value_current - value_last);
  }else{
    value_current = getSquareWave(t,T);
    analog_out += value_current;
  }
}else{
  if(keys_0_last & KEY_3_MSK){
    T = PERIODS[current_pitch_0 + 3];
    value_current = getSquareWave(t, T);
    analog_out -= value_current;
  }
}

if(keys_0 & KEY_4_MSK){
  T = PERIODS[current_pitch_0 + 4];
  if(keys_0_last & KEY_4_MSK){
    value_current = getSquareWave(t, T);
    value_last = getSquareWave(t-SAMPLE_TIME, T);
    analog_out += (value_current - value_last);
  }else{
    value_current = getSquareWave(t,T);
    analog_out += value_current;
  }
}else{
  if(keys_0_last & KEY_4_MSK){
    T = PERIODS[current_pitch_0 + 4];
    value_current = getSquareWave(t, T);
    analog_out -= value_current;
  }
}

if(keys_0 & KEY_5_MSK){
  T = PERIODS[current_pitch_0 + 5];
  if(keys_0_last & KEY_5_MSK){
    value_current = getSquareWave(t, T);
    value_last = getSquareWave(t-SAMPLE_TIME, T);
    analog_out += (value_current - value_last);
  }else{
    value_current = getSquareWave(t,T);
    analog_out += value_current;
  }
}else{
  if(keys_0_last & KEY_5_MSK){
    T = PERIODS[current_pitch_0 + 5];
    value_current = getSquareWave(t, T);
    analog_out -= value_current;
  }
}

if(keys_6 & KEY_0_MSK){
  T = PERIODS[current_pitch_0 + 6];
  if(keys_6_last & KEY_0_MSK){
    value_current = getSquareWave(t, T);
    value_last = getSquareWave(t-SAMPLE_TIME, T);
    analog_out += (value_current - value_last);
  }else{
    value_current = getSquareWave(t,T);
    analog_out += value_current;
  }
}else{
  if(keys_6_last & KEY_0_MSK){
    T = PERIODS[current_pitch_0 + 6];
    value_current = getSquareWave(t, T);
    analog_out -= value_current;
  }
}

if(keys_6 & KEY_1_MSK){
  T = PERIODS[current_pitch_0 + 7];
  if(keys_6_last & KEY_1_MSK){
    value_current = getSquareWave(t, T);
    value_last = getSquareWave(t-SAMPLE_TIME, T);
    analog_out += (value_current - value_last);
  }else{
    value_current = getSquareWave(t,T);
    analog_out += value_current;
  }
}else{
  if(keys_6_last & KEY_1_MSK){
    T = PERIODS[current_pitch_0 + 7];
    value_current = getSquareWave(t, T);
    analog_out -= value_current;
  }
}

if(keys_6 & KEY_2_MSK){
  T = PERIODS[current_pitch_0 + 8];
  if(keys_6_last & KEY_2_MSK){
    value_current = getSquareWave(t, T);
    value_last = getSquareWave(t-SAMPLE_TIME, T);
    analog_out += (value_current - value_last);
  }else{
    value_current = getSquareWave(t,T);
    analog_out += value_current;
  }
}else{
  if(keys_6_last & KEY_2_MSK){
    T = PERIODS[current_pitch_0 + 8];
    value_current = getSquareWave(t, T);
    analog_out -= value_current;
  }
}

if(keys_6 & KEY_3_MSK){
  T = PERIODS[current_pitch_0 + 9];
  if(keys_6_last & KEY_3_MSK){
    value_current = getSquareWave(t, T);
    value_last = getSquareWave(t-SAMPLE_TIME, T);
    analog_out += (value_current - value_last);
  }else{
    value_current = getSquareWave(t,T);
    analog_out += value_current;
  }
}else{
  if(keys_6_last & KEY_3_MSK){
    T = PERIODS[current_pitch_0 + 9];
    value_current = getSquareWave(t, T);
    analog_out -= value_current;
  }
}

if(keys_6 & KEY_4_MSK){
  T = PERIODS[current_pitch_0 + 10];
  if(keys_6_last & KEY_4_MSK){
    value_current = getSquareWave(t, T);
    value_last = getSquareWave(t-SAMPLE_TIME, T);
    analog_out += (value_current - value_last);
  }else{
    value_current = getSquareWave(t,T);
    analog_out += value_current;
  }
}else{
  if(keys_6_last & KEY_4_MSK){
    T = PERIODS[current_pitch_0 + 10];
    value_current = getSquareWave(t, T);
    analog_out -= value_current;
  }
}

if(keys_6 & KEY_5_MSK){
  T = PERIODS[current_pitch_0 + 11];
  if(keys_6_last & KEY_5_MSK){
    value_current = getSquareWave(t, T);
    value_last = getSquareWave(t-SAMPLE_TIME, T);
    analog_out += (value_current - value_last);
  }else{
    value_current = getSquareWave(t,T);
    analog_out += value_current;
  }
}else{
  if(keys_6_last & KEY_5_MSK){
    T = PERIODS[current_pitch_0 + 11];
    value_current = getSquareWave(t, T);
    analog_out -= value_current;
  }
}

if(keys_12 & KEY_0_MSK){
  T = PERIODS[current_pitch_0 + 12];
  if(keys_12_last & KEY_0_MSK){
    value_current = getSquareWave(t, T);
    value_last = getSquareWave(t-SAMPLE_TIME, T);
    analog_out += (value_current - value_last);
  }else{
    value_current = getSquareWave(t,T);
    analog_out += value_current;
  }
}else{
  if(keys_12_last & KEY_0_MSK){
    T = PERIODS[current_pitch_0 + 12];
    value_current = getSquareWave(t, T);
    analog_out -= value_current;
  }
}

if(keys_12 & KEY_1_MSK){
  T = PERIODS[current_pitch_0 + 13];
  if(keys_12_last & KEY_1_MSK){
    value_current = getSquareWave(t, T);
    value_last = getSquareWave(t-SAMPLE_TIME, T);
    analog_out += (value_current - value_last);
  }else{
    value_current = getSquareWave(t,T);
    analog_out += value_current;
  }
}else{
  if(keys_12_last & KEY_1_MSK){
    T = PERIODS[current_pitch_0 + 13];
    value_current = getSquareWave(t, T);
    analog_out -= value_current;
  }
}

if(keys_12 & KEY_2_MSK){
  T = PERIODS[current_pitch_0 + 14];
  if(keys_12_last & KEY_2_MSK){
    value_current = getSquareWave(t, T);
    value_last = getSquareWave(t-SAMPLE_TIME, T);
    analog_out += (value_current - value_last);
  }else{
    value_current = getSquareWave(t,T);
    analog_out += value_current;
  }
}else{
  if(keys_12_last & KEY_2_MSK){
    T = PERIODS[current_pitch_0 + 14];
    value_current = getSquareWave(t, T);
    analog_out -= value_current;
  }
}

if(keys_12 & KEY_3_MSK){
  T = PERIODS[current_pitch_0 + 15];
  if(keys_12_last & KEY_3_MSK){
    value_current = getSquareWave(t, T);
    value_last = getSquareWave(t-SAMPLE_TIME, T);
    analog_out += (value_current - value_last);
  }else{
    value_current = getSquareWave(t,T);
    analog_out += value_current;
  }
}else{
  if(keys_12_last & KEY_3_MSK){
    T = PERIODS[current_pitch_0 + 15];
    value_current = getSquareWave(t, T);
    analog_out -= value_current;
  }
}

if(keys_12 & KEY_4_MSK){
  T = PERIODS[current_pitch_0 + 16];
  if(keys_12_last & KEY_4_MSK){
    value_current = getSquareWave(t, T);
    value_last = getSquareWave(t-SAMPLE_TIME, T);
    analog_out += (value_current - value_last);
  }else{
    value_current = getSquareWave(t,T);
    analog_out += value_current;
  }
}else{
  if(keys_12_last & KEY_4_MSK){
    T = PERIODS[current_pitch_0 + 16];
    value_current = getSquareWave(t, T);
    analog_out -= value_current;
  }
}

if(keys_12 & KEY_5_MSK){
  T = PERIODS[current_pitch_0 + 17];
  if(keys_12_last & KEY_5_MSK){
    value_current = getSquareWave(t, T);
    value_last = getSquareWave(t-SAMPLE_TIME, T);
    analog_out += (value_current - value_last);
  }else{
    value_current = getSquareWave(t,T);
    analog_out += value_current;
  }
}else{
  if(keys_12_last & KEY_5_MSK){
    T = PERIODS[current_pitch_0 + 17];
    value_current = getSquareWave(t, T);
    analog_out -= value_current;
  }
}

if(keys_18 & KEY_0_MSK){
  T = PERIODS[current_pitch_0 + 18];
  if(keys_18_last & KEY_0_MSK){
    value_current = getSquareWave(t, T);
    value_last = getSquareWave(t-SAMPLE_TIME, T);
    analog_out += (value_current - value_last);
  }else{
    value_current = getSquareWave(t,T);
    analog_out += value_current;
  }
}else{
  if(keys_18_last & KEY_0_MSK){
    T = PERIODS[current_pitch_0 + 18];
    value_current = getSquareWave(t, T);
    analog_out -= value_current;
  }
}

if(keys_18 & KEY_1_MSK){
  T = PERIODS[current_pitch_0 + 19];
  if(keys_18_last & KEY_1_MSK){
    value_current = getSquareWave(t, T);
    value_last = getSquareWave(t-SAMPLE_TIME, T);
    analog_out += (value_current - value_last);
  }else{
    value_current = getSquareWave(t,T);
    analog_out += value_current;
  }
}else{
  if(keys_18_last & KEY_1_MSK){
    T = PERIODS[current_pitch_0 + 19];
    value_current = getSquareWave(t, T);
    analog_out -= value_current;
  }
}

if(keys_18 & KEY_2_MSK){
  T = PERIODS[current_pitch_0 + 20];
  if(keys_18_last & KEY_2_MSK){
    value_current = getSquareWave(t, T);
    value_last = getSquareWave(t-SAMPLE_TIME, T);
    analog_out += (value_current - value_last);
  }else{
    value_current = getSquareWave(t,T);
    analog_out += value_current;
  }
}else{
  if(keys_18_last & KEY_2_MSK){
    T = PERIODS[current_pitch_0 + 20];
    value_current = getSquareWave(t, T);
    analog_out -= value_current;
  }
}

if(keys_18 & KEY_3_MSK){
  T = PERIODS[current_pitch_0 + 21];
  if(keys_18_last & KEY_3_MSK){
    value_current = getSquareWave(t, T);
    value_last = getSquareWave(t-SAMPLE_TIME, T);
    analog_out += (value_current - value_last);
  }else{
    value_current = getSquareWave(t,T);
    analog_out += value_current;
  }
}else{
  if(keys_18_last & KEY_3_MSK){
    T = PERIODS[current_pitch_0 + 21];
    value_current = getSquareWave(t, T);
    analog_out -= value_current;
  }
}

if(keys_18 & KEY_4_MSK){
  T = PERIODS[current_pitch_0 + 22];
  if(keys_18_last & KEY_4_MSK){
    value_current = getSquareWave(t, T);
    value_last = getSquareWave(t-SAMPLE_TIME, T);
    analog_out += (value_current - value_last);
  }else{
    value_current = getSquareWave(t,T);
    analog_out += value_current;
  }
}else{
  if(keys_18_last & KEY_4_MSK){
    T = PERIODS[current_pitch_0 + 22];
    value_current = getSquareWave(t, T);
    analog_out -= value_current;
  }
}

if(keys_18 & KEY_5_MSK){
  T = PERIODS[current_pitch_0 + 23];
  if(keys_18_last & KEY_5_MSK){
    value_current = getSquareWave(t, T);
    value_last = getSquareWave(t-SAMPLE_TIME, T);
    analog_out += (value_current - value_last);
  }else{
    value_current = getSquareWave(t,T);
    analog_out += value_current;
  }
}else{
  if(keys_18_last & KEY_5_MSK){
    T = PERIODS[current_pitch_0 + 23];
    value_current = getSquareWave(t, T);
    analog_out -= value_current;
  }
}

if(keys_24 & KEY_0_MSK){
  T = PERIODS[current_pitch_0 + 24];
  if(keys_24_last & KEY_0_MSK){
    value_current = getSquareWave(t, T);
    value_last = getSquareWave(t-SAMPLE_TIME, T);
    analog_out += (value_current - value_last);
  }else{
    value_current = getSquareWave(t,T);
    analog_out += value_current;
  }
}else{
  if(keys_24_last & KEY_0_MSK){
    T = PERIODS[current_pitch_0 + 24];
    value_current = getSquareWave(t, T);
    analog_out -= value_current;
  }
}

if(keys_24 & KEY_1_MSK){
  T = PERIODS[current_pitch_0 + 25];
  if(keys_24_last & KEY_1_MSK){
    value_current = getSquareWave(t, T);
    value_last = getSquareWave(t-SAMPLE_TIME, T);
    analog_out += (value_current - value_last);
  }else{
    value_current = getSquareWave(t,T);
    analog_out += value_current;
  }
}else{
  if(keys_24_last & KEY_1_MSK){
    T = PERIODS[current_pitch_0 + 25];
    value_current = getSquareWave(t, T);
    analog_out -= value_current;
  }
}

if(keys_24 & KEY_2_MSK){
  T = PERIODS[current_pitch_0 + 26];
  if(keys_24_last & KEY_2_MSK){
    value_current = getSquareWave(t, T);
    value_last = getSquareWave(t-SAMPLE_TIME, T);
    analog_out += (value_current - value_last);
  }else{
    value_current = getSquareWave(t,T);
    analog_out += value_current;
  }
}else{
  if(keys_24_last & KEY_2_MSK){
    T = PERIODS[current_pitch_0 + 26];
    value_current = getSquareWave(t, T);
    analog_out -= value_current;
  }
}

if(keys_24 & KEY_3_MSK){
  T = PERIODS[current_pitch_0 + 27];
  if(keys_24_last & KEY_3_MSK){
    value_current = getSquareWave(t, T);
    value_last = getSquareWave(t-SAMPLE_TIME, T);
    analog_out += (value_current - value_last);
  }else{
    value_current = getSquareWave(t,T);
    analog_out += value_current;
  }
}else{
  if(keys_24_last & KEY_3_MSK){
    T = PERIODS[current_pitch_0 + 27];
    value_current = getSquareWave(t, T);
    analog_out -= value_current;
  }
}

if(keys_24 & KEY_4_MSK){
  T = PERIODS[current_pitch_0 + 28];
  if(keys_24_last & KEY_4_MSK){
    value_current = getSquareWave(t, T);
    value_last = getSquareWave(t-SAMPLE_TIME, T);
    analog_out += (value_current - value_last);
  }else{
    value_current = getSquareWave(t,T);
    analog_out += value_current;
  }
}else{
  if(keys_24_last & KEY_4_MSK){
    T = PERIODS[current_pitch_0 + 28];
    value_current = getSquareWave(t, T);
    analog_out -= value_current;
  }
}

if(keys_24 & KEY_5_MSK){
  T = PERIODS[current_pitch_0 + 29];
  if(keys_24_last & KEY_5_MSK){
    value_current = getSquareWave(t, T);
    value_last = getSquareWave(t-SAMPLE_TIME, T);
    analog_out += (value_current - value_last);
  }else{
    value_current = getSquareWave(t,T);
    analog_out += value_current;
  }
}else{
  if(keys_24_last & KEY_5_MSK){
    T = PERIODS[current_pitch_0 + 29];
    value_current = getSquareWave(t, T);
    analog_out -= value_current;
  }
}

if(keys_30 & KEY_0_MSK){
  T = PERIODS[current_pitch_0 + 30];
  if(keys_30_last & KEY_0_MSK){
    value_current = getSquareWave(t, T);
    value_last = getSquareWave(t-SAMPLE_TIME, T);
    analog_out += (value_current - value_last);
  }else{
    value_current = getSquareWave(t,T);
    analog_out += value_current;
  }
}else{
  if(keys_30_last & KEY_0_MSK){
    T = PERIODS[current_pitch_0 + 30];
    value_current = getSquareWave(t, T);
    analog_out -= value_current;
  }
}

if(keys_30 & KEY_1_MSK){
  T = PERIODS[current_pitch_0 + 31];
  if(keys_30_last & KEY_1_MSK){
    value_current = getSquareWave(t, T);
    value_last = getSquareWave(t-SAMPLE_TIME, T);
    analog_out += (value_current - value_last);
  }else{
    value_current = getSquareWave(t,T);
    analog_out += value_current;
  }
}else{
  if(keys_30_last & KEY_1_MSK){
    T = PERIODS[current_pitch_0 + 31];
    value_current = getSquareWave(t, T);
    analog_out -= value_current;
  }
}

if(keys_30 & KEY_2_MSK){
  T = PERIODS[current_pitch_0 + 32];
  if(keys_30_last & KEY_2_MSK){
    value_current = getSquareWave(t, T);
    value_last = getSquareWave(t-SAMPLE_TIME, T);
    analog_out += (value_current - value_last);
  }else{
    value_current = getSquareWave(t,T);
    analog_out += value_current;
  }
}else{
  if(keys_30_last & KEY_2_MSK){
    T = PERIODS[current_pitch_0 + 32];
    value_current = getSquareWave(t, T);
    analog_out -= value_current;
  }
}

if(keys_30 & KEY_3_MSK){
  T = PERIODS[current_pitch_0 + 33];
  if(keys_30_last & KEY_3_MSK){
    value_current = getSquareWave(t, T);
    value_last = getSquareWave(t-SAMPLE_TIME, T);
    analog_out += (value_current - value_last);
  }else{
    value_current = getSquareWave(t,T);
    analog_out += value_current;
  }
}else{
  if(keys_30_last & KEY_3_MSK){
    T = PERIODS[current_pitch_0 + 33];
    value_current = getSquareWave(t, T);
    analog_out -= value_current;
  }
}

if(keys_30 & KEY_4_MSK){
  T = PERIODS[current_pitch_0 + 34];
  if(keys_30_last & KEY_4_MSK){
    value_current = getSquareWave(t, T);
    value_last = getSquareWave(t-SAMPLE_TIME, T);
    analog_out += (value_current - value_last);
  }else{
    value_current = getSquareWave(t,T);
    analog_out += value_current;
  }
}else{
  if(keys_30_last & KEY_4_MSK){
    T = PERIODS[current_pitch_0 + 34];
    value_current = getSquareWave(t, T);
    analog_out -= value_current;
  }
}

if(keys_30 & KEY_5_MSK){
  T = PERIODS[current_pitch_0 + 35];
  if(keys_30_last & KEY_5_MSK){
    value_current = getSquareWave(t, T);
    value_last = getSquareWave(t-SAMPLE_TIME, T);
    analog_out += (value_current - value_last);
  }else{
    value_current = getSquareWave(t,T);
    analog_out += value_current;
  }
}else{
  if(keys_30_last & KEY_5_MSK){
    T = PERIODS[current_pitch_0 + 35];
    value_current = getSquareWave(t, T);
    analog_out -= value_current;
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
  init_timer_1();
  init_pins();
  //init_serial();
  //Enable interrupts
  SREG |= 0x80;

  PORTB |= B00100000;

  while(1){
    //Checking keys 0:5 by setting PB0 to 0, a no_operation is required for sync, see datasheet p60
    PORTB &= ~0x01;
    nop();
    keys_0 = ~(PIND);
    PORTB |= 0x01;
    //Checking keys 6:11 by setting PB2 to 0
    PORTB &= ~0x04;
    nop();
    keys_6 = ~(PIND);
    PORTB |= 0x04;
    //Checking keys 12:17 by setting PB3 to 0
    PORTB &= ~0x08;
    nop();
    keys_12 = ~(PIND);
    PORTB |= 0x08;
    //Checking keys 18:23 by setting PB4 to 0
    PORTB &= ~0x10;
    nop();
    keys_18 = ~(PIND);
    PORTB |= 0x10;
    //Checking keys 24:29 by setting PC0 to 0
    PORTC &= ~0x01;
    nop();
    keys_24 = ~(PIND);
    PORTC |= 0x01;
    //Checking keys 30:35 by setting PC1 to 0
    PORTC &= ~0x02;
    nop();
    keys_30 = ~(PIND);
    PORTC |= 0x02;
    //Checking buttons_settings_1 by setting PC2 to 0
    PORTC &= ~0x04;
    nop();
    buttons_settings_1 = ~(PIND);
    PORTC |= 0x04;
    //Checking buttons_settings_2 by setting PC3 to 0
    PORTC &= ~0x08;
    nop();
    buttons_settings_2 = ~(PIND);
    PORTC |= 0x08;

#ifdef DEBUG
    //TOREMOVE : sets PB5 to high by putting PD2 to GND and to low by putting PD3 to GND
    if(keys_0 & KEY_0_MSK){
      PORTB |= 1<<DDB5;
    }
    if(keys_0 & KEY_1_MSK){
      PORTB &= ~ (1<<DDB5);
    }
    if(keys_0 & KEY_2_MSK){
      PORTB |= 1<<DDB5;
    }
#endif

    if(flag_request_update){
      setAnalogOut();
      keys_0_last = keys_0;
      keys_6_last = keys_6;
      keys_12_last = keys_12;
      keys_18_last = keys_18;
      keys_24_last = keys_24;
      keys_30_last = keys_30;
      flag_request_update = 0;
    }
  }
  //Program won't actually go outside this loop

  return EXIT_FAILURE;
}
