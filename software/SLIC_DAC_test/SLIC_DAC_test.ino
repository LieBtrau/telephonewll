/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
 
  This example code is in the public domain.
 */
 

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


