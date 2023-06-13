// #include <Wire.h>
// int ADXL_345 = 0x53;
// String x,y,z;
// void setup(){
//   Serial.begin(9600);
//   Wire.begin();
// }
// void loop()
// {
//   Wire.beginTransmission(ADXL_345);
//   Wire.write(0x32);
//   Wire.endTransmission(false);
//   Wire.requestFrom(ADXL_345,6,true);
//   x=getWireRead();
//   y=getWireRead();
//   z=getWireRead();
//   Serial.println("X:"+ x +" g Y:"+ y +"g Z:"+ z + "g");
//   delay(100);
// }
// float getWireRead(){
//   float value= (Wire.read() | Wire.read()<<8);
//   return value/256;
// }
#include <Wire.h>

// ADXL345 registers
#define ADXL345_ADDRESS 0x53
#define ADXL345_REG_POWER_CTL 0x2D
#define ADXL345_REG_DATA_X0 0x32
#define ADXL345_REG_DATA_FORMAT 0x31
String x, y, z;
void setup() {
  Serial.begin(9600);
  Wire.begin();
  
  // Configure ADXL345
  writeToADXL345(ADXL345_REG_POWER_CTL, 0x08); // Enable measurement mode
  writeToADXL345(ADXL345_REG_DATA_FORMAT, 0x01); // Set range to +/- 4g
}

void loop() {
  // Read accelerometer data
  
  readAccelerometerData(x, y, z);
  
  // Print the values
  Serial.print("X: ");
  Serial.print(x);
  Serial.print(" Y: ");
  Serial.print(y);
  Serial.print(" Z: ");
  Serial.println(z);
  
  delay(500); // Adjust the delay according to your needs
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
}