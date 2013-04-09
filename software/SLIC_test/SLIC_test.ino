/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
 
  This example code is in the public domain.
 */
 
byte pinFR=6;
byte pinRM=3;
byte pinSHK=2;
byte pinPD=4;
long lastDebounceTime = 0; 
long debounceDelay = 10;
int buttonStateSHK;
boolean bInterruptProcessed=false;

void setup() {
  pinMode(pinFR, OUTPUT);
  pinMode(pinRM, OUTPUT);
  pinMode(pinSHK, INPUT);
  pinMode(pinPD, OUTPUT);
  digitalWrite(pinFR, LOW);
  digitalWrite(pinRM, LOW);
  digitalWrite(pinPD, LOW);
  attachInterrupt(0,pinSHKchange,CHANGE);
  Serial.begin(115200);
  ringPhone();
}

void loop() {
  processShkDebounce();
}

//Ring phone and stop ringing when phone is picked up
void ringPhone(){
  byte yState=0;
  do
  {
    yState=processRinging(yState);
    processShkDebounce();
  }while(buttonStateSHK==LOW);
}

byte processRinging(byte yState){
  static long lStartDelay=0;
  static byte yPeriodCounter=0;
  
  switch(yState)
  {
    case 0:
      digitalWrite(pinRM, HIGH);
      digitalWrite(pinFR, LOW);
      lStartDelay=millis();
      yPeriodCounter=0;
      return 1;
    case 1:
      if((millis()-lStartDelay) >25)
      {
        if(yPeriodCounter++<20){
          digitalWrite(pinFR, HIGH);
          lStartDelay=millis();
          return 2;
        }
        return 3;
      }
      return 1;
    case 2:
      if((millis()-lStartDelay) >25)
      {
        digitalWrite(pinFR, LOW);
        lStartDelay=millis();
        return 1;
      }
      return 2;
    case 3:
      digitalWrite(pinRM, LOW);
      digitalWrite(pinFR, HIGH);
      lStartDelay=millis();
      return 4;
    case 4:
      if((millis()-lStartDelay) >3000)
      {
        return 0;
      }
      return 4;
  }
}

void pinSHKchange(){
  lastDebounceTime = millis();
  bInterruptProcessed=false;
}

void processShkDebounce(){
  if (((millis() - lastDebounceTime) > debounceDelay) && !bInterruptProcessed) 
  {
    buttonStateSHK = digitalRead(pinSHK);
    bInterruptProcessed=true;
    Serial.println(buttonStateSHK,DEC);
  }
}
