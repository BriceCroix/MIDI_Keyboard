#include "keyboard.h"
#include "analog_behaviour.h"
#include "midi_behaviour.h"

/**
 * \brief main function containing the choice between analog and MIDI mode
 */
int main(){
  // Disable interrupts while initializing, cf p11 doc
  SREG &= ~0x80;
  init_pins();

  #if (defined ENABLE_VIBRATO) || (defined ENABLE_TREMOLO)
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
