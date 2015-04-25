#ifndef MMA7361_h
#define MMA7361_h
#include "Arduino.h"
//#include <SoftwareSerial.h>

class MMA7361
{
  public:
    MMA7361();  
    void setPins(int thePins[3]);
    void readVoltage();
//    void printVoltage(SoftwareSerial &theSerial, int &accelNo);
    void updateVoltage();
    int _theVoltages[3];
    void resetRunningAvg();
    	
  private:
//    Pins - X, Y, Z
    int _thePins[3];
    int ravgCounter;
    long rAvg[3];
//    Voltage - X, Y, Z
//    int _theVoltages[4];
};
#endif
