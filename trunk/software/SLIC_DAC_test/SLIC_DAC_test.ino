

void setup() {
  setupRinger();
  ringPhone();
  startDialTone();
  delay(2000);
  stopDialTone();
}

void loop() {
  processShkDebounce();
}


