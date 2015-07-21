#include "Arduino.h"
#include "MotorActuator.h"
MotorActuator::MotorActuator(){
}

// sets the Pins for actuation
void MotorActuator::setPin(int pinA){
  _pinA = pinA;
  pinMode(_pinA, OUTPUT);
  // Initially both are set to low
  turnOff();
}

void MotorActuator::turnOff(){
  digitalWrite(_pinA, LOW);
}

void MotorActuator::turnOn(){	
  digitalWrite(_pinA, HIGH);
}

void MotorActuator::setParameters(int phi, int activeT){
	setPhi(phi);
        setActiveT(activeT);
}
void MotorActuator::setPhi(int phi){
	_phi = phi/10;
      _phi = _phi*10;
}

void MotorActuator::setActiveT(int activeT){
	_activeT = activeT;
}

int MotorActuator::getPhi(){
	return _phi;
}
int MotorActuator::getActiveT(){
	return _activeT;
}

int MotorActuator::getSliderVal(){
  return _sliderVal;
}

void MotorActuator::setSliderVal(int sliderVal){
  _sliderVal = sliderVal;
}

