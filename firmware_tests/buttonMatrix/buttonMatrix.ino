#define MUXREG PORTC
#define BSHIFT 3
#define ADDRLEN 6
#define MUXAX A0
#define MUXBX A1
unsigned int lastPressedBitmap=0x0000;
unsigned int pressedBitmap=0x0000;
void setup(){
  Serial.begin(9600);
  DDRC|=~(0xff<<ADDRLEN);
  pinMode(MUXAX,OUTPUT);
  pinMode(MUXBX,INPUT_PULLUP);
}
void loop(){
  digitalWrite(MUXAX,LOW);
  for(unsigned char a = 0; a<16; a++){
    // delay(100);
    unsigned char muxa=floor(a/4);
    unsigned char muxb=a%4;
    unsigned char addr = muxa|(muxb<<BSHIFT);
    MUXREG=addr|(MUXREG &(0xff<<ADDRLEN));
    if(!digitalRead(MUXBX)){
      pressedBitmap|=0x1<<a;
    }else{
      pressedBitmap&=~(0x1<<a);
    }
  }
  if(lastPressedBitmap!=pressedBitmap){
    Serial.println("change");
    Serial.println(((pressedBitmap>>0)&0xf)|1<<5,BIN);
    Serial.println(((pressedBitmap>>4)&0xf)|1<<5,BIN);
    Serial.println(((pressedBitmap>>8)&0xf)|1<<5,BIN);
    Serial.println(((pressedBitmap>>12)&0xf)|1<<5,BIN);
    lastPressedBitmap=pressedBitmap;
  }
}
