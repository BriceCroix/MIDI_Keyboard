/**
 * \brief code for the keyboard MIDI behaviour
 * \author Brice Croix
 */

#include "midi_behaviour.h"

void init_USART(){
  // See documentation p159

  // Baud rate is 9600
  // UBRR = F_CPU/(16*BAUD) - 1 = 16MHz / (16*9600) - 1 = 103
  UBRR0 = 103;

  // Not double speed
  UCSR0A &= ~(1<<U2X0);

  // Enable transmitter TX
  UCSR0B |= (1<<TXEN0);

  // Asynchronous mode
  UCSR0C &= ~(1<<UMSEL00 | 1<<UMSEL01);

  // 8 bits words
  UCSR0C &= ~(1<<UCSZ00 | 1<<UCSZ01 | 1<<UCSZ02);
  UCSR0C |= (1<<UCSZ00 | 1<<UCSZ01);
}


void process_keys_MIDI(){
  // A simple iterator
  uint8_t i;

  // In order not to access memory multiple times
  uint8_t current_pitch_0 = pitch_0;

  // An array to store notes to turn on, KEYS_NUMBER is probably way oversized
  uint8_t note_ON_queue[KEYS_NUMBER];
  // Actual number of notes to write
  uint8_t note_ON_queue_len = 0;

  // An array to store notes to turn off, KEYS_NUMBER is probably way oversized
  uint8_t note_OFF_queue[KEYS_NUMBER];
  // Actual number of notes to write
  uint8_t note_OFF_queue_len = 0;

  // fill note ON and OFF queues

  // Is there a change in keys 0 to 5?
  if(keys_0 != keys_0_last){
    // Is key 0 just being pressed ?
    if((keys_0 & KEY_0_MSK) && !(keys_0_last & KEY_0_MSK)){
      note_ON_queue[note_ON_queue_len] = MIDI_C0 + current_pitch_0 + 0;
      note_ON_queue_len += 1;
    // Or is key 0 just being released ?
    }else if(!(keys_0 & KEY_0_MSK) && (keys_0_last & KEY_0_MSK)){
      note_OFF_queue[note_OFF_queue_len] = MIDI_C0 + current_pitch_0 + 0;
      note_OFF_queue_len += 1;
    }
    // Is key 1 just being pressed ?
    if((keys_0 & KEY_1_MSK) && !(keys_0_last & KEY_1_MSK)){
      note_ON_queue[note_ON_queue_len] = MIDI_C0 + current_pitch_0 + 1;
      note_ON_queue_len += 1;
    // Or is key 1 just being released ?
    }else if(!(keys_0 & KEY_1_MSK) && (keys_0_last & KEY_1_MSK)){
      note_OFF_queue[note_OFF_queue_len] = MIDI_C0 + current_pitch_0 + 1;
      note_OFF_queue_len += 1;
    }
    // Is key 2 just being pressed ?
    if((keys_0 & KEY_2_MSK) && !(keys_0_last & KEY_2_MSK)){
      note_ON_queue[note_ON_queue_len] = MIDI_C0 + current_pitch_0 + 2;
      note_ON_queue_len += 1;
    // Or is key 2 just being released ?
    }else if(!(keys_0 & KEY_2_MSK) && (keys_0_last & KEY_2_MSK)){
      note_OFF_queue[note_OFF_queue_len] = MIDI_C0 + current_pitch_0 + 2;
      note_OFF_queue_len += 1;
    }
    // Is key 3 just being pressed ?
    if((keys_0 & KEY_3_MSK) && !(keys_0_last & KEY_3_MSK)){
      note_ON_queue[note_ON_queue_len] = MIDI_C0 + current_pitch_0 + 3;
      note_ON_queue_len += 1;
    // Or is key 3 just being released ?
    }else if(!(keys_0 & KEY_3_MSK) && (keys_0_last & KEY_3_MSK)){
      note_OFF_queue[note_OFF_queue_len] = MIDI_C0 + current_pitch_0 + 3;
      note_OFF_queue_len += 1;
    }
    // Is key 4 just being pressed ?
    if((keys_0 & KEY_4_MSK) && !(keys_0_last & KEY_4_MSK)){
      note_ON_queue[note_ON_queue_len] = MIDI_C0 + current_pitch_0 + 4;
      note_ON_queue_len += 1;
    // Or is key 4 just being released ?
    }else if(!(keys_0 & KEY_4_MSK) && (keys_0_last & KEY_4_MSK)){
      note_OFF_queue[note_OFF_queue_len] = MIDI_C0 + current_pitch_0 + 4;
      note_OFF_queue_len += 1;
    }
    // Is key 5 just being pressed ?
    if((keys_0 & KEY_5_MSK) && !(keys_0_last & KEY_5_MSK)){
      note_ON_queue[note_ON_queue_len] = MIDI_C0 + current_pitch_0 + 5;
      note_ON_queue_len += 1;
    // Or is key 5 just being released ?
    }else if(!(keys_0 & KEY_5_MSK) && (keys_0_last & KEY_5_MSK)){
      note_OFF_queue[note_OFF_queue_len] = MIDI_C0 + current_pitch_0 + 5;
      note_OFF_queue_len += 1;
    }
  }

  // Is there a change in keys 6 to 11?
  if(keys_6 != keys_6_last){
    // Is key 6 just being pressed ?
    if((keys_6 & KEY_0_MSK) && !(keys_6_last & KEY_0_MSK)){
      note_ON_queue[note_ON_queue_len] = MIDI_C0 + current_pitch_0 + 6;
      note_ON_queue_len += 1;
    // Or is key 6 just being released ?
    }else if(!(keys_6 & KEY_0_MSK) && (keys_6_last & KEY_0_MSK)){
      note_OFF_queue[note_OFF_queue_len] = MIDI_C0 + current_pitch_0 + 6;
      note_OFF_queue_len += 1;
    }
    // Is key 7 just being pressed ?
    if((keys_6 & KEY_1_MSK) && !(keys_6_last & KEY_1_MSK)){
      note_ON_queue[note_ON_queue_len] = MIDI_C0 + current_pitch_0 + 7;
      note_ON_queue_len += 1;
    // Or is key 7 just being released ?
    }else if(!(keys_6 & KEY_1_MSK) && (keys_6_last & KEY_1_MSK)){
      note_OFF_queue[note_OFF_queue_len] = MIDI_C0 + current_pitch_0 + 7;
      note_OFF_queue_len += 1;
    }
    // Is key 8 just being pressed ?
    if((keys_6 & KEY_2_MSK) && !(keys_6_last & KEY_2_MSK)){
      note_ON_queue[note_ON_queue_len] = MIDI_C0 + current_pitch_0 + 8;
      note_ON_queue_len += 1;
    // Or is key 8 just being released ?
    }else if(!(keys_6 & KEY_2_MSK) && (keys_6_last & KEY_2_MSK)){
      note_OFF_queue[note_OFF_queue_len] = MIDI_C0 + current_pitch_0 + 8;
      note_OFF_queue_len += 1;
    }
    // Is key 9 just being pressed ?
    if((keys_6 & KEY_3_MSK) && !(keys_6_last & KEY_3_MSK)){
      note_ON_queue[note_ON_queue_len] = MIDI_C0 + current_pitch_0 + 9;
      note_ON_queue_len += 1;
    // Or is key 9 just being released ?
    }else if(!(keys_6 & KEY_3_MSK) && (keys_6_last & KEY_3_MSK)){
      note_OFF_queue[note_OFF_queue_len] = MIDI_C0 + current_pitch_0 + 9;
      note_OFF_queue_len += 1;
    }
    // Is key 10 just being pressed ?
    if((keys_6 & KEY_4_MSK) && !(keys_6_last & KEY_4_MSK)){
      note_ON_queue[note_ON_queue_len] = MIDI_C0 + current_pitch_0 + 10;
      note_ON_queue_len += 1;
    // Or is key 10 just being released ?
    }else if(!(keys_6 & KEY_4_MSK) && (keys_6_last & KEY_4_MSK)){
      note_OFF_queue[note_OFF_queue_len] = MIDI_C0 + current_pitch_0 + 10;
      note_OFF_queue_len += 1;
    }
    // Is key 11 just being pressed ?
    if((keys_6 & KEY_5_MSK) && !(keys_6_last & KEY_5_MSK)){
      note_ON_queue[note_ON_queue_len] = MIDI_C0 + current_pitch_0 + 11;
      note_ON_queue_len += 1;
    // Or is key 11 just being released ?
    }else if(!(keys_6 & KEY_5_MSK) && (keys_6_last & KEY_5_MSK)){
      note_OFF_queue[note_OFF_queue_len] = MIDI_C0 + current_pitch_0 + 11;
      note_OFF_queue_len += 1;
    }
  }

  #if KEYS_NUMBER >= 12
  // Is there a change in keys 12 to 17?
  if(keys_12 != keys_12_last){
    // Is key 12 just being pressed ?
    if((keys_12 & KEY_0_MSK) && !(keys_12_last & KEY_0_MSK)){
      note_ON_queue[note_ON_queue_len] = MIDI_C0 + current_pitch_0 + 12;
      note_ON_queue_len += 1;
    // Or is key 12 just being released ?
    }else if(!(keys_12 & KEY_0_MSK) && (keys_12_last & KEY_0_MSK)){
      note_OFF_queue[note_OFF_queue_len] = MIDI_C0 + current_pitch_0 + 12;
      note_OFF_queue_len += 1;
    }
    // Is key 13 just being pressed ?
    if((keys_12 & KEY_1_MSK) && !(keys_12_last & KEY_1_MSK)){
      note_ON_queue[note_ON_queue_len] = MIDI_C0 + current_pitch_0 + 13;
      note_ON_queue_len += 1;
    // Or is key 13 just being released ?
    }else if(!(keys_12 & KEY_1_MSK) && (keys_12_last & KEY_1_MSK)){
      note_OFF_queue[note_OFF_queue_len] = MIDI_C0 + current_pitch_0 + 13;
      note_OFF_queue_len += 1;
    }
    // Is key 14 just being pressed ?
    if((keys_12 & KEY_2_MSK) && !(keys_12_last & KEY_2_MSK)){
      note_ON_queue[note_ON_queue_len] = MIDI_C0 + current_pitch_0 + 14;
      note_ON_queue_len += 1;
    // Or is key 14 just being released ?
    }else if(!(keys_12 & KEY_2_MSK) && (keys_12_last & KEY_2_MSK)){
      note_OFF_queue[note_OFF_queue_len] = MIDI_C0 + current_pitch_0 + 14;
      note_OFF_queue_len += 1;
    }
    // Is key 15 just being pressed ?
    if((keys_12 & KEY_3_MSK) && !(keys_12_last & KEY_3_MSK)){
      note_ON_queue[note_ON_queue_len] = MIDI_C0 + current_pitch_0 + 15;
      note_ON_queue_len += 1;
    // Or is key 15 just being released ?
    }else if(!(keys_12 & KEY_3_MSK) && (keys_12_last & KEY_3_MSK)){
      note_OFF_queue[note_OFF_queue_len] = MIDI_C0 + current_pitch_0 + 15;
      note_OFF_queue_len += 1;
    }
    // Is key 16 just being pressed ?
    if((keys_12 & KEY_4_MSK) && !(keys_12_last & KEY_4_MSK)){
      note_ON_queue[note_ON_queue_len] = MIDI_C0 + current_pitch_0 + 16;
      note_ON_queue_len += 1;
    // Or is key 16 just being released ?
    }else if(!(keys_12 & KEY_4_MSK) && (keys_12_last & KEY_4_MSK)){
      note_OFF_queue[note_OFF_queue_len] = MIDI_C0 + current_pitch_0 + 16;
      note_OFF_queue_len += 1;
    }
    // Is key 17 just being pressed ?
    if((keys_12 & KEY_5_MSK) && !(keys_12_last & KEY_5_MSK)){
      note_ON_queue[note_ON_queue_len] = MIDI_C0 + current_pitch_0 + 17;
      note_ON_queue_len += 1;
    // Or is key 17 just being released ?
    }else if(!(keys_12 & KEY_5_MSK) && (keys_12_last & KEY_5_MSK)){
      note_OFF_queue[note_OFF_queue_len] = MIDI_C0 + current_pitch_0 + 17;
      note_OFF_queue_len += 1;
    }
  }
  #endif

  #if KEYS_NUMBER >= 18
  // Is there a change in keys 18 to 23?
  if(keys_18 != keys_18_last){
    // Is key 18 just being pressed ?
    if((keys_18 & KEY_0_MSK) && !(keys_18_last & KEY_0_MSK)){
      note_ON_queue[note_ON_queue_len] = MIDI_C0 + current_pitch_0 + 18;
      note_ON_queue_len += 1;
    // Or is key 18 just being released ?
    }else if(!(keys_18 & KEY_0_MSK) && (keys_18_last & KEY_0_MSK)){
      note_OFF_queue[note_OFF_queue_len] = MIDI_C0 + current_pitch_0 + 18;
      note_OFF_queue_len += 1;
    }
    // Is key 19 just being pressed ?
    if((keys_18 & KEY_1_MSK) && !(keys_18_last & KEY_1_MSK)){
      note_ON_queue[note_ON_queue_len] = MIDI_C0 + current_pitch_0 + 19;
      note_ON_queue_len += 1;
    // Or is key 19 just being released ?
    }else if(!(keys_18 & KEY_1_MSK) && (keys_18_last & KEY_1_MSK)){
      note_OFF_queue[note_OFF_queue_len] = MIDI_C0 + current_pitch_0 + 19;
      note_OFF_queue_len += 1;
    }
    // Is key 20 just being pressed ?
    if((keys_18 & KEY_2_MSK) && !(keys_18_last & KEY_2_MSK)){
      note_ON_queue[note_ON_queue_len] = MIDI_C0 + current_pitch_0 + 20;
      note_ON_queue_len += 1;
    // Or is key 20 just being released ?
    }else if(!(keys_18 & KEY_2_MSK) && (keys_18_last & KEY_2_MSK)){
      note_OFF_queue[note_OFF_queue_len] = MIDI_C0 + current_pitch_0 + 20;
      note_OFF_queue_len += 1;
    }
    // Is key 21 just being pressed ?
    if((keys_18 & KEY_3_MSK) && !(keys_18_last & KEY_3_MSK)){
      note_ON_queue[note_ON_queue_len] = MIDI_C0 + current_pitch_0 + 21;
      note_ON_queue_len += 1;
    // Or is key 21 just being released ?
    }else if(!(keys_18 & KEY_3_MSK) && (keys_18_last & KEY_3_MSK)){
      note_OFF_queue[note_OFF_queue_len] = MIDI_C0 + current_pitch_0 + 21;
      note_OFF_queue_len += 1;
    }
    // Is key 22 just being pressed ?
    if((keys_18 & KEY_4_MSK) && !(keys_18_last & KEY_4_MSK)){
      note_ON_queue[note_ON_queue_len] = MIDI_C0 + current_pitch_0 + 22;
      note_ON_queue_len += 1;
    // Or is key 22 just being released ?
    }else if(!(keys_18 & KEY_4_MSK) && (keys_18_last & KEY_4_MSK)){
      note_OFF_queue[note_OFF_queue_len] = MIDI_C0 + current_pitch_0 + 22;
      note_OFF_queue_len += 1;
    }
    // Is key 23 just being pressed ?
    if((keys_18 & KEY_5_MSK) && !(keys_18_last & KEY_5_MSK)){
      note_ON_queue[note_ON_queue_len] = MIDI_C0 + current_pitch_0 + 23;
      note_ON_queue_len += 1;
    // Or is key 23 just being released ?
    }else if(!(keys_18 & KEY_5_MSK) && (keys_18_last & KEY_5_MSK)){
      note_OFF_queue[note_OFF_queue_len] = MIDI_C0 + current_pitch_0 + 23;
      note_OFF_queue_len += 1;
    }
  }
  #endif

  #if KEYS_NUMBER >= 24
  // Is there a change in keys 24 to 29?
  if(keys_24 != keys_24_last){
    // Is key 24 just being pressed ?
    if((keys_24 & KEY_0_MSK) && !(keys_24_last & KEY_0_MSK)){
      note_ON_queue[note_ON_queue_len] = MIDI_C0 + current_pitch_0 + 24;
      note_ON_queue_len += 1;
    // Or is key 24 just being released ?
    }else if(!(keys_24 & KEY_0_MSK) && (keys_24_last & KEY_0_MSK)){
      note_OFF_queue[note_OFF_queue_len] = MIDI_C0 + current_pitch_0 + 24;
      note_OFF_queue_len += 1;
    }
    // Is key 25 just being pressed ?
    if((keys_24 & KEY_1_MSK) && !(keys_24_last & KEY_1_MSK)){
      note_ON_queue[note_ON_queue_len] = MIDI_C0 + current_pitch_0 + 25;
      note_ON_queue_len += 1;
    // Or is key 25 just being released ?
    }else if(!(keys_24 & KEY_1_MSK) && (keys_24_last & KEY_1_MSK)){
      note_OFF_queue[note_OFF_queue_len] = MIDI_C0 + current_pitch_0 + 25;
      note_OFF_queue_len += 1;
    }
    // Is key 26 just being pressed ?
    if((keys_24 & KEY_2_MSK) && !(keys_24_last & KEY_2_MSK)){
      note_ON_queue[note_ON_queue_len] = MIDI_C0 + current_pitch_0 + 26;
      note_ON_queue_len += 1;
    // Or is key 26 just being released ?
    }else if(!(keys_24 & KEY_2_MSK) && (keys_24_last & KEY_2_MSK)){
      note_OFF_queue[note_OFF_queue_len] = MIDI_C0 + current_pitch_0 + 26;
      note_OFF_queue_len += 1;
    }
    // Is key 27 just being pressed ?
    if((keys_24 & KEY_3_MSK) && !(keys_24_last & KEY_3_MSK)){
      note_ON_queue[note_ON_queue_len] = MIDI_C0 + current_pitch_0 + 27;
      note_ON_queue_len += 1;
    // Or is key 27 just being released ?
    }else if(!(keys_24 & KEY_3_MSK) && (keys_24_last & KEY_3_MSK)){
      note_OFF_queue[note_OFF_queue_len] = MIDI_C0 + current_pitch_0 + 27;
      note_OFF_queue_len += 1;
    }
    // Is key 28 just being pressed ?
    if((keys_24 & KEY_4_MSK) && !(keys_24_last & KEY_4_MSK)){
      note_ON_queue[note_ON_queue_len] = MIDI_C0 + current_pitch_0 + 28;
      note_ON_queue_len += 1;
    // Or is key 28 just being released ?
    }else if(!(keys_24 & KEY_4_MSK) && (keys_24_last & KEY_4_MSK)){
      note_OFF_queue[note_OFF_queue_len] = MIDI_C0 + current_pitch_0 + 28;
      note_OFF_queue_len += 1;
    }
    // Is key 29 just being pressed ?
    if((keys_24 & KEY_5_MSK) && !(keys_24_last & KEY_5_MSK)){
      note_ON_queue[note_ON_queue_len] = MIDI_C0 + current_pitch_0 + 29;
      note_ON_queue_len += 1;
    // Or is key 29 just being released ?
    }else if(!(keys_24 & KEY_5_MSK) && (keys_24_last & KEY_5_MSK)){
      note_OFF_queue[note_OFF_queue_len] = MIDI_C0 + current_pitch_0 + 29;
      note_OFF_queue_len += 1;
    }
  }
  #endif

  #if KEYS_NUMBER >= 30
  // Is there a change in keys 30 to 35?
  if(keys_30 != keys_30_last){
    // Is key 30 just being pressed ?
    if((keys_30 & KEY_0_MSK) && !(keys_30_last & KEY_0_MSK)){
      note_ON_queue[note_ON_queue_len] = MIDI_C0 + current_pitch_0 + 30;
      note_ON_queue_len += 1;
    // Or is key 30 just being released ?
    }else if(!(keys_30 & KEY_0_MSK) && (keys_30_last & KEY_0_MSK)){
      note_OFF_queue[note_OFF_queue_len] = MIDI_C0 + current_pitch_0 + 30;
      note_OFF_queue_len += 1;
    }
    // Is key 31 just being pressed ?
    if((keys_30 & KEY_1_MSK) && !(keys_30_last & KEY_1_MSK)){
      note_ON_queue[note_ON_queue_len] = MIDI_C0 + current_pitch_0 + 31;
      note_ON_queue_len += 1;
    // Or is key 31 just being released ?
    }else if(!(keys_30 & KEY_1_MSK) && (keys_30_last & KEY_1_MSK)){
      note_OFF_queue[note_OFF_queue_len] = MIDI_C0 + current_pitch_0 + 31;
      note_OFF_queue_len += 1;
    }
    // Is key 32 just being pressed ?
    if((keys_30 & KEY_2_MSK) && !(keys_30_last & KEY_2_MSK)){
      note_ON_queue[note_ON_queue_len] = MIDI_C0 + current_pitch_0 + 32;
      note_ON_queue_len += 1;
    // Or is key 32 just being released ?
    }else if(!(keys_30 & KEY_2_MSK) && (keys_30_last & KEY_2_MSK)){
      note_OFF_queue[note_OFF_queue_len] = MIDI_C0 + current_pitch_0 + 32;
      note_OFF_queue_len += 1;
    }
    // Is key 33 just being pressed ?
    if((keys_30 & KEY_3_MSK) && !(keys_30_last & KEY_3_MSK)){
      note_ON_queue[note_ON_queue_len] = MIDI_C0 + current_pitch_0 + 33;
      note_ON_queue_len += 1;
    // Or is key 33 just being released ?
    }else if(!(keys_30 & KEY_3_MSK) && (keys_30_last & KEY_3_MSK)){
      note_OFF_queue[note_OFF_queue_len] = MIDI_C0 + current_pitch_0 + 33;
      note_OFF_queue_len += 1;
    }
    // Is key 34 just being pressed ?
    if((keys_30 & KEY_4_MSK) && !(keys_30_last & KEY_4_MSK)){
      note_ON_queue[note_ON_queue_len] = MIDI_C0 + current_pitch_0 + 34;
      note_ON_queue_len += 1;
    // Or is key 34 just being released ?
    }else if(!(keys_30 & KEY_4_MSK) && (keys_30_last & KEY_4_MSK)){
      note_OFF_queue[note_OFF_queue_len] = MIDI_C0 + current_pitch_0 + 34;
      note_OFF_queue_len += 1;
    }
    // Is key 35 just being pressed ?
    if((keys_30 & KEY_5_MSK) && !(keys_30_last & KEY_5_MSK)){
      note_ON_queue[note_ON_queue_len] = MIDI_C0 + current_pitch_0 + 35;
      note_ON_queue_len += 1;
    // Or is key 35 just being released ?
    }else if(!(keys_30 & KEY_5_MSK) && (keys_30_last & KEY_5_MSK)){
      note_OFF_queue[note_OFF_queue_len] = MIDI_C0 + current_pitch_0 + 35;
      note_OFF_queue_len += 1;
    }
  }
  #endif

  #if KEYS_NUMBER >= 36
  // Is there a change in keys 36 to 41?
  if(keys_36 != keys_36_last){
    // Is key 36 just being pressed ?
    if((keys_36 & KEY_0_MSK) && !(keys_36_last & KEY_0_MSK)){
      note_ON_queue[note_ON_queue_len] = MIDI_C0 + current_pitch_0 + 36;
      note_ON_queue_len += 1;
    // Or is key 36 just being released ?
    }else if(!(keys_36 & KEY_0_MSK) && (keys_36_last & KEY_0_MSK)){
      note_OFF_queue[note_OFF_queue_len] = MIDI_C0 + current_pitch_0 + 36;
      note_OFF_queue_len += 1;
    }
    // Is key 37 just being pressed ?
    if((keys_36 & KEY_1_MSK) && !(keys_36_last & KEY_1_MSK)){
      note_ON_queue[note_ON_queue_len] = MIDI_C0 + current_pitch_0 + 37;
      note_ON_queue_len += 1;
    // Or is key 37 just being released ?
    }else if(!(keys_36 & KEY_1_MSK) && (keys_36_last & KEY_1_MSK)){
      note_OFF_queue[note_OFF_queue_len] = MIDI_C0 + current_pitch_0 + 37;
      note_OFF_queue_len += 1;
    }
    // Is key 38 just being pressed ?
    if((keys_36 & KEY_2_MSK) && !(keys_36_last & KEY_2_MSK)){
      note_ON_queue[note_ON_queue_len] = MIDI_C0 + current_pitch_0 + 38;
      note_ON_queue_len += 1;
    // Or is key 38 just being released ?
    }else if(!(keys_36 & KEY_2_MSK) && (keys_36_last & KEY_2_MSK)){
      note_OFF_queue[note_OFF_queue_len] = MIDI_C0 + current_pitch_0 + 38;
      note_OFF_queue_len += 1;
    }
    // Is key 39 just being pressed ?
    if((keys_36 & KEY_3_MSK) && !(keys_36_last & KEY_3_MSK)){
      note_ON_queue[note_ON_queue_len] = MIDI_C0 + current_pitch_0 + 39;
      note_ON_queue_len += 1;
    // Or is key 39 just being released ?
    }else if(!(keys_36 & KEY_3_MSK) && (keys_36_last & KEY_3_MSK)){
      note_OFF_queue[note_OFF_queue_len] = MIDI_C0 + current_pitch_0 + 39;
      note_OFF_queue_len += 1;
    }
    // Is key 40 just being pressed ?
    if((keys_36 & KEY_4_MSK) && !(keys_36_last & KEY_4_MSK)){
      note_ON_queue[note_ON_queue_len] = MIDI_C0 + current_pitch_0 + 40;
      note_ON_queue_len += 1;
    // Or is key 40 just being released ?
    }else if(!(keys_36 & KEY_4_MSK) && (keys_36_last & KEY_4_MSK)){
      note_OFF_queue[note_OFF_queue_len] = MIDI_C0 + current_pitch_0 + 40;
      note_OFF_queue_len += 1;
    }
    // Is key 41 just being pressed ?
    if((keys_36 & KEY_5_MSK) && !(keys_36_last & KEY_5_MSK)){
      note_ON_queue[note_ON_queue_len] = MIDI_C0 + current_pitch_0 + 41;
      note_ON_queue_len += 1;
    // Or is key 41 just being released ?
    }else if(!(keys_36 & KEY_5_MSK) && (keys_36_last & KEY_5_MSK)){
      note_OFF_queue[note_OFF_queue_len] = MIDI_C0 + current_pitch_0 + 41;
      note_OFF_queue_len += 1;
    }
  }
  #endif

  #if KEYS_NUMBER >= 42
  // Is there a change in keys 42 to 47?
  if(keys_42 != keys_42_last){
    // Is key 42 just being pressed ?
    if((keys_42 & KEY_0_MSK) && !(keys_42_last & KEY_0_MSK)){
      note_ON_queue[note_ON_queue_len] = MIDI_C0 + current_pitch_0 + 42;
      note_ON_queue_len += 1;
    // Or is key 42 just being released ?
    }else if(!(keys_42 & KEY_0_MSK) && (keys_42_last & KEY_0_MSK)){
      note_OFF_queue[note_OFF_queue_len] = MIDI_C0 + current_pitch_0 + 42;
      note_OFF_queue_len += 1;
    }
    // Is key 43 just being pressed ?
    if((keys_42 & KEY_1_MSK) && !(keys_42_last & KEY_1_MSK)){
      note_ON_queue[note_ON_queue_len] = MIDI_C0 + current_pitch_0 + 43;
      note_ON_queue_len += 1;
    // Or is key 43 just being released ?
    }else if(!(keys_42 & KEY_1_MSK) && (keys_42_last & KEY_1_MSK)){
      note_OFF_queue[note_OFF_queue_len] = MIDI_C0 + current_pitch_0 + 43;
      note_OFF_queue_len += 1;
    }
    // Is key 44 just being pressed ?
    if((keys_42 & KEY_2_MSK) && !(keys_42_last & KEY_2_MSK)){
      note_ON_queue[note_ON_queue_len] = MIDI_C0 + current_pitch_0 + 44;
      note_ON_queue_len += 1;
    // Or is key 44 just being released ?
    }else if(!(keys_42 & KEY_2_MSK) && (keys_42_last & KEY_2_MSK)){
      note_OFF_queue[note_OFF_queue_len] = MIDI_C0 + current_pitch_0 + 44;
      note_OFF_queue_len += 1;
    }
    // Is key 45 just being pressed ?
    if((keys_42 & KEY_3_MSK) && !(keys_42_last & KEY_3_MSK)){
      note_ON_queue[note_ON_queue_len] = MIDI_C0 + current_pitch_0 + 45;
      note_ON_queue_len += 1;
    // Or is key 45 just being released ?
    }else if(!(keys_42 & KEY_3_MSK) && (keys_42_last & KEY_3_MSK)){
      note_OFF_queue[note_OFF_queue_len] = MIDI_C0 + current_pitch_0 + 45;
      note_OFF_queue_len += 1;
    }
    // Is key 46 just being pressed ?
    if((keys_42 & KEY_4_MSK) && !(keys_42_last & KEY_4_MSK)){
      note_ON_queue[note_ON_queue_len] = MIDI_C0 + current_pitch_0 + 46;
      note_ON_queue_len += 1;
    // Or is key 46 just being released ?
    }else if(!(keys_42 & KEY_4_MSK) && (keys_42_last & KEY_4_MSK)){
      note_OFF_queue[note_OFF_queue_len] = MIDI_C0 + current_pitch_0 + 46;
      note_OFF_queue_len += 1;
    }
    // Is key 47 just being pressed ?
    if((keys_42 & KEY_5_MSK) && !(keys_42_last & KEY_5_MSK)){
      note_ON_queue[note_ON_queue_len] = MIDI_C0 + current_pitch_0 + 47;
      note_ON_queue_len += 1;
    // Or is key 47 just being released ?
    }else if(!(keys_42 & KEY_5_MSK) && (keys_42_last & KEY_5_MSK)){
      note_OFF_queue[note_OFF_queue_len] = MIDI_C0 + current_pitch_0 + 47;
      note_OFF_queue_len += 1;
    }
  }
  #endif

  // Actually send midi note ON and OFF data

  if(note_ON_queue_len + note_OFF_queue_len > 0){
    // Send note ON event en channel 0
    USART_SEND(MIDI_MSG_NOTE_ON | MIDI_CHANNEL);
    for(i=0 ; i<note_ON_queue_len ; i++){
      // Write note
      USART_SEND(note_ON_queue[i]);
      // Write corresponding velocity, 0x40 is default
      USART_SEND(0x40);
    }
    // Send note OFF event with running mode, velocity is 0
    for(i=0 ; i<note_OFF_queue_len ; i++){
      // Write note
      USART_SEND(note_OFF_queue[i]);
      // Write corresponding velocity, 0x00 tu turn off
      USART_SEND(0x00);
    }
  }
}


