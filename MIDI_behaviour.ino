/**
 * \file MIDI_behaviour.ino
 * \brief code for the keyboard MIDI behaviour
 * \author Brice Croix
 */

// Midi code for C0
#define MIDI_C0 0x0c


/**
 * \var MIDI_NOTES
 * \brief MIDI code for each note
 */
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
 * \fn init_serial()
 * \brief initialize the serial peripheral interface
 */
void init_serial(){
  // Baud rate
  Serial.begin(9600);
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

  // fill note ON queue

  // Is key 0 just being pressed ?
  if((keys_0 & KEY_0_MSK) && !(keys_0_last & KEY_0_MSK)){
    note_ON_queue[note_ON_queue_len] = MIDI_C0 + current_pitch_0 + 0;
    note_ON_queue_len += 1;
  }
  // Is key 1 just being pressed ?
  if((keys_0 & KEY_1_MSK) && !(keys_0_last & KEY_1_MSK)){
    note_ON_queue[note_ON_queue_len] = MIDI_C0 + current_pitch_0 + 1;
    note_ON_queue_len += 1;
  }
  // Is key 2 just being pressed ?
  if((keys_0 & KEY_2_MSK) && !(keys_0_last & KEY_2_MSK)){
    note_ON_queue[note_ON_queue_len] = MIDI_C0 + current_pitch_0 + 2;
    note_ON_queue_len += 1;
  }
  // Is key 3 just being pressed ?
  if((keys_0 & KEY_3_MSK) && !(keys_0_last & KEY_3_MSK)){
    note_ON_queue[note_ON_queue_len] = MIDI_C0 + current_pitch_0 + 3;
    note_ON_queue_len += 1;
  }
  // Is key 4 just being pressed ?
  if((keys_0 & KEY_4_MSK) && !(keys_0_last & KEY_4_MSK)){
    note_ON_queue[note_ON_queue_len] = MIDI_C0 + current_pitch_0 + 4;
    note_ON_queue_len += 1;
  }
  // Is key 5 just being pressed ?
  if((keys_0 & KEY_5_MSK) && !(keys_0_last & KEY_5_MSK)){
    note_ON_queue[note_ON_queue_len] = MIDI_C0 + current_pitch_0 + 5;
    note_ON_queue_len += 1;
  }
  // Is key 6 just being pressed ?
  if((keys_6 & KEY_0_MSK) && !(keys_6_last & KEY_0_MSK)){
    note_ON_queue[note_ON_queue_len] = MIDI_C0 + current_pitch_0 + 6;
    note_ON_queue_len += 1;
  }
  // Is key 7 just being pressed ?
  if((keys_6 & KEY_1_MSK) && !(keys_6_last & KEY_1_MSK)){
    note_ON_queue[note_ON_queue_len] = MIDI_C0 + current_pitch_0 + 7;
    note_ON_queue_len += 1;
  }
  // Is key 8 just being pressed ?
  if((keys_6 & KEY_2_MSK) && !(keys_6_last & KEY_2_MSK)){
    note_ON_queue[note_ON_queue_len] = MIDI_C0 + current_pitch_0 + 8;
    note_ON_queue_len += 1;
  }
  // Is key 9 just being pressed ?
  if((keys_6 & KEY_3_MSK) && !(keys_6_last & KEY_3_MSK)){
    note_ON_queue[note_ON_queue_len] = MIDI_C0 + current_pitch_0 + 9;
    note_ON_queue_len += 1;
  }
  // Is key 10 just being pressed ?
  if((keys_6 & KEY_4_MSK) && !(keys_6_last & KEY_4_MSK)){
    note_ON_queue[note_ON_queue_len] = MIDI_C0 + current_pitch_0 + 10;
    note_ON_queue_len += 1;
  }
  // Is key 11 just being pressed ?
  if((keys_6 & KEY_5_MSK) && !(keys_6_last & KEY_5_MSK)){
    note_ON_queue[note_ON_queue_len] = MIDI_C0 + current_pitch_0 + 11;
    note_ON_queue_len += 1;
  }
  // Is key 12 just being pressed ?
  if((keys_12 & KEY_0_MSK) && !(keys_12_last & KEY_0_MSK)){
    note_ON_queue[note_ON_queue_len] = MIDI_C0 + current_pitch_0 + 12;
    note_ON_queue_len += 1;
  }
  // Is key 13 just being pressed ?
  if((keys_12 & KEY_1_MSK) && !(keys_12_last & KEY_1_MSK)){
    note_ON_queue[note_ON_queue_len] = MIDI_C0 + current_pitch_0 + 13;
    note_ON_queue_len += 1;
  }
  // Is key 14 just being pressed ?
  if((keys_12 & KEY_2_MSK) && !(keys_12_last & KEY_2_MSK)){
    note_ON_queue[note_ON_queue_len] = MIDI_C0 + current_pitch_0 + 14;
    note_ON_queue_len += 1;
  }
  // Is key 15 just being pressed ?
  if((keys_12 & KEY_3_MSK) && !(keys_12_last & KEY_3_MSK)){
    note_ON_queue[note_ON_queue_len] = MIDI_C0 + current_pitch_0 + 15;
    note_ON_queue_len += 1;
  }
  // Is key 16 just being pressed ?
  if((keys_12 & KEY_4_MSK) && !(keys_12_last & KEY_4_MSK)){
    note_ON_queue[note_ON_queue_len] = MIDI_C0 + current_pitch_0 + 16;
    note_ON_queue_len += 1;
  }
  // Is key 17 just being pressed ?
  if((keys_12 & KEY_5_MSK) && !(keys_12_last & KEY_5_MSK)){
    note_ON_queue[note_ON_queue_len] = MIDI_C0 + current_pitch_0 + 17;
    note_ON_queue_len += 1;
  }
  // Is key 18 just being pressed ?
  if((keys_18 & KEY_0_MSK) && !(keys_18_last & KEY_0_MSK)){
    note_ON_queue[note_ON_queue_len] = MIDI_C0 + current_pitch_0 + 18;
    note_ON_queue_len += 1;
  }
  // Is key 19 just being pressed ?
  if((keys_18 & KEY_1_MSK) && !(keys_18_last & KEY_1_MSK)){
    note_ON_queue[note_ON_queue_len] = MIDI_C0 + current_pitch_0 + 19;
    note_ON_queue_len += 1;
  }
  // Is key 20 just being pressed ?
  if((keys_18 & KEY_2_MSK) && !(keys_18_last & KEY_2_MSK)){
    note_ON_queue[note_ON_queue_len] = MIDI_C0 + current_pitch_0 + 20;
    note_ON_queue_len += 1;
  }
  // Is key 21 just being pressed ?
  if((keys_18 & KEY_3_MSK) && !(keys_18_last & KEY_3_MSK)){
    note_ON_queue[note_ON_queue_len] = MIDI_C0 + current_pitch_0 + 21;
    note_ON_queue_len += 1;
  }
  // Is key 22 just being pressed ?
  if((keys_18 & KEY_4_MSK) && !(keys_18_last & KEY_4_MSK)){
    note_ON_queue[note_ON_queue_len] = MIDI_C0 + current_pitch_0 + 22;
    note_ON_queue_len += 1;
  }
  // Is key 23 just being pressed ?
  if((keys_18 & KEY_5_MSK) && !(keys_18_last & KEY_5_MSK)){
    note_ON_queue[note_ON_queue_len] = MIDI_C0 + current_pitch_0 + 23;
    note_ON_queue_len += 1;
  }
  // Is key 24 just being pressed ?
  if((keys_24 & KEY_0_MSK) && !(keys_24_last & KEY_0_MSK)){
    note_ON_queue[note_ON_queue_len] = MIDI_C0 + current_pitch_0 + 24;
    note_ON_queue_len += 1;
  }
  // Is key 25 just being pressed ?
  if((keys_24 & KEY_1_MSK) && !(keys_24_last & KEY_1_MSK)){
    note_ON_queue[note_ON_queue_len] = MIDI_C0 + current_pitch_0 + 25;
    note_ON_queue_len += 1;
  }
  // Is key 26 just being pressed ?
  if((keys_24 & KEY_2_MSK) && !(keys_24_last & KEY_2_MSK)){
    note_ON_queue[note_ON_queue_len] = MIDI_C0 + current_pitch_0 + 26;
    note_ON_queue_len += 1;
  }
  // Is key 27 just being pressed ?
  if((keys_24 & KEY_3_MSK) && !(keys_24_last & KEY_3_MSK)){
    note_ON_queue[note_ON_queue_len] = MIDI_C0 + current_pitch_0 + 27;
    note_ON_queue_len += 1;
  }
  // Is key 28 just being pressed ?
  if((keys_24 & KEY_4_MSK) && !(keys_24_last & KEY_4_MSK)){
    note_ON_queue[note_ON_queue_len] = MIDI_C0 + current_pitch_0 + 28;
    note_ON_queue_len += 1;
  }
  // Is key 29 just being pressed ?
  if((keys_24 & KEY_5_MSK) && !(keys_24_last & KEY_5_MSK)){
    note_ON_queue[note_ON_queue_len] = MIDI_C0 + current_pitch_0 + 29;
    note_ON_queue_len += 1;
  }
  // Is key 30 just being pressed ?
  if((keys_30 & KEY_0_MSK) && !(keys_30_last & KEY_0_MSK)){
    note_ON_queue[note_ON_queue_len] = MIDI_C0 + current_pitch_0 + 30;
    note_ON_queue_len += 1;
  }
  // Is key 31 just being pressed ?
  if((keys_30 & KEY_1_MSK) && !(keys_30_last & KEY_1_MSK)){
    note_ON_queue[note_ON_queue_len] = MIDI_C0 + current_pitch_0 + 31;
    note_ON_queue_len += 1;
  }
  // Is key 32 just being pressed ?
  if((keys_30 & KEY_2_MSK) && !(keys_30_last & KEY_2_MSK)){
    note_ON_queue[note_ON_queue_len] = MIDI_C0 + current_pitch_0 + 32;
    note_ON_queue_len += 1;
  }
  // Is key 33 just being pressed ?
  if((keys_30 & KEY_3_MSK) && !(keys_30_last & KEY_3_MSK)){
    note_ON_queue[note_ON_queue_len] = MIDI_C0 + current_pitch_0 + 33;
    note_ON_queue_len += 1;
  }
  // Is key 34 just being pressed ?
  if((keys_30 & KEY_4_MSK) && !(keys_30_last & KEY_4_MSK)){
    note_ON_queue[note_ON_queue_len] = MIDI_C0 + current_pitch_0 + 34;
    note_ON_queue_len += 1;
  }
  // Is key 35 just being pressed ?
  if((keys_30 & KEY_5_MSK) && !(keys_30_last & KEY_5_MSK)){
    note_ON_queue[note_ON_queue_len] = MIDI_C0 + current_pitch_0 + 35;
    note_ON_queue_len += 1;
  }
#if KEYS_NUMBER == 48
  // Is key 36 just being pressed ?
  if((keys_36 & KEY_0_MSK) && !(keys_36_last & KEY_0_MSK)){
    note_ON_queue[note_ON_queue_len] = MIDI_C0 + current_pitch_0 + 36;
    note_ON_queue_len += 1;
  }
  // Is key 37 just being pressed ?
  if((keys_36 & KEY_1_MSK) && !(keys_36_last & KEY_1_MSK)){
    note_ON_queue[note_ON_queue_len] = MIDI_C0 + current_pitch_0 + 37;
    note_ON_queue_len += 1;
  }
  // Is key 38 just being pressed ?
  if((keys_36 & KEY_2_MSK) && !(keys_36_last & KEY_2_MSK)){
    note_ON_queue[note_ON_queue_len] = MIDI_C0 + current_pitch_0 + 38;
    note_ON_queue_len += 1;
  }
  // Is key 39 just being pressed ?
  if((keys_36 & KEY_3_MSK) && !(keys_36_last & KEY_3_MSK)){
    note_ON_queue[note_ON_queue_len] = MIDI_C0 + current_pitch_0 + 39;
    note_ON_queue_len += 1;
  }
  // Is key 40 just being pressed ?
  if((keys_36 & KEY_4_MSK) && !(keys_36_last & KEY_4_MSK)){
    note_ON_queue[note_ON_queue_len] = MIDI_C0 + current_pitch_0 + 40;
    note_ON_queue_len += 1;
  }
  // Is key 41 just being pressed ?
  if((keys_36 & KEY_5_MSK) && !(keys_36_last & KEY_5_MSK)){
    note_ON_queue[note_ON_queue_len] = MIDI_C0 + current_pitch_0 + 41;
    note_ON_queue_len += 1;
  }
  // Is key 42 just being pressed ?
  if((keys_42 & KEY_0_MSK) && !(keys_42_last & KEY_0_MSK)){
    note_ON_queue[note_ON_queue_len] = MIDI_C0 + current_pitch_0 + 42;
    note_ON_queue_len += 1;
  }
  // Is key 43 just being pressed ?
  if((keys_42 & KEY_1_MSK) && !(keys_42_last & KEY_1_MSK)){
    note_ON_queue[note_ON_queue_len] = MIDI_C0 + current_pitch_0 + 43;
    note_ON_queue_len += 1;
  }
  // Is key 44 just being pressed ?
  if((keys_42 & KEY_2_MSK) && !(keys_42_last & KEY_2_MSK)){
    note_ON_queue[note_ON_queue_len] = MIDI_C0 + current_pitch_0 + 44;
    note_ON_queue_len += 1;
  }
  // Is key 45 just being pressed ?
  if((keys_42 & KEY_3_MSK) && !(keys_42_last & KEY_3_MSK)){
    note_ON_queue[note_ON_queue_len] = MIDI_C0 + current_pitch_0 + 45;
    note_ON_queue_len += 1;
  }
  // Is key 46 just being pressed ?
  if((keys_42 & KEY_4_MSK) && !(keys_42_last & KEY_4_MSK)){
    note_ON_queue[note_ON_queue_len] = MIDI_C0 + current_pitch_0 + 46;
    note_ON_queue_len += 1;
  }
  // Is key 47 just being pressed ?
  if((keys_42 & KEY_5_MSK) && !(keys_42_last & KEY_5_MSK)){
    note_ON_queue[note_ON_queue_len] = MIDI_C0 + current_pitch_0 + 47;
    note_ON_queue_len += 1;
  }
#endif

  // Fill note OFF queue

  // Is key 0 just being released ?
  if(!(keys_0 & KEY_0_MSK) && (keys_0_last & KEY_0_MSK)){
    note_OFF_queue[note_OFF_queue_len] = MIDI_C0 + current_pitch_0 + 0;
    note_OFF_queue_len += 1;
  }
  // Is key 1 just being released ?
  if(!(keys_0 & KEY_1_MSK) && (keys_0_last & KEY_1_MSK)){
    note_OFF_queue[note_OFF_queue_len] = MIDI_C0 + current_pitch_0 + 1;
    note_OFF_queue_len += 1;
  }
  // Is key 2 just being released ?
  if(!(keys_0 & KEY_2_MSK) && (keys_0_last & KEY_2_MSK)){
    note_OFF_queue[note_OFF_queue_len] = MIDI_C0 + current_pitch_0 + 2;
    note_OFF_queue_len += 1;
  }
  // Is key 3 just being released ?
  if(!(keys_0 & KEY_3_MSK) && (keys_0_last & KEY_3_MSK)){
    note_OFF_queue[note_OFF_queue_len] = MIDI_C0 + current_pitch_0 + 3;
    note_OFF_queue_len += 1;
  }
  // Is key 4 just being released ?
  if(!(keys_0 & KEY_4_MSK) && (keys_0_last & KEY_4_MSK)){
    note_OFF_queue[note_OFF_queue_len] = MIDI_C0 + current_pitch_0 + 4;
    note_OFF_queue_len += 1;
  }
  // Is key 5 just being released ?
  if(!(keys_0 & KEY_5_MSK) && (keys_0_last & KEY_5_MSK)){
    note_OFF_queue[note_OFF_queue_len] = MIDI_C0 + current_pitch_0 + 5;
    note_OFF_queue_len += 1;
  }
  // Is key 6 just being released ?
  if(!(keys_6 & KEY_0_MSK) && (keys_6_last & KEY_0_MSK)){
    note_OFF_queue[note_OFF_queue_len] = MIDI_C0 + current_pitch_0 + 6;
    note_OFF_queue_len += 1;
  }
  // Is key 7 just being released ?
  if(!(keys_6 & KEY_1_MSK) && (keys_6_last & KEY_1_MSK)){
    note_OFF_queue[note_OFF_queue_len] = MIDI_C0 + current_pitch_0 + 7;
    note_OFF_queue_len += 1;
  }
  // Is key 8 just being released ?
  if(!(keys_6 & KEY_2_MSK) && (keys_6_last & KEY_2_MSK)){
    note_OFF_queue[note_OFF_queue_len] = MIDI_C0 + current_pitch_0 + 8;
    note_OFF_queue_len += 1;
  }
  // Is key 9 just being released ?
  if(!(keys_6 & KEY_3_MSK) && (keys_6_last & KEY_3_MSK)){
    note_OFF_queue[note_OFF_queue_len] = MIDI_C0 + current_pitch_0 + 9;
    note_OFF_queue_len += 1;
  }
  // Is key 10 just being released ?
  if(!(keys_6 & KEY_4_MSK) && (keys_6_last & KEY_4_MSK)){
    note_OFF_queue[note_OFF_queue_len] = MIDI_C0 + current_pitch_0 + 10;
    note_OFF_queue_len += 1;
  }
  // Is key 11 just being released ?
  if(!(keys_6 & KEY_5_MSK) && (keys_6_last & KEY_5_MSK)){
    note_OFF_queue[note_OFF_queue_len] = MIDI_C0 + current_pitch_0 + 11;
    note_OFF_queue_len += 1;
  }
  // Is key 12 just being released ?
  if(!(keys_12 & KEY_0_MSK) && (keys_12_last & KEY_0_MSK)){
    note_OFF_queue[note_OFF_queue_len] = MIDI_C0 + current_pitch_0 + 12;
    note_OFF_queue_len += 1;
  }
  // Is key 13 just being released ?
  if(!(keys_12 & KEY_1_MSK) && (keys_12_last & KEY_1_MSK)){
    note_OFF_queue[note_OFF_queue_len] = MIDI_C0 + current_pitch_0 + 13;
    note_OFF_queue_len += 1;
  }
  // Is key 14 just being released ?
  if(!(keys_12 & KEY_2_MSK) && (keys_12_last & KEY_2_MSK)){
    note_OFF_queue[note_OFF_queue_len] = MIDI_C0 + current_pitch_0 + 14;
    note_OFF_queue_len += 1;
  }
  // Is key 15 just being released ?
  if(!(keys_12 & KEY_3_MSK) && (keys_12_last & KEY_3_MSK)){
    note_OFF_queue[note_OFF_queue_len] = MIDI_C0 + current_pitch_0 + 15;
    note_OFF_queue_len += 1;
  }
  // Is key 16 just being released ?
  if(!(keys_12 & KEY_4_MSK) && (keys_12_last & KEY_4_MSK)){
    note_OFF_queue[note_OFF_queue_len] = MIDI_C0 + current_pitch_0 + 16;
    note_OFF_queue_len += 1;
  }
  // Is key 17 just being released ?
  if(!(keys_12 & KEY_5_MSK) && (keys_12_last & KEY_5_MSK)){
    note_OFF_queue[note_OFF_queue_len] = MIDI_C0 + current_pitch_0 + 17;
    note_OFF_queue_len += 1;
  }
  // Is key 18 just being released ?
  if(!(keys_18 & KEY_0_MSK) && (keys_18_last & KEY_0_MSK)){
    note_OFF_queue[note_OFF_queue_len] = MIDI_C0 + current_pitch_0 + 18;
    note_OFF_queue_len += 1;
  }
  // Is key 19 just being released ?
  if(!(keys_18 & KEY_1_MSK) && (keys_18_last & KEY_1_MSK)){
    note_OFF_queue[note_OFF_queue_len] = MIDI_C0 + current_pitch_0 + 19;
    note_OFF_queue_len += 1;
  }
  // Is key 20 just being released ?
  if(!(keys_18 & KEY_2_MSK) && (keys_18_last & KEY_2_MSK)){
    note_OFF_queue[note_OFF_queue_len] = MIDI_C0 + current_pitch_0 + 20;
    note_OFF_queue_len += 1;
  }
  // Is key 21 just being released ?
  if(!(keys_18 & KEY_3_MSK) && (keys_18_last & KEY_3_MSK)){
    note_OFF_queue[note_OFF_queue_len] = MIDI_C0 + current_pitch_0 + 21;
    note_OFF_queue_len += 1;
  }
  // Is key 22 just being released ?
  if(!(keys_18 & KEY_4_MSK) && (keys_18_last & KEY_4_MSK)){
    note_OFF_queue[note_OFF_queue_len] = MIDI_C0 + current_pitch_0 + 22;
    note_OFF_queue_len += 1;
  }
  // Is key 23 just being released ?
  if(!(keys_18 & KEY_5_MSK) && (keys_18_last & KEY_5_MSK)){
    note_OFF_queue[note_OFF_queue_len] = MIDI_C0 + current_pitch_0 + 23;
    note_OFF_queue_len += 1;
  }
  // Is key 24 just being released ?
  if(!(keys_24 & KEY_0_MSK) && (keys_24_last & KEY_0_MSK)){
    note_OFF_queue[note_OFF_queue_len] = MIDI_C0 + current_pitch_0 + 24;
    note_OFF_queue_len += 1;
  }
  // Is key 25 just being released ?
  if(!(keys_24 & KEY_1_MSK) && (keys_24_last & KEY_1_MSK)){
    note_OFF_queue[note_OFF_queue_len] = MIDI_C0 + current_pitch_0 + 25;
    note_OFF_queue_len += 1;
  }
  // Is key 26 just being released ?
  if(!(keys_24 & KEY_2_MSK) && (keys_24_last & KEY_2_MSK)){
    note_OFF_queue[note_OFF_queue_len] = MIDI_C0 + current_pitch_0 + 26;
    note_OFF_queue_len += 1;
  }
  // Is key 27 just being released ?
  if(!(keys_24 & KEY_3_MSK) && (keys_24_last & KEY_3_MSK)){
    note_OFF_queue[note_OFF_queue_len] = MIDI_C0 + current_pitch_0 + 27;
    note_OFF_queue_len += 1;
  }
  // Is key 28 just being released ?
  if(!(keys_24 & KEY_4_MSK) && (keys_24_last & KEY_4_MSK)){
    note_OFF_queue[note_OFF_queue_len] = MIDI_C0 + current_pitch_0 + 28;
    note_OFF_queue_len += 1;
  }
  // Is key 29 just being released ?
  if(!(keys_24 & KEY_5_MSK) && (keys_24_last & KEY_5_MSK)){
    note_OFF_queue[note_OFF_queue_len] = MIDI_C0 + current_pitch_0 + 29;
    note_OFF_queue_len += 1;
  }
  // Is key 30 just being released ?
  if(!(keys_30 & KEY_0_MSK) && (keys_30_last & KEY_0_MSK)){
    note_OFF_queue[note_OFF_queue_len] = MIDI_C0 + current_pitch_0 + 30;
    note_OFF_queue_len += 1;
  }
  // Is key 31 just being released ?
  if(!(keys_30 & KEY_1_MSK) && (keys_30_last & KEY_1_MSK)){
    note_OFF_queue[note_OFF_queue_len] = MIDI_C0 + current_pitch_0 + 31;
    note_OFF_queue_len += 1;
  }
  // Is key 32 just being released ?
  if(!(keys_30 & KEY_2_MSK) && (keys_30_last & KEY_2_MSK)){
    note_OFF_queue[note_OFF_queue_len] = MIDI_C0 + current_pitch_0 + 32;
    note_OFF_queue_len += 1;
  }
  // Is key 33 just being released ?
  if(!(keys_30 & KEY_3_MSK) && (keys_30_last & KEY_3_MSK)){
    note_OFF_queue[note_OFF_queue_len] = MIDI_C0 + current_pitch_0 + 33;
    note_OFF_queue_len += 1;
  }
  // Is key 34 just being released ?
  if(!(keys_30 & KEY_4_MSK) && (keys_30_last & KEY_4_MSK)){
    note_OFF_queue[note_OFF_queue_len] = MIDI_C0 + current_pitch_0 + 34;
    note_OFF_queue_len += 1;
  }
  // Is key 35 just being released ?
  if(!(keys_30 & KEY_5_MSK) && (keys_30_last & KEY_5_MSK)){
    note_OFF_queue[note_OFF_queue_len] = MIDI_C0 + current_pitch_0 + 35;
    note_OFF_queue_len += 1;
  }
#if KEYS_NUMBER == 48
  // Is key 36 just being released ?
  if(!(keys_36 & KEY_0_MSK) && (keys_36_last & KEY_0_MSK)){
    note_OFF_queue[note_OFF_queue_len] = MIDI_C0 + current_pitch_0 + 36;
    note_OFF_queue_len += 1;
  }
  // Is key 37 just being released ?
  if(!(keys_36 & KEY_1_MSK) && (keys_36_last & KEY_1_MSK)){
    note_OFF_queue[note_OFF_queue_len] = MIDI_C0 + current_pitch_0 + 37;
    note_OFF_queue_len += 1;
  }
  // Is key 38 just being released ?
  if(!(keys_36 & KEY_2_MSK) && (keys_36_last & KEY_2_MSK)){
    note_OFF_queue[note_OFF_queue_len] = MIDI_C0 + current_pitch_0 + 38;
    note_OFF_queue_len += 1;
  }
  // Is key 39 just being released ?
  if(!(keys_36 & KEY_3_MSK) && (keys_36_last & KEY_3_MSK)){
    note_OFF_queue[note_OFF_queue_len] = MIDI_C0 + current_pitch_0 + 39;
    note_OFF_queue_len += 1;
  }
  // Is key 40 just being released ?
  if(!(keys_36 & KEY_4_MSK) && (keys_36_last & KEY_4_MSK)){
    note_OFF_queue[note_OFF_queue_len] = MIDI_C0 + current_pitch_0 + 40;
    note_OFF_queue_len += 1;
  }
  // Is key 41 just being released ?
  if(!(keys_36 & KEY_5_MSK) && (keys_36_last & KEY_5_MSK)){
    note_OFF_queue[note_OFF_queue_len] = MIDI_C0 + current_pitch_0 + 41;
    note_OFF_queue_len += 1;
  }
  // Is key 42 just being released ?
  if(!(keys_42 & KEY_0_MSK) && (keys_42_last & KEY_0_MSK)){
    note_OFF_queue[note_OFF_queue_len] = MIDI_C0 + current_pitch_0 + 42;
    note_OFF_queue_len += 1;
  }
  // Is key 43 just being released ?
  if(!(keys_42 & KEY_1_MSK) && (keys_42_last & KEY_1_MSK)){
    note_OFF_queue[note_OFF_queue_len] = MIDI_C0 + current_pitch_0 + 43;
    note_OFF_queue_len += 1;
  }
  // Is key 44 just being released ?
  if(!(keys_42 & KEY_2_MSK) && (keys_42_last & KEY_2_MSK)){
    note_OFF_queue[note_OFF_queue_len] = MIDI_C0 + current_pitch_0 + 44;
    note_OFF_queue_len += 1;
  }
  // Is key 45 just being released ?
  if(!(keys_42 & KEY_3_MSK) && (keys_42_last & KEY_3_MSK)){
    note_OFF_queue[note_OFF_queue_len] = MIDI_C0 + current_pitch_0 + 45;
    note_OFF_queue_len += 1;
  }
  // Is key 46 just being released ?
  if(!(keys_42 & KEY_4_MSK) && (keys_42_last & KEY_4_MSK)){
    note_OFF_queue[note_OFF_queue_len] = MIDI_C0 + current_pitch_0 + 46;
    note_OFF_queue_len += 1;
  }
  // Is key 47 just being released ?
  if(!(keys_42 & KEY_5_MSK) && (keys_42_last & KEY_5_MSK)){
    note_OFF_queue[note_OFF_queue_len] = MIDI_C0 + current_pitch_0 + 47;
    note_OFF_queue_len += 1;
  }
#endif

  // Actually send midi note ON and OFF data

  if(note_ON_queue_len + note_OFF_queue_len > 0){
    // Send note ON event en channel 0
    Serial.write(0x90);
    for(i=0 ; i<note_ON_queue_len ; i++){
      // Write note
      Serial.write(note_ON_queue[i]);
      // Write corresponding velocity, 0x40 is default
      Serial.write(0x40);
    }
    // Send note OFF event with running mode, velocity is 0
    for(i=0 ; i<note_OFF_queue_len ; i++){
      // Write note
      Serial.write(note_OFF_queue[i]);
      // Write corresponding velocity, 0x00 tu turn off
      Serial.write(0x00);
    }
  }
}


/**
 * \fn void midi_behaviour()
 * \brief sets the arduino in order to output midi data
 */
void midi_behaviour(){
  // Turn OFF analog LED
  PORTB &= ~(1<<DDB5);

  // End the initialization process
  init_serial();
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

    // Send MIDI note ON and OFF events
    process_keys_MIDI();
    // Keep previous value of keys
    keys_0_last = keys_0;
    keys_6_last = keys_6;
    keys_12_last = keys_12;
    keys_18_last = keys_18;
    keys_24_last = keys_24;
    keys_30_last = keys_30;
#if KEYS_NUMBER == 48
    keys_36_last = keys_36;
    keys_42_last = keys_42;
#endif
  }
}
