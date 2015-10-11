/**PerFoRo Obstacle Avoidance Module ***/
// include the library code:
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
const int ledPin =  13;
const int LEFT_TRIG = 0; // TRIGGER
const int LEFT_ECHO = 1;  // ECHO
const int CENTER_TRIG = 6; // TRIGGER
const int CENTER_ECHO = 7;  // ECHO
const int RIGHT_TRIG = 8; // TRIGGER
const int RIGHT_ECHO = 9;  // ECHO
long distance, cms;
long left, center, right;
char incomingByte;
char incomingStream[10];
int i=0;

long sense_sonar(int TRIG, int ECHO);
void display_sonar();
void sense_serial();
void toggle_led();
  
void setup() {
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("PerFoRo!");
  pinMode(ledPin, OUTPUT);
  pinMode(CENTER_TRIG, OUTPUT);
  pinMode(CENTER_ECHO,INPUT);
  pinMode(LEFT_TRIG, OUTPUT);
  pinMode(LEFT_ECHO,INPUT);
  pinMode(RIGHT_TRIG, OUTPUT);
  pinMode(RIGHT_ECHO,INPUT);
  Serial.begin(38400);
}

void loop() {
  left = sense_sonar(LEFT_TRIG,LEFT_ECHO);
  delay(20);
  center = sense_sonar(CENTER_TRIG,CENTER_ECHO);
  delay(20);
  right = sense_sonar(RIGHT_TRIG,RIGHT_ECHO);
  display_sonar();
  sense_serial();
  toggle_led();
}

long sense_sonar(int TRIG, int ECHO)  {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  distance = pulseIn(ECHO, HIGH);
  cms= distance/58;
  if (cms > 350) {
    cms = 350;
  }
  return cms;
}

void display_sonar() {
  lcd.clear();
  lcd.print("PerFoRo!");
  lcd.setCursor(0, 1);
  lcd.print(" ");
  lcd.print(left);
  lcd.print(" ");
  lcd.print(center);
  lcd.print(" ");
  lcd.print(right);
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
