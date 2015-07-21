#ifndef MotorActuator_h
#define MotorActuator_h
#include "Arduino.h"

class MotorActuator
{
  public:
    MotorActuator();  
    void setPin(int pinA);
    void turnOff();
    void turnOn();
    void setParameters(int phi, int activeT);  
    int getPhi();
    void setPhi(int);
    int getActiveT();
    void setActiveT(int);
    int getSliderVal();
    void setSliderVal(int);
    
  private:
    int _pinA;
    int _phi;
    int _activeT;
    int _sliderVal;
};
#endif
