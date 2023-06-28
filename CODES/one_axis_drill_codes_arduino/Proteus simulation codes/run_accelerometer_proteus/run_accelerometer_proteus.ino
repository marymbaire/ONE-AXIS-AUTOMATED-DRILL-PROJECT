const int potXPin = A0; // Potentiometer for X-axis
const int potYPin = A1; // Potentiometer for Y-axis
const int potZPin = A2; // Potentiometer for Z-axis
void setup() {
  // put your setup code here, to run once:
Serial.begin (9600);
}

void loop() {
  // put your main code here, to run repeatedly:
// Read the potentiometer values
  int potXValue = analogRead(potXPin);
  int potYValue = analogRead(potYPin);
  int potZValue = analogRead(potZPin);

  // Map the potentiometer values to a range of -100 to +100
  float xDisplacement = map(potXValue, 0, 1023, -4, 4);
  float yDisplacement = map(potYValue, 0, 1023, -4, 4);
  float zDisplacement = map(potZValue, 0, 1023, -4, 4);

  // Print the displacements on the serial monitor
  Serial.print("X Displacement: ");
  Serial.print(xDisplacement);
  Serial.print("\tY Displacement: ");
  Serial.print(yDisplacement);
  Serial.print("\tZ Displacement: ");
  Serial.println(zDisplacement);

  delay(500); // Delay for smoother serial monitor output (adjust if needed)
}
