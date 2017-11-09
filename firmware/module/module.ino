
#include "_name_signals.h"
#include "MonoSequencer.h"
#include "x28_Hardware.h"
#include "Midi.h"


#include <LiquidCrystal.h>

LiquidCrystal lcd(49, 48, 47, 46, 45, 44);


uint8_t globalMicroStep = 0;
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

uint8_t test_messageCounter = 0;
uint8_t test_lastHeader = 0;
void loop() {
  //check/read midi input
  int8_t inBuf [3];
  uint8_t bufHead = 0;
  
  lcd.setCursor(0, 0);
  lcd.print("<");
  lcd.print(midi.lostBytes);
  lcd.print("-");
  lcd.print(String(midi.lastLostByte, HEX));
  lcd.print("-");
  lcd.print(test_messageCounter);
  lcd.print("-");
  lcd.print(test_lastHeader);
  lcd.print(">");

  module.loop();
  hardware.loop();
  midi.loop();
}

void microStep() {
  globalMicroStep++;
  if (globalMicroStep >= tickLen) {
    globalMicroStep = 0;
    module.step();
  }
}

void midiInCallback(uint8_t a, uint8_t b, uint8_t c) {
  test_messageCounter++;
  test_lastHeader=a;
  module.midiIn(a, b, c);
  switch (a) {
    case 250: globalMicroStep = 0; break;

    case 248: {
        if (b == 248) {
          lcd.setCursor(1, 1);
          lcd.print("!");
        }
        microStep();
        break;
      }
  }
}



