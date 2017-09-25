//https://codebender.cc/sketch:80438#Neopixel%20Rainbow.ino
#include <Adafruit_NeoPixel.h>

// constants won't change. They're used here to
// set pin numbers:
const int ledPin = 43;     // the number of the neopixel strip
const int numLeds = 28;

char ledColors [28];

//Adafruit_NeoPixel pixels = Adafruit_NeoPixel(8, ledPin);
Adafruit_NeoPixel strip = Adafruit_NeoPixel(numLeds, ledPin, NEO_GRB + NEO_KHZ800);


void setup() {
  strip.begin();
  strip.setBrightness(20); // 1/6 brightness

}

void loop() {


  rainbow(30);


}

static const uint8_t analog_pins[] = {A8,A9,A10,A11,A12,A13,A14};
void rainbow(uint8_t wait) {
  uint16_t i, j, k;



  //H, columns
#define XREG PORTH //bits 3-7, digital
#define XREGMASK 0b00000111
  DDRH = 0xFF;
  //K, rows
#define YREG PORTK //bits 0-6, analog, receive
//#define YREGMASK 0b00111111
  DDRK = 0x00;
  int inpinbase = 8;
  for(k=0; k<7; k++){
    pinMode(analog_pins[k], INPUT_PULLUP);
  }

  for (k = 0; k < numLeds; k++) {
    uint16_t col = k % 4;
    XREG &= XREGMASK;
    //XREG=0;
    //not 1 because starts in PH3
    XREG = 0b1000 << col;
    //digitalWrite(6+k,HIGH);
    uint16_t row = k / 4;
    //digitalWrite(analog_pins[k], HIGH);
    delay(3);
    
    int an = analogRead(analog_pins[k]);
    if (an > 10) {
      ledColors[k] = an;
      strip.setPixelColor(k, strip.Color(an, an, an));
      
      strip.show();
    } else {
      an = 0;
      ledColors[k] = 0;
      strip.setPixelColor(k, strip.Color(0, 6, 12));
      
      strip.show();

    }
    //digitalWrite(analog_pins[k], LOW);
    //digitalWrite(6+k,LOW);
    delay(3);
  }
  /*
    //for(j=0; j<256; j++) {
    for (i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, strip.Color(ledColors[i],ledColors[i],ledColors[i])); //Wheel((i*1+j)
    }
    strip.show();
    //}
  */
}


// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if (WheelPos < 85) {
    return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
  else if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  else {
    WheelPos -= 170;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}



