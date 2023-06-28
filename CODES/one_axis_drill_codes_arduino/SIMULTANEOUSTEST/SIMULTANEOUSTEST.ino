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
  Serial.begin(9600);
  pinMode(stepPin,OUTPUT); 
  pinMode(dirPin,OUTPUT);
  pinMode(enPin,OUTPUT);
  digitalWrite(enPin,LOW);
	// Turn off motors - Initial state
	digitalWrite(in1, LOW);
	digitalWrite(in2, LOW);
}

// Main loop
void loop() {
  limitSwitchStateStart = digitalRead(limitSwitchPinStart);
  limitSwitchStateEnd = digitalRead(limitSwitchPinEnd);
  //limitSwitchStateMiddle= digitalRead(limitSwitchPinMiddle);
  Serial.println(limitSwitchStateEnd);
  if (limitSwitchStateStart ==HIGH) {
      digitalWrite(dirPin,LOW);
     // readtemp();
 // Enables the motor to move in a particular direction
    for(int i=0;i<1200;i++){
      digitalWrite(stepPin,HIGH); 
       delayMicroseconds(1800); 
      digitalWrite(stepPin,LOW); 
       delayMicroseconds(1800);
    }
    //delay(1000);
    // 	digitalWrite(in1, HIGH);
    // 	digitalWrite(in2, LOW);
    //    analogWrite(enA, 200);
    // digitalWrite(stepPin,HIGH); 
    // delayMicroseconds(10000); 
    // digitalWrite(stepPin,LOW); 
    // delayMicroseconds(10000); 
    runStepperMotor();
   // runDCMotor();
    
  }
   else if(limitSwitchStateEnd ==HIGH){
     digitalWrite(dirPin,HIGH);
    for(int i=0;i<200;i++){
      //readtemp();
                    digitalWrite(stepPin,HIGH); 
              delayMicroseconds(1800); 
              digitalWrite(stepPin,LOW); 
              delayMicroseconds(1800);
      	digitalWrite(in1,LOW);
	      digitalWrite(in2, LOW);
        analogWrite(enA, 0);
        //delay(10000);
    }
  }
//   else if(limitSwitchStateMiddle ==HIGH){
//             runStepperMotor();
//              runDCMotor();
//      //        readtemp();
// }
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
}}

// Function to run the stepper motor
void runStepperMotor() {
  // Set the stepper motor direction
  digitalWrite(dirPin,LOW);
 // Enables the motor to move in a particular direction
 for(int j=0;j<600;j++){
    digitalWrite(stepPin,HIGH); 
    delayMicroseconds(10000); 
    digitalWrite(stepPin,LOW); 
    delayMicroseconds(10000); 
    runDCMotor();
 }
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
  readtemp();
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
void readtemp(){
    int sensorValue = analogRead(lm35Pin); // Read analog value from LM35 pin
  float temperature = (sensorValue * 5.0 / 1023.0) * 100.0; // Convert analog value to temperature in Celsius

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");
}
