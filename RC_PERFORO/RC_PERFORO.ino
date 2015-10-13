/**PerFoRo Obstacle Avoidance Module ***/
// include the library code:
#include <LiquidCrystal.h>

#define  MOTOR_DELAY_HIGH    500
#define  MOTOR_DELAY_MEDIUM  100
#define  MOTOR_DELAY_LOW    50

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
const int ledPin =  13;
const int LEFT_TRIG = 0;    // TRIGGER
const int LEFT_ECHO = 1;    // ECHO
const int CENTER_TRIG = 6;  // TRIGGER
const int CENTER_ECHO = 7;  // ECHO
const int RIGHT_TRIG = 8;   // TRIGGER
const int RIGHT_ECHO = 9;   // ECHO
const int PWM1 = 23;        //LEFT MOTOR       
const int IN1A = 14;
const int IN1B = 15;
const int PWM2 = 22;        //RIGHT MOTOR
const int IN2A = 16;
const int IN2B = 17;
long distance, cms;
long left, center, right;
char incomingByte;
char incomingStream[10];
int i = 0, trap_count = 0;
int MODE = 0;

void obstacle_avoidance();
void sense_sonars();
long sense_sonar(int TRIG, int ECHO);
void display_sonar();
void avoid_obstacle(long left,long center, long right);
void sense_serial();
void toggle_led();
void move_front();
void move_back();
void move_left();
void move_right();
  
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
  pinMode(PWM1, OUTPUT);
  pinMode(IN1A, OUTPUT);
  pinMode(IN1B, OUTPUT);
  pinMode(PWM2, OUTPUT);
  pinMode(IN2A, OUTPUT);
  pinMode(IN2B, OUTPUT);
  analogWriteResolution(10);
  Serial.begin(38400);
}

void loop() {
  
  sense_serial();
  toggle_led();
}

void obstacle_avoidance()  {
  sense_sonars();
  avoid_obstacle(left,center,right);
}

void sense_sonars()  {
  left = sense_sonar(LEFT_TRIG,LEFT_ECHO);
  delay(50);
  center = sense_sonar(CENTER_TRIG,CENTER_ECHO);
  delay(50);
  right = sense_sonar(RIGHT_TRIG,RIGHT_ECHO);
  display_sonar();
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

void display_sonar()  {
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

void avoid_obstacle(long left,long center, long right)  {
  if ( trap_count == 5)  {
      trap_count = 0;
      avoid_trap();
  }  else  {
    if ((left < 30) || (center < 30) || (right < 30))  {
        trap_count++;
        move_back(random(1000));
        if ( (left < right) && (left < center) )  {
          move_right(random(1000));
        }  else  if ( (right < left) && (right < center) )  {
          move_left(random(1000));
        }  else  {
          move_back(1000);
          if ((random(1000) % 2) == 1) {
            move_left(random(2000));
          }  else  {
            move_right(random(2000));
          } 
        }
    }  else if ((left > 50) && (center > 50) && (right > 50))  {
        trap_count = 0;
        move_front(MOTOR_DELAY_MEDIUM);
    }  else if ((left > 50) && (center > 50) && (right < 50))  {
        trap_count = 0;
        move_left(MOTOR_DELAY_MEDIUM);
    }  else if ((left > 50) && (center < 50) && (right > 50))  {
        trap_count++;
        if ((random(1000) % 2) == 1) {
            move_left(MOTOR_DELAY_HIGH);
        }  else  {
            move_right(MOTOR_DELAY_HIGH);
        }
    }  else if ((left > 50) && (center < 50) && (right < 50))  {
        trap_count = 0;
        move_left(MOTOR_DELAY_HIGH);
    }  else if ((left < 50) && (center > 50) && (right < 50))  {
        trap_count = 0;
        move_right(MOTOR_DELAY_MEDIUM);
    }  else if ((left < 50) && (center > 50) && (right < 50))  {
        trap_count++;
        if ((random(1000) % 2) == 1) {
            move_left(MOTOR_DELAY_HIGH);
        }  else  {
            move_right(MOTOR_DELAY_HIGH);
        }
    }  else if ((left < 50) && (center < 50) && (right > 50))  {
        trap_count = 0;
        move_right(MOTOR_DELAY_HIGH);
    }  else if ((left < 50) && (center < 50) && (right < 50))  {
        trap_count++;
        if ((random(1000) % 2) == 1) {
            move_left(MOTOR_DELAY_HIGH);
        }  else  {
            move_right(MOTOR_DELAY_HIGH);
        }
    }
  }
}

void avoid_trap()  {
  lcd.clear();
  lcd.print("TRAP encountered");
  lcd.setCursor(0, 1);
  move_back(1000);
  if ((random(1000) % 2) == 1) {
      lcd.print("LEFT");
      move_left(3000);
  }  else  {
      lcd.print("RIGHT");
      move_right(3000);
  }
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

void move_front(int motor_delay) 
{
  analogWrite(PWM1, 250);
  digitalWrite(IN1A, HIGH);
  digitalWrite(IN1B, LOW);
  analogWrite(PWM2, 250);
  digitalWrite(IN2A, HIGH);
  digitalWrite(IN2B, LOW);
  delay(motor_delay);
}

void move_back(int motor_delay) 
{
  analogWrite(PWM1, 250);
  digitalWrite(IN1A, LOW);
  digitalWrite(IN1B, HIGH);
  analogWrite(PWM2, 250);
  digitalWrite(IN2A, LOW);
  digitalWrite(IN2B, HIGH);
  delay(motor_delay);
}

void move_left(int motor_delay) 
{
  analogWrite(PWM1, 250);
  digitalWrite(IN1A, HIGH);
  digitalWrite(IN1B, HIGH);
  analogWrite(PWM2, 250);
  digitalWrite(IN2A, HIGH);
  digitalWrite(IN2B, LOW);
  delay(motor_delay);
}

void move_right(int motor_delay) 
{
  analogWrite(PWM1, 250);
  digitalWrite(IN1A, HIGH);
  digitalWrite(IN1B, LOW);
  analogWrite(PWM2, 250);
  digitalWrite(IN2A, HIGH);
  digitalWrite(IN2B, HIGH);
  delay(motor_delay);
}
