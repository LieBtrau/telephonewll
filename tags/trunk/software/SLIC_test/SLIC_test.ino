/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
 
  This example code is in the public domain.
 */
 

void setup() {
  setupRinger();
  Serial.begin(115200);
  ringPhone();
}

void loop() {
  processShkDebounce();
}


