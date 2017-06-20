#define interval 0
#include <Wire.h>
void setup() {
  Wire.begin(); // join i2c bus (address optional for master)
  //Serial.begin(9600);
}

byte x = 0;
int messagesPerInterval=0;
long lastSent = 0;

void loop() {
  long now = millis();
  if (now - interval >= lastSent) {
    lastSent=now;
    sendClock();
    x++;
    messagesPerInterval=0;
  }/*
  Wire.beginTransmission(8);
  Wire.write("impractical text" + messagesPerInterval);
  Wire.endTransmission();
  messagesPerInterval++;
  delay(5);*/
}
void sendClock() {
  Wire.beginTransmission(8);
  Wire.write(0x87);
  Wire.write("ac" + x);
  Wire.endTransmission();
}
