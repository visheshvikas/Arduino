#include <MsTimer2.h>
#include <Array.h>

const int noofLimbs = 3;
int limbPins[noofLimbs] = {5,10,9};
int incomingByte;
int endStateInt, startStateInt;
int startState[noofLimbs], endState[noofLimbs];
int matSize = pow(2,noofLimbs);
int timerFreq = 10; //ms
int stateTimer;
int* currentState;
int activeT[noofLimbs] = {2100,1000,2020};
int phiT[noofLimbs] = {0,1000,0};
int maintainStrength[noofLimbs] = {50, 40, 130};
int adjustT[noofLimbs] ={50,50,50};
int cycleT, restTime;

int noofReps, repCounter;

void turnAllOff();

void dec2bin(int dec, int (&bin)[noofLimbs]);
void printState(int (&bin)[noofLimbs]);
void printState(int*);
void implementState(int (&bin)[noofLimbs]);

void turnPinOn(int);
void turnPinOff(int);
void maintainPin(int thepin, int strength);

void timerFunc();
void restFunc();
int restCounter;

void setup()
{
  Serial.begin(115200);
  cycleT = 0;
  for(int ii=0; ii<noofLimbs; ii++)
  {
    pinMode(limbPins[ii], OUTPUT);
    turnPinOff(limbPins[ii]);
    if(cycleT<(activeT[ii]+phiT[ii]))
    {
      cycleT=(activeT[ii]+phiT[ii]);
    }
  }
  Serial.println("I've set everything up! Ready to roll");
  Serial.print("The number of nodes is ");
  Serial.println(matSize);
  MsTimer2::set(timerFreq, timerFunc);
  stateTimer = 0;
  noofReps = 5;
  repCounter=0;
  restTime = 4000;
  restCounter = 0;
}

void loop()
{
//  while(!Serial.available());
  Serial.flush();
  Serial.println("Starting State (integer) = ");
  while(!Serial.available());
  startStateInt = Serial.read() - 48;
  Serial.println("End State (integer) =");   
  while(!Serial.available());
  endStateInt = Serial.read() -48;
  Serial.print("So, we're moving from ");
  dec2bin(startStateInt, startState);
  dec2bin(endStateInt, endState);
  printState(&startState[0]); Serial.print(" to "); printState(&endState[0]);
  currentState = startState;
  MsTimer2::start();
  Serial.println("");
}

void timerFunc()
{
// use activeT, phiT  
  for(int ii=0; ii<noofLimbs; ii++)
  {    
    if(currentState[ii]==1)
    {
      if(stateTimer==phiT[ii])
      {
        turnPinOn(limbPins[ii]);
      }
      else if((currentState==endState)&(endState[ii]==startState[ii]))
      {
//        Serial.println("Maintain");
        maintainPin(limbPins[ii], maintainStrength[ii]);
      }
    }
  }
  stateTimer =stateTimer+timerFreq;  
  if(stateTimer==cycleT)
  {
    if(currentState==endState)
    {
      turnAllOff();
      repCounter = repCounter+1;
      currentState = startState;
      stateTimer = 0;
      for(int ii=0; ii<noofLimbs; ii++)
      {
        activeT[ii] = activeT[ii] - adjustT[ii];
      }
      Serial.println(repCounter);
      MsTimer2::stop();
      MsTimer2::set(restTime,restFunc);
      MsTimer2::start();
    }
    else if(currentState==startState)
    {
      stateTimer=0;
      currentState = endState;
      turnAllOff();
      Serial.println("Switching!");
    }
  }
  
}

void restFunc()
{
  if(restCounter==1)
  {
    Serial.println("All rested");
    MsTimer2::stop();
    if(repCounter<noofReps)
    {
      MsTimer2::set(timerFreq, timerFunc);
      MsTimer2::start();
    }
    restCounter=0;
  }
  restCounter=restCounter+1;
}

void implementState(int (&bin)[noofLimbs])
{
  for(int ii=0; ii<noofLimbs; ii++)
  {    
    if(bin[ii]==0)
    {
      turnPinOff(limbPins[ii]);
    }    
    else
    {
      turnPinOn(limbPins[ii]);
    }
  }  
}
void printState(int* bin)
{
  for(int ii=0; ii<noofLimbs; ii++)
  {    
    Serial.print(bin[ii]);    
  }
//  Serial.println("");
}

void printState(int (&bin)[noofLimbs])
{
  for(int ii=0; ii<noofLimbs; ii++)
  {    
    Serial.print(bin[ii]);    
  }
//  Serial.println("");
}

void dec2bin(int dec, int (&bin)[noofLimbs])
{
  for(int ii=0; ii<noofLimbs; ii++)
  {    
//    Serial.println((1<<ii));
    bin[noofLimbs-ii-1] = (dec&(1<<ii))>>ii;    
  }
}


void turnAllOff()
{
  for(int ii=0; ii<noofLimbs; ii++)
  {
//    digitalWrite(limbPins[ii],HIGH);    
    turnPinOff(limbPins[ii]);
  }
}
void turnPinOn(int thepin)
{
//  digitalWrite(thepin, LOW);
  analogWrite(thepin, 0);
}

void maintainPin(int thepin, int strength)
{
  analogWrite(thepin,255-strength);
}

void turnPinOff(int thepin)
{
    analogWrite(thepin,255);      
//  digitalWrite(thepin, HIGH);
}


