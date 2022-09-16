#include "Nextion.h"                   // Incudes Nextion Library
#include <PulseSensorPlayground.h>     // Includes the PulseSensorPlayground Library.   

#define USE_ARDUINO_INTERRUPTS true    // Set-up low-level interrupts for most acurate BPM math.
#define outputA 4
#define outputB 5


int counter = 0;
int rotations = 0;
int aState;
int aLastState;

const int PulseWire = 0;       // PulseSensor PURPLE WIRE connected to ANALOG PIN 0
int Threshold = 550;           // Determine which Signal to "count as a beat" and which to ignore.
int myBPM = 0;

PulseSensorPlayground pulseSensor;  // Creates an instance of the PulseSensorPlayground object called "pulseSensor"


//------------------------ Age ------------------------------
NexText textNumber = NexText(1, 5, "textNumber");
NexButton buttonPlus = NexButton(1, 3, "buttonPlus");
NexButton buttonMinus = NexButton(1, 4, "buttonMinus");
//-----------------------------------------------------------

//---------------------- Weight -----------------------------
NexText textWeight = NexText(1, 8, "textWeight");
NexButton buttonPlus1 = NexButton(1, 6, "buttonPlus1");
NexButton buttonMinus1 = NexButton(1, 7, "buttonMinus1");
//-----------------------------------------------------------

//--------------------------Sex------------------------------
NexText textSex = NexText(1, 13, "textSex");
NexButton buttonMale = NexButton(1, 11, "buttonMale");
NexButton buttonFemale = NexButton(1, 12, "buttonFemale");
//-----------------------------------------------------------

//----------------------- Quick Start -----------------------
NexText textQHR = NexText(3, 5, "textQHR");
NexButton buttonPlusQHR = NexButton(3, 7, "buttonPlusQHR");
NexButton buttonMinusQHR = NexButton(3, 8, "buttonMinusQHR");

NexText textQT = NexText(3, 10, "textQT");
NexButton buttonPlusQT = NexButton(3, 12, "buttonPlusQT");
NexButton buttonMinusQT = NexButton(3, 13, "buttonMinusQT");

NexText textQRV = NexText(3, 15, "textQRV");
NexButton buttonPlusQRV = NexButton(3, 16, "buttonPlusQRV");
NexButton buttonMinusQRV = NexButton(3, 17, "buttonMinusQRV");
//------------------------------------------------------------

//------------------------ Warm Up ---------------------------
NexButton buttonWarmUp3 = NexButton(4, 7, "buttonWarmUp3");
NexButton buttonWarmUp5 = NexButton(4, 8, "buttonWarmUp5");
NexButton buttonWarmUpNT = NexButton(4, 9, "buttonWarmUp3");
//------------------------------------------------------------

//------------------------ Fat Burn --------------------------
NexText textFWT = NexText(5, 7, "textFWT");
NexButton buttonPlusFWT = NexButton(5, 9, "buttonPlusFWT");
NexButton buttonMinusFWT = NexButton(5, 10, "buttonMinusFWT");

NexText textFZT = NexText(5, 14, "textFZT");
NexButton buttonPlusFZT = NexButton(5, 16, "buttonPlusFZT");
NexButton buttonMinusFZT = NexButton(5, 17, "buttonMinusFZT");
//------------------------------------------------------------

//------------------------ Heart Health ----------------------
NexText textHWT = NexText(6, 8, "textHWT");
NexButton buttonPlusHWT = NexButton(6, 10, "buttonPlusHWT");
NexButton buttonMinusHWT = NexButton(6, 11, "buttonMinusHWT");

NexText textHZT = NexText(6, 15, "textHZT");
NexButton buttonPlusHZT = NexButton(6, 17, "buttonPlusHZT");
NexButton buttonMinusHZT = NexButton(6, 18, "buttonMinusHZT");
//------------------------------------------------------------

//------------------------ Performance -----------------------
NexText textPWT = NexText(7, 7, "textPWT");
NexButton buttonPlusPWT = NexButton(7, 9, "buttonPlusPWT");
NexButton buttonMinusPWT = NexButton(7, 10, "buttonMinusPWT");

