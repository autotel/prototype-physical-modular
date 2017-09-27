//https://codebender.cc/sketch:80438#Neopixel%20Rainbow.ino
#include <Adafruit_NeoPixel.h>

// constants won't change. They're used here to
// set pin numbers:
const int ledPin = 43;     // the number of the neopixel strip
const int numLeds = 28;


//Adafruit_NeoPixel pixels = Adafruit_NeoPixel(8, ledPin);
Adafruit_NeoPixel strip = Adafruit_NeoPixel(numLeds, ledPin, NEO_GRB + NEO_KHZ800);

static const uint8_t analogButtonY[] = {A8, A9, A10, A11, A12, A13, A14, A15};

void setup() {
  strip.begin();
  strip.setBrightness(20); // 1/6 brightness
  Serial.begin(115200);
  for (uint8_t i = 0; i < 6; i++) {
    pinMode(analogButtonY[i], INPUT_PULLUP);
  }
}


void loop() {



  uint16_t i, j, k;




  //POX = pin out register n., PIN= pin in register n.
  //H, columns, set to output.
#define POX PORTH //bits 3-7, digital
#define PIX PINH
#define PORTXMASK 0b00000111
  DDRH = 0xFF;
  //K, rows. Set to input, pullup
#define POY PORTK //bits 0-6, analog
#define PIY PINK
  //#define YREGMASK 0b00111111
  //DDRK = 0x00;
  //POY = 0xFF;




  for (k = 0; k < numLeds; k++) {
    uint16_t col = k % 4;
    uint16_t row = k / 4;

    POX &= PORTXMASK;

    //not 1<< because starts in PH3
    //logic is inverse to harness the internal pullups
    POX = ~(0b1000 << col);
    uint16_t test = 0b1 << row;

    //delay(3);
#define ANALOG false
#if ANALOG
    //analog, pressure sensitive option:
    int an = 1023 - analogRead(analogButtonY[row]);
    //an = 1024 - (sq(an + 1) / sq(1024) * 255);
    an=max(0,an-500)*8;
    
    for (i = 0; i < an; i += 32) {
        Serial.print(".");
      }
      Serial.println("-");
#else
    //digital read option
    char an = PIY & test;
#endif
    if (an > 100) {
      //ledColors[k] = an;
      strip.setPixelColor(k, strip.Color(an, an, 0));
      strip.show();

      
    } else {
      an = 0;
      strip.setPixelColor(k, strip.Color(0, 6, 12));
      strip.show();
    }
    //digitalWrite(analogButtonY[k], LOW);
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



