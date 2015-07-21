#include <Esplora.h>
#include <FlexiTimer2.h>

//   Motor parameters
//  int T = 2000; // ms
  int T, T1_active, T2_active, phase12;
  

//   For the Board Inputs
  // 4 Buttons 
  int leftButton, rightButton, upButton, downButton;
  int lastLeft, lastRight, lastUp, lastDown;
  // Slider
  int sliderVal, lastSlider;
  
  void setup(){
    Serial.begin(9600);
    
//    Initializing Buttons
    lastLeft=HIGH;
    lastRight=HIGH;
    lastUp=HIGH;
    lastDown=HIGH;
//    Initializing Slider
    lastSlider = 0;
    
//    Initializing Motor parameters - all in milliseconds (ms)
    T = 2000;
    T1_active = 500;
    T2_active = 600;
    phase12   = 200;
    
//    Defining a timer
    FlexiTimer2::set(1000, timerFunc);
    FlexiTimer2::start();
  }
  
  void loop()
  {    
    leftButton = Esplora.readButton(SWITCH_LEFT);
    rightButton = Esplora.readButton(SWITCH_RIGHT);
    upButton = Esplora.readButton(SWITCH_UP);
    downButton = Esplora.readButton(SWITCH_DOWN);
    sliderVal = Esplora.readSlider();
    if(leftButton==LOW && leftButton!=lastLeft)
    {
//      The Code
      Serial.println("LEFT DOWN");
    }
    if(rightButton==LOW && rightButton!=lastRight)
    {
      //The Code
      Serial.println("RIGHT DOWN");
    }
    if(upButton==LOW && upButton!=lastUp)
    {
      //The Code
      Serial.println("UP DOWN");
    }
    if(downButton==LOW && downButton!=lastDown)
    {
      //The Code
      Serial.println("DOWN DOWN");
    }
//    if(abs(sliderVal-lastSlider)>1)
//    {
//      //The Code
//      Serial.print("Slider Value is ");
//      Serial.println(sliderVal);
//    }
      lastLeft = leftButton;
      lastRight = rightButton;
      lastUp = upButton;
      lastDown=downButton;
      lastSlider=sliderVal;
  }
  
  
  void timerFunc()
  {
    Serial.print("Slider is "); Serial.println(sliderVal);
  }
