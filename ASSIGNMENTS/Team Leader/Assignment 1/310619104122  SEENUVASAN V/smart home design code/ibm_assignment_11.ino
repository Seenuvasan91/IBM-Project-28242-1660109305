#include <Servo.h>

int smokeVal = A5;
int smokeOutPin = 10;
int smokeInPin = A5;

int doorOutPin = 9;
int doorInPin = A4;
int redOutPin = 12;
int greenOutPin = 13;
Servo door;

int fanManPin = A1;
int pirInPin = A3;
int ldrInPin = A2;
int fanGreenPin = 4;
int fanRedPin = 2;
int lightOutPin = 7;

void setup()
{
  pinMode(smokeOutPin,OUTPUT);
  pinMode(smokeInPin,INPUT);

  door.attach(doorOutPin);
  
  pinMode(lightOutPin, OUTPUT);
  pinMode(fanRedPin, OUTPUT);
  pinMode(fanGreenPin, OUTPUT);
  pinMode(pirInPin, INPUT);
  pinMode(ldrInPin, INPUT);
  
  Serial.begin(9600); 
}

void checkSmoke() {
  smokeVal = analogRead(smokeInPin);
  if (smokeVal > 500) {
    digitalWrite(smokeOutPin, HIGH);
  }
  else {
    digitalWrite(smokeOutPin, LOW);
  }
}

long readUltraSonicDistance() {
  pinMode(doorInPin, OUTPUT);
  digitalWrite(doorInPin, LOW);
  delayMicroseconds(2);
  
  digitalWrite(doorInPin, HIGH);
  delayMicroseconds(10);
  
  digitalWrite(doorInPin, LOW);
  pinMode(doorInPin, INPUT);
  
  return pulseIn(doorInPin, HIGH); 
}

void openDoor() {
  door.write(90);
  digitalWrite(redOutPin, LOW);
  digitalWrite(greenOutPin, HIGH);
}

void closeDoor() {
  door.write(0);
  digitalWrite(redOutPin, HIGH);
  digitalWrite(greenOutPin, LOW);
}

void automateDoor() {
  int distanceCm = 0.01723 * readUltraSonicDistance();
  if (distanceCm < 50) {
    openDoor();
  }
  else {
    closeDoor();
  }
}

void resetPir() {
  digitalWrite(pirInPin, LOW);
}

void automateFan() {
  bool fanState = digitalRead(pirInPin) || digitalRead(fanManPin);
  digitalWrite(fanGreenPin, fanState);
  digitalWrite(fanRedPin, !fanState);
  
  delayMicroseconds(10);
  resetPir();
}

void automateLight() {
  if (analogRead(ldrInPin) < 500) {
    digitalWrite(lightOutPin, digitalRead(pirInPin));
  }
  else {
    digitalWrite(lightOutPin, LOW);
  }
  
  delayMicroseconds(10);
  resetPir();
}

void loop()
{
  checkSmoke();
  automateDoor();
  automateFan();
  automateLight();
}