//This program is designed to learn the state transition rewards 
//author - Vishesh Vikas
//behavior/states = 0 or 1 (binary)
//robot state transition = N States
// State 1  -> State 2 -> State 3 -> State 4 ...
#include <MsTimer2.h>

// ====
// Specifications of a robot
const int noofLimbs = 3;
int limbPins[noofLimbs] = {5,10,9};
int matSize = pow(2,noofLimbs);

// ====
// Timer stats
int timerFreq = 10; //ms
int timerCounter;
int stateTimer;    // I don't know what this does


// ====
// Now the limb specifications
int adjustT[noofLimbs] ={
  120,60,200};

int activeStrength[noofLimbs] = {
  200, 150, 210};
int maintainStrength[noofLimbs] = {
  100, 60, 80};
int cycleT = 3000, cycleCounter = 1;
int restTime;

int noofReps=3, repCounter;

void turnAllOff();

void dec2bin(int dec, int (&bin)[noofLimbs]);
void printState(int (&bin)[noofLimbs]);
void printState(int*);
//void implementState(int (&bin)[noofLimbs]);
void implementState(int (&currentStateBin)[noofLimbs], int (&nextStateBin)[noofLimbs]);

void turnPinOn(int);
void turnPinOff(int);
void maintainPin(int thepin, int strength);

void timerFunc();
//void restFunc();
//int restCounter;

int totalNoofStates, theStatesInt[30], theStatesBin[30][noofLimbs];
int theCurrentStateCounter;
int* currentState;



void setup()
{
  Serial.begin(115200);
  for(int ii=0; ii<noofLimbs; ii++)
  {
    pinMode(limbPins[ii], OUTPUT);
    turnPinOff(limbPins[ii]);

  }
  Serial.println("I've set everything up! Ready to roll");
  Serial.print("The number of nodes is ");
  Serial.println(matSize);
  MsTimer2::set(timerFreq, timerFunc);
  // Input stuff
  int isComplete = 0;
  while(isComplete==0)
  {
    Serial.flush();
    Serial.print("The total number of states = ");
    while(!Serial.available());
    totalNoofStates = Serial.read()-48;
    Serial.println(totalNoofStates);
    theStatesInt[0] = 0;
    dec2bin(theStatesInt[0], theStatesBin[0]);
    for(int ii=1;ii<=totalNoofStates; ii++)
    {
      Serial.print("State "); Serial.print(ii); Serial.print(" : = ");
      while(!Serial.available());
      theStatesInt[ii] = Serial.read()-48;
      dec2bin(theStatesInt[ii], theStatesBin[ii]);  
      Serial.print(theStatesInt[ii]);
      Serial.print(" "); printState(&theStatesBin[ii][0]);
      Serial.println("");
    }
    Serial.println("");
    for(int ii=0;ii<=totalNoofStates;ii++)
    {
      printState(&theStatesBin[ii][0]); 
      if(ii<(totalNoofStates))
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
  totalNoofStates = totalNoofStates+1;
  theStatesInt[totalNoofStates] = 0;
  dec2bin(theStatesInt[totalNoofStates], theStatesBin[totalNoofStates]);
//  currentState = theStatesBin[1];

  MsTimer2::start();
  repCounter = 0;

  
}

void loop()
{
  // Nothing is in the loop function.
  // Everything is done using timer function.
}


void timerFunc()
{
  if(timerCounter==cycleCounter*cycleT)
  {
    implementState(theStatesBin[cycleCounter], theStatesBin[cycleCounter-1]);
    if(cycleCounter==totalNoofStates)
    {
      MsTimer2::stop();
      Serial.println("All done");
//      timerCounter=-timerFreq;
//      cycleCounter=0;
    }
    cycleCounter = cycleCounter+1;
  }

  timerCounter = timerCounter + timerFreq;
  
}

void implementState(int (&currentStateBin)[noofLimbs], int (&lastStateBin)[noofLimbs])
{
  printState(&currentStateBin[0]); 
  Serial.print(" with Strengths = (");
  for(int ii=0; ii<noofLimbs; ii++)
  {    
    if(currentStateBin[ii]==0)
    {
      turnPinOff(limbPins[ii]);
      Serial.print(" = 0 ");
    }    
    else
    {
      if(currentStateBin[ii] == lastStateBin[ii])
      {
        // Maintain Pin
        maintainPin(limbPins[ii], maintainStrength[ii]);
        Serial.print(ii); 
        Serial.print(" = ");
        Serial.print(maintainStrength[ii]);
        Serial.print(" ");
      }
      else
      {
        // Turn on full speed!
        Serial.print(ii); 
        Serial.print(" = ");
        Serial.print(activeStrength[ii]);
        Serial.print(" ");
        maintainPin(limbPins[ii], activeStrength[ii]);
      }
    } 
  }
  Serial.println(")");  
}

//void restFunc()
//{
//  if(restCounter==1)
//  {
//    Serial.println("All rested");
//    MsTimer2::stop();
//    if(repCounter<noofReps)
//    {
//      for(int ii=0; ii<noofLimbs; ii++)
//      {
////        activeT[ii] = activeT[ii] - adjustT[ii];
//        phiT[ii] = phiT[ii] + adjustT[ii];
//      }
//      MsTimer2::set(timerFreq, timerFunc);
//      MsTimer2::start();
//    }
//    restCounter=0;
////    initializeCounters();
//  }
//  restCounter=restCounter+1;
//}
//
//
//void implementState(int (&bin)[noofLimbs])
//{
//  for(int ii=0; ii<noofLimbs; ii++)
//  {    
//    if(bin[ii]==0)
//    {
//      turnPinOff(limbPins[ii]);
//    }    
//    else
//    {
//      turnPinOn(limbPins[ii]);
//    }
//  }  
//}


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



