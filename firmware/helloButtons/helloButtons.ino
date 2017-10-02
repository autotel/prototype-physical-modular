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
uint16_t n=0;
void loop() {
  n++;
  n%=256;

  
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
  POY=0xFF;
  int inpinbase = 8;

  for (k = 0; k < numLeds; k++) {
    uint16_t col = k % 4;
    uint16_t row = k / 4;
    
    POX &= PORTXMASK;
    
    //not 1<< because starts in PH3
    POX = ~(0b1000 << col);
    uint16_t test = 0b1 << row;
    
    //digitalWrite(6+k,HIGH);
    //digitalWrite(analog_pins[k], HIGH);
    
    //delay(3);
    
    //int an = digitalRead(analog_pins[k]);
    char an=PIY & test;
    if (!an) {
      ledColors[k] = an;
      strip.setPixelColor(k, Wheel(n));
      
      strip.show();
    } else {
      an = 0;
      ledColors[k] = 0;
      strip.setPixelColor(k, strip.Color(0, 6, 12));
      
      strip.show();

    }
    //digitalWrite(analog_pins[k], LOW);
    //digitalWrite(6+k,LOW);
   // delay(3);
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



