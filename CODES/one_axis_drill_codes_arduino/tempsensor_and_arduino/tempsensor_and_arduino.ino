const int lm35Pin = A0;
const int numReadings = 10;   // Number of readings to average
int readings[numReadings];    // Array to store readings
int index = 0;                // Current index
float total = 0;              // Running total

void setup() {
  Serial.begin(9600);
}

void loop() {
  // Read the raw ADC value from the LM35 sensor
  int rawValue = analogRead(lm35Pin);

  // Subtract the previously stored reading at the current index
  total = total - readings[index];

  // Store the new reading in the array
  readings[index] = rawValue;

  // Add the new reading to the total
  total = total + readings[index];

  // Move to the next index
  index = (index + 1) % numReadings;

  // Calculate the average
  float average = total / numReadings;

  // Convert the average to voltage
  float voltage = (average / 1023.0) * 5.0;

  // Convert the voltage to temperature in degrees Celsius
  float temperature = (voltage) * 100.0;

  // Print the temperature
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");

  delay(1000);
}