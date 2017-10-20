//https://codebender.cc/sketch:80438#Neopixel%20Rainbow.ino
#include <Adafruit_NeoPixel.h>
#include <LiquidCrystal.h>
LiquidCrystal lcd(49, 48, 47, 46, 45, 44);
// constants won't change. They're used here to
// set pin numbers:
const int ledPin = 43;     // the number of the neopixel strip
const int numLeds = 28;
int pressure = 0;
int tresh = 0;
char ledColors [28];


int8_t enc_last = 0;
int8_t enc_sub = 0;
unsigned int encoder0Pos = 0;
#define divideEncoderRotation 4
const uint8_t grayToBinary = 0b10110100;

//Adafruit_NeoPixel pixels = Adafruit_NeoPixel(8, ledPin);
Adafruit_NeoPixel strip = Adafruit_NeoPixel(numLeds, ledPin, NEO_GRB + NEO_KHZ800);
long lastFrame = 0;

int analogPins[] = {A8, A9, A10, A11, A12, A13, A14, A15};

void setup() {
  strip.begin();
  strip.setBrightness(20); // 1/6 brightness
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("hello, world!");

}
uint16_t n = 0;
void loop() {
  n++;
  n %= 256;


  uint16_t i, j, k;

  if (millis() - lastFrame > 200) {
    lastFrame = millis();
    pressure--;


    lcd.setCursor(0, 1);
    // print the number of seconds since reset:
    lcd.print(pressure);
  }
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
    uint16_t test = 0b1 << row;
    PORTK = 0xFF;
    int an = 1024 - analogRead(analogPins[row]);

    // char an=PIY & test;

    if (an > 20) {
      ledColors[k] = an;
      strip.setPixelColor(k, Wheel(n));
      pressure = an;
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

char sign(char x) {
  return (x > 0) - (x < 0);
}
void doEncoder() {
  //TODO: adapt code to this hardware
  //encread turns around as follows: <- 0,1,3,2 ->
  //upon conversion it will turn as: <- 0,1,2,3 ->
  int8_t enc_read = (grayToBinary >> ( ( (PINA >> 6) & 0x3) * 2 ) ) & 0x3;
  if (enc_read != enc_last) {
    int8_t enc_inc = enc_read - enc_last;

    if (enc_inc > 2) {
      enc_inc = -1;
    }
    if (enc_inc < -2) {
      enc_inc = +1;
    }

    enc_sub += enc_inc;
    if (abs(enc_sub) >= divideEncoderRotation) {
      encoder0Pos += sign(enc_sub);
      tresh = encoder0Pos;
      enc_sub = 0;
    }
    enc_last = enc_read;
    // lcdPrintA(String(enc_read,HEX));
  }
  // lcdPrintB(String(PINA,BIN));
}



