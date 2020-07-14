/*Written by Kishan Savant(Fruition Tech Labs)*/

#define LaserPin 7 // pin 2 for  sensor
#define Led 2 // pin 8 for action to do someting
#define RPM_SHIFT 4
float RPM;
float RPS;
byte val;
bool zeroFlag=false;
bool firstFlag=false;
bool secondFlag=false;
bool thirdFlag=true;
double timeStart;
double timeEnd;
double timePeriod;
int rpm_input;
int rpm_output;
long rpm_reg;
int RPMfiltered=0;

void setup() {
  
  Serial.begin(9600);
  pinMode(LaserPin, INPUT);//define detect input pin
  pinMode(Led, OUTPUT);//define ACTION output pin

}

void loop() {



  val=digitalRead(LaserPin);

  //Calculating RPM
  
  if(val==1 && !zeroFlag ){

     timeStart=millis();
     firstFlag=true;
  }
  else if (val==0 && firstFlag){
     zeroFlag=true;

  }  
  else if (val==1 && zeroFlag){
    timeEnd=millis();

    secondFlag=true;
    firstFlag=false;

  }
  else if (val==0 && zeroFlag && secondFlag ){
    secondFlag=false;
    thirdFlag=true;
    zeroFlag=false;
  }

  if(thirdFlag){
    
    thirdFlag=false;
    timePeriod=timeEnd-timeStart;
    timeEnd=0;
    timeStart=0;
    
    if (timePeriod<0){timePeriod=-timePeriod;}
    RPS=(1000/timePeriod);
    RPM=RPS*60;
    rpm_input = RPM;
    rpm_reg = rpm_reg - (rpm_reg >> RPM_SHIFT) + rpm_input;
    RPMfiltered = rpm_reg >> RPM_SHIFT;

    Serial.println(RPMfiltered);  
  }
  
 
 
}
