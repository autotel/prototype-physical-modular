
#include "uiHardware.h"

long lastTick = 0;
long tickLen = 200;

Hardware hardware = Hardware();

int patMem [16][3];
int playHead = 0;

void setup() {
  hardware.init();
  hardware.setButtonCallbacks(onButtonPressed, onButtonReleased);
  for (uint16_t b = 0; b < 16; b++) {
    for (uint16_t a = 0; a < 3; a++) {
      patMem[b][a] = 0;
    }
  }
}



void loop() {
  if (millis() - lastTick >= tickLen) {
    lastTick = millis();
    playHead++;
    playHead %= 16;
  }

  for (uint16_t b = 0; b < 16; b++) {
    if (patMem[b][0] == 0) {
      hardware.setButtonColor(b + 8, 0, 0, 0);
    } else {
      if (patMem[b][2] >= 0x80) {
        hardware.setButtonColor(b + 8, patMem[b][2], 0, 0);
      } else {
        hardware.setButtonColor(b + 8, patMem[b][2]/2, 6, 6);
      }
    }
  }

  hardware.setButtonColor(playHead + 8, 100, 100, 100);
  //hardware.strip.setPixelColor(playHead+8, hardware.strip.Color(50,50,50));
  hardware.loop();
}


