#include "Midi.h"
#include "_name_signals.h"
#include "x28_Hardware.h"
#ifndef MONOSEQUENCERH
#define MONOSEQUENCERH
//TODO: separate .cpp and .h, capitalize filename
class MonoSequencer {

  private:
    int patMem [16][3];
    int playHead = 0;
    Midi *midi;
    Hardware *hardware;
  public:
    MonoSequencer() {
      for (uint16_t b = 0; b < 16; b++) {
        for (uint16_t a = 0; a < 3; a++) {
          patMem[b][a] = 0;
        }
      }

    }
    void setup(Hardware *t_hardware, Midi *t_midi) {
      hardware = t_hardware;
      midi = t_midi;
    }

    void onButtonPressed(byte button, uint32_t pressedButtonsBitmap) {
      button -= 8;
      button %= 16;
      if (patMem[button][0] == 0) {
        patMem[button][0] = 0x90;
        patMem[button][1] = 0x40;
        patMem[button][2] = 80;
      } else {
        if (patMem[button][2] == 0x90) {
          patMem[button][2] = 127;
        } else {
          patMem[button][0] = 0;
        }
      }
    }
    void onButtonReleased(byte button) {
    }
    void onEncoderScroll(int absolute, int delta) {
    }
    void onEncoderButtonPressed() {
    }
    void loop() {
      for (uint16_t b = 0; b < 16; b++) {
        if (patMem[b][0] == 0) {
          hardware->setButtonColor(b + 8, 0, 0, 0);
        } else {
          if (patMem[b][2] >= MIDI_noteOn) {
            hardware->setButtonColor(b + 8, patMem[b][2], 0, 0);
          } else {
            hardware->setButtonColor(b + 8, patMem[b][2] / 2, 6, 6);
          }
        }
      }
      hardware->setButtonColor(playHead + 8, 100, 100, 100);
    }

    void midiIn(uint8_t a, uint8_t b, uint8_t c) {
      switch (a) {
        case 250: playHead = 0; break;
      }
    }
    void step() {
      if (patMem[playHead][0] != 0) {
        midi->out(patMem[playHead][0], patMem[playHead][1], patMem[playHead][2]);
      }
      playHead++;
      playHead %= 16;
    }

};
#endif;
