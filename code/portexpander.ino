/*  Port RX expander for RLC-Club Deluxe Controller
 *   Chrissy Hart, KC8UFV
 *   
 *   DIP switch functions: (ON)
 *   1- LOGIC LOW COR on Link port
 *   2- LOGIC LOW PL on Link Port
 *   3- PL NOT REQUIRED on Link Port 
 *   4- LOGIC LOW COR on Rpt port
 *   5- LOGIC LOW COR on Rpt port
 *   6- PL NOT REQUIRED on Rpt Port to override Link
 *   7- LOGIC LOW COR OUT
 *   8- LOGIC LOW PL OUT
 * 
 * 
 *   Atmega should be programmed to run on internal clock, as there are no high speed, timing critical signals in this project. 
 *   A constant loop is used. Pins are constantly checked for changes, so no need to reset the board for new logic states.
 *   
 */

// First define all the pin functions

#define RCORLED A2
#define RPLLED  A3
#define LCORLED 12
#define LPLLED  13
#define PLOUTPIN    A4
#define COROUTPIN   A5
#define AUDIOSW  1


#define LINKENA  0
#define LINKPL   11
#define LINKCOR  10
#define RPTCOR   A0
#define RPTPL    A1


#define LCORDIP 2
#define LPLDIP  3
#define LPLREQ  4
#define RCORDIP 5
#define RPLDIP  6
#define RPLREQ  7
#define CORDIP   8
#define PLDIP    9

// Define variables to determine proper outputs to be set after read logic tree. These will be written ONCE per loop.

bool COROUT = false;
bool PLOUT = false;
bool SOURCE = false;


void setup() {
  // Set pinmodes for all relevant IO pins:
  pinMode(RCORLED, OUTPUT);
  pinMode(RPLLED, OUTPUT);
  pinMode(LCORLED, OUTPUT);
  pinMode(LPLLED, OUTPUT);
  pinMode(PLOUTPIN, OUTPUT);
  pinMode(COROUTPIN, OUTPUT);
  pinMode(AUDIOSW, OUTPUT);
  
  pinMode(LCORDIP, INPUT_PULLUP);
  pinMode(LPLDIP, INPUT_PULLUP);
  pinMode(LPLREQ, INPUT_PULLUP);
  pinMode(RCORDIP, INPUT_PULLUP);
  pinMode(RPLDIP, INPUT_PULLUP);
  pinMode(RPLREQ, INPUT_PULLUP);
  pinMode(CORDIP, INPUT_PULLUP);
  pinMode(PLDIP, INPUT_PULLUP);

  pinMode(LINKENA, INPUT_PULLUP);
  pinMode(LINKPL, INPUT);
  pinMode(LINKCOR, INPUT);
  pinMode(RPTCOR, INPUT);
  pinMode(RPTPL, INPUT);

 

}

void loop() {         
  // put your main code here, to run repeatedly:   

  // First clear the variables to determine appropriate outputs to the main controller

  COROUT = false;
  PLOUT = false;
  SOURCE = false; // False is the default state for the audio switch for it to pass audio from the RPT port
  
if(digitalRead(LINKENA) == LOW){     // check if link is enabled, 
  if(digitalRead(LINKCOR) == digitalRead(LCORDIP)){  //If enabled, check the link port
    digitalWrite(LCORLED, HIGH);
    if(digitalRead(LPLREQ) == LOW){
      COROUT = true;
      PLOUT = true;
      SOURCE = true;
    } else if(digitalRead(LINKPL) == digitalRead(LPLDIP)){
      digitalWrite(LPLLED, HIGH);
      COROUT = true;
      PLOUT = true;
      SOURCE = true;
    } else {
      digitalWrite(LCORLED, LOW);
    }
  } else {
    digitalWrite(LCORLED, LOW);
    if(digitalRead(LINKPL) == digitalRead(LPLDIP)){
      digitalWrite(LPLLED, HIGH);
    } else {
      digitalWrite(LPLLED, LOW);
    }
  }

    if(digitalRead(RPTCOR) == digitalRead(RCORDIP)){  // Now check the Repeater port, it overrides the link port if it hears something with both PL and COR. Sets COR out to true if it hears COR at all
    digitalWrite(RCORLED, HIGH);
    COROUT = true;
    if(digitalRead(RPLREQ) == LOW){
      PLOUT = false;
      SOURCE = false;
    } else if(digitalRead(RPTPL) == digitalRead(RPLDIP)){
      digitalWrite(RPLLED, HIGH);
      PLOUT = true;
      SOURCE = false;
    } else {
      digitalWrite(RCORLED, LOW);
    }
  } else {
    digitalWrite(RCORLED, LOW);
    if(digitalRead(RPTPL) == digitalRead(RPLDIP)){
      digitalWrite(RPLLED, HIGH);
    } else {
      digitalWrite(RPLLED, LOW);
    }
  }

} else {    // Link is disabled. Logic greatly simplified, link port pins still monitored to give LED status. 
  if(digitalRead(RPTCOR) == digitalRead(RCORDIP)){ 
    digitalWrite(RCORLED, HIGH);
    COROUT = true;
  } else { 
    digitalWrite(RCORLED, LOW);
  }
  if(digitalRead(RPTPL) == digitalRead(RPLDIP)){ 
    digitalWrite(RPLLED, HIGH);
    PLOUT = true;
  } else { 
    digitalWrite(RPLLED, LOW);
  }  

    if(digitalRead(LINKCOR) == digitalRead(LCORDIP)){ 
    digitalWrite(LCORLED, HIGH);
  } else { 
    digitalWrite(LCORLED, LOW);
  }
  if(digitalRead(LINKPL) == digitalRead(LPLDIP)){ 
    digitalWrite(LPLLED, HIGH);
  } else { 
    digitalWrite(LPLLED, LOW);
  }  
}

// Now that the inputs have been read, and the variables set, send the outputs to the controller

digitalWrite(AUDIOSW, SOURCE);
if(COROUT){
  digitalWrite(COROUTPIN, digitalRead(CORDIP)); 
} else {
  digitalWrite(COROUTPIN, !digitalRead(CORDIP));
}
if(PLOUT){
  digitalWrite(PLOUTPIN, digitalRead(PLDIP)); 
} else {
  digitalWrite(PLOUTPIN, !digitalRead(PLDIP));
}


}
