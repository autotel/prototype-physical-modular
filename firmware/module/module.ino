
#include "_name_signals.h"
#include "MonoSequencer.h"
#include "x28_Hardware.h"
#include "Midi.h"


#include <LiquidCrystal.h>

LiquidCrystal lcd(49, 48, 47, 46, 45, 44);


uint8_t microStep = 0;
uint8_t tickLen = 12;

Hardware hardware = Hardware();
MonoSequencer module = MonoSequencer();
Midi midi = Midi();


void setup() {

  lcd.begin(16, 2);
  lcd.print("PMV1");

  hardware.setup();
  hardware.setButtonCallbacks(onButtonPressed, onButtonReleased);

  midi.setup();
  midi.onMidiIn(midiInCallback);

  module.setup(& hardware, & midi);
}

void onButtonPressed(byte button, uint32_t pressedButtonsBitmap) {
  module.onButtonPressed(button, pressedButtonsBitmap);
}
void onButtonReleased(byte button) {
}

void loop() {
  //check/read midi input
  int8_t inBuf [3];
  uint8_t bufHead = 0;

  lcd.setCursor(0, 0);
  lcd.print(midi.lostBytes);
  lcd.print(String(midi.lastLostByte, HEX));

  if (microStep >= tickLen) {
    microStep = 0;
    module.step();
  }
  module.loop();
  hardware.loop();
  midi.loop();
}

void midiInCallback(uint8_t a, uint8_t b, uint8_t c) {
  module.midiIn(a, b, c);
  switch (a) {
    case 250: microStep = 0; break;

    case 248: {
        if (b == 248) {
          lcd.setCursor(1, 1);
          lcd.print("!");
        }
        microStep++;
        break;
      }
  }
}



