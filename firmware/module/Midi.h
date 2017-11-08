#include "_name_signals.h"
#ifndef MIDIH
#define MIDIH
class Midi {
  private:
    uint8_t waitingMidi = 0;

    void (*CB_midiIn)(uint8_t, uint8_t, uint8_t) = 0;
    void midiInCallback(uint8_t a , uint8_t b , uint8_t c) {
      if ( 0 != CB_midiIn ) {
        (*CB_midiIn)(a, b, c);
      }
    }
  public:

    Midi() {
    }
    uint8_t lostBytes = 0;
    uint8_t lastLostByte = 0;
    void setup() {
      Serial1.begin(31250);
    }
    void onMidiIn( void (*fpa)(uint8_t, uint8_t, uint8_t)) {
      CB_midiIn = fpa;
    }
    void loop() {
      //this reception algorhithm doesn't work well yet.|
      while (Serial1.available()) {
        if (Serial1.available() >= 3) {
          in(Serial1.read(), Serial1.read(), Serial1.read());
        } else {
          //timeout between bytes
          if (waitingMidi > 2) {
            while (Serial1.available() > 0) {
              uint8_t t = Serial1.read();

              in(t, 0, 0);
              if (t != 0xfe) {
                lostBytes++;
                lastLostByte = t;
              }
            }
            waitingMidi = 0;
          }
          waitingMidi++;
          break;
        }

      }
    }

    void out(uint8_t a, uint8_t b, uint8_t c) {
      Serial1.write(a);
      Serial1.write(b);
      Serial1.write(c);
    }
    void in(uint8_t a, uint8_t b, uint8_t c) {
      midiInCallback(a, b, c);

    }

};
#endif
