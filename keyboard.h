/**
 * \brief Header file associated with the keyboard's main functions and macros
 * \author Brice Croix
 * \date january 28 of 2020
 */
#ifndef KEYBOARD_H
#define KEYBOARD_H

/********** END-USER, PLEASE ADJUST AND/OR COMMENT TO FIT YOUR NEEDS **********/
// Define the number of keys, between 12 and 48
#define KEYS_NUMBER 36

// Enables the tremolo and vibrato potentiometers
#define ENABLE_TREMOLO
//#define ENABLE_VIBRATO

// Defines the first key of the keyboard, between 0 for C and 11 for B on a standart keyboard
#define FIRST_KEY 0

// Defines at which note is set the keyboard, 0 for octave 0, 12 for octave 1, 24 for octave 2...
#define DEFAULT_PITCH_0_OFFSET 36

/********** END OF END-USER MODIFICATIONS *************************************/

#include <stdlib.h>
#include <avr/io.h>

// Enables the debug functionalities
#define DEBUG

// The no-operation function as defined in assembly language
#define nop() __asm__("nop\n\t")

// Number of notes per octave
#define OCTAVE 12

// Number of available notes
#define NOTE_NUMBER 108

// Initial value of pitch_0
#define DEFAULT_PITCH_0 (FIRST_KEY + DEFAULT_PITCH_0_OFFSET)

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
 * \brief Stores state of keys 0 to 5, same goes for key_6, key_12 etc. Only the 6 MSB are relevant on these variables. [k5,k4,k3,k2,k1,k0,xx,xx]
 */
extern volatile uint8_t keys_0;

/**
 * \brief stores the previous state of the keys_0 variable
 */
extern volatile uint8_t keys_0_last;
extern volatile uint8_t keys_6;
extern volatile uint8_t keys_6_last;
#if KEYS_NUMBER >= 12
extern volatile uint8_t keys_12;
extern volatile uint8_t keys_12_last;
#endif
#if KEYS_NUMBER >= 18
extern volatile uint8_t keys_18;
extern volatile uint8_t keys_18_last;
#endif
#if KEYS_NUMBER >= 24
extern volatile uint8_t keys_24;
extern volatile uint8_t keys_24_last;
#endif
#if KEYS_NUMBER >= 30
extern volatile uint8_t keys_30;
extern volatile uint8_t keys_30_last;
#endif
#if KEYS_NUMBER >= 36
extern volatile uint8_t keys_36;
extern volatile uint8_t keys_36_last;
#endif
#if KEYS_NUMBER >= 42
extern volatile uint8_t keys_42;
extern volatile uint8_t keys_42_last;
#endif
extern volatile uint8_t buttons_settings;
extern volatile uint8_t buttons_settings_last;

/**
 * \brief index of the lowest key, 0 for C0, 2 for D0, 12 for C1...
 */
extern volatile int8_t pitch_0;


#ifdef ENABLE_VIBRATO
/**
 * \brief Stores the value of the vibrato potentiometer
 */
extern volatile uint8_t ADC_vibrato;

/**
 * \brief A flag to indicate that the ADC vibrato value has been updated
 */
extern volatile uint8_t ADC_vibrato_flag;
#endif

#ifdef ENABLE_TREMOLO
/**
 * \brief Stores the value of the tremolo potentiometer
 */
extern volatile uint8_t ADC_tremolo;

/**
 * \brief A flag to indicate that the ADC tremolo value has been updated
 */
extern volatile uint8_t ADC_tremolo_flag;
#endif

/**
 * \brief Sets relevant pins to outputs or inputs
 */
void init_pins();

#if (defined ENABLE_VIBRATO) || (defined ENABLE_TREMOLO)
/**
 * \brief Enables the ADC for volume and pitch bends
 */
void init_adc();

/**
 * \brief update, if available, the value of potentiometers
 */
void read_pots();

#endif

/**
 * \brief update the values of all keys and option buttons
 */
void read_buttons();

/**
 * \brief Handles the settings buttons, without updating buttons_settings_last
 */
void process_settings();

#endif
