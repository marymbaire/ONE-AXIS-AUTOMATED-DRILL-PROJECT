//Define the DC motor connnection
int IN1 = 10;
int IN2 = 11;
int EN = 9;
int SPEED = 255;
int i;
int time = 3000;

// Limit Switch pin
int pin = 21;

// Define stepper motor connections and steps per revolution :
#define dirPin 50
#define stepPin 48
#define stepsPerRevolution 1600

void setup() 
{
  // Declare Stepper Motor pins as output:
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);

  // Declare DC motor pins as output
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(EN, OUTPUT);

  //Interrupt function for the limit switch 
  attachInterrupt(digitalPinToInterrupt(21), Go_Home, CHANGE);
}

void Go_Home() {
  for (i=0; i<=100000; i++) {
    digitalWrite(dirPin, LOW);
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(500);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(500);
    if (digitalRead(20) == 1)
    {
      break;
    }
  }
}

void loop() 
{
  digitalWrite(dirPin,HIGH);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(EN,255);
  digitalWrite(stepPin, HIGH);
  delayMicroseconds(20000);
  digitalWrite(stepPin, LOW);
  delayMicroseconds(20000);
}
