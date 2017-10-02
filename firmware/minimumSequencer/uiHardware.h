#include <Adafruit_NeoPixel.h>
//useful about callback functions https://stackoverflow.com/questions/14427917/call-function-in-main-program-from-a-library-in-arduino
//hardware controlling functions for physical modular rev 1 board
class Hardware {
  public:
    Hardware() {};
    const int ledPin = 43;     // the number of the neopixel strip
    const int numLeds = 28;
    Adafruit_NeoPixel strip = Adafruit_NeoPixel(numLeds, ledPin, NEO_GRB + NEO_KHZ800);
    void init() {
      strip.begin();
      strip.setBrightness(20); // 1/6 brightnesss
    }
    void loop() {
      readMatrixButtons();
      refreshLeds();
    }
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

      for (k = 0; k < numLeds; k++) {
        uint16_t col = k % 4;
        uint16_t row = k / 4;

        POX &= PORTXMASK;

        //not 1<< because starts in PH3
        POX = ~(0b1000 << col);
        uint32_t test = 0b1 << row;

        //digitalWrite(6+k,HIGH);
        //digitalWrite(analog_pins[k], HIGH);

        //delay(3);

        //int an = digitalRead(analog_pins[k]);
        char an = PIY & test;

        if (!an) {
          //button is pressed
          if (!(test & pressedButtonsBitmap)) {
            //button was not pressed in last iteration
            buttonPressedCallback(k, pressedButtonsBitmap);
          }

        } else {
          //button is pressed
          if (test & pressedButtonsBitmap) {
            //button was pressed in last iteration
            buttonReleasedCallback(k);
          }
        }
      }

    }
    void refreshLeds() {
      //uint16_t a;
      /*for (a = 0; a < strip.numPixels(); a++) {
        strip.setPixelColor(a, strip.Color(ledColors[a], ledColors[a], ledColors[a])); //Wheel((i*1+j)
        }*/
      strip.show();
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
    void setButtonCallbacks( void (*fpa)(byte, uint32_t), void (*fpb)(byte) ) {
      CB_buttonPressed = fpa;
      CB_buttonReleased = fpb;
    }
    void setButtonColor(uint16_t button, uint8_t a, uint8_t b, uint8_t c ) {
      if (colorModeHSB) {
        //TODO: change LED library to fastLed and use HSV http://fastled.io/
        strip.setPixelColor(a, Wheel(a));
      } else {
        strip.setPixelColor(button, a,b,c);
      }
    }
    void HSBColorMode() {
      colorModeHSB = true;
    }
    void RGBColorMode() {
      colorModeHSB = false;
    }
  private:
    boolean colorModeHSB = false;
    void (*CB_buttonPressed)(byte, uint32_t) = 0;
    void (*CB_buttonReleased)(byte) = 0;
    uint32_t pressedButtonsBitmap = 0;
    void buttonPressedCallback(byte button, uint32_t bitmap) {
      if ( 0 != CB_buttonPressed ) {
        (*CB_buttonPressed)(button, bitmap);
      }
      else {
        for (uint16_t a = 0; a < strip.numPixels(); a++) {
          strip.setPixelColor(a, Wheel(a * 12)); //Wheel((i*1+j)
        }
      }
    }
    void buttonReleasedCallback(byte button) {
      if ( 0 != CB_buttonReleased ) {
        (*CB_buttonReleased)(button);
      }
      else {
      }
    }
};
