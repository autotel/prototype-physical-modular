#include "SendOnlySoftwareSerial.h"
#include "ReceiveOnlySoftwareSerial.h"

#define comPin 4
#define debugPin 10

SendOnlySoftwareSerial    comTX = SendOnlySoftwareSerial(comPin);
ReceiveOnlySoftwareSerial comRX = ReceiveOnlySoftwareSerial(comPin);

const int myID = 2;

void setup(){
  pinMode(debugPin,OUTPUT);
  //don't know why, this is needed:
  pinMode(comPin,OUTPUT);
  outputMode();
  comTX.print("in");
  comTX.print(myID);
  inputMode();
}

void loop(){
  if( comRX.available() ){
    int i = int(comRX.read());
    if( i == myID ){

      outputMode();
      comTX.print("I am ");
      comTX.println(myID);
      inputMode();

      digitalWrite(debugPin,HIGH);
      delay(100);
      digitalWrite(debugPin,LOW);
    }
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

