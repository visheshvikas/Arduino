int theAPins[4][4] = {
  {A0, A1, A2, A3}
  , {
    A4, A5, A6, A7    }
  , {
    A8, A9, A10, A11    }
  , {
    A12, A13, A14, A15    }
};



void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200); //Set the communication baud rate - bits/sec
  
}

void loop() {
  // put your main code here, to run repeatedly:

  //Read Voltage
  for(int ii=0; ii<1; ii++){
    //Create variables to hold the output rates.
    Serial.print(" Vs = (");
    for(int kk=0;kk<3;kk++){
      Serial.print(analogRead(theAPins[ii+1][kk]));
      Serial.print(",");
    }
    Serial.println(") ");
  }
  delay(100);
}
