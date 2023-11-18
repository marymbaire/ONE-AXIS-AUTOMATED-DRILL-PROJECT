#include <Adafruit_MLX90614.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
#include "HX711.h"

const int limitSwitchPin = 2; // Connect the limit switch to digital pin 2

#define DOUT 24
#define CLK 22
HX711 scale(DOUT, CLK);
float calibration_factor = 96650;
Adafruit_MLX90614 mlx = Adafruit_MLX90614();
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified();
const int stepPin = 48;
const int dirPin = 50;
const int enPin = 46;
int enA = 40;
int in1 = 42;
int in2 = 44;
bool stopMachine = false; // Flag to stop the machine
bool motorEnabled = false; // Motor enable flag
bool checkstop=false;
int stepperDirection = 1;  // 1 for forward, -1 for backward
unsigned long previousMillis = 0;
const long interval = 100;  // Interval to read sensors (milliseconds)
int timebt = 0; // Declare timebt here

// Define delay values for different commands
int delayForCommand(const String &command) {
  if (command == "fastdrill") {
    return 13000;
  } else if (command == "slowdrill") {
    return 13000;
  } else if (command == "highfeed") {
    return 4000;
  } else if (command == "slowfeed") {
    return 16000;
  } else {
    return 0; // Default delay if the command is not recognized
  }
}

void setup() {
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  Serial.begin(9600);
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(enPin, OUTPUT);
  digitalWrite(enPin, LOW);
  pinMode(limitSwitchPin, INPUT_PULLUP); // Enable internal pull-up resistor
  // Turn off motors - Initial state
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  

  // limit.setDebounceTime(50); // Adjust the debounce time if needed
  // limit.setCountMode(COUNT_FALLING);

  while (!Serial);

  if (!mlx.begin()) {
    Serial.println("Error connecting to MLX sensor. Check wiring.");
    while (1);
  } else if (!accel.begin()) {
    Serial.println("No valid sensor found");
    while (1);
  }

  scale.set_scale(157860); // Calibration Factor obtained from first sketch
  scale.tare(); // Reset the scale to 0
}

void loop() {
  unsigned long currentMillis = millis();
  int limitSwitchState = digitalRead(limitSwitchPin);
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    command.trim();
      if (command == "stopmachine") {
        //stopMachine = true; // Set the flag to stop the machine
        //motorEnabled = false; // Disable motor
        analogWrite(enA, 0);
        digitalWrite(enPin, HIGH);
        timebt=0;
    }
    if (command == "highfeed" || command == "slowfeed" || command == "slowdrill" || command == "fastdrill") {
      motorEnabled = true; // Enable the motor
      int speed = 0;
      timebt = delayForCommand(command); // Assign the value to timebt
      if (command == "highfeed") {
        speed = 200; // Set your desired speed
      } else if (command == "slowfeed") {
        speed = 200; // Set your desired speed
      } else if (command == "slowdrill") {
        speed = 160; // Set your desired speed
      } else if (command == "fastdrill") {
        speed = 255; // Set your desired speed
      }
      runDCMotor(speed); // Run the DC motor with the specified speed
    }
  }

  // Check if the limit switch is activated
  if (limitSwitchState == LOW) {
    //if (motorEnabled) {
      // If the motor is enabled, reverse the stepper motor direction
      for(int m=0;m<5;m++){
        if(checkstop==false){
          for(int i=0;i<200;i++){
              digitalWrite(dirPin, HIGH);
              digitalWrite(enPin, LOW);

            // Step the motor
              digitalWrite(stepPin, HIGH);
              delayMicroseconds(13000);
              digitalWrite(stepPin, LOW);
              delayMicroseconds(13000);
          }}
        if (Serial.available() > 0) {
          String command = Serial.readStringUntil('\n');
          command.trim();
          if (command == "stopmachine") {
            analogWrite(enA, 0);
            digitalWrite(enPin, HIGH);
            timebt=0;
            checkstop=true;
        }
        }
      }}
    
  
  // Run the stepper motor continuously
  if (timebt>0){
   runStepperMotor(timebt * stepperDirection);
  }
  else{
    digitalWrite(enPin, HIGH);
  }

  // Read sensors at a specified interval
  if (currentMillis - previousMillis >= interval) {
    readsensors();
    previousMillis = currentMillis;
  }
}

void readsensors() {
  int potValue = analogRead(A0);
  float objectTemperatureC = mlx.readObjectTempC();
  sensors_event_t event;
  accel.getEvent(&event);
  float vibration = event.acceleration.z;
  float load = scale.get_units();
  String datatosend = String(objectTemperatureC) + "," + String(potValue) + "," + String(vibration) + "," + String(load);
  Serial.println(datatosend);
}

void runStepperMotor(int delayBetweenPulses) {
  // Set the stepper motor direction
  digitalWrite(dirPin, LOW);
  digitalWrite(enPin, LOW);

  // Step the motor
  digitalWrite(stepPin, HIGH);
  delayMicroseconds(delayBetweenPulses);
  digitalWrite(stepPin, LOW);
  delayMicroseconds(delayBetweenPulses);
}

void runDCMotor(int speed) {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  analogWrite(enA, speed);
}







