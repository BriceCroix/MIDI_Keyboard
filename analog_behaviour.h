/**
 * \brief Header file associated with the keyboard's analog behaviour
 * \author Brice Croix
 * \date january 28 of 2020
 */
#ifndef ANALOG_BEHAVIOUR_H
#define ANALOG_BEHAVIOUR_H

#include "keyboard.h"
#ifdef DEBUG
#include "midi_behaviour.h"
#endif
#include <stdlib.h>
#include <avr/interrupt.h>

// The timer Auto-Reload Value, that defines the resolution and the sampling rate
#define TIMER_ARR 384u
// The analog amplitude for one note, defining how many notes can be played at once
#define PWM_NOTE_AMP (TIMER_ARR>>3)
// Minimum duty value for analog output, = PWM_NOTE_AMP/2
#define PWM_MIN (PWM_NOTE_AMP >> 1)
// The frequency of the ATMega328p, 16 MHz
#define ATMEGA_FREQUENCY 16000000
// Sample frequency in Hz
#define SAMPLE_FREQUENCY (ATMEGA_FREQUENCY / TIMER_ARR)
// Sample time in micro second, WARNING : TIMER_ARR must be chosen so that SAMPLE_TIME is an integer
#define SAMPLE_TIME (1000000 / SAMPLE_FREQUENCY)

/**
 * \brief time in micro second
 */
extern volatile uint64_t t;

/**
 * \brief a token, created for each sample update, each SAMPLE_TIME 
 */
extern volatile uint8_t token_sample_update;

/**
 * \brief a function pointer to the wave function to use to compute analog output
 */
extern float (*get_wave_shape_ptr)(uint16_t period);

/**
 * \brief Sets timer 1 in condition to generate PWM signal
 */
void init_timer_1();

/**
 * \brief computes the value of a square wave at time t
 * \param[in] period period of the wave to compute, in us
 * \return value of wave to compute, between 0 and 1
 */
float getSquareWave(uint16_t period);

/**
 * \brief computes the value of a triangle wave at time t
 * \param[in] period period of the wave to compute, in us
 * \return value of wave to compute, between 0 and 1
 */
float getTriangleWave(uint16_t period);

/**
 * \brief computes the value of a rising saw wave at time t
 * \param[in] period period of the wave to compute, in us
 * \return value of wave to compute, between 0 and 1
 */
float getSawWave(uint16_t period);

/**
 * \brief Sets the analog_out value according to the current keys state.
 */
void setAnalogOut();

/**
 * \brief sets the arduino in order to output analog signal, contains infinite loop
 */
void analog_behaviour();

#endif
