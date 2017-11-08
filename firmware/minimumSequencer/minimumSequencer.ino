
#include "uiHardware.h"
#include <LiquidCrystal.h>
LiquidCrystal lcd(49, 48, 47, 46, 45, 44);

#define MIDI_noteOn 0x90
#define MIDI_noteOff 0x80

uint8_t midiTick = 0;
uint8_t tickLen = 12;

Hardware hardware = Hardware();

int patMem [16][3];
int playHead = 0;

void setup() {
  Serial1.begin(31250);
  lcd.begin(16, 2);
  lcd.print("PMV1");

  hardware.init();
  hardware.setButtonCallbacks(onButtonPressed, onButtonReleased);
  for (uint16_t b = 0; b < 16; b++) {
    for (uint16_t a = 0; a < 3; a++) {
      patMem[b][a] = 0;
    }
  }
}


uint8_t waitingMidi = 0;
uint8_t lostBytes = 0;
uint8_t lastLostByte = 0;
void loop() {
  //check/read midi input
  int8_t inBuf [3];
  uint8_t bufHead = 0;

  //this reception algorhithm doesn't work well yet.|
  while (Serial1.available()) {
    if (Serial1.available() >= 3) {
      midiIn(Serial1.read(), Serial1.read(), Serial1.read());
    } else {
      //timeout between bytes
      if (waitingMidi > 2) {
        while (Serial1.available() > 0) {
          uint8_t t = Serial1.read();

          midiIn(t, 0, 0);
          if (t != 0xfe) {
            lostBytes++;
            lastLostByte=t;
          }
        }
        waitingMidi = 0;
      }
      waitingMidi++;
      break;
    }

  }

  lcd.setCursor(0, 0);
  lcd.print(lostBytes);
  lcd.print(String(lastLostByte,HEX));

  if (midiTick >= tickLen) {
    midiTick = 0;
    playHead++;
    playHead %= 16;
    if (patMem[playHead][0] != 0) {
      midiOut(patMem[playHead][0], patMem[playHead][1], patMem[playHead][2]);
    }
  }

  for (uint16_t b = 0; b < 16; b++) {
    if (patMem[b][0] == 0) {
      hardware.setButtonColor(b + 8, 0, 0, 0);
    } else {
      if (patMem[b][2] >= MIDI_noteOn) {
        hardware.setButtonColor(b + 8, patMem[b][2], 0, 0);
      } else {
        hardware.setButtonColor(b + 8, patMem[b][2] / 2, 6, 6);
      }
    }
  }

  hardware.setButtonColor(playHead + 8, 100, 100, 100);
  //hardware.strip.setPixelColor(playHead+8, hardware.strip.Color(50,50,50));
  hardware.loop();
}
void midiOut(uint8_t a, uint8_t b, uint8_t c) {
  Serial1.write(a);
  Serial1.write(b);
  Serial1.write(c);
}
void midiIn(uint8_t a, uint8_t b, uint8_t c) {
  switch (a) {
    case 248: {
        if (b == 248) {
          lcd.setCursor(1, 1);
          lcd.print("!");
        }
        midiTick++;
        break;
      }
    case 250: playHead = 0; midiTick = 0; break;

  }
}


