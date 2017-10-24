void checkMidi() {
  if (Serial1.available()) {
    uint8_t head = Serial1.read();
    if (head != 0xFE) {//0xFE midi Active Sense
      delay(1);
      sendToBrainData[0] = head;
      sendToBrainData[1] = Serial1.read();
      sendToBrainData[2] = Serial1.read();
      sendToBrain(TH_MidiReceived, 3);
    }
  }
}

