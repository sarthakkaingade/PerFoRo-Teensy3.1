/**PerFoRo Obstacle Avoidance Module ***/
// include the library code:
#include <LiquidCrystal.h>

#define  MOTOR_DELAY_HIGH        200
#define  MOTOR_DELAY_MEDIUM      100
#define  MOTOR_DELAY_LOW         50
#define  MOTOR_SPEED             250
#define  MOTOR_DELAY_FOLLOW_ME   100

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
int i = 0, trap_count = 0;
int MODE = 0,NAVIGATE = 0;
String str;

void control_manual();
void obstacle_avoidance();
void follow_me();
void sense_sonars();
long sense_sonar(int TRIG, int ECHO);
void display_sonar();
void avoid_obstacle(long left,long center, long right);
void sense_serial();
void parse_serial(String str);
void toggle_led();
void move_front();
void move_back();
void move_left();
void move_right();
void motor_stop();
  
void setup() {
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("PerFoRo!");
  delay(2000);
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
  if (MODE == 1)  {
    control_manual();
  } else if (MODE == 2)  {
    obstacle_avoidance();
  } else if (MODE == 3)  {
    follow_me();
  } else if (MODE == 4)  {
    motor_stop();
  }
  toggle_led();
}

void control_manual()  {
  lcd.setCursor(0, 1);
  lcd.print("Navigate: ");
  if (NAVIGATE == 1)  {
    lcd.print("F");
    move_front(MOTOR_DELAY_HIGH);
  } else if (NAVIGATE == 2)  {
    lcd.print("B");
    move_back(MOTOR_DELAY_HIGH);
  } else if (NAVIGATE == 3)  {
    lcd.print("L");
    move_left(MOTOR_DELAY_HIGH);
  } else if (NAVIGATE == 4)  {
    lcd.print("R");
    move_right(MOTOR_DELAY_HIGH);
  }
  NAVIGATE = 0;
}

void obstacle_avoidance()  {
  sense_sonars();
  avoid_obstacle(left,center,right);
}

void follow_me()  {
  lcd.setCursor(0, 1);
  lcd.print(" ");
  if (NAVIGATE == 3)  {
    lcd.print("L");
    move_left(0);
  } else if (NAVIGATE == 4)  {
    lcd.print("R");
    move_right(0);
  } else if (NAVIGATE == 1)  {
    lcd.print("F");
    move_front(0);
  } else if (NAVIGATE == 0)  {
    lcd.print("C");
    motor_stop();
  }
  //motor_stop();
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
  lcd.setCursor(0, 1);
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
  sense_serial();
  if (MODE == 2)  {
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
}

void sense_serial()  {
  while(Serial.available() > 0)  {
        char recieved = Serial.read();
        str += recieved;

        // Process message when new line character is recieved
        if (recieved == '!')  {
          parse_serial(str);
          str = ""; // Clear recieved buffer
        }
   }
}

void parse_serial(String str)  {
  if (str.length() == 3)  {
     if (str[0] == 'M')  {
       lcd.clear();
       lcd.print("Mode: ");
       if ((str[1] - 48) == 1)  {
         MODE = 1;
         lcd.print("MANUAL");
       } else if ((str[1] - 48) == 2)  {
         MODE = 2;
         lcd.print("Obstacle A");
       } else if ((str[1] - 48) == 3)  {
         MODE = 3;
         lcd.print("FOLLOW ME");
       } else if ((str[1] - 48) == 4)  {
         MODE = 4;
         lcd.print("STOP");
       }
       NAVIGATE = 0;
     } else if (str[0] == 'N')  {
       if ((MODE == 1) || (MODE == 3))  {
         if ((str[1] - 48) == 1)  {
           NAVIGATE = 1;
         } else if ((str[1] - 48) == 2)  {
           NAVIGATE = 2;
         } else if ((str[1] - 48) == 3)  {
           NAVIGATE = 3;
         } else if ((str[1] - 48) == 4)  {
           NAVIGATE = 4;
         } else if ((str[1] - 48) == 0)  {
           NAVIGATE = 0;
         }
       }
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
  analogWrite(PWM1, MOTOR_SPEED);
  digitalWrite(IN1A, HIGH);
  digitalWrite(IN1B, LOW);
  analogWrite(PWM2, MOTOR_SPEED);
  digitalWrite(IN2A, HIGH);
  digitalWrite(IN2B, LOW);
  delay(motor_delay);
}

void move_back(int motor_delay) 
{
  analogWrite(PWM1, MOTOR_SPEED);
  digitalWrite(IN1A, LOW);
  digitalWrite(IN1B, HIGH);
  analogWrite(PWM2, MOTOR_SPEED);
  digitalWrite(IN2A, LOW);
  digitalWrite(IN2B, HIGH);
  delay(motor_delay);
}

void move_left(int motor_delay) 
{
  analogWrite(PWM1, MOTOR_SPEED);
  digitalWrite(IN1A, HIGH);
  digitalWrite(IN1B, LOW);
  analogWrite(PWM2, MOTOR_SPEED);
  digitalWrite(IN2A, HIGH);
  digitalWrite(IN2B, HIGH);
  delay(motor_delay);
}

void move_right(int motor_delay) 
{
  analogWrite(PWM1, MOTOR_SPEED);
  digitalWrite(IN1A, HIGH);
  digitalWrite(IN1B, HIGH);
  analogWrite(PWM2, MOTOR_SPEED);
  digitalWrite(IN2A, HIGH);
  digitalWrite(IN2B, LOW);
  delay(motor_delay);
}

void motor_stop()
{
  analogWrite(PWM1, 0);
  analogWrite(PWM2, 0);
}
