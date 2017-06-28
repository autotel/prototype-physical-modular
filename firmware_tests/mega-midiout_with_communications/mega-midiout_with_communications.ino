
// include the library code:
#include <LiquidCrystal.h>

#include "WS2812.h"
//TBN
#include "TBN.h"

TBN network;

char test=0;
unsigned char testByte=0;

#define LEDS 16
WS2812 LED(LEDS);
cRGB value;
String impracticalString = "----------------";

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(52, 53, 50, 51, 48, 49);

#define POLI 4
#define LEN 32
unsigned char currentModulus = 16;
int pat[LEN][POLI];
int notesOn[POLI];
unsigned long lastStepMicroStep = 0;
unsigned long currentMicroStep = 0;
int mpi = 0;

#define INTERVAL 24
unsigned char currentStep = 0;
unsigned char microStepSource=0xff;

void setup() {
  //TBN
  network.start();
  network.onData(onMessage);
  //arrays are not empty by default. !?
  for (unsigned char a = 0; a < LEN; a++) {
    for (unsigned char b = 0; b < POLI; b++) {
      pat[a][b] = 0;
    }
  }
  LED.setOutput(46);
  Serial1.begin(31250);
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("hello, world!");

}

void loop() {
  //TBN
  unsigned char testSignal [] = {testByte,testByte+1,testByte+2,testByte+3,testByte+4,testByte+5};
  network.write(testSignal,5);//test message. it doesn't write,  but makes it available for next token reception
  network.loop();//there should be an ISR timer rather than monitorization

  if (currentMicroStep  > INTERVAL) {
    currentMicroStep = 0;
    for (unsigned char layer = 0; layer < POLI; layer++) {
      if (random(10) > 8) {
        unsigned char repeats = ceil(LEN / currentModulus);
        int newVal = random();
        for (unsigned char a = 0; a < repeats; a++) {
          if (currentStep + (a * currentModulus) < LEN) {
            if (random(10) > 5) {
              pat[currentStep + (a * currentModulus)][layer] = random();
            } else {
              pat[currentStep + (a * currentModulus)][layer] = pat[(currentStep + ((a + 1) * currentModulus)) % LEN][layer];
            }
            /*if (random(10) > 5) {
              pat[currentStep + (a * currentModulus)][layer] = 0;
              }*/
          }
        }
      }
      if (random(10) > 8) {
        currentModulus = random(LEN);
      }
      if (pat[currentStep][layer] != 0) {
        noteOn(pat[currentStep][layer] >> 12, pat[currentStep][layer] >> 4, (pat[currentStep][layer] & 0x0E) << 4);
      }
    }
    currentStep++;
    currentStep %= LEN;
    // set the cursor to column 0, line 1
    // (note: line 1 is the second row, since counting begins with 0):

  }
  for (unsigned char stp = 0; stp < LEDS; stp++) {
    value.r = 0; value.g = 0; value.b = 0;
    for (unsigned char voic = 0; voic < POLI; voic++) {
      if (pat[stp][voic] != 0) {
        value.b += 1;
      }
      if (stp == currentStep % LEDS) {
        value.g = 1;
        value.r = 1; // RGB Value -> Blue
      }
    }
    LED.set_crgb_at(stp, value); // Set value at LED found at index 0
  }
  LED.sync();
  lcd.setCursor(0, 0);
  lcd.print(impracticalString);
  lcd.setCursor(0, 1);
  lcd.print(String(mpi,HEX));

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


//TBN
void onMessage(unsigned char origin,unsigned char header,unsigned char * data, unsigned char len){
  if(microStepSource==0xff){
    microStepSource=origin;
  }
  if(origin==microStepSource){
    currentMicroStep++;
  }
  testByte=(data[0]+1);
  lcd.print(" I:");
  lcd.print(network.ownID);
  lcd.print(" s:");
  lcd.print(String(microStepSource,HEX));
  lcd.print(" ms:");
  lcd.print(String(currentMicroStep,HEX));

}