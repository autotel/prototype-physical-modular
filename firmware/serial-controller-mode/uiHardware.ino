#include "FastLED.h"

#define NUM_LEDS 28
#define DATA_PIN 43

CRGB leds[NUM_LEDS];

//this is a conversion "lookup table" for the encoder;
#define divideEncoderRotation 5
const uint8_t grayToBinary = 0b10110100;
uint8_t enc_last = 0;
uint8_t enc_sub = 0;
unsigned int encoder0Pos = 0;

void hardware_init() {
  delay(2000);
  FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
  LEDS.setBrightness(84);
  for (uint16_t a = 0; a < NUM_LEDS; a++) {
    if (a < 1) {
      leds[a] = CRGB::Orange;
    } else if (a < 8) {
      leds[a] = CRGB::Indigo;
    } else if (a < 24) {
      leds[a] = CRGB::Gray;
    } else {
      leds[a] = CRGB::Indigo;
    }

  }
}

void hardware_loop() {
  readMatrixButtons();
  doEncoderButton();
}


void doEncoderButton() {}

char sign(char x) {
  return (x > 0) - (x < 0);
}
void doEncoder() {
  //TODO: adapt code to this hardware
  //encread turns around as follows: <- 0,1,3,2 ->
  //upon conversion it will turn as: <- 0,1,2,3 ->
  byte enc_read = (grayToBinary >> ( ( (PINC >> 4) & 0x3) * 2 ) ) & 0x3;
  if (enc_read != enc_last) {
    signed char enc_inc = enc_read - enc_last;

    if (enc_inc > 2) {
      enc_inc = -1;
    }
    if (enc_inc < -2) {
      enc_inc = +1;
    }
    enc_sub += enc_inc;
    if (abs(enc_sub) >= divideEncoderRotation) {
      encoder0Pos += sign(enc_sub);
      enc_sub = 0;
      //onEncoderScroll(encoder0Pos, enc_inc);
    }
    enc_last = enc_read;
  }
}
//TODO: shouldn't this be void?
int readMatrixButtons() {
  uint16_t i, j, k;
  //POX = pin out register n., PIN= pin in register n.
  //H, columns
#define POX PORTH //bits 3-7, digital
#define PIX PINH
#define PORTXMASK 0b00000111
  DDRH = 0xFF;
  //K, rows
#define POY PORTK //bits 0-6, analog
#define PIY PINK
  //#define YREGMASK 0b00111111
  DDRK = 0x00;
  POY = 0xFF;
  int inpinbase = 8;

  for (k = 0; k < NUM_LEDS; k++) {
    uint16_t col = k % 4;
    uint16_t row = k / 4;

    POX &= PORTXMASK;

    //not 1<< because starts in PH3
    POX = ~(0b1000 << col);
    //set test to a mask according to the row we want to check
    uint32_t test = 1UL << row;
    uint32_t an = PIY & test;

    //we checked the row, now we want to use the test to compare with the pixel number.
    //I am recycling the variable
    test = 1UL << k;
    //check button is pressed, but in inverted logic
    if (!an) {
      //button is pressed, and not the last time
      if (!(test & pressedButtonsBitmap)) {
        onButtonPressed(k);
        pressedButtonsBitmap = pressedButtonsBitmap | test;

      }

    } else {
      //button is depressed, and was pressed last time
      if (test & pressedButtonsBitmap) {
        onButtonReleased(k);
        pressedButtonsBitmap = pressedButtonsBitmap & (~test);
      }
    }

  }

}
void refreshLeds() {
  FastLED.show();
}


void setButtonColor(uint16_t button, uint8_t a, uint8_t b, uint8_t c ) {
  if (a | b | c > 0) {
    //c |= 80;
    leds[button] = CRGB(a, b, c);//CHSV
  } else {
    leds[button] = CRGB::Black;
  }
}

//taken from fast-leds example

uint8_t hue = 0;
void animationFrame() {
  delay(2);
  for (int i = 0; i < NUM_LEDS; i++) {
    int mh=hue+i;
    int osci=sin(mh/255.00*TWO_PI)*50+100;
    leds[i] = CHSV(mh, osci,osci);
    FastLED.show();
  }
  hue++;

}