NexText textPZT = NexText(7, 14, "textPZT");
NexButton buttonPlusPZT = NexButton(7, 16, "buttonPlusPZT");
NexButton buttonMinusPZT = NexButton(7, 17, "buttonMinusPZT");
//------------------------------------------------------------

//---------------------- Quick Start Time --------------------
NexText textTimeQS = NexText(8, 2, "textTimeQS");
NexText textTimeQSM = NexText(8, 3, "textTimeQSM");
NexButton buttonStartQS = NexButton(3, 18, "buttonStartQS");
NexButton buttonDoneQS = NexButton(8, 5, "buttonDoneQS");
//------------------------------------------------------------

NexText textRevsQS = NexText(8, 7, "textRevsQS");
NexText textPulseQS = NexText(8, 9, "textPulseQS");

int number = 30;
int weight = 120;
int sex = 1;

int quickStartHR = 80;
double quickStartTime = 5.5;
int quickStartRev = 500;

int fatBurnWoTime = 30;
int fatBurnZnTime = 30;

int healthWoTime = 15;
int healthZnTime = 15;

int performanceWoTime = 10;
int performanceZnTime = 10;

int startTime = 0;
int timeQS = 0;
int timeQSM = 0;

char buffer[10] = {0};

NexTouch *nex_listen_list[] =
{
  &textNumber, &buttonPlus, &buttonMinus,
  &textWeight, &buttonPlus1, &buttonMinus1,
  &textSex, &buttonMale, &buttonFemale,
  &textQHR, &buttonPlusQHR, &buttonMinusQHR,
  &textQT, &buttonPlusQT, &buttonMinusQT,
  &textQRV, &buttonPlusQRV, &buttonMinusQRV,
  &textFWT, &buttonPlusFWT, &buttonMinusFWT,
  &textFZT, &buttonPlusFZT, &buttonMinusFZT,
  &textHWT, &buttonPlusHWT, &buttonMinusHWT,
  &textHZT, &buttonPlusHZT, &buttonMinusHZT,
  &textPWT, &buttonPlusPWT, &buttonMinusPWT,
  &textPZT, &buttonPlusPZT, &buttonMinusPZT,
  &textTimeQS, &textTimeQSM, &buttonStartQS, &buttonDoneQS,
  NULL
};

//---------------------Age Buttons---------------------------
void buttonPlusPopCallback(void *ptr)
{
  dbSerialPrintln("buttonPlusPopCallback");

  if (number < 100) number += 1;

  memset(buffer, 0, sizeof(buffer));  // clear buffer
  itoa(number, buffer, 10);
  textNumber.setText(buffer);
}

void buttonMinusPopCallback(void *ptr)
{
  dbSerialPrintln("buttonMinusPopCallback");

  if (number > 0) number -= 1;

  memset(buffer, 0, sizeof(buffer));  // clear buffer
  itoa(number, buffer, 10);
  textNumber.setText(buffer);
}
//------------------------------------------------------------

//--------------------Weight Buttons--------------------------
void buttonPlus1PopCallback(void *ptr)
{
  dbSerialPrintln("buttonPlus1PopCallback");

  if (weight < 300) weight += 1;

  memset(buffer, 0, sizeof(buffer));  // clear buffer
  itoa(weight, buffer, 10);
  textWeight.setText(buffer);
}

void buttonMinus1PopCallback(void *ptr)
{
  dbSerialPrintln("buttonMinus1PopCallback");

  if (weight > 70) weight -= 1;

  memset(buffer, 0, sizeof(buffer));  // clear buffer
  itoa(weight, buffer, 10);
  textWeight.setText(buffer);
}
//------------------------------------------------------------

//------------------------Sex Buttons-------------------------
void buttonMalePopCallback(void *ptr)
{
  dbSerialPrintln("buttonMalePopCallback");

  if (sex < 1) sex += 1;

  memset(buffer, 0, sizeof(buffer));  // clear buffer
  itoa(sex, buffer, 10);
  textSex.setText(buffer);
}

