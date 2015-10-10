/**PerFoRo DC Motor Control Module ***/

const int PWM1 =  23;
const int INA = 14;
const int INB = 15;

void setup()   {                
  pinMode(PWM1, OUTPUT);
  pinMode(INA, OUTPUT);
  pinMode(INB, OUTPUT);
  analogWriteResolution(10);
}

void loop()                     
{
  analogWrite(PWM1, 250);
  delay(500);
  digitalWrite(INA, HIGH);
  delay(100);
  digitalWrite(INB, LOW);
  delay(100);
}
