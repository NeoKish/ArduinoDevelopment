//#include <Plotter.h>


#include <math.h>

const int numReadings =10;
#define FILTER_SHIFT 4
#define RPM_SHIFT 6
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
int readings[numReadings];      // the readings from the analog input
int values[numReadings];  
int readIndex = 0;     
long valueIndex = 0;    // the index of the current reading
int total = 0;                  // the running total
int valueTotal=0;
int average = 0;   
double timeAverage=0;
int oldAverage=0;
int newAverage=0;
int maxValue=0;
int minValue=0;
double timeMax=0;
double timeSecondMax=0;
double timeSecondMin=0;
double timeMin=0;
double timeStart=0;
double timeStartMax=0;
double timeEndMax=0;
double timeStartMin=0;
double timeEndMin=0;
double timePeriod=0;
long timeMaxinterval=0;
long timeMininterval=0;
long SampletimeStart=0;
int inputPin = A0;
boolean firstMaxflag=false;
boolean secondMaxflag=false;
boolean firstMinflag=false;
boolean secondMinflag=false;
boolean diffflag=false;
float RPM=0;
float RPS=0;
int RPMfiltered=0;
int sensoropfiltered=0;

float RPStotal=0;
float RPSaverage=0;
int currentValue=0;
int newValue=0;
int firstMax=0;
int secondMaxAverage=0;
int firstMin=0;
int secondMin=0;
int devMax=0;
int devMin=0;
int rpmCount=0;
int maxAverage=0;
int minAverage=0;
int count=0;
int counter=0;
int filter_input;
int filter_output;
long filter_reg;
int rpm_input;
int rpm_output;
long rpm_reg;
int diff=0;
//Plotter p; // create plotter
//

float A = 0;
float B = 0.108;
float var1 = 0;
boolean toggle = true; // true = Enabling Serial Plotter Output
long timeSecs;
//

void setup() {
  // initialize serial communication with computer:
  sbi(ADCSRA, ADPS2);
  cbi(ADCSRA, ADPS1);
  cbi(ADCSRA, ADPS0);
  Serial.begin(2000000);
  // initialize all the readings to 0:
  pinMode(inputPin,INPUT);
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
     values[thisReading]=0;
//Serial.print("timestamp ");
//Serial.print(",");
//Serial.print("Average ");
//Serial.print(",");
//Serial.print("Max ");
//Serial.print(",");
//Serial.print("Min");
//Serial.println();
     
  }
//  p.Begin(); // start plotter
//  
//  p.AddTimeGraph( "RPM vs Time elapsed", 1500,"RPM", RPMfiltered ); // add any graphs you want


  
}


