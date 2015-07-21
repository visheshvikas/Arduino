// For the breakout, you can use any (2 or) 3 pins
#define sclk 15
#define mosi 16
#define cs   7
#define dc   0
#define rst  1  // you can also connect this to the Arduino reset


#include <Esplora.h>
#include "MotorActuator.h"
#include <FlexiTimer2.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library
#include <SPI.h>

#if defined(__SAM3X8E__)
    #undef __FlashStringHelper::F(string_literal)
    #define F(string_literal) string_literal
#endif

// Option 1: use any pins but a little slower
//Adafruit_ST7735 tft = Adafruit_ST7735(cs, dc, mosi, sclk, rst);

// Option 2: must use the hardware SPI pins
// (for UNO thats sclk = 13 and sid = 11) and pin 10 must be
// an output. This is much faster - also required if you want
// to use the microSD card (see the image drawing example)
Adafruit_ST7735 tft = Adafruit_ST7735(cs, dc, rst);


// Motors and parameters
MotorActuator motorA, motorB;
#define thePinA 11
#define thePinB 3

//Declaring Parameters
  int T; //, T1_active, T2_active, phase12;
  int exT, exT1_active, exT2_active, exphase12;//, absexphase12;

// Establishing Global Time
  int theTime;
  int timeResolution;
  int clockResolution;
  int totalCycles;
  boolean isRunning;

