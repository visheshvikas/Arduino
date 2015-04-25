// Vishesh Vikas
// created on 6/17/2014

//////////////////////////////////////////////////////////////////////////////////////////
#include "MMA7361.h"
#include <SoftwareSerial.h>
// XBee's DOUT (TX) is connected to pin 2 (Arduino's Software RX)
// XBee's DIN (RX) is connected to pin 3 (Arduino's Software TX)
SoftwareSerial XBee(2, 3); // RX, TX


MMA7361 theAs[4];    // The array of accelerometers
int theAPins[4][3] = {
{ A0, A1, A2}, 
{ A3, A4, A5}, 
{ A6, A7, A8}, 
{ A9, A10, A11}
};

// Encoder Stuff
int PIN_CS = 5;
int PIN_CLOCK = 6;
int PIN_DATA = 7;
volatile int encodercount;


int sentCounter;
//////////////////////////////////////////////////////////////////////////////////////
//The Wire library is used for I2C communication
#include <Wire.h>

//This is a list of registers in the ITG-3200. Registers are parameters that determine how the sensor will behave, or they can hold data that represent the
//sensors current status.
//To learn more about the registers on the ITG-3200, download and read the datasheet.
char WHO_AM_I = 0x00;
char SMPLRT_DIV= 0x15;
char DLPF_FS = 0x16;
char GYRO_XOUT_H = 0x1D;
char GYRO_XOUT_L = 0x1E;
char GYRO_YOUT_H = 0x1F;
char GYRO_YOUT_L = 0x20;
char GYRO_ZOUT_H = 0x21;
char GYRO_ZOUT_L = 0x22;

//This is a list of settings that can be loaded into the registers.
//DLPF, Full Scale Register Bits
//FS_SEL must be set to 3 for proper operation
//Set DLPF_CFG to 3 for 1kHz Fint and 42 Hz Low Pass Filter
char DLPF_CFG_0 = 1<<0;
char DLPF_CFG_1 = 1<<1;
char DLPF_CFG_2 = 1<<2;
char DLPF_FS_SEL_0 = 1<<3;
char DLPF_FS_SEL_1 = 1<<4;

//I2C devices each have an address. The address is defined in the datasheet for the device. The ITG-3200 breakout board can have different address depending on how
//the jumper on top of the board is configured. By default, the jumper is connected to the VDD pin. When the jumper is connected to the VDD pin the I2C address
//is 0x69.
char itgAddress = 0x69;
//////////////////////////////////////////////////////////////////////////////////////

void setup()
{
  //  KEEP IN MIND THE BAUD
  Serial.begin(115200); //Set the communication baud rate - bits/sec
  XBee.begin(115200); //Same with XBees
  //  Setting pins
  //  Setting counter and running Avg to zero
  for(int ii=0; ii<4; ii++){
    theAs[ii].setPins(theAPins[ii]);
  }
  sentCounter=0;

  /////////////////// Setup Gyro
  //Initialize the I2C communication. This will set the Arduino up as the 'Master' device.
  Wire.begin();

  //Read the WHO_AM_I register and print the result
  char id=0; 
  id = itgRead(itgAddress, 0x00);  
//  Serial.print("ID: ");
//  Serial.println(id, HEX);

  //Configure the gyroscope
  //Set the gyroscope scale for the outputs to +/-2000 degrees per second
  itgWrite(itgAddress, DLPF_FS, (DLPF_FS_SEL_0|DLPF_FS_SEL_1|DLPF_CFG_0));
  //Set the sample rate to 100 hz
  itgWrite(itgAddress, SMPLRT_DIV, 9);
  
  
  ///////////////////// ENCODER
  pinMode(PIN_CS, OUTPUT);
  pinMode(PIN_CLOCK, OUTPUT);
  pinMode(PIN_DATA, INPUT);

  digitalWrite(PIN_CLOCK, HIGH);
  digitalWrite(PIN_CS, LOW);

}