void loop() {

  
//  minMax();
///new addition
  // subtract the last reading:
//int analoginput=analogRead(inputPin);
//int average=analogRead(inputPin);

//
  filter_input = analogRead(inputPin);
//  Serial.println(filter_input);
  filter_reg = filter_reg - (filter_reg >> FILTER_SHIFT) + filter_input;
  sensoropfiltered = filter_reg >> FILTER_SHIFT;
  total = total - readings[readIndex];
  // read from the sensor:
  readings[readIndex] =sensoropfiltered;
  // add the reading to the total:
  total = total + readings[readIndex];
  // advance to the next position in the array:
  readIndex = readIndex + 1;

  // if we're at the end of the array...
  if (readIndex >= numReadings) {
    // ...wrap around to the beginning:
    readIndex = 0;
  }
  // calculate the average:
  average = total / numReadings; 
//  Serial.println(average );

  timeAverage=micros();
//    Serial.print("A ");


//      counter++;
//    Serial.print("C ");
//Serial.println(counter);
if (maxAverage==0){
  maxAverage=average;
//  timeStart=micros();
}
else if((average>maxAverage)&&(!firstMaxflag)){
    maxAverage=average;
    timeMax=timeAverage;
    }
else if((average<maxAverage)&&(minAverage==0)){
    firstMaxflag=true;
    timeStartMax=timeMax;
    minAverage=average;
//    Serial.print("Max: ");
//    Serial.println(maxAverage );
 
//    Serial.println("MaxTime:");
//    Serial.println(timeStartMax);
  }
else if((average<minAverage)&&(firstMaxflag)&&(!firstMinflag)){ 
     minAverage=average;
     timeMin=timeAverage;
    }
else if ((average>minAverage)&&(firstMaxflag)&&(!secondMaxflag )){
    secondMaxflag=true;
    timeStartMin=timeMin;
    firstMinflag=true;
//    Serial.print("Min: ");
//    Serial.println(minAverage );
  



//    Serial.println("MinTime:");
//    Serial.println(timeStartMin);
  }
//  if(diff>5){
//    diffflag=true;

//    }
//
//    Serial.print(timeAverage);
//    Serial.print(",");
//    Serial.print(",");
//    
//    Serial.print(",");
//    
//    Serial.println();

  if(firstMaxflag&&firstMinflag&&secondMaxflag){
    timePeriod=timeStartMin-timeStartMax;
    diff=maxAverage-minAverage;
    firstMaxflag=false;
    firstMinflag=false;
    secondMaxflag=false;
    diffflag=false;
    maxAverage=0;
    minAverage=0;
    timeStartMax=0;
    timeStartMin=0;
    timeSecondMax=0;

    if(diff>5){
//            Serial.print("diff: ");
//      Serial.println(diff);
    if (timePeriod<0){timePeriod=-timePeriod;}
    RPS=(1000000/(4*timePeriod));
    RPM=RPS*60;
//    Serial.println(RPM);
    rpm_input = RPM;
    rpm_reg = rpm_reg - (rpm_reg >> RPM_SHIFT) + rpm_input;
    RPMfiltered = rpm_reg >> RPM_SHIFT;

//    
//    Serial.println(timePeriod); 
//    Serial.println("") ; 
    Serial.println(RPMfiltered);
//     timeSecs=millis();
    }
//         
//          p.Plot();
         
}


    
}

int minMax(int value){
   total = total - readings[readIndex];
  // read from the sensor:
  readings[readIndex] = value;
  // add the reading to the total:
  total = total + readings[readIndex];
  // advance to the next position in the array:
  readIndex = readIndex + 1;

  // if we're at the end of the array...
  if (readIndex >= numReadings) {
    // ...wrap around to the beginning:
    readIndex = 0;
  }
  // calculate the average:
  average = total / numReadings; 
 return(average);
}


//
//void minMax()
//{
//   // subtract the last reading:
////
////  total = total - readings[readIndex];
//////   read from the sensor:
//
//  while(readIndex<numReadings){
////       sineWave(); 
//  readings[readIndex] = analogRead(inputPin);
//   
////    readings[readIndex] = var1;
////         Serial.println(var1); 
//  // add the reading to the total:
////  if (readIndex==0){
////    maxValue=readings[readIndex];
////    minValue=readings[readIndex];
////    }
////  else{
////    maxValue=max(maxValue,readings[readIndex]);
////    minValue=min(minValue,readings[readIndex]);
////    }  
// 
//  total = total + readings[readIndex];
//  // advance to the next position in the array:
//  readIndex = readIndex + 1;
//  }
//    // ...wrap around to the beginning:
//    average = total / numReadings;
////      Serial.println(average); 
//    Serial.print("Average: ");
//    Serial.println(average);
//    timeAverage=micros();
//    Serial.println("timeMicros: ");
//    Serial.println(timeAverage);
//  
////////     delay(0.001);
//    Serial.print("Count ");
//    Serial.println(count);
//    readIndex = 0;
//    total=0;
////  devMax=maxValue-average;
////  devMin=average-minValue;
//
//}
//
//void sineWave(){
//
//      A += B;
////  analogWrite(Output1, var1);
////  if (toggle == true)
////  {
////    Serial.println(var1);
////
////  }
//     if(A>=2*PI){
//      A=0;}
//  var1=255*sin(A);
////  Serial.println(var1); 
//
//  }
