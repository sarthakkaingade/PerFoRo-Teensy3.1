/**PerFoRo DC Motor Control Module ***/

const int PWM1 =  23;
const int IN1A = 14;
const int IN1B = 15;
const int PWM2 =  22;
const int IN2A = 16;
const int IN2B = 17;

void front();
void back();

void setup()   {                
  pinMode(PWM1, OUTPUT);
  pinMode(IN1A, OUTPUT);
  pinMode(IN1B, OUTPUT);
  pinMode(PWM2, OUTPUT);
  pinMode(IN2A, OUTPUT);
  pinMode(IN2B, OUTPUT);
  analogWriteResolution(10);
}

void loop()                     
{
  front();
  delay(3000);
  back();
  delay(3000);
}

void front() 
{
  analogWrite(PWM1, 250);
  digitalWrite(IN1A, HIGH);
  digitalWrite(IN1B, LOW);
  analogWrite(PWM2, 250);
  digitalWrite(IN2A, HIGH);
  digitalWrite(IN2B, LOW);
}

void back() 
{
  analogWrite(PWM1, 250);
  digitalWrite(IN1A, LOW);
  digitalWrite(IN1B, HIGH);
  analogWrite(PWM2, 250);
  digitalWrite(IN2A, LOW);
  digitalWrite(IN2B, HIGH);
}