void buttonFemalePopCallback(void *ptr)
{
  dbSerialPrintln("buttonFemalePopCallback");

  if (sex > 0) sex -= 1;

  memset(buffer, 0, sizeof(buffer));  // clear buffer
  itoa(sex, buffer, 10);
  textSex.setText(buffer);
}
//------------------------------------------------------------

//-------------------QuickStart Buttons-----------------------
//------------------------------------------------------------
//----------------------Heart Rate----------------------------
void buttonPlusQHRPopCallback(void *ptr)
{
  dbSerialPrintln("buttonPlusQHRPopCallback");

  if (quickStartHR < 200) quickStartHR += 1;

  memset(buffer, 0, sizeof(buffer));  // clear buffer
  itoa(quickStartHR, buffer, 10);
  textQHR.setText(buffer);
}

void buttonMinusQHRPopCallback(void *ptr)
{
  dbSerialPrintln("buttonMinusQHRPopCallback");

  if (quickStartHR > 60) quickStartHR -= 1;

  memset(buffer, 0, sizeof(buffer));  // clear buffer
  itoa(quickStartHR, buffer, 10);
  textQHR.setText(buffer);
}
//-------------------------------------------------------------
//---------------------------Time------------------------------
void buttonPlusQTPopCallback(void *ptr)
{
  dbSerialPrintln("buttonPlusQTPopCallback");

  if (quickStartTime < 120) quickStartTime += 0.5;

  memset(buffer, 0, sizeof(buffer));  // clear buffer
  itoa(quickStartTime, buffer, 10);
  textQT.setText(buffer);
}

void buttonMinusQTPopCallback(void *ptr)
{
  dbSerialPrintln("buttonMinusQTPopCallback");

  if (quickStartTime > 0) quickStartTime -= 0.5;

  memset(buffer, 0, sizeof(buffer));  // clear buffer
  itoa(quickStartTime, buffer, 10);
  textQT.setText(buffer);
}
//------------------------------------------------------------
//-------------------------Revs-------------------------------
void buttonPlusQRVPopCallback(void *ptr)
{
  dbSerialPrintln("buttonPlusQRVPopCallback");

  if (quickStartRev < 2000) quickStartRev += 10;

  memset(buffer, 0, sizeof(buffer));  // clear buffer
  itoa(quickStartRev, buffer, 10);
  textQRV.setText(buffer);
}

void buttonMinusQRVPopCallback(void *ptr)
{
  dbSerialPrintln("buttonMinusQRVPopCallback");

  if (quickStartRev > 0) quickStartRev -= 10;

  memset(buffer, 0, sizeof(buffer));  // clear buffer
  itoa(quickStartRev, buffer, 10);
  textQRV.setText(buffer);
}
//---------------------- End Quick Start ----------------------
//-------------------------------------------------------------

//----------------------- FatBurn Buttons ---------------------
//-------------------------------------------------------------
//------------------------ Workout Time -----------------------
void buttonPlusFWTPopCallback(void *ptr)
{
  dbSerialPrintln("buttonPlusFWTPopCallback");
  
  if (fatBurnWoTime < 65) fatBurnWoTime += 5;

  memset(buffer, 0, sizeof(buffer));  //clear buffer
  itoa(fatBurnWoTime, buffer, 10);
  textFWT.setText(buffer);
}

void buttonMinusFWTPopCallback(void *ptr)
{
  dbSerialPrintln("buttonMinusFWTPopCallback");
  
  if (fatBurnWoTime > 10) fatBurnWoTime -= 5;

  memset(buffer, 0, sizeof(buffer));  //clear buffer
  itoa(fatBurnWoTime, buffer, 10);
  textFWT.setText(buffer);
}
//------------------------------------------------------------

//----------------------- Zone Time --------------------------
void buttonPlusFZTPopCallback(void *ptr)
{
  dbSerialPrintln("buttonPlusFZTPopCallback");
  
  if (fatBurnZnTime < 100) fatBurnZnTime += 5;

  memset(buffer, 0, sizeof(buffer));  //clear buffer
  itoa(fatBurnZnTime, buffer, 10);
  textFZT.setText(buffer);
}

