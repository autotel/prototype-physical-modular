
// include the library code:
#include <LiquidCrystal.h>

#include <Wire.h>

#include "WS2812.h"
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

#define INTERVAL 120
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
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("hello, world!");
  Wire.begin(8);                // join i2c bus with address #8
  Wire.onReceive(receiveEvent); // register event
  Serial.begin(9600);           // start serial for output
}

void loop() {
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

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
unsigned char currentHeader = 0;
void receiveEvent(int howMany) {
  int count = 0;
  char currentHeader = Wire.read();
  count++;
  while (Wire.available()) {
    char c = Wire.read();
    if (count == 0) {
      currentHeader = c;
    }
    /*
    if (currentHeader == 'i') {
      if (count == howMany) {
        mpi++;
      } else {
        impracticalString[count % 15] = c;
      }
    }*/
    count++;
  }
  //if (currentHeader == 0x87) {
    mpi=0;
    currentMicroStep++;
  //}

}
