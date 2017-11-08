void onButtonPressed(byte button, uint32_t pressedButtonsBitmap) {
  //hardware.strip.setPixelColor(button, hardware.strip.Color(100,0,0));
  //TODO: hardware should make distinct callbacks for matrix buttons and tact switches, and perhaps a global button listener, for portability
  button-=8;
  button%=16;
  if (patMem[button][0] == 0) {
    patMem[button][0] = 0x90;
    patMem[button][1] = 0x40;
    patMem[button][2] = 80;
  } else {
    if (patMem[button][2] == 0x90) {
      patMem[button][2] = 127;
    } else {
      patMem[button][0] = 0;
    }
  }
}
void onButtonReleased(byte button) {
}
void onEncoderScroll(int absolute, int delta) {
}
void onEncoderButtonPressed() {
}

