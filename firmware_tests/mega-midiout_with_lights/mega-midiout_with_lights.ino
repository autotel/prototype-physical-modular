
#include "WS2812.h"
#define LEDS 16
WS2812 LED(LEDS);
cRGB value;

#define POLI 4
#define LEN 32
unsigned char currentModulus=16;
int pat[LEN][POLI];
int notesOn[POLI];
long lastStepMillis = 0;
#define INTERVAL 125
unsigned char currentStep = 0;
void setup() {
  //arrays are not empty by default. !?
  for (unsigned char a = 0; a < LEN; a++) {
    for (unsigned char b = 0; b < POLI; b++) {
      pat[a][b] = 0;
    }
  }
  LED.setOutput(46);
  Serial1.begin(31250);
}

void loop() {
  long now = millis();
  if (now - INTERVAL >= lastStepMillis) {
    lastStepMillis = now;
    for (unsigned char layer = 0; layer < POLI; layer++) {
      if (random(10) > 8) {
        unsigned char repeats=ceil(LEN/currentModulus);
        int newVal=random();
        for(unsigned char a=0; a<repeats; a++){
          if(currentStep+(a*currentModulus)<LEN){
            if(random(10)>5){
              pat[currentStep+(a*currentModulus)][layer] = random();
            }else{
              pat[currentStep+(a*currentModulus)][layer] = pat[(currentStep+((a+1)*currentModulus))%LEN][layer];
            }
            if(random(10)>5){
              pat[currentStep+(a*currentModulus)][layer] = 0;
            }
          }
        }
      }
      if(random(10) > 8){
        currentModulus=random(LEN);
      }
      if (pat[currentStep][layer] != 0) {
        noteOn(pat[currentStep][layer] >> 12, pat[currentStep][layer] >> 4, (pat[currentStep][layer] & 0x0E) << 4);
      }
    }
    currentStep++;
    currentStep %= LEN;
  }
  for (unsigned char stp = 0; stp < LEDS; stp++) {
    value.r=0; value.g=0; value.b=0;
    for (unsigned char voic = 0; voic < POLI; voic++) {
      if(pat[stp][voic] != 0){
        value.b += 10;
      }
      if(stp==currentStep%LEDS){
        value.g = 10;
        value.r = 10; // RGB Value -> Blue
      }
    }
    LED.set_crgb_at(stp, value); // Set value at LED found at index 0
  }
  LED.sync();
}

void noteOn(char chan, char pitch, char velocity) {
  char cmd = 0x90 | (chan & 0xF);

  Serial1.write(cmd);
  Serial1.write(pitch & 127); //WTF midi sends signed?
  Serial1.write(velocity & 127);

  bool registered = false;
  for (unsigned char a = 0; a < POLI; a++) {
    if (notesOn[a] == 0) {
      if (!registered) {
        notesOn[a] = (pitch & 127) | (cmd << 8);
        registered = true;
      }
    } else {
      Serial1.write(notesOn[a] >> 8);
      Serial1.write(notesOn[a] & 0xff);
      Serial1.write(0);
      notesOn[a] = 0;
    }
  }
}
