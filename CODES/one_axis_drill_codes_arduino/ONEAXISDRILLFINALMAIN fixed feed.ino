#include <Adafruit_GFX.h>
#include <Adafruit_TFTLCD.h>
#include <TouchScreen.h>
#include <Adafruit_MLX90614.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
#include "HX711.h"

const int limitSwitchPin = 36;
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
int speed=0;
bool stopMachine = false;
bool motorEnabled = false;
bool checkstop = false;
int stepperDirection = 1;
unsigned long previousMillis = 0;
const long interval = 50;
int timebt = 0;
#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0
#define LCD_RESET A4
#define BLACK 0x0000
#define ILI9341_WHITE 0xFFFF

#include <MCUFRIEND_kbv.h>
MCUFRIEND_kbv tft;

const int encoderPin = 19;
const int PPR = 443;
volatile long pulses = 0;
float RPM = 0;
const int numReadings = 10;
float rpmReadings[numReadings];
int rpmIndex = 0;

int16_t x = 20;
int16_t y = 40;

// Define states for the state machine
enum State {
  IDLE,
  RUN_DC_MOTOR,
  RUN_STEPPER_MOTOR,
  READ_SENSORS
};

State currentState = IDLE;
unsigned long previousMillisDCMotor = 0;
unsigned long previousMillisStepperMotor = 0;
unsigned long previousMillisSensors = 0;

void setup() {
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  Serial.begin(9600);
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(enPin, OUTPUT);
  digitalWrite(enPin, LOW);
  pinMode(limitSwitchPin, INPUT_PULLUP);
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);

  while (!Serial);

  if (!mlx.begin()) {
    Serial.println("Error connecting to MLX sensor. Check wiring.");
    while (1);
  } else if (!accel.begin()) {
    Serial.println("No valid sensor found");
    while (1);
  }

  scale.set_scale(157860);
  scale.tare();

  uint16_t identifier = tft.readID();
  identifier = 0x9341;
  tft.begin(identifier);
  tft.setRotation(1);
  tft.fillScreen(BLACK);
  tft.setTextSize(3);
  tft.setTextColor(ILI9341_WHITE);
  tft.setCursor(x, y);
  tft.print("Temp: ");
  tft.setCursor(x, y + 50);
  tft.print("PWM: ");
  tft.setCursor(x, y + 100);
  tft.print("Vib: ");
  tft.setCursor(x, y + 150);
  tft.print("Load: ");
}

void loop() {
  int limitSwitchState = digitalRead(limitSwitchPin);

  // Handle serial commands
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    command.trim();
    if (command == "stopmachine") {
      analogWrite(enA, 0);
      digitalWrite(enPin, HIGH);
      timebt = 0;
      speed=0;
      // checkstop=true;
    } else if (command == "slowdrill" || command == "fastdrill") {
      motorEnabled = true;
      speed = 0;
      timebt = 100;

      if (command == "slowdrill") {
        speed = 200;
      } else if (command == "fastdrill") {
        speed = 255;
      }

      runDCMotor(speed);
    }
  }

  // Check limit switch and update state
  if (limitSwitchState == HIGH) {
    for(int m=0;m<5;m++){
        if(checkstop==false){
          for(int i=0;i<600;i++){
              digitalWrite(dirPin, HIGH);
              digitalWrite(enPin, LOW);

            // Step the motor
              digitalWrite(stepPin, HIGH);
              delayMicroseconds(2000);
              digitalWrite(stepPin, LOW);
              delayMicroseconds(2000);
          }}
        if (Serial.available() > 0) {
          String command = Serial.readStringUntil('\n');
          command.trim();
          if (command == "stopmachine") {
            analogWrite(enA, 0);
            digitalWrite(enPin, HIGH);
            timebt=0;
            checkstop=true;
            speed=0;
        }
        }
      }
  }
  currentState = RUN_STEPPER_MOTOR;
  // Run stepper motor based on state
  if (currentState == RUN_STEPPER_MOTOR) {
    runStepperMotor(timebt); // You may adjust the number of steps
    currentState = IDLE;
  }


  // Read sensors based on state
  unsigned long currentMillisSensors = millis();
  if (currentMillisSensors - previousMillisSensors >= interval) {
    readsensors();
    previousMillisSensors = currentMillisSensors;
  }
}

void readsensors() {
  float avgRPM = 0;
  float objectTemperatureC = mlx.readObjectTempC();
  sensors_event_t event;
  accel.getEvent(&event);
  float vibration = event.acceleration.z;
  float load = scale.get_units();
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    command.trim();
  if (command == "slowdrill" || command == "fastdrill"|| command== "stopmachine") {
      if (command == "slowdrill") {
        speed = 200;
      } else if (command == "fastdrill") {
        speed = 255;
      }
      else if (command == "stopmachine") {
        analogWrite(enA, 0);
        digitalWrite(enPin, HIGH);
        timebt = 0;
        speed = 0;
      }
  }}
  String datatosend = String(objectTemperatureC) + "," + String(speed) + "," + String(vibration) + "," + String(load);
  Serial.println(datatosend);
  int16_t xRect = x + 160;
  int16_t yRect = y;
  int16_t widthRect = 25;
  int16_t heightRect = tft.height();
  tft.setCursor(xRect, yRect);
  tft.fillRect(xRect, yRect, 80, 100, BLACK);
  tft.print(objectTemperatureC, 1);

  int16_t yRect1 = y + 50;
  tft.setCursor(xRect, yRect1);
  tft.fillRect(xRect, yRect1, 80, 100, BLACK);
  tft.print(speed, 1);

  int16_t yRect2 = y + 100;
  tft.setCursor(xRect, yRect2);
  tft.fillRect(xRect, yRect2, 80, 100, BLACK);
  tft.print(vibration, 1);

  int16_t yRect3 = y + 150;
  tft.setCursor(xRect, yRect3);
  tft.fillRect(xRect, yRect3, 80, 100, BLACK);
  tft.print(load, 1);
}

void runStepperMotor(int steps) {
  digitalWrite(dirPin, LOW);
  digitalWrite(enPin, LOW);

  for (int i = 0; i < steps; i++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(8000);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(8000);
  }
}

void runDCMotor(int speed) {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  analogWrite(enA, speed);
}

