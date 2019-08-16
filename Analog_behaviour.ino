/**
 * \file Analog_behaviour.ino
 * \brief code for the keyboard analog behaviour
 * \author Brice Croix
 */


// Minimum duty value for analog output, notice NOTE_AMP/2
#define PWM_MIN 32
// The analog amplitude for one note, defining how many notes can be played at once, here 8 notes because 8*64 = 512
#define NOTE_AMP 64
// Sample frequency in Hz
#define SAMPLE_FREQUENCY 31250
// Sample time in micro second
#define SAMPLE_TIME 32


/**
 * \var PERIODS
 * \brief Each period in us from C0 to B8
 */
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
 * \fn void init_timer_1()
 * \brief Sets timer 1 in condition to generate PWM signal
 */
void init_timer_1(){
  // Enable timer1 with no prescaler, cf doc p110
  TCCR1B &= ~0x07; // Clears bits CS10:12 that handle timer activation
  TCCR1B |= (1<<CS10); // The timer is supposed to count at 16 Mhz, an external precision clock could be used if necessary

  // Reset counter value
  TCNT1 = 0;

  // Enable only OVF interrupts, cf doc p112
  TIMSK1 = 0;
  TIMSK1 |= 0x01;

  // Timer 1 in fast PWM mode, cf p108, WGM13:10 = 1110
  TCCR1A &= ~0x03; // Clear bits WGM10:11
  TCCR1A |= 1<<WGM11; // Set WGM11 to 1
  TCCR1B |= 1<<WGM13 | 1<<WGM12; // Set WGM13:12 to 1
  // In this mode the Auto-Reload value (TOP, or ARR for a STM32) is in the Input Capture Register
  ICR1 = 0x01FF; // Sampling Frequency = 31250 Hz with 9 bits resolution

  // Connect OC1A to pin PB1 in non-inverting PWM mode, cf p108
  TCCR1A &= ~0xF0;
  TCCR1A |= 1<<COM1A1;

  /*arbitrary min value, 512 as ARR authorize 8 notes of 64 as velocity
   let us use 32 as min value and 480 as max, this means that it will
   saturate above 7 simultaneous notes*/
  OCR1A = PWM_MIN;

  // N.B. OC1A (channel A) output is on pin 9 and OC1B on pin 10
}


/**
 * \fn ISR(TIMER1_OVF_vect)
 * \brief interruption code when an ovf occurs on timer 1
 */
ISR(TIMER1_OVF_vect){
  // Let us update the analog pin output value
  OCR1A = analog_out;

  // Let us update time
  t += SAMPLE_TIME;

  // Update analog value for next sample
  setAnalogOut();
}


