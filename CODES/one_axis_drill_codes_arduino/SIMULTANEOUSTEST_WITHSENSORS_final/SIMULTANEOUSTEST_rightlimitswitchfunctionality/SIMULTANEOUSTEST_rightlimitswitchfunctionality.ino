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

#include <Wire.h>
#include <Adafruit_MLX90614.h>
Adafruit_MLX90614 mlx = Adafruit_MLX90614();


// ADXL345 registers
#define ADXL345_ADDRESS 0x53
#define ADXL345_REG_POWER_CTL 0x2D
#define ADXL345_REG_DATA_X0 0x32
#define ADXL345_REG_DATA_FORMAT 0x31
String x, y, z;

#include "HX711.h" //You must have this library in your arduino library folder
#define DOUT 4
#define CLK 5
HX711 scale(DOUT, CLK);
float calibration_factor = 96650;
// Setup function
void setup() {
  // Initialize the stepper motor pins as outputs
  pinMode(enA, OUTPUT);
	pinMode(in1, OUTPUT);
	pinMode(in2, OUTPUT);
  pinMode(limitSwitchPinStart, INPUT);
  pinMode(limitSwitchPinEnd, INPUT);
  pinMode(limitSwitchPinMiddle,INPUT);
  Serial.begin(9600);
    while (!Serial);

  if (!mlx.begin()) {
    Serial.println("Error connecting to MLX sensor. Check wiring.");
    while (1);
  };
  pinMode(stepPin,OUTPUT); 
  pinMode(dirPin,OUTPUT);
  pinMode(enPin,OUTPUT);
  digitalWrite(enPin,LOW);
	// Turn off motors - Initial state
	digitalWrite(in1, LOW);
	digitalWrite(in2, LOW);

    Wire.begin();
  writeToADXL345(ADXL345_REG_POWER_CTL, 0x08); // Enable measurement mode
  writeToADXL345(ADXL345_REG_DATA_FORMAT, 0x01); // Set range to +/- 4g

scale.set_scale(157860); //Calibration Factor obtained from first sketch
scale.tare(); //Reset the scale to 0 
}

// Main loop
void loop() {
  limitSwitchStateStart = digitalRead(limitSwitchPinStart);
  limitSwitchStateEnd = digitalRead(limitSwitchPinEnd);
 //Serial.println(limitSwitchStateStart);
  if (limitSwitchStateStart ==HIGH) {
      digitalWrite(dirPin,LOW);
      
     // readtemp();
 // Enables the motor to move in a particular direction
 for(int m=0;m<6;m++){
    for(int i=0;i<200;i++){
            
      digitalWrite(stepPin,HIGH); 
       delayMicroseconds(1800); 
      digitalWrite(stepPin,LOW); 
       delayMicroseconds(1800);
    }
    getdataloadcell();
    // readtemp();
    // readAccelerometerData(x,y,z);
    // Serial.println("-----------------------------------------------------------------");
 }
    runStepperMotor();
    //getdataloadcell();
  }
   else if(limitSwitchStateEnd ==HIGH){
     digitalWrite(dirPin,HIGH);
     for(int b=0;b<9;b++){
    for(int i=0;i<200;i++){
      //readtemp();
     // getdataloadcell();
      digitalWrite(stepPin,HIGH); 
      delayMicroseconds(1800); 
      digitalWrite(stepPin,LOW); 
      delayMicroseconds(1800);
      digitalWrite(in1,LOW);
	    digitalWrite(in2, LOW);
      analogWrite(enA, 0);
    }
    getdataloadcell();
    // readtemp();
    // readAccelerometerData(x,y,z);
    // Serial.println("-----------------------------------------------------------------");
     }
     delay(10000);
  }
  else{
            
            digitalWrite(dirPin,HIGH);
            analogWrite(enA, 0);
 // Enables the motor to move in a particular direction
           for(int i=0;i<200;i++){
              digitalWrite(stepPin,HIGH); 
              delayMicroseconds(1800); 
              digitalWrite(stepPin,LOW); 
              delayMicroseconds(1800);
   }
  getdataloadcell();
 // readtemp();
 // readAccelerometerData(x,y,z);
 // Serial.println("-----------------------------------------------------------------");
}
}

// Function to run the stepper motor
void runStepperMotor() {
  // Set the stepper motor direction
  digitalWrite(dirPin,LOW);
 // Enables the motor to move in a particular direction
 for (int n=0;n<4;n++){
 for(int j=0;j<200;j++){
    digitalWrite(stepPin,HIGH); 
    delayMicroseconds(16000); 
    digitalWrite(stepPin,LOW); 
    delayMicroseconds(16000); 
    runDCMotor();
 }
  getdataloadcell();
 // readtemp();
 //readAccelerometerData(x,y,z);
  //Serial.println("-----------------------------------------------------------------");
}
}
// Function to run the DC motor
void runDCMotor() {
// Turn on motor A 
	digitalWrite(in1, LOW);
	digitalWrite(in2, HIGH);
  analogWrite(enA, 255);

//	delay(1000); delay also removes pararell function
	// Turn off motors
	// digitalWrite(in1, LOW);
	// digitalWrite(in2, LOW);
  // delay(2000);
  // Stop the DC motor
//  analogWrite(enA, 0);    //by adding this line, the code runs sequentially instead of simultaneously to run dc and stepper motors together
}
void readtemp(){
  Serial.print("Ambient temperature = "); 
  Serial.print(mlx.readAmbientTempC());
  Serial.print("°C");      
  Serial.print("   ");
  Serial.print("Object temperature = "); 
  Serial.print(mlx.readObjectTempC()); 
  Serial.println("°C");

  //   Serial.print(mlx.readAmbientTempC()); when using seria plotter, use this format
  // Serial.println();
}
void getdataloadcell(){
 //Serial.print("Weight: ");
 Serial.print(scale.get_units(), 3); //Up to 3 decimal points
 Serial.println();
 //Serial.println(" kg"); //Change this to kg and re-adjust the calibration factor if you follow lbs
//  if(Serial.available())
//  {
//  char temp = Serial.read();
//  if(temp == 't' || temp == 'T')
//  scale.tare(); //Reset the scale to zero 
//  }
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