void loop()
{
  ////// STEP 1: ENCODER
  digitalWrite(PIN_CS, HIGH);
  digitalWrite(PIN_CS, LOW);
  encodercount = 0;
  for (int i=0; i<10; i++) {
    digitalWrite(PIN_CLOCK, LOW);
    digitalWrite(PIN_CLOCK, HIGH);
    
    byte b = digitalRead(PIN_DATA) == HIGH ? 1 : 0;
    encodercount += b * pow(2, 10-(i+1));
  }
  for (int i=0; i<6; i++) {
    digitalWrite(PIN_CLOCK, LOW);
    digitalWrite(PIN_CLOCK, HIGH);
  }
  digitalWrite(PIN_CLOCK, LOW);
  digitalWrite(PIN_CLOCK, HIGH);


  /////// STEP 2: GYRO READS
  //GyroRead
  int xRate, yRate, zRate;
  xRate = readX();  yRate = readY();  zRate = readZ();
  //Print the output rates to the terminal, seperated by a TAB character.
  //  Serial.print(xRate);
  //  Serial.print('\t');
  //  Serial.print(yRate);
  //  Serial.print('\t');
  //  Serial.println(zRate);  

  //////// STEP 3: Accelerometer Reads
  //Read Voltage
  for(int ii=0; ii<4; ii++){
    theAs[ii].readVoltage();
  } 
  

  /////// Writing to XBee
  if(XBee.available()>0)
  {
    Serial.println("Something came in");
    //Message coming in from XBee - indication to send message from here
    XBee.flush();

    //    Writing Voltage
    //  Accelerometers - total bytes = 4 X 3 X 2 = 24
//    int ID = 343;
//    XBee.write(ID);
//    XBee.write("L1");
    for(int jj=0;jj<4;jj++){
      //      Write the analog reading to XBee port
      //Writing voltages
//      Serial.print(jj);
//      Serial.print(" Vs = (");
      for(int ii=0; ii<3; ii++){
      // send the two bytes that comprise an integer
//        Serial.print(lowByte(theAs[jj]._theVoltages[ii]));
//        Serial.print("-");
//        Serial.print(highByte(theAs[jj]._theVoltages[ii]));        
//        Serial.print(" [");
//        Serial.print(theAs[jj]._theVoltages[ii]);        
//        Serial.print("],");
//        XBee.write(theAs[jj]._theVoltages[ii]); // send the low byte
        XBee.write(lowByte(theAs[jj]._theVoltages[ii])); // send the low byte
        XBee.write(highByte(theAs[jj]._theVoltages[ii])); // send the high byte
      }
//  theAs[jj].printVoltage(XBee,jj); 
      //      Reset the running average and the counter for the accelerometer
//      theAs[jj].resetRunningAvg(); 
//      for(int kk=0;kk<3;kk++){
//        Serial.print(theAs[jj]._theVoltages[kk]);
//        Serial.print(",");
//      }
//      Serial.println(")");
    }
    sentCounter++;
//    Serial.print("Sent = ");
//    Serial.println(sentCounter);
    //Sending encoder count should not be tough - convert encoder count to highBtye and lowByte = 2 bytes
    //Gyroscope - 3 (X,Y,Z) X 2 = 6
    //Remember - we need to average gyro readings too and then reset them too
//    XBee.write(xRate);
//    XBee.write(yRate);
//    XBee.write(zRate);
//    XBee.write(encodercount);

    XBee.write(lowByte(xRate)); XBee.write(highByte(xRate));
    XBee.write(lowByte(yRate)); XBee.write(highByte(yRate));
    XBee.write(lowByte(zRate)); XBee.write(highByte(zRate));
//    // Encoder bytes = 2
    XBee.write(lowByte(encodercount)); XBee.write(highByte(encodercount));    
    // GRAND TOTAL BYTES
    // Grand total should be 1 + 4 X 3X2  + 1 X 1X2 + 1 X 3X2 = 33
    }

}


//////////////////////////////////////////////////////////////////////////////////////////
///////////////////////// GYROSCOPE FUNCTIONS ////////////////////////////////////////////

//This function will write a value to a register on the itg-3200.
//Parameters:
// char address: The I2C address of the sensor. For the ITG-3200 breakout the address is 0x69.
// char registerAddress: The address of the register on the sensor that should be written to.
// char data: The value to be written to the specified register.
void itgWrite(char address, char registerAddress, char data)
{
  //Initiate a communication sequence with the desired i2c device
  Wire.beginTransmission(address);
  //Tell the I2C address which register we are writing to
  Wire.write(registerAddress);
  //Send the value to write to the specified register
  Wire.write(data);
  //End the communication sequence
  Wire.endTransmission();
}

//This function will read the data from a specified register on the ITG-3200 and return the value.
//Parameters:
// char address: The I2C address of the sensor. For the ITG-3200 breakout the address is 0x69.
// char registerAddress: The address of the register on the sensor that should be read
//Return:
// unsigned char: The value currently residing in the specified register
unsigned char itgRead(char address, char registerAddress)
{
  //This variable will hold the contents read from the i2c device.
  unsigned char data=0;

  //Send the register address to be read.
  Wire.beginTransmission(address);
  //Send the Register Address
  Wire.write(registerAddress);
  //End the communication sequence.
  Wire.endTransmission();

  //Ask the I2C device for data
  Wire.beginTransmission(address);
  Wire.requestFrom(address, 1);

  //Wait for a response from the I2C device
  if(Wire.available()){
    //Save the data sent from the I2C device
    data = Wire.read();
  }

  //End the communication sequence.
  Wire.endTransmission();

  //Return the data read during the operation
  return data;
}

//This function is used to read the X-Axis rate of the gyroscope. The function returns the ADC value from the Gyroscope
//NOTE: This value is NOT in degrees per second.
//Usage: int xRate = readX();
int readX(void)
{
  int data=0;
  data = itgRead(itgAddress, GYRO_XOUT_H)<<8;
  data |= itgRead(itgAddress, GYRO_XOUT_L);

  return data;
}

//This function is used to read the Y-Axis rate of the gyroscope. The function returns the ADC value from the Gyroscope
//NOTE: This value is NOT in degrees per second.
//Usage: int yRate = readY();
int readY(void)
{
  int data=0;
  data = itgRead(itgAddress, GYRO_YOUT_H)<<8;
  data |= itgRead(itgAddress, GYRO_YOUT_L);

  return data;
}

//This function is used to read the Z-Axis rate of the gyroscope. The function returns the ADC value from the Gyroscope
//NOTE: This value is NOT in degrees per second.
//Usage: int zRate = readZ();
int readZ(void)
{
  int data=0;
  data = itgRead(itgAddress, GYRO_ZOUT_H)<<8;
  data |= itgRead(itgAddress, GYRO_ZOUT_L);

  return data;
}



