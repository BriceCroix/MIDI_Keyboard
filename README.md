# MIDI & Analog Arduino Keyboard

## Description

The goal of this project is to create the software associated with a musical keyboard. This keyboard will be able to output MIDI data using UART protocol, and will also be able to output analog signal for earphones or speakers through a band-pass filter. A proposition for this filter is available below.

This software is meant to be downloaded on an ATMega328p chip. This chip is the one used on Arduino Nano boards. The provided code is not meant to be portable and is highly related to the architecture of the ATMega chip. This is due to high constraints on computation time for analog data. This constraints may not have been met with a high-level library such as the ones provided by the Arduino IDE.

The provided code features a lot of comments in order to ease the comprehension. In the case one might have questions on how to use or modify it, feel free to contact me.

## How can I modify the code to fit my needs ?

The provided code features a lot of preprocessor commands and macros meant to adapt the software to your needs. Indeed you can modify :
- The number of keys `KEYS_NUMBER`, default is 36
- If you want a pitch wheel, define `ENABLE_VIBRATO`, defined by default
- If you want a volume wheel, define `ENABLE_TREMOLO`, defined by default

## How do I use it ?

At boot, the chip will turn on both the MIDI and Analog LEDs, meaning that it it waiting for you to select the mode you wish to use. Press the first key of your keyboard to use analog mode, or the second one to use the MIDI mode. That's it !

You can tune your keyboard an octave up or down by pressing the first or second settings button. Alternatively, it is also possible to tune it a semitone up or down by pressing the third or fourth settings button. Press the fifth settings button in order to reset your pitch. In Analog mode, the sixth settings button is used to switch between three available wave shapes : Square, Triangle, and Saw.

Enjoy !

## Credits

*Brice Croix, 2020*