//   For the Board Inputs
  // 4 Buttons 
  int leftButton, rightButton, upButton, downButton;
  int lastLeft, lastRight, lastUp, lastDown;
  // Slider
  int sliderVal, lastSliderVal;
  // Joystick
  int xValue, yValue, joyStickButton, lastJoyStickButton;
 
 
  
  void setup(){
    //Serial communication rate 
    Serial.begin(9600);
    
//    Initializations     
//    1. Initializing Buttons
    lastLeft=HIGH;
    lastRight=HIGH;
    lastUp=HIGH;
    lastDown=HIGH;
    lastJoyStickButton = HIGH;
    joyStickButton = HIGH;
//    2. Initializing Slider
    lastSliderVal = 0;
    
//    3. Initializing Motors and their parameters - all in milliseconds (ms)
//    3a. Set pins
    motorA.setPin(thePinA);
    motorB.setPin(thePinB);
//    3b. active Time, etc.
    T = 1200;      // Time cycle time    
    motorA.setParameters(0,300);    // motorA phi and T_active
    motorB.setParameters(1000,300);  // motorB phi and T_active
    
//    4. Timer and it's paramters
    timeResolution = 10; // The time is in milliseconds
    clockResolution = 1/1000; // Analogous to the second hand on the clock
//    FlexiTimer2::set(timeResolution, clockResolution, timerFunc); 
    FlexiTimer2::set(timeResolution, timerFunc); //Default - clockResolution is 1 ms
//    Initializing time
    theTime = 0;
    totalCycles = 0;
//    updateParameters();
    FlexiTimer2::start();

//    5. Screen Initializations
    tft.initR(INITR_BLACKTAB);   // initialize a ST7735S chip, black tab
    tft.setRotation(1);
    
//    
    isRunning = LOW;
  }

  void printScreen(){
//    tft.setTextSize(1);
//    tft.println("   M-BOT");
    T = max(max(motorA.getActiveT() + motorA.getPhi(), motorB.getActiveT() + motorB.getPhi())+40, T);

    tft.fillScreen(ST7735_BLACK);
   tft.setCursor(0, 0);
    tft.setTextColor(ST7735_WHITE);
   printParameter("Cycle time (ms): ", T);
//  printParameter("MotorA phase (ms): ", motorA.getPhi());
  printParameter("MotorA active (ms): ", motorA.getActiveT());
  printParameter("phase (ms): ", motorB.getPhi());
  printParameter("MotorB active (ms): ", motorB.getActiveT());
  if(isRunning){
      tft.setTextColor(ST7735_GREEN);      
      tft.println("RUNNING");
      tft.setTextColor(ST7735_WHITE);     
  }
  else{
      tft.setTextColor(ST7735_RED);      
      tft.println("STOP");
      tft.setTextColor(ST7735_WHITE);     
    }
  }
  
  void printParameter(char* text, int value){
    tft.setTextSize(1);
    tft.println(text);
    tft.setTextSize(2);
    tft.println(value);    
  }
  


  void loop()
  {
    leftButton = Esplora.readButton(SWITCH_LEFT);
    rightButton = Esplora.readButton(SWITCH_RIGHT);
    upButton = Esplora.readButton(SWITCH_UP);
    downButton = Esplora.readButton(SWITCH_DOWN);
    sliderVal = Esplora.readSlider();
    xValue = Esplora.readJoystickX();
    yValue = Esplora.readJoystickY();
    joyStickButton = Esplora.readJoystickButton();
        if(!isRunning){

    if(joyStickButton==LOW && joyStickButton!=lastJoyStickButton){
      //Reset Parameters
      Serial.println("RESET");
    }
    
    if(leftButton==LOW && leftButton!=lastLeft)
    {
//    Buttons add or subtract 20ms from T1 and T2 active
      motorB.setActiveT(motorB.getActiveT() - 20);
      printScreen();
    }
    if(rightButton==LOW && rightButton!=lastRight)
    {
      motorB.setActiveT(motorB.getActiveT() + 20);
      printScreen();
    }
    if(upButton==LOW && upButton!=lastUp)
    {
      motorA.setActiveT(motorA.getActiveT() + 20);
      printScreen();
    }
    if(downButton==LOW && downButton!=lastDown)
    {
      motorA.setActiveT(motorA.getActiveT() - 20);
      printScreen();
    }
    if(abs(sliderVal-motorB.getSliderVal())>10){
      motorB.setSliderVal(sliderVal);
      motorB.setPhi(map(sliderVal, 0, 1023, 2000, 0));
      printScreen();
    }
    if(abs(xValue)>20){
      T = T - 20*xValue/abs(xValue);
      printScreen();
    }
  }
      lastLeft = leftButton;
      lastRight = rightButton;
      lastUp = upButton;
      lastDown=downButton;
      lastSliderVal=sliderVal;
      lastJoyStickButton = joyStickButton;
  
  }
   
    
  void timerFunc()
  {
 if(theTime == 0){
//   Update Parmaters
    exT            = T;
    exT1_active    = motorA.getActiveT();
    exT2_active    = motorB.getActiveT();
    exphase12      = motorB.getPhi();
 }   
    
    //Note: for some reason Y value is inverted... negative # means joystick is UP relative to 
    //board, and pos # means joystick is DOWN relative to board
    //Also note: Joystick not perfectly calibrated... hovers between 2 and -3, must account for this
    if(isRunning)
    {      
      if(theTime==0){
        motorA.turnOn();
  //      analogWrite(3, 255);
        Serial.print("Time = ");
        Serial.println(theTime);
        Serial.println("MOTOR 1 TURNED ON");
      }
      if(theTime==exT1_active){
        motorA.turnOff();
        Serial.print("Time = ");
        Serial.println(theTime);
        Serial.println("MOTOR 1 TURNED OFF");
      }
      if(theTime==exphase12){
        motorB.turnOn();
        Serial.print("Time = ");
        Serial.println(theTime);
        Serial.println("MOTOR 2 TURNED ON");
      }
      if(theTime==(exphase12+exT2_active)){
        motorB.turnOff();
        Serial.print("Time = ");
        Serial.println(theTime);
        Serial.println("MOTOR 2 TURNED OFF");
      }   
    //    The time is incremented by timeResolution
      if(theTime == exT){
    //    RESETS THE CLOCK EVERY exT time period
        theTime=0;
        totalCycles = totalCycles + 1;
      }
      else{
        theTime = theTime + timeResolution;
    }
      if(abs(yValue)<20){
      isRunning = LOW;
      motorA.turnOff();
      motorB.turnOff();
      printScreen();
      theTime = 0;
      }
   }
  else{
    if(yValue<-20){
    //IF Y JOYSTICK IS POSITIVE
      motorA.setPin(thePinA);
      motorB.setPin(thePinB);
      isRunning = HIGH;
      printScreen();
      tft.setTextColor(ST7735_GREEN);      
      tft.println("Forward");
      tft.setTextColor(ST7735_WHITE);
    }
    if(yValue>20){
       //IF Y JOYSTICK IS NEGATIVE
      motorA.setPin(thePinB);
      motorB.setPin(thePinA);
      isRunning = HIGH;
      printScreen();
      tft.setTextColor(ST7735_GREEN);      
      tft.println("Backward");
      tft.setTextColor(ST7735_WHITE);
    }
  } 
 
//  //    The time is incremented by timeResolution
//    if(theTime == exT){
//  //    RESETS THE CLOCK EVERY exT time period
//      theTime=0;
//      totalCycles = totalCycles + 1;
//    }
//    else{
//      theTime = theTime + timeResolution;
//    }

  }
  
  
