#include "Arduino.h"
#include "EMS22A.h"
// EMS22A Encoder consists of 6 pins
// Vcc = 3.3 V, GND
EMS22A::EMS22A()
{
}

//Sets the digital pins and initializes the running averages, counter
void EMS22A::setPins(int PIN_CS, int PIN_CLOCK, int PIN_DATA)
{
	_pinCS 		= PIN_CS;
	_pinClock 	= PIN_CLOCK;
	_pinData 	= PIN_DATA;
  pinMode(PIN_CS, OUTPUT);
  pinMode(PIN_CLOCK, OUTPUT);
  pinMode(PIN_DATA, INPUT);

  digitalWrite(PIN_CLOCK, HIGH);
  digitalWrite(PIN_CS, LOW);
}

void EMS22A::updateEncoder()
{
//  Serial.print(_pinCS);
//  Serial.print(" ");
//  Serial.print(_pinClock);
//  Serial.print(" ");
//  Serial.println(_pinData);
  Serial.print(_pinCS);
  Serial.print(" ");
  Serial.print(_pinClock);
  Serial.print(" ");
  Serial.println(_pinData);
  digitalWrite(_pinCS, HIGH);
  digitalWrite(_pinCS, LOW);
  delay(5);
  _absolutePosition = 0;
  for (int i=0; i<10; i++) {
    digitalWrite(_pinClock, LOW);
    digitalWrite(_pinClock, HIGH);
    
    byte b = digitalRead(_pinData) == HIGH ? 1 : 0;
//    Serial.print(b);
    _absolutePosition += b * pow(2, 10-(i+1));
  }
  for (int i=0; i<6; i++) {
    digitalWrite(_pinClock, LOW);
    digitalWrite(_pinClock, HIGH);
  }
//  delay(5);
  digitalWrite(_pinClock, LOW);
  digitalWrite(_pinClock, HIGH);
//  Serial.println(_absolutePosition);
//	_absoluteAngle = _absolutePosition*360/1024;
}

int EMS22A::getAbsPos()
{
	return _absolutePosition;
}
