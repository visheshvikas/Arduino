//This program is designed to learn the state transition rewards 
//author - Vishesh Vikas
//behavior/states = 0 or 1 (binary)
//robot state transition = N States
// State 1  -> State 2 -> State 3 -> State 4 ...
#include <MsTimer2.h>

// Specifications of a robot
const int noofLimbs = 3;
int limbPins[noofLimbs] = {5,10,9};
int incomingByte;
int endStateInt, startStateInt;
int startState[noofLimbs], endState[noofLimbs];
int matSize = pow(2,noofLimbs);
int timerFreq = 10; //ms
int stateTimer;
//int activeT[noofLimbs] = {
//  2100,1200,2020};
int activeT[noofLimbs] = {
  1400,1000,2020};
int phiT[noofLimbs] = {
  610,1000,00};
int maintainStrength[noofLimbs] = {
  50, 100, 40};
int adjustT[noofLimbs] ={
  120,60,200};
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

int totalNoofStates, theStatesInt[10], theStatesBin[10][noofLimbs];
int theCurrentStateCounter;
int* currentState;

void initializeCounters()
{
  stateTimer = 0;
  noofReps = 10;
  repCounter=0;
  restTime = 4000;
  restCounter = 0;
  theCurrentStateCounter = 0;
}


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
  initializeCounters(); 
  int isComplete = 0;
  while(isComplete==0)
  {
    Serial.flush();
    Serial.print("The total number of states = ");
// totalNoofStates, theStatesInt[noofLimbs], theStatesBin[noofLimbs][noofLimbs];
    while(!Serial.available());
    totalNoofStates = Serial.read()-48;
    Serial.println(totalNoofStates);
    for(int ii=0;ii<totalNoofStates; ii++)
    {
      Serial.print("State "); Serial.print(ii+1); Serial.print(" : = ");
      while(!Serial.available());
      theStatesInt[ii] = Serial.read()-48;
      dec2bin(theStatesInt[ii], theStatesBin[ii]);  
      Serial.print(theStatesInt[ii]);
      Serial.print(" "); printState(&theStatesBin[ii][0]);
      Serial.println("");
    }
    Serial.println("");
    for(int ii=0;ii<totalNoofStates;ii++)
    {
      printState(&theStatesBin[ii][0]); 
      if(ii<(totalNoofStates-1))
      {
        Serial.print(" to ");
      }
    }
    Serial.println("");
    Serial.print("Does that transition look good? (1 if Yes)");
    while(!Serial.available());
    isComplete = Serial.read()-48;
    if(isComplete==1) {Serial.println(" Yes");}
    else {Serial.println(" No");}
  }

  currentState = theStatesBin[0];

  MsTimer2::start();

  
}

void loop()
{
  // Nothing is in the loop function.
  // Everything is done using timer function.
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
//        Serial.print(ii); Serial.print("="); Serial.print(stateTimer);
      }
      else if(ii>0 &(currentState[ii]==theStatesBin[theCurrentStateCounter-11][ii]))
      {
        //        Serial.println("Maintain");
        maintainPin(limbPins[ii], maintainStrength[ii]);
      }
    }
  }
  stateTimer =stateTimer+timerFreq;  
  if(stateTimer==cycleT)
  {
//    Serial.print("stateTimer = "); Serial.println(stateTimer);
    // If cycleT is over
    if(currentState==theStatesBin[totalNoofStates])
    {
      Serial.println("Alls is done");
      turnAllOff();
      stateTimer = 0;
      MsTimer2::stop();
    }
    else //if(currentState==theStatesBin[theCurrentStateCounter])
    {
      stateTimer=0;
//      Serial.println("Switching!");
      for(int jj=0;jj<noofLimbs; jj++)
      {
//      Serial.println(theCurrentStateCounter);
//        Serial.print(theCurrentStateCounter); Serial.print(" ");Serial.println(jj);
//        Serial.print(currentState[jj]); Serial.print("vs"); Serial.println(theStatesBin[theCurrentStateCounter+1][jj]);
        if((currentState[jj]==theStatesBin[theCurrentStateCounter+1][jj])&(currentState[jj]==1))
        {
          Serial.print("Adjusted Leg "); Serial.println(jj);
          phiT[jj] = phiT[jj] + adjustT[jj];
        }
      }
      Serial.print("Going from "); printState(currentState);

      currentState = theStatesBin[theCurrentStateCounter+1];
      theCurrentStateCounter = theCurrentStateCounter+1;
      Serial.print(" to "); printState(currentState); Serial.println("");
      turnAllOff();
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
      for(int ii=0; ii<noofLimbs; ii++)
      {
//        activeT[ii] = activeT[ii] - adjustT[ii];
        phiT[ii] = phiT[ii] + adjustT[ii];
      }
      MsTimer2::set(timerFreq, timerFunc);
      MsTimer2::start();
    }
    restCounter=0;
//    initializeCounters();
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



