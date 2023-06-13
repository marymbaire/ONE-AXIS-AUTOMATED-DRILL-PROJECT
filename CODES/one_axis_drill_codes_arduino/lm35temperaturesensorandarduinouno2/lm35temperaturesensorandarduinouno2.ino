// LM35 Temperature Sensor
const int lm35Pin = A0; // Connect LM35 output to analog pin A0

void setup() {
  Serial.begin(9600); // Initialize serial communication
}

void loop() {
  int sensorValue = analogRead(lm35Pin); // Read analog value from LM35 pin
  float temperature = (sensorValue * 5.0 / 1023.0) * 100.0; // Convert analog value to temperature in Celsius

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");

  delay(1000); // Delay for 1 second
}