void midi_behaviour(){
  // Turn OFF analog LED
  PORTB &= ~(1<<DDB5);

  // End the initialization process
  init_USART();
  // Enable interrupts
  SREG |= 0x80;

  while(1){
    #if (defined ENABLE_VIBRATO) || (defined ENABLE_TREMOLO)
    // Recover value from vibrato and tremolo pots
    read_pots();
    #endif
    #ifdef ENABLE_VIBRATO
    // Send vibrato message if necessary
    if(ADC_vibrato_flag){
      USART_SEND(MIDI_MSG_PITCH_BEND); // Indicates a pitch change
      USART_SEND(0x00); // LSByte of pitch change
      USART_SEND(ADC_vibrato); // MSByte of pitch change, 7 MSBits of ADC value
      // Reset flag
      ADC_vibrato_flag = 0;
    }
    #endif
    #ifdef ENABLE_TREMOLO
    // Send tremolo message if necessary
    if(ADC_tremolo_flag){
      USART_SEND(MIDI_MSG_CONTROLLER_CHANGE); // Controller change
      USART_SEND(0x07); // Overall volume code
      USART_SEND(ADC_tremolo); // Volume value
      // Reset flag
      ADC_tremolo_flag = 0;
    }
    #endif

    // Update the buttons and keys value
    read_buttons();

    // Handle octave and semitones change
    process_settings();
    // Keep previous value of settings buttons
    buttons_settings_last = buttons_settings;

    // Send MIDI note ON and OFF events
    process_keys_MIDI();
    // Keep previous value of keys
    keys_0_last = keys_0;
    keys_6_last = keys_6;
    #if KEYS_NUMBER >= 12
    keys_12_last = keys_12;
    #endif
    #if KEYS_NUMBER >= 18
    keys_18_last = keys_18;
    #endif
    #if KEYS_NUMBER >= 24
    keys_24_last = keys_24;
    #endif
    #if KEYS_NUMBER >= 30
    keys_30_last = keys_30;
    #endif
    #if KEYS_NUMBER >= 36
    keys_36_last = keys_36;
    #endif
    #if KEYS_NUMBER >= 42
    keys_42_last = keys_42;
    #endif
  }
}