void buttonMinusFZTPopCallback(void *ptr)
{
  dbSerialPrintln("buttonMinusFZTPopCallback");
  
  if (fatBurnZnTime > 10) fatBurnZnTime -= 5;

  memset(buffer, 0, sizeof(buffer));  //clear buffer
  itoa(fatBurnZnTime, buffer, 10);
  textFZT.setText(buffer);
}
//---------------------- End Fat Burn -------------------------
//-------------------------------------------------------------

//---------------------- Heart Health -------------------------
//-------------------------------------------------------------
//---------------------- Workout Time -------------------------
void buttonPlusHWTPopCallback(void *ptr)
{
  dbSerialPrintln("buttonPlusHWTPopCallback");

  if (healthWoTime < 60) healthWoTime +=5;

  memset(buffer, 0, sizeof(buffer)); //clear buffer
  itoa(healthWoTime, buffer, 10);
  textHWT.setText(buffer);
}

void buttonMinusHWTPopCallback(void *ptr)
{
  dbSerialPrintln("buttonMinusHWTPopCallback");

  if (healthWoTime > 5) healthWoTime -=5;

  memset(buffer, 0, sizeof(buffer)); //clear buffer
  itoa(healthWoTime, buffer, 10);
  textHWT.setText(buffer);
}
//-----------------------------------------------------------

//----------------------- Zone Time -------------------------
void buttonPlusHZTPopCallback(void *ptr)
{
  dbSerialPrintln("buttonPlusHZTPopCallback");

  if (healthZnTime < 60) healthZnTime +=5;

  memset(buffer, 0, sizeof(buffer)); //clear buffer
  itoa(healthZnTime, buffer, 10);
  textHZT.setText(buffer);
}

void buttonMinusHZTPopCallback(void *ptr)
{
  dbSerialPrintln("buttonMinusHZTPopCallback");

  if (healthZnTime > 5) healthZnTime -=5;

  memset(buffer, 0, sizeof(buffer)); //clear buffer
  itoa(healthZnTime, buffer, 10);
  textHZT.setText(buffer);
}
//-----------------------------------------------------------
//-----------------------------------------------------------

//--------------------- Performance -------------------------
//-----------------------------------------------------------
//--------------------- Workout Time ------------------------
void buttonPlusPWTPopCallback(void *ptr)
{
  dbSerialPrintln("buttonPlusPWTPopCallback");

  if (performanceWoTime < 60) performanceWoTime +=5;

  memset(buffer, 0, sizeof(buffer)); //clear buffer
  itoa(performanceWoTime, buffer, 10);
  textPWT.setText(buffer);
}

void buttonMinusPWTPopCallback(void *ptr)
{
  dbSerialPrintln("buttonMinusPWTPopCallback");

  if (performanceWoTime > 5) performanceWoTime -=5;

  memset(buffer, 0, sizeof(buffer)); //clear buffer
  itoa(performanceWoTime, buffer, 10);
  textPWT.setText(buffer);
}
//----------------------------------------------------------

//---------------------- Zone Time -------------------------
void buttonPlusPZTPopCallback(void *ptr)
{
  dbSerialPrintln("buttonPlusPZTPopCallback");

  if (performanceZnTime < 60) performanceZnTime +=5;

  memset(buffer, 0, sizeof(buffer)); //clear buffer
  itoa(performanceZnTime, buffer, 10);
  textPZT.setText(buffer);
}

void buttonMinusPZTPopCallback(void *ptr)
{
  dbSerialPrintln("buttonMinusPZTPopCallback");

  if (performanceZnTime > 5) performanceZnTime -=5;

  memset(buffer, 0, sizeof(buffer)); //clear buffer
  itoa(performanceZnTime, buffer, 10);
  textPZT.setText(buffer);
}
//-------------------- End Performance --------------------
//---------------------------------------------------------

