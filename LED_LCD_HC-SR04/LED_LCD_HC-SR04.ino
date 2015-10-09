/*
The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)
*/
// include the library code:
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
const int ledPin =  13;
const int TRIG = 0; // TRIGGER
const int ECHO = 1;  // ECHO
const int TRIG1 = 6; // TRIGGER
const int ECHO1 = 7;  // ECHO
long distance;
long cm[2];
char incomingByte;
char incomingStream[10];
int i=0;

void sense_sonar();
void sense_serial();
void toggle_led();
  
void setup() {
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("PerFoRo!");
  pinMode(ledPin, OUTPUT);
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO,INPUT);
  pinMode(TRIG1, OUTPUT);
  pinMode(ECHO1,INPUT);
  Serial.begin(38400);
}

void loop() {
  sense_sonar();
  sense_serial();
  toggle_led();
}

void sense_sonar()  {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  distance = pulseIn(ECHO, HIGH);
  cm[0]= distance/58;                        
  delay(20);
  digitalWrite(TRIG1, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG1, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG1, LOW);
  distance = pulseIn(ECHO1, HIGH);
  cm[1]= distance/58;
  lcd.clear();
  Serial.print("Distance in cms");
  lcd.print("PerFoRo!");
  lcd.setCursor(0, 1);
  lcd.print("1=");
  lcd.print(cm[0]);
  lcd.print(" 2=");
  lcd.print(cm[1]);
}

void sense_serial()  {
  if (Serial.available() > 0) {
    incomingByte = Serial.read();
    if ( incomingByte == 32 || i == 10 ) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Data: ");
      for ( int j = 0; j < i; j++ )  {
        lcd.print(incomingStream[j]);
      }
      i = 0;
    } else {
      incomingStream[i] = incomingByte;
      i++;
    }
  }
}

void toggle_led()  {
  digitalWrite(ledPin, HIGH);
  delay(100);
  digitalWrite(ledPin, LOW);
  delay(100);
}
