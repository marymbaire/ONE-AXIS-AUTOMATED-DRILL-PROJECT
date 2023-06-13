// Pin assignments
const int stepperStepPin = 2;
const int stepperDirPin = 3;
const int dcMotorPWM = 9;
const int dcMotorDir = 10;

// Variables
const int dcMotorSpeed = 250; // PWM value for DC motor speed
const int drillingTime = 10000; // Duration of drilling process in milliseconds

void setup() {
  // Set pin modes
  pinMode(stepperStepPin, OUTPUT);
  pinMode(stepperDirPin, OUTPUT);
  pinMode(dcMotorPWM, OUTPUT);
  pinMode(dcMotorDir, OUTPUT);

  // Set initial motor directions
  digitalWrite(stepperDirPin, HIGH); // Set stepper motor direction as desired
  digitalWrite(dcMotorDir, HIGH); // Set DC motor direction as desired
}

void loop() {
  // Move the stepper motor slowly to provide linear feed
  moveStepperMotor(1000); // Slow speed

  // Turn on the DC motor simultaneously
  startDrilling();

  // Delay for the drilling process
  delay(drillingTime);

  // Stop the DC motor
  stopDrilling();

  // Move the stepper motor at high speed to retract from the workpiece
  moveStepperMotor(1000); // High speed

  // Move the stepper motor at high speed to return to the starting position
  moveStepperMotor(1000); // High speed
}

void moveStepperMotor(int speed) {
  digitalWrite(stepperStepPin, HIGH);
  delayMicroseconds(speed);
  digitalWrite(stepperStepPin, LOW);
  delayMicroseconds(speed);
}

void startDrilling() {
  // Turn on the DC motor by setting PWM value
  analogWrite(dcMotorPWM, dcMotorSpeed);
}

void stopDrilling() {
  // Stop the DC motor by setting PWM value to 0
  analogWrite(dcMotorPWM, 0);
}






