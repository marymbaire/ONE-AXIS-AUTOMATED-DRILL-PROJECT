// Constants
const int motorPin = 9;  // Motor pin (PWM)
const int encoderPinA = 2;  // Encoder pin A (interrupt)
const int encoderPinB = 3;  // Encoder pin B

// Variables
volatile int pulseCount = 0;  // Counter for pulses
unsigned long prevTime = 0;  // Previous time for calculating speed

void setup() {
  // Motor setup
  pinMode(motorPin, OUTPUT);
  analogWrite(motorPin, 0);  // Stop motor initially
  
  // Encoder setup
  pinMode(encoderPinA, INPUT_PULLUP);
  pinMode(encoderPinB, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(encoderPinA), countPulses, RISING);
  
  // Serial monitor setup
  Serial.begin(9600);
}

void loop() {
  // Read motor speed
  unsigned long currentTime = millis();
  unsigned long elapsedTime = currentTime - prevTime;
  prevTime = currentTime;
  
  // Calculate speed in RPM (revolutions per minute)
  float speed = (pulseCount / 48.0) / (elapsedTime / 60000.0);
  pulseCount = 0;  // Reset pulse count
  
  // Print speed to serial monitor
  Serial.print("Speed: ");
  Serial.print(speed);
  Serial.println(" RPM");
  
  // Adjust motor speed based on the calculated speed
  if (speed < 100) {
    analogWrite(motorPin, 100);  // Set motor speed to 100 (0-255 range)
  } else {
    analogWrite(motorPin, 0);  // Stop motor if speed exceeds 100 RPM
  }
}

// Interrupt service routine to count pulses
void countPulses() {
  // Read the state of the other encoder pin
  int encoderPinBState = digitalRead(encoderPinB);

  // Check the direction of rotation based on the state of encoder pin B
  if (encoderPinBState == HIGH) {
    pulseCount++;
  } else {
    pulseCount--;
  }
}