//----------------------Quick Start Time ------------------
void buttonStartQSPopCallback(void *ptr)
{
  dbSerialPrintln("buttonStartQSPopCallback");

  startTime = millis();
  
/*
while (myBPM > 0) {

  int myBPM = pulseSensor.getBeatsPerMinute();  // Calls function on our pulseSensor object that returns BPM as an "int".
                                               // "myBPM" hold this BPM value now. 

  if (pulseSensor.sawStartOfBeat()) {            // Constantly test to see if "a beat happened".   

    memset(buffer, 0, sizeof(buffer)); //clear buffer
    itoa(myBPM, buffer, 10);
    textPulseQS.setText(buffer);
}

  delay(1000);  
}
*/

while(timeQSM < 15) {
  /*
    timeQS = (millis() - startTime)/1000;
    if (timeQS > 10) {
      timeQSM += 1;
      startTime = millis();
      //timeQS = (millis() - startTime)/1000;
      */

      timeQS += 1;
      delay(1000);

      if (timeQS > 59) {
        timeQS = 0;
        timeQSM += 1;
      }

      
   // }
    
    memset(buffer, 0, sizeof(buffer)); //clear buffer
    itoa(timeQS , buffer, 10);
    textTimeQS.setText(buffer);
   

    memset(buffer, 0, sizeof(buffer)); //clear buffer
    itoa(timeQSM, buffer, 10);
    textTimeQSM.setText(buffer);
  }


 while (rotations < 10000) {
  aState = digitalRead(outputA);
  if (aState != aLastState) {
    if (digitalRead(outputB) != aState) {
      counter ++;
    } else {
      counter --;
    }

    if (counter % 30 == 0) {
      rotations += 1;
      memset(buffer, 0, sizeof(buffer)); //clear buffer
      itoa(rotations, buffer, 10);
      textRevsQS.setText(buffer);
    }
  }
  aLastState = aState;

 }
  
}
//----------------------------------------------------------

//---------------------- Quick Start Done ------------------
void buttonDoneQSPopCallback(void *ptr)
{
  dbSerialPrintln("buttonDoneQSPopCallback");

  timeQS = 0;
  timeQSM = 0;
  
}



void setup(void) {
  pinMode (outputA, INPUT);
  pinMode (outputB, INPUT);

 // Serial.begin (9600);

  aLastState = digitalRead(outputA);

  pulseSensor.analogInput(PulseWire);   
  pulseSensor.setThreshold(Threshold); 


nexInit();

// Serial.begin(9600);

buttonPlus.attachPop(buttonPlusPopCallback);
buttonMinus.attachPop(buttonMinusPopCallback);

buttonPlus1.attachPop(buttonPlus1PopCallback);
buttonMinus1.attachPop(buttonMinus1PopCallback);

buttonMale.attachPop(buttonMalePopCallback);
buttonFemale.attachPop(buttonFemalePopCallback);

buttonPlusQHR.attachPop(buttonPlusQHRPopCallback);
buttonMinusQHR.attachPop(buttonMinusQHRPopCallback);

buttonPlusQT.attachPop(buttonPlusQTPopCallback);
buttonMinusQT.attachPop(buttonMinusQTPopCallback);

buttonPlusQRV.attachPop(buttonPlusQRVPopCallback);
buttonMinusQRV.attachPop(buttonMinusQRVPopCallback);

buttonPlusFWT.attachPop(buttonPlusFWTPopCallback);
buttonMinusFWT.attachPop(buttonMinusFWTPopCallback);

buttonPlusFZT.attachPop(buttonPlusFZTPopCallback);
buttonMinusFZT.attachPop(buttonMinusFZTPopCallback);

buttonPlusHWT.attachPop(buttonPlusHWTPopCallback);
buttonMinusHWT.attachPop(buttonMinusHWTPopCallback);

buttonPlusHZT.attachPop(buttonPlusHZTPopCallback);
buttonMinusHZT.attachPop(buttonMinusHZTPopCallback);

buttonPlusPWT.attachPop(buttonPlusPWTPopCallback);
buttonMinusPWT.attachPop(buttonMinusPWTPopCallback);

buttonPlusPZT.attachPop(buttonPlusPZTPopCallback);
buttonMinusPZT.attachPop(buttonMinusPZTPopCallback);

buttonStartQS.attachPop(buttonStartQSPopCallback);
buttonDoneQS.attachPop(buttonDoneQSPopCallback);

dbSerialPrintln("Setup done");
}

void loop(void) {
nexLoop(nex_listen_list);
}
