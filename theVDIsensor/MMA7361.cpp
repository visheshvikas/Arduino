#include "Arduino.h"
#include "MMA7361.h"
// MMA7361 Accelerometer consists of 9 pins such that
// Vcc = 3.3 V, GND, XOUT, YOUT, ZOUT, SLP, etc.
// SLP (Sleep pin) should be set to high (3.3 V) for the accelerometer to be active
MMA7361::MMA7361()
{
}

//Sets the analog pins and initializes the running averages, counter
void MMA7361::setPins(int thePins[3])
{
  for(int ii=0; ii<3; ii++){
    _thePins[ii] = thePins[ii];
    pinMode(_thePins[ii], INPUT);
  }
//  Serial.print("Pins set as");
//  Serial.println(_thePins[0]);
  resetRunningAvg();
}

//Reads Voltage from the Analog pins
void MMA7361::readVoltage()
{
  for(int ii=0; ii<3; ii++){
    _theVoltages[ii] = analogRead(_thePins[ii]);
  }
}

//writes the Voltages to the serial port (XBee port)
//Total bytes transferred = 2X3 = 6 (as an integer is 2 bytes)
//Note: The serial is passed by reference and not value, otherwise the copy only works once
//void MMA7361::printVoltage(SoftwareSerial &theSerial, int &accelNo)
//{
//  //Total integers being sent out = 3 = 3X2 bytes
//  for(int ii=0; ii<3; ii++){
//  // send the two bytes that comprise an integer
//    theSerial.write(lowByte(_theVoltages[ii])); // send the low byte
//    theSerial.write(highByte(_theVoltages[ii])); // send the high byte
//  }
//}

//Updating the running average and updating the counter
void MMA7361::updateVoltage()
{
  readVoltage();
//  for(int ii=0;ii<3;ii++){
//    rAvg[ii] = (rAvg[ii]*ravgCounter + _theVoltages[ii])/(ravgCounter+1);
//  }
//  ravgCounter++;
}

//Resetting the running averages and the counter to zero
void MMA7361::resetRunningAvg()
{
  ravgCounter = 0;
  for(int ii=0;ii<3;ii++){
    rAvg[ii]=0;
  }
}