uint8_t getSquareWave(uint16_t period, float tremolo_multiplier){
  if(t%period < (period>>1)){
    //High
    return (NOTE_AMP * tremolo_multiplier);
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
  // Variable to temporarily store analog out
  uint16_t analog_out_temp = PWM_MIN;

  // Update the frequency with its pitch shift
  // This formula allows for 7 semitones up, more down
  float vibrato_T_multiplier = ADC_vibrato * (-0.002598282) + 1.329981791;

  // Update the velocity multiplier
  // This formula allows for nulling or doubling the velocity (number is 1/127)
  float tremolo_multiplier = ADC_tremolo * 0.007874016;

  // Is key 0 pressed ?
  if(keys_0 & KEY_0_MSK){
  T = PERIODS[current_pitch_0 + 0] * vibrato_T_multiplier;
  analog_out_temp += getSquareWave(T, tremolo_multiplier);
  }
  // Is key 1 pressed ?
  if(keys_0 & KEY_1_MSK){
  T = PERIODS[current_pitch_0 + 1] * vibrato_T_multiplier;
  analog_out_temp += getSquareWave(T, tremolo_multiplier);
  }
  // Is key 2 pressed ?
  if(keys_0 & KEY_2_MSK){
  T = PERIODS[current_pitch_0 + 2] * vibrato_T_multiplier;
  analog_out_temp += getSquareWave(T, tremolo_multiplier);
  }
  // Is key 3 pressed ?
  if(keys_0 & KEY_3_MSK){
  T = PERIODS[current_pitch_0 + 3] * vibrato_T_multiplier;
  analog_out_temp += getSquareWave(T, tremolo_multiplier);
  }
  // Is key 4 pressed ?
  if(keys_0 & KEY_4_MSK){
  T = PERIODS[current_pitch_0 + 4] * vibrato_T_multiplier;
  analog_out_temp += getSquareWave(T, tremolo_multiplier);
  }
  // Is key 5 pressed ?
  if(keys_0 & KEY_5_MSK){
  T = PERIODS[current_pitch_0 + 5] * vibrato_T_multiplier;
  analog_out_temp += getSquareWave(T, tremolo_multiplier);
  }
  // Is key 6 pressed ?
  if(keys_6 & KEY_0_MSK){
  T = PERIODS[current_pitch_0 + 6] * vibrato_T_multiplier;
  analog_out_temp += getSquareWave(T, tremolo_multiplier);
  }
  // Is key 7 pressed ?
  if(keys_6 & KEY_1_MSK){
  T = PERIODS[current_pitch_0 + 7] * vibrato_T_multiplier;
  analog_out_temp += getSquareWave(T, tremolo_multiplier);
  }
  // Is key 8 pressed ?
  if(keys_6 & KEY_2_MSK){
  T = PERIODS[current_pitch_0 + 8] * vibrato_T_multiplier;
  analog_out_temp += getSquareWave(T, tremolo_multiplier);
  }
  // Is key 9 pressed ?
  if(keys_6 & KEY_3_MSK){
  T = PERIODS[current_pitch_0 + 9] * vibrato_T_multiplier;
  analog_out_temp += getSquareWave(T, tremolo_multiplier);
  }
  // Is key 10 pressed ?
  if(keys_6 & KEY_4_MSK){
  T = PERIODS[current_pitch_0 + 10] * vibrato_T_multiplier;
  analog_out_temp += getSquareWave(T, tremolo_multiplier);
  }
  // Is key 11 pressed ?
  if(keys_6 & KEY_5_MSK){
  T = PERIODS[current_pitch_0 + 11] * vibrato_T_multiplier;
  analog_out_temp += getSquareWave(T, tremolo_multiplier);
  }
  // Is key 12 pressed ?
  if(keys_12 & KEY_0_MSK){
  T = PERIODS[current_pitch_0 + 12] * vibrato_T_multiplier;
  analog_out_temp += getSquareWave(T, tremolo_multiplier);
  }
  // Is key 13 pressed ?
  if(keys_12 & KEY_1_MSK){
  T = PERIODS[current_pitch_0 + 13] * vibrato_T_multiplier;
  analog_out_temp += getSquareWave(T, tremolo_multiplier);
  }
  // Is key 14 pressed ?
  if(keys_12 & KEY_2_MSK){
  T = PERIODS[current_pitch_0 + 14] * vibrato_T_multiplier;
  analog_out_temp += getSquareWave(T, tremolo_multiplier);
  }
  // Is key 15 pressed ?
  if(keys_12 & KEY_3_MSK){
  T = PERIODS[current_pitch_0 + 15] * vibrato_T_multiplier;
  analog_out_temp += getSquareWave(T, tremolo_multiplier);
  }
  // Is key 16 pressed ?
  if(keys_12 & KEY_4_MSK){
  T = PERIODS[current_pitch_0 + 16] * vibrato_T_multiplier;
  analog_out_temp += getSquareWave(T, tremolo_multiplier);
  }
  // Is key 17 pressed ?
  if(keys_12 & KEY_5_MSK){
  T = PERIODS[current_pitch_0 + 17] * vibrato_T_multiplier;
  analog_out_temp += getSquareWave(T, tremolo_multiplier);
  }
  // Is key 18 pressed ?
  if(keys_18 & KEY_0_MSK){
  T = PERIODS[current_pitch_0 + 18] * vibrato_T_multiplier;
  analog_out_temp += getSquareWave(T, tremolo_multiplier);
  }
  // Is key 19 pressed ?
  if(keys_18 & KEY_1_MSK){
  T = PERIODS[current_pitch_0 + 19] * vibrato_T_multiplier;
  analog_out_temp += getSquareWave(T, tremolo_multiplier);
  }
  // Is key 20 pressed ?
  if(keys_18 & KEY_2_MSK){
  T = PERIODS[current_pitch_0 + 20] * vibrato_T_multiplier;
  analog_out_temp += getSquareWave(T, tremolo_multiplier);
  }
  // Is key 21 pressed ?
  if(keys_18 & KEY_3_MSK){
  T = PERIODS[current_pitch_0 + 21] * vibrato_T_multiplier;
  analog_out_temp += getSquareWave(T, tremolo_multiplier);
  }
  // Is key 22 pressed ?
  if(keys_18 & KEY_4_MSK){
  T = PERIODS[current_pitch_0 + 22] * vibrato_T_multiplier;
  analog_out_temp += getSquareWave(T, tremolo_multiplier);
  }
  // Is key 23 pressed ?
  if(keys_18 & KEY_5_MSK){
  T = PERIODS[current_pitch_0 + 23] * vibrato_T_multiplier;
  analog_out_temp += getSquareWave(T, tremolo_multiplier);
  }
  // Is key 24 pressed ?
  if(keys_24 & KEY_0_MSK){
  T = PERIODS[current_pitch_0 + 24] * vibrato_T_multiplier;
  analog_out_temp += getSquareWave(T, tremolo_multiplier);
  }
  // Is key 25 pressed ?
  if(keys_24 & KEY_1_MSK){
  T = PERIODS[current_pitch_0 + 25] * vibrato_T_multiplier;
  analog_out_temp += getSquareWave(T, tremolo_multiplier);
  }
  // Is key 26 pressed ?
  if(keys_24 & KEY_2_MSK){
  T = PERIODS[current_pitch_0 + 26] * vibrato_T_multiplier;
  analog_out_temp += getSquareWave(T, tremolo_multiplier);
  }
  // Is key 27 pressed ?
  if(keys_24 & KEY_3_MSK){
  T = PERIODS[current_pitch_0 + 27] * vibrato_T_multiplier;
  analog_out_temp += getSquareWave(T, tremolo_multiplier);
  }
  // Is key 28 pressed ?
  if(keys_24 & KEY_4_MSK){
  T = PERIODS[current_pitch_0 + 28] * vibrato_T_multiplier;
  analog_out_temp += getSquareWave(T, tremolo_multiplier);
  }
  // Is key 29 pressed ?
  if(keys_24 & KEY_5_MSK){
  T = PERIODS[current_pitch_0 + 29] * vibrato_T_multiplier;
  analog_out_temp += getSquareWave(T, tremolo_multiplier);
  }
  // Is key 30 pressed ?
  if(keys_30 & KEY_0_MSK){
  T = PERIODS[current_pitch_0 + 30] * vibrato_T_multiplier;
  analog_out_temp += getSquareWave(T, tremolo_multiplier);
  }
  // Is key 31 pressed ?
  if(keys_30 & KEY_1_MSK){
  T = PERIODS[current_pitch_0 + 31] * vibrato_T_multiplier;
  analog_out_temp += getSquareWave(T, tremolo_multiplier);
  }
  // Is key 32 pressed ?
  if(keys_30 & KEY_2_MSK){
  T = PERIODS[current_pitch_0 + 32] * vibrato_T_multiplier;
  analog_out_temp += getSquareWave(T, tremolo_multiplier);
  }
  // Is key 33 pressed ?
  if(keys_30 & KEY_3_MSK){
  T = PERIODS[current_pitch_0 + 33] * vibrato_T_multiplier;
  analog_out_temp += getSquareWave(T, tremolo_multiplier);
  }
  // Is key 34 pressed ?
  if(keys_30 & KEY_4_MSK){
  T = PERIODS[current_pitch_0 + 34] * vibrato_T_multiplier;
  analog_out_temp += getSquareWave(T, tremolo_multiplier);
  }
  // Is key 35 pressed ?
  if(keys_30 & KEY_5_MSK){
  T = PERIODS[current_pitch_0 + 35] * vibrato_T_multiplier;
  analog_out_temp += getSquareWave(T, tremolo_multiplier);
  }

#if KEY_NUMBER == 48
  // Is key 36 pressed ?
  if(keys_36 & KEY_0_MSK){
  T = PERIODS[current_pitch_0 + 36] * vibrato_T_multiplier;
  analog_out_temp += getSquareWave(T, tremolo_multiplier);
  }
  // Is key 37 pressed ?
  if(keys_36 & KEY_1_MSK){
  T = PERIODS[current_pitch_0 + 37] * vibrato_T_multiplier;
  analog_out_temp += getSquareWave(T, tremolo_multiplier);
  }
  // Is key 38 pressed ?
  if(keys_36 & KEY_2_MSK){
  T = PERIODS[current_pitch_0 + 38] * vibrato_T_multiplier;
  analog_out_temp += getSquareWave(T, tremolo_multiplier);
  }
  // Is key 39 pressed ?
  if(keys_36 & KEY_3_MSK){
  T = PERIODS[current_pitch_0 + 39] * vibrato_T_multiplier;
  analog_out_temp += getSquareWave(T, tremolo_multiplier);
  }
  // Is key 40 pressed ?
  if(keys_36 & KEY_4_MSK){
  T = PERIODS[current_pitch_0 + 40] * vibrato_T_multiplier;
  analog_out_temp += getSquareWave(T, tremolo_multiplier);
  }
  // Is key 41 pressed ?
  if(keys_36 & KEY_5_MSK){
  T = PERIODS[current_pitch_0 + 41] * vibrato_T_multiplier;
  analog_out_temp += getSquareWave(T, tremolo_multiplier);
  }
  // Is key 42 pressed ?
  if(keys_42 & KEY_0_MSK){
  T = PERIODS[current_pitch_0 + 42] * vibrato_T_multiplier;
  analog_out_temp += getSquareWave(T, tremolo_multiplier);
  }
  // Is key 43 pressed ?
  if(keys_42 & KEY_1_MSK){
  T = PERIODS[current_pitch_0 + 43] * vibrato_T_multiplier;
  analog_out_temp += getSquareWave(T, tremolo_multiplier);
  }
  // Is key 44 pressed ?
  if(keys_42 & KEY_2_MSK){
  T = PERIODS[current_pitch_0 + 44] * vibrato_T_multiplier;
  analog_out_temp += getSquareWave(T, tremolo_multiplier);
  }
  // Is key 45 pressed ?
  if(keys_42 & KEY_3_MSK){
  T = PERIODS[current_pitch_0 + 45] * vibrato_T_multiplier;
  analog_out_temp += getSquareWave(T, tremolo_multiplier);
  }
  // Is key 46 pressed ?
  if(keys_42 & KEY_4_MSK){
  T = PERIODS[current_pitch_0 + 46] * vibrato_T_multiplier;
  analog_out_temp += getSquareWave(T, tremolo_multiplier);
  }
  // Is key 47 pressed ?
  if(keys_42 & KEY_5_MSK){
  T = PERIODS[current_pitch_0 + 47] * vibrato_T_multiplier;
  analog_out_temp += getSquareWave(T, tremolo_multiplier);
  }
#endif

  // Actually update the analog value
  analog_out = analog_out_temp;
}


/**
 * \fn void analog_behaviour()
 * \brief sets the arduino in order to output analog signal
 */
void analog_behaviour(){
  // Turn OFF the midi LED
  PORTC &= ~(1<<DDC5);

  // End the initialization process
  init_timer_1();
  // Enable interrupts
  SREG |= 0x80;

  while(1){
    // Recover value from vibrato and tremolo pots
    read_pots();

    // Update the buttons and keys value
    read_buttons();

    // Handle octave and semitones change
    process_settings();
    // Keep previous value of settings buttons
    buttons_settings_last = buttons_settings;

#ifdef DEBUG
    // TOREMOVE : sets PC5 to high by putting PD3 to GND and to low by putting PD4 to GND
    if(keys_0 & KEY_1_MSK){
      PORTC |= (1<<DDC5);
    }
    if(keys_0 & KEY_2_MSK){
      PORTC &= ~ (1<<DDC5);
    }
#endif

    }
}
