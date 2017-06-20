#include "SendOnlySoftwareSerial.h"
#include "ReceiveOnlySoftwareSerial.h"

#define comPin 10
#define debugPin 9

SendOnlySoftwareSerial    comTX = SendOnlySoftwareSerial(comPin);
ReceiveOnlySoftwareSerial comRX = ReceiveOnlySoftwareSerial(comPin);

const int myID = 0;

void setup(){
  Serial.begin(9600);
  Serial1.begin(9600);
  pinMode(debugPin,OUTPUT);
  Serial.println("init");
  outputMode();
  comTX.write('a');
  comTX.write('a');
  comTX.write('a');
  comTX.write('a');
  inputMode();
}

void loop(){
  if( comRX.available() ){
    char c = comRX.read();
    Serial.write(c);
  }
  if( Serial1.available() ){
    char c = Serial1.read();
    Serial.write(c);
  }
  if( Serial.available()){
    outputMode();
    while(Serial.available()){
      int i = int(Serial.read())-48;
      comTX.write(i);
    }
    inputMode();
  }
}
void outputMode(){
  comRX.end();
  pinMode(comPin,OUTPUT);
  comTX.begin(9600);
}
void inputMode(){
  comTX.end();
  pinMode(comPin,INPUT_PULLUP);
  comRX.begin(9600);
}



