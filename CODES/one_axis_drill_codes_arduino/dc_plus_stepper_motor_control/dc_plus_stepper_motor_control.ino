const int stepPin = 48; 
const int dirPin = 50; 
const int enPin = 46;
int enA = 10;
int in1 = 38;
int in2 = 36;
unsigned long previoustimerunstepperfast=millis();
long timeintervalrunstepperfast=2010;
unsigned long previoustimerunstepperslow=millis();
long timeintervalrunstepperslow=1010;
unsigned long previoustimedirectioncontrol=millis();
long timeintervaldirectioncontrol=1000;
unsigned long previoustimerunstepperbehind=millis();
long timeintervalrunstepperbehind=2000;
void setup() {
  // put your setup code here, to run once:
  pinMode(enA, OUTPUT);
	pinMode(in1, OUTPUT);
	pinMode(in2, OUTPUT);
  
  pinMode(stepPin,OUTPUT); 
  pinMode(dirPin,OUTPUT);
  pinMode(enPin,OUTPUT);
  digitalWrite(enPin,LOW);
	// Turn off motors - Initial state
	digitalWrite(in1, LOW);
	digitalWrite(in2, LOW);
  
  // digitalWrite(enPin,LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  runstepperfast();
    unsigned long currenttime=millis();
  if (currenttime-previoustimerunstepperfast>timeintervalrunstepperfast){
     runstepperslow();
  }
  if (currenttime-previoustimerunstepperslow>timeintervalrunstepperslow){
    directionControl();
  }
  runstepperbehind();
  if (currenttime-previoustimedirectioncontrol>timeintervaldirectioncontrol){
      runstepperfast();
  }

}
void directionControl() {
  
	// Set motors to maximum speed
	// For PWM maximum possible values are 0 to 255
  unsigned long currenttime1=millis();
  if (currenttime1-previoustimerunstepperbehind>timeintervalrunstepperbehind){
      analogWrite(enA, 250);
    // Turn on motor A & B
      digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW);
      //delay(5000);
	
	// Now change motor directions

	// Turn off motors
	    analogWrite(enA,0);
    	
}
}
void runstepperslow(){
        digitalWrite(dirPin,LOW);
        for(int y = 0; y < 400; y++) {
          digitalWrite(stepPin,HIGH); 
          delayMicroseconds(3000); 
          digitalWrite(stepPin,LOW); 
          delayMicroseconds(3000); 
        }
  }

void runstepperfast(){
digitalWrite(dirPin,LOW); // Enables the motor to move in a particular direction
  for(int x = 0; x < 1600; x++) {
    digitalWrite(stepPin,HIGH); 
    delayMicroseconds(1500); 
    digitalWrite(stepPin,LOW); 
    delayMicroseconds(1500); 
  }
}
void runstepperbehind(){
  digitalWrite(dirPin,HIGH); //Changes the direction of rotation
  for(int x = 0; x < 1800; x++) {
    digitalWrite(stepPin,HIGH);
    delayMicroseconds(1500);
    digitalWrite(stepPin,LOW);
    delayMicroseconds(1500);
  }
}
