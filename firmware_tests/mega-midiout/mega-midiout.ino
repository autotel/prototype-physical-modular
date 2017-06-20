#define POLI 4
#define LEN 16
int pat[LEN][POLI];
int notesOn[POLI];
long lastStepMillis=0;
#define INTERVAL 100
unsigned char step=0;
void setup() {
  //arrays are not empty by default. !?
  for(unsigned char a=0; a<LEN;a++){
    for(unsigned char b=0; b<POLI; b++){
      pat[a][b]=0;
    }
  }
  Serial1.begin(31250);
}

void loop() {
  long now=millis();
  if(now-INTERVAL>=lastStepMillis){
    lastStepMillis=now;
    for (unsigned char layer = 0; layer < POLI; layer++) {
      if(random(10)>8){
        pat[step][layer]=random();
      }
      if(pat[step][layer]!=0){
        noteOn(pat[step][layer]>>12,pat[step][layer]>>4,(pat[step][layer]&0x0E)<<4);
      }
    }
    step++;
    step%=LEN;
  }
}

void noteOn(char chan, char pitch, char velocity) {
  char cmd=0x90|(chan&0xF);

  Serial1.write(cmd);
  Serial1.write(pitch&127);//WTF midi sends signed?
  Serial1.write(velocity&127);

  bool registered=false;
  for(unsigned char a=0; a<POLI; a++){
    if(notesOn[a]==0){
      if(!registered){
        notesOn[a]=(pitch&127)|(cmd<<8);
        registered=true;
      }
    }else{
      Serial1.write(notesOn[a]>>8);
      Serial1.write(notesOn[a]&0xff);
      Serial1.write(0);
      notesOn[a]=0;
    }
  }
}
