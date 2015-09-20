int dypOutputPin = 0; // TRIG
int dypInputPin = 1;  // ECHO
long distance;
long cm;

void setup(){
  pinMode(dypOutputPin, OUTPUT);
  pinMode(dypInputPin,INPUT);
  Serial.begin(38400);
}

void loop()
{
  // The DYP-ME007 pings on the low-high flank...
  digitalWrite(dypOutputPin, LOW);
  delayMicroseconds(2);
  digitalWrite(dypOutputPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(dypOutputPin, LOW);   
  // the distance is proportional to the time interval
  // between HIGH and LOW
  distance = pulseIn(dypInputPin, HIGH);
  cm= distance/58;                        
  delay(20);
  Serial.println("Distance = %d",cm);
}
