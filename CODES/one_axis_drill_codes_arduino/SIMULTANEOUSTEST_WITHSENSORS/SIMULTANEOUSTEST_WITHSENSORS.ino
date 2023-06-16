#include <Wire.h>
#include <Adafruit_MLX90614.h>
Adafruit_MLX90614 mlx = Adafruit_MLX90614();

#include <HX711_ADC.h>
#if defined(ESP8266)|| defined(ESP32) || defined(AVR)
#include <EEPROM.h>
#endif
// ADXL345 registers
#define ADXL345_ADDRESS 0x53
#define ADXL345_REG_POWER_CTL 0x2D
#define ADXL345_REG_DATA_X0 0x32
#define ADXL345_REG_DATA_FORMAT 0x31
String x, y, z;

const int HX711_dout = 4; //mcu > HX711 dout pin
const int HX711_sck = 5; //mcu > HX711 sck pin
//HX711 constructor:
HX711_ADC LoadCell(HX711_dout, HX711_sck);

const int calVal_eepromAdress = 0;
unsigned long t = 0;


const int limitSwitchPinStart=8;
const int limitSwitchPinEnd = 9;
const int limitSwitchPinMiddle=13;
const int lm35Pin = A1;
int limitSwitchStateStart = 0;
int limitSwitchStateEnd = 0;
int limitSwitchStateMiddle=0;
const int stepPin = 48; 
const int dirPin = 50; 
const int enPin = 46;
int enA = A0;
int in1 = 10;
int in2 = 11;   
int time = 3000;
int SPEED = 255;   // Speed of the DC motor (PWM value, 0-255)
// Setup function
void setup() {
  // Initialize the stepper motor pins as outputs
  pinMode(enA, OUTPUT);
	pinMode(in1, OUTPUT);
	pinMode(in2, OUTPUT);
  pinMode(limitSwitchPinStart, INPUT);
  pinMode(limitSwitchPinEnd, INPUT);
  pinMode(limitSwitchPinMiddle,INPUT);
  pinMode(stepPin,OUTPUT); 
  pinMode(dirPin,OUTPUT);
  pinMode(enPin,OUTPUT);
  digitalWrite(enPin,LOW);
	// Turn off motors - Initial state
	digitalWrite(in1, LOW);
	digitalWrite(in2, LOW);

  Serial.begin(57600);
  Wire.begin();
  writeToADXL345(ADXL345_REG_POWER_CTL, 0x08); // Enable measurement mode
  writeToADXL345(ADXL345_REG_DATA_FORMAT, 0x01); // Set range to +/- 4g

    while (!Serial);

  if (!mlx.begin()) {
    Serial.println("Error connecting to MLX sensor. Check wiring.");
    while (1);
  };

  setuploadcell();
}

