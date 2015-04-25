#ifndef EMS22A_h
#define EMS22A_h
#include "Arduino.h"
#include <SoftwareSerial.h>

class EMS22A
{
  public:
    EMS22A();  
    void setPins(int PIN_CS, int PIN_CLOCK, int PIN_DATA);
//    void readVoltage();
    void printVoltage(SoftwareSerial &theSerial, int &accelNo);
    void updateEncoder();
	int getAbsPos();
//	double getAngle();
//    int _theVoltages[3];
//    void resetRunningAvg();
    	
  private:
//    Pins - 0-PIN_CS, 1-PIN_CLOCK, 2-PIN_DATA
      int _pinCS;
	int _pinClock;
	int _pinData;
	int _absolutePosition;
	double _absoluteAngle;
//    int ravgCounter;
//    long rAvg[3];
//    Voltage - X, Y, Z
//    int _theVoltages[4];
};
#endif