// Main loop
void loop() {
  limitSwitchStateStart = digitalRead(limitSwitchPinStart);
  limitSwitchStateEnd = digitalRead(limitSwitchPinEnd);
  limitSwitchStateMiddle= digitalRead(limitSwitchPinMiddle);
  if (limitSwitchStateStart ==HIGH) {
      digitalWrite(dirPin,HIGH);
      readAccelerometerData(x, y, z);
      readtemp();
      getdataloadcell();
 // Enables the motor to move in a particular direction
    for(int i=0;i<1800;i++){
      digitalWrite(stepPin,HIGH); 
       delayMicroseconds(1800); 
      digitalWrite(stepPin,LOW); 
       delayMicroseconds(1800);
    }
  }
  // else if(limitSwitchStateEnd ==HIGH){
  //  // for(int i=0;i<1000;i++){
  //     readAccelerometerData(x, y, z);
  //     readtemp();
  //     getdataloadcell();
  //     digitalWrite(stepPin,LOW); 
  //     	digitalWrite(in1,LOW);
	//       digitalWrite(in2, LOW);
  //       analogWrite(enA, 0);
  //       delay(10000);
  // //  }
  
  else if(limitSwitchStateMiddle ==HIGH){
            runStepperMotor();
             runDCMotor();
             readAccelerometerData(x, y, z);
             readtemp();
             getdataloadcell();
}
  else{
            
            digitalWrite(dirPin,LOW);
            readAccelerometerData(x, y, z);
            readtemp();
            getdataloadcell();
            analogWrite(enA, 0);
 // Enables the motor to move in a particular direction
           for(int i=0;i<600;i++){
              digitalWrite(stepPin,HIGH); 
              delayMicroseconds(1800); 
              digitalWrite(stepPin,LOW); 
              delayMicroseconds(1800);
    }

    }

}
// Function to run the stepper motor
void runStepperMotor() {
  // Set the stepper motor direction
  digitalWrite(dirPin,LOW);
 // Enables the motor to move in a particular direction
    digitalWrite(stepPin,HIGH); 
    delayMicroseconds(5500); 
    digitalWrite(stepPin,LOW); 
    delayMicroseconds(5500); 
}
// Function to run the DC motor
void runDCMotor() {
// Turn on motor A 
	digitalWrite(in1, HIGH);
	digitalWrite(in2, LOW);
  analogWrite(enA, 200);

//	delay(1000); delay also removes pararell function
	// Turn off motors
	// digitalWrite(in1, LOW);
	// digitalWrite(in2, LOW);
  // delay(2000);
  // Stop the DC motor
//  analogWrite(enA, 0);    //by adding this line, the code runs sequentially instead of simultaneously to run dc and stepper motors together
}
void runStepperMotorFast() {
  
  // Set the stepper motor direction
  digitalWrite(dirPin,LOW);
 // Enables the motor to move in a particular direction
 for(int i=0;i<400;i++){
    digitalWrite(stepPin,HIGH); 
    delayMicroseconds(5000); 
    digitalWrite(stepPin,LOW); 
    delayMicroseconds(5000); 
 }
}
void writeToADXL345(byte regAddress, byte data) {
  Wire.beginTransmission(ADXL345_ADDRESS);
  Wire.write(regAddress);
  Wire.write(data);
  Wire.endTransmission();
}
float getWireRead(){
  float value= (Wire.read() | Wire.read()<<8);
  return value/256;
}
void readAccelerometerData(String& x, String& y, String& z) {
  Wire.beginTransmission(ADXL345_ADDRESS);
  Wire.write(ADXL345_REG_DATA_X0);
  Wire.endTransmission(false);
  
  Wire.requestFrom(ADXL345_ADDRESS, 6, true);
  
  // Read data
  x = getWireRead();
  y = getWireRead();
  z = getWireRead();
  Serial.print("X: ");
  Serial.print(x);
  Serial.print(" Y: ");
  Serial.print(y);
  Serial.print(" Z: ");
  Serial.println(z);
}
void readtemp(){
    Serial.print("Ambient temperature = "); 
  Serial.print(mlx.readAmbientTempC());
  Serial.print("°C");      
  Serial.print("   ");
  Serial.print("Object temperature = "); 
  Serial.print(mlx.readObjectTempC()); 
  Serial.println("°C");
}
void setuploadcell(){
  Serial.begin(57600);
  // delay(10);
  Serial.println();
  Serial.println("Starting...");

  LoadCell.begin();
  //LoadCell.setReverseOutput(); //uncomment to turn a negative output value to positive
  float calibrationValue; // calibration value (see example file "Calibration.ino")
  //calibrationValue = 696.0; // uncomment this if you want to set the calibration value in the sketch
#if defined(ESP8266)|| defined(ESP32)
  //EEPROM.begin(512); // uncomment this if you use ESP8266/ESP32 and want to fetch the calibration value from eeprom
#endif
  EEPROM.get(calVal_eepromAdress, calibrationValue); // uncomment this if you want to fetch the calibration value from eeprom

  unsigned long stabilizingtime = 2000; // preciscion right after power-up can be improved by adding a few seconds of stabilizing time
  boolean _tare = true; //set this to false if you don't want tare to be performed in the next step
  LoadCell.start(stabilizingtime, _tare);
  if (LoadCell.getTareTimeoutFlag()) {
    Serial.println("Timeout, check MCU>HX711 wiring and pin designations");
    while (1);
  }
  else {
    LoadCell.setCalFactor(calibrationValue); // set calibration value (float)
    Serial.println("Startup is complete");
  }
}
void getdataloadcell(){
    static boolean newDataReady = 0;
  const int serialPrintInterval = 0; //increase value to slow down serial print activity

  // check for new data/start next conversion:
  if (LoadCell.update()) newDataReady = true;

  // get smoothed value from the dataset:
  if (newDataReady) {
    if (millis() > t + serialPrintInterval) {
      float i = LoadCell.getData();
      Serial.print("Load_cell output val: ");
      Serial.println(i);
      newDataReady = 0;
      t = millis();
    }
  }

  // receive command from serial terminal, send 't' to initiate tare operation:
  if (Serial.available() > 0) {
    char inByte = Serial.read();
    if (inByte == 't') LoadCell.tareNoDelay();
  }

  // check if last tare operation is complete:
  if (LoadCell.getTareStatus() == true) {
    Serial.println("Tare complete");
  }
}

