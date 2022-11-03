#include <avr/wdt.h>                   // Includes Watch Dog Timer to Reset Arduino
#define USE_ARDUINO_INTERRUPTS true    // Set-up low-level interrupts for most acurate BPM math.
#include "Nextion.h"                   // Incudes Nextion Library
#include <PulseSensorPlayground.h>     // Includes the PulseSensorPlayground Library.   

#define outputA 4
#define outputB 5

int stopTime;
int counter = 0;
int rotations = 0;
int aState;
int aLastState;

unsigned long previousMillis;
const long interval = 1000;

const int PulseWire = 0;       // PulseSensor PURPLE WIRE connected to ANALOG PIN 0
int Threshold = 550;           // Determine which Signal to "count as a beat" and which to ignore.
int myBPM;
int totalBPM = 0;
int numBPM = 0;

// Check vibrate variables
int vibrateTimer = 3;
boolean vibrateStarted = false;
int restTime = 0;
boolean inRange = false;

int currentPage;

const int motorPin = 3;        // Vibration Motor Pin

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

//------------------ Quick Start Workout Page ----------------
NexText textTimeQS = NexText(8, 2, "textTimeQS");
NexText textTimeQSC = NexText(8, 12, "textTimeQSC");
NexText textTimeQSM = NexText(8, 3, "textTimeQSM");
NexText textTimeQMC = NexText(8, 13, "textTimeQMC");
NexButton buttonStartQS = NexButton(3, 18, "buttonStartQS");
NexText textRevsQS = NexText(8, 7, "textRevsQS");
NexText textPulseQS = NexText(8, 9, "textPulseQS");
//------------------------------------------------------------

//-------------------- Warm Up Workout Page ------------------
NexButton buttonWarmUp3 = NexButton(4, 7, "buttonWarmUp3");
NexButton buttonWarmUp5 = NexButton(4, 8, "buttonWarmUp5");
NexButton buttonWarmUpNT = NexButton(4, 9, "buttonWarmUpNT");
NexButton buttonDoneWU = NexButton(9, 11, "buttonDoneWU");
NexText textWUHRlow = NexText(9, 16, "textWUHRlow");
NexText textWUHRhigh = NexText(9, 18, "textWUHRhigh");
NexText textTimeWUS = NexText(9, 10, "textTimeWUS");
NexText textTimeWUSC = NexText(9, 10, "textTimeWUSC");
NexText textTimeWUM = NexText(9, 8, "textTimeWUM");
NexText textTimeWUMC = NexText(9, 13, "textTimeWUMC");
NexText textRevsWU = NexText(9, 7, "textRevsWU");
NexText textPulseWU = NexText(9, 6, "textPulseWU");
//------------------------------------------------------------

//------------------- Fat Burn Workout Page ------------------
NexButton buttonStartFB = NexButton(5, 18, "buttonStartFB");
NexText textFBHRlow = NexText(10, 17, "textFBHRlow");
NexText textFBHRhigh = NexText(10, 19, "textFBHRhigh");
NexText textTimeFBM = NexText(10, 4, "textTimeFBM");
NexText textTimeFBMC = NexText(10, 21, "textTimeFBMC");
NexText textTimeFBS = NexText(10, 6, "textTimeFBS");
NexText textTimeFBSC = NexText(10, 20, "textTimeFBSC");
NexText textPulseFB = NexText(10, 8, "textPulseFB");
NexText textTimeFBMHR = NexText(10, 11, "textTimeFBMHR");
NexText textTimeFBMCHR = NexText(10, 23, "textTimeFBMCHR");
NexText textTimeFBSHR = NexText(10, 13, "textTimeFBSHR");
NexText textTimeFBSCHR =NexText(10, 22, "textTimeFBSCHR");
//------------------------------------------------------------

//----------------- Heart Health Workout Page ----------------
NexButton buttonStartHH = NexButton(6, 19, "buttonStartHH");
NexText textHHHRlow = NexText(11, 18, "textHHHRlow");
NexText textHHHRhigh = NexText(11, 20, "textHHHRhigh");
NexText textTimeHHMC = NexText(11, 22, "textTimeHHMC");
NexText textTimeHHM = NexText(11, 3, "textTimeHHM");
NexText textTimeHHS = NexText(11, 5, "textTimeHHS");
NexText textTimeHHSC = NexText(11, 21, "textTimeHHSC");
NexText textPulseHH = NexText(11, 7, "textPulseHH");
NexText textTimeHHMCHR = NexText(11, 24, "textTimeHHMCHR");
NexText textTimeHHMHR = NexText(11, 10, "textTimeHHMHR");
NexText textTimeHHSCHR = NexText(11, 23, "textTimeHHSCHR");
NexText textTimeHHSHR = NexText(11, 12, "textTimeHHSHR");
//------------------------------------------------------------

//----------------- Performance Workout Page -----------------
NexButton buttonStartP = NexButton(7, 18, "buttonStartP");
NexText textPHRlow = NexText(12, 21, "textPHRlow");
NexText textPHRhigh = NexText(12, 23, "textPHRhigh");
NexText textTimePMC = NexText(12, 16, "textTimePMC");
NexText textTimePM = NexText(12, 3, "textTimePM");
NexText textTimePSC = NexText(12, 15, "textTimePSC");
NexText textTimePS = NexText(12, 5, "textTimePS");
NexText textPulseP = NexText(12, 7, "textPulseP");
NexText textTimePMCHR = NexText(12, 18, "textTimePMCHR");
NexText textTimePMHR = NexText(12, 10, "textTimePMHR");
NexText textTimePSCHR = NexText(12, 17, "textTimePSCHR");
NexText textTimePSHR = NexText(12, 12, "textTimePSHR");
//------------------------------------------------------------

//------------- Quick Start/Warm Up Workout Summary ----------
NexButton buttonDoneQS = NexButton(8, 4, "buttonDoneQS");
NexText totalTimeQMC = NexText(13, 14, "totalTimeQMC");
NexText totalTimeQSM = NexText(13, 15, "totalTimeQSM");
NexText totalTimeQSC = NexText(13, 17, "totalTimeQSC");
NexText totalTimeQS = NexText(13, 18, "totalTimeQS");
NexText totalRevsQS = NexText(13, 11, "totalRevsQS");
NexText bpmAvgQS = NexText(13, 12, "bpmAvgQS");
NexText calsBurnQS = NexText(13, 13, "calsBurnQS");
NexButton buttonFinishQW = NexButton(13, 19, "buttonFinishQW");
//-------------------------------------------------------------

//--------- Fat Burn/Health/Performance Workout Page ----------
NexButton buttonDoneFB = NexButton(10, 14, "buttonDoneFB");
NexButton buttonDoneHH = NexButton(11, 15, "buttonDoneHH");
NexButton buttonDoneP = NexButton(12, 13, "buttonDoneP");
NexText totalTimePMC = NexText(14, 11, "totalTimePMC");
NexText totalTimePSM = NexText(14, 12, "totalTimePSM");
NexText totalTimePSC = NexText(14, 14, "totalTimePSC");
NexText totalTimePS = NexText(14, 15, "totalTimePS");
NexText totalTimePMCHR = NexText(14, 16, "totalTimePMCHR");
NexText totalTimePSMHR = NexText(14, 17, "totalTimePSMHR");
NexText totalTimePSCHR = NexText(14, 19, "totalTimePSCHR");
NexText totalTimePSHR = NexText(14, 20, "totalTimePSHR");
NexText bpmAvgPH = NexText(14, 21, "bpmAvgPH");
NexText calsBurnPH = NexText(14, 22, "calsBurnPH");
NexButton buttonFinishPH = NexButton(14, 23, "buttonFinishPH");
//-------------------------------------------------------------

NexPage page0 = NexPage(0, 0, "page0");
NexPage page13 = NexPage(13, 0, "page13");
NexPage page14 = NexPage(14, 0, "page14");

int calsMale;
int calsFemale;

int number = 30;
int weight = 120;
int sex = 1;

int maxHR = 220 - number;
//int warmUpHRlow = maxHR * 0.6;
//int warmUpHRhigh = maxHR * 0.7;
int fatBurnHRlow;
int fatBurnHRhigh;
int heartHealthHRlow;
int heartHealthHRhigh;
int performanceHRlow;
int performanceHRhigh;

int warmupTime;

int quickStartHR = 80;
int quickStartTime = 5;
int quickStartRev = 500;

int fatBurnWoTime = 30;
int fatBurnZnTime = 30;

int healthWoTime = 15;
int healthZnTime = 15;

int performanceWoTime = 10;
int performanceZnTime = 10;

int timeQS = 0;

int timePSHR = 0;

char buffer[10] = {0};

NexTouch *nex_listen_list[] =
{ 
  &buttonPlus, &buttonMinus, &buttonPlus1, &buttonMinus1, &buttonMale, &buttonFemale,
  &buttonPlusQHR, &buttonMinusQHR, &buttonPlusQT, &buttonMinusQT, &buttonPlusQRV, &buttonMinusQRV,
  &buttonPlusFWT, &buttonMinusFWT, &buttonPlusFZT, &buttonMinusFZT,
  &buttonPlusHWT, &buttonMinusHWT, &buttonPlusHZT, &buttonMinusHZT,
  &buttonPlusPWT, &buttonMinusPWT, &buttonPlusPZT, &buttonMinusPZT,
  &buttonStartQS, &buttonDoneQS, &textPulseQS, &totalRevsQS,
  &buttonWarmUp3, &buttonDoneWU, &buttonWarmUp5, &buttonWarmUpNT,
  &buttonStartFB, &buttonDoneFB,
  &buttonStartHH, &buttonDoneHH,
  &buttonStartP, &buttonDoneP,
  &buttonFinishQW, &buttonFinishPH,
  &page0, &page13, &page14,
  NULL
};

void displayText(int numberToDisplay, NexText textField) {
  memset (buffer, 0, sizeof(buffer));
  itoa(numberToDisplay, buffer, 10);
  textField.setText(buffer);
}

void updateTime(int seconds, NexText oneSecond, NexText tenSecond, NexText oneMinute, NexText tenMinute) {
  displayText(seconds%10, oneSecond);            // Shows the seconds and resets after 10
  displayText(((seconds/10)%6), tenSecond);     // Shows the ten seconds and resets after 6
  displayText(((seconds/60)%10), oneMinute);     
  displayText((seconds/600)%10, tenMinute);
}

void countRevs(NexText revText) {
  aState = digitalRead(outputA);
  if (aState != aLastState) {
    if (digitalRead(outputB) != aState) {
    counter ++;
    } else {
      counter --;
    }
    
    if (counter <= -20 || counter >= 20) {
      rotations++;
      counter = 0;
      displayText(rotations, revText);
    } 
    //aLastState = aState;        
  }
}

void displayCals(int timer, NexText calsText) {
  if (sex == 1) {
   calsMale = (timer/60)*(0.6309*(totalBPM/numBPM) - 0.1988*weight + 0.2017*number - 55.0969)/4.184;
   displayText(calsMale, calsText);
  }

  if (sex == 0) {
   calsFemale = (timer/60)*(0.4472*(totalBPM/numBPM) - 0.1263*weight + 0.074*number - 20.4022)/4.184;
   displayText(calsFemale, calsText);
  }
}

/**
 * Helper function for vibrateInRange() to check if BPM is in target range
 */
boolean heartrateInRange(int minHeartrate, int maxHeartrate) {
  return (myBPM >= minHeartrate && myBPM <= maxHeartrate);
}

int inRangeWorkoutVibrate = 0;
boolean workout = true;
boolean firstBPM = false;

void hearRateVibe(int hrLow, int hrHigh, int hrTime) {
  if (inRangeWorkoutVibrate > 0) {
    digitalWrite(motorPin, HIGH);
    inRangeWorkoutVibrate--;
  } else {
    digitalWrite(motorPin, LOW);
  }
  
  if (workout) {
    if (heartrateInRange(hrLow, hrHigh)) {
      if (!inRange) {
        inRange = true;
        inRangeWorkoutVibrate++;
      }
      timePSHR++;
      firstBPM = true;
    } else {
      inRange = false;
    }
    
    if ((timePSHR)%hrTime == 0 && firstBPM) {
      workout = false;
      inRange = false;
      inRangeWorkoutVibrate = 3;
    }
  } else {
    restTime++;
    if (restTime%hrTime == 0) {
      workout = true;
      firstBPM = false;
      restTime = 0;
      inRangeWorkoutVibrate = 3;
    }
  }
}

//----------------------------------------------------------------------------------Personal Info Page Set Up ---------------------------------------------------------------------
//---------------------Age Buttons---------------------------
void buttonPlusPushCallback(void *ptr)
{
  dbSerialPrintln("buttonPlusPushCallback");
  if (number < 100) number += 1;
  displayText(number, textNumber);
}

void buttonMinusPopCallback(void *ptr)
{
  dbSerialPrintln("buttonMinusPopCallback");
  if (number > 0) number -= 1;
  displayText(number, textNumber);
}

//////////////////////////////////////////////////////////////

//--------------------Weight Buttons--------------------------
void buttonPlus1PopCallback(void *ptr)
{
  dbSerialPrintln("buttonPlus1PopCallback");
  if (weight < 300) weight += 1;
  displayText(weight, textWeight);
}

void buttonMinus1PopCallback(void *ptr)
{
  dbSerialPrintln("buttonMinus1PopCallback");

  if (weight > 70) weight -= 1;
  displayText(weight, textWeight);
}
/////////////////////////////////////////////////////////////

//------------------------Sex Buttons-------------------------
void buttonMalePopCallback(void *ptr)
{
  dbSerialPrintln("buttonMalePopCallback");
  sex = 1;
  textSex.setText("Male");
}

void buttonFemalePopCallback(void *ptr)
{
  dbSerialPrintln("buttonFemalePopCallback");
  sex = 0;
  textSex.setText("Female");
}
/////////////////////////////////////////////////////////////////////////////// End Personal Info Page \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

//---------------------------------------------------------------------------- QuickStart Set Up Buttons -------------------------------------------------------------------------------
//----------------------Heart Rate----------------------------
void buttonPlusQHRPopCallback(void *ptr)
{
  dbSerialPrintln("buttonPlusQHRPopCallback");
  if (quickStartHR < 200) quickStartHR += 1;
  displayText(quickStartHR, textQHR);
}

void buttonMinusQHRPopCallback(void *ptr)
{
  dbSerialPrintln("buttonMinusQHRPopCallback");
  if (quickStartHR > 60) quickStartHR -= 1;
  displayText(quickStartHR, textQHR);
}
//////////////////////////////////////////////////////////////

//---------------------------Time-----------------------------
void buttonPlusQTPopCallback(void *ptr)
{
  dbSerialPrintln("buttonPlusQTPopCallback");
  if (quickStartTime < 60) quickStartTime += 1;
  displayText(quickStartTime, textQT);
}

void buttonMinusQTPopCallback(void *ptr)
{
  dbSerialPrintln("buttonMinusQTPopCallback");
  if (quickStartTime > 1) quickStartTime -= 1;
  displayText(quickStartTime, textQT);
}
///////////////////////////////////////////////////////////////

//-------------------------Revs-------------------------------
void buttonPlusQRVPopCallback(void *ptr)
{
  dbSerialPrintln("buttonPlusQRVPopCallback");
  if (quickStartRev < 2000) quickStartRev += 10;
  displayText(quickStartRev, textQRV);
}

void buttonMinusQRVPopCallback(void *ptr)
{
  dbSerialPrintln("buttonMinusQRVPopCallback");
  if (quickStartRev > 0) quickStartRev -= 10;
  displayText(quickStartRev, textQRV);
}
////////////////////////////////////////////////////////////////////////////// End Quick Start Set Up Buttons \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

//------------------------------------------------------------------------------- Fat Burn Set Up Buttons -------------------------------------------------------------------------------
//---------------------- Workout Time ------------------------
void buttonPlusFWTPopCallback(void *ptr)
{
  dbSerialPrintln("buttonPlusFWTPopCallback");
  if (fatBurnWoTime < 60) fatBurnWoTime += 5;
  displayText(fatBurnWoTime, textFWT);
}

void buttonMinusFWTPopCallback(void *ptr)
{
  dbSerialPrintln("buttonMinusFWTPopCallback");
  if (fatBurnWoTime > 10) fatBurnWoTime -= 5;
  displayText(fatBurnWoTime, textFWT);
}
/////////////////////////////////////////////////////////////

//----------------------- Zone Time --------------------------
void buttonPlusFZTPopCallback(void *ptr)
{
  dbSerialPrintln("buttonPlusFZTPopCallback");
  if (fatBurnZnTime < 100) fatBurnZnTime += 5;
  displayText(fatBurnZnTime, textFZT);
}

void buttonMinusFZTPopCallback(void *ptr)
{
  dbSerialPrintln("buttonMinusFZTPopCallback");
  if (fatBurnZnTime > 10) fatBurnZnTime -= 5;
  displayText(fatBurnZnTime, textFZT);
}
////////////////////////////////////////////////////////////////////////////// End Fat Burn Set Up Buttons \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

//------------------------------------------------------------------------------ Heart Health Set Up Buttons --------------------------------------------------------------------------
//---------------------- Workout Time -------------------------
void buttonPlusHWTPopCallback(void *ptr)
{
  dbSerialPrintln("buttonPlusHWTPopCallback");
  if (healthWoTime < 60) healthWoTime +=5;
  displayText(healthWoTime, textHWT);
}

void buttonMinusHWTPopCallback(void *ptr)
{
  dbSerialPrintln("buttonMinusHWTPopCallback");
  if (healthWoTime > 5) healthWoTime -=5;
  displayText(healthWoTime, textHWT);
}
//////////////////////////////////////////////////////////////

//----------------------- Zone Time -------------------------
void buttonPlusHZTPopCallback(void *ptr)
{
  dbSerialPrintln("buttonPlusHZTPopCallback");
  if (healthZnTime < 60) healthZnTime +=5;
  displayText(healthZnTime, textHZT);
}

void buttonMinusHZTPopCallback(void *ptr)
{
  dbSerialPrintln("buttonMinusHZTPopCallback");
  if (healthZnTime > 5) healthZnTime -=5;
  displayText(healthZnTime, textHZT);
}
/////////////////////////////////////////////////////////////////////////// End Heart Helath Set Up Page \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

//-------------------------------------------------------------------------------- Performance Set Up -------------------------------------------------------------------------------
//--------------------- Workout Time ------------------------
void buttonPlusPWTPopCallback(void *ptr)
{
  dbSerialPrintln("buttonPlusPWTPopCallback");
  if (performanceWoTime < 60) performanceWoTime +=5;
  displayText(performanceWoTime, textPWT);
}

void buttonMinusPWTPopCallback(void *ptr)
{
  dbSerialPrintln("buttonMinusPWTPopCallback");
  if (performanceWoTime > 5) performanceWoTime -=5;
  displayText(performanceWoTime, textPWT);
}
////////////////////////////////////////////////////////////

//---------------------- Zone Time -------------------------
void buttonPlusPZTPopCallback(void *ptr)
{
  dbSerialPrintln("buttonPlusPZTPopCallback");
  if (performanceZnTime < 60) performanceZnTime +=5;
  displayText(performanceZnTime, textPZT);
}

void buttonMinusPZTPopCallback(void *ptr)
{
  dbSerialPrintln("buttonMinusPZTPopCallback");
  if (performanceZnTime > 5) performanceZnTime -=5;
  displayText(performanceZnTime, textPZT);
}
//////////////////////////////////////////////////////////////////////////////// End Performance \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

//------------------------------------------------------------------------- Quick Start Workout Page -------------------------------------------------------------------------------
void buttonStartQSPopCallback(void *ptr) {
  dbSerialPrintln("buttonStartQSPopCallback");
  currentPage = 8;
  textRevsQS.setText("0");
  previousMillis = 0;       
}
//////////////////////////////////////////////////////////////////////////////// End QuickStart Workout Page \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

//------------------------------------------------------------------------------- Warm Up Mode Workout Page --------------------------------------------------------------------------
//--------------------- 3 Minute Warm Up -----------------------
void buttonWarmUp3PopCallback(void *ptr)
{
  dbSerialPrintln("buttonWarmUp3PopCallback");

  timeQS = 0;
  warmupTime = 3;
  currentPage = 9;
  
  maxHR = 220 - number;
  int warmUpHRlow = maxHR * 0.6;
  int warmUpHRhigh = maxHR * 0.7;
  
  displayText(warmUpHRlow, textWUHRlow);
  displayText(warmUpHRhigh, textWUHRhigh);

  previousMillis = 0;
}
/////////////////////////////////////////////////////////////////

//------------------------ 5 Minute Warm Up ---------------------
void buttonWarmUp5PopCallback(void *ptr)
{
  dbSerialPrintln("buttonWarmUp5PopCallback");

  timeQS = 0;
  warmupTime = 5;
  currentPage = 9;
  
  maxHR = 220 - number;
  int warmUpHRlow = maxHR * 0.6;
  int warmUpHRhigh = maxHR * 0.7;
  
  displayText(warmUpHRlow, textWUHRlow);
  displayText(warmUpHRhigh, textWUHRhigh);

  previousMillis = 0;
}
///////////////////////////////////////////////////////////////////

//-------------------------- No Time Warm Up ----------------------
void buttonWarmUpNTPopCallback(void *ptr)
{
  dbSerialPrintln("buttonWarmUpNTPopCallback");

  timeQS = 0;
  warmupTime = 60;
  currentPage = 9;
  
  maxHR = 220 - number;
  int warmUpHRlow = maxHR * 0.6;
  int warmUpHRhigh = maxHR * 0.7;
  
  displayText(warmUpHRlow, textWUHRlow);
  displayText(warmUpHRhigh, textWUHRhigh);

  previousMillis = 0;
}
//////////////////////////////////////////////////////////////////////////////// End Warm Up Workout Page \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

//------------------------------------------------------------------------------- Fat Burn Workout Page --------------------------------------------------------------------------------
void buttonStartFBPopCallback(void *ptr)
{
  dbSerialPrintln("buttonStartFBPopCallback");

  timeQS = 0;
  currentPage = 10;
  maxHR = 220 - number;
  
  fatBurnHRlow = maxHR * 0.7;
  fatBurnHRhigh = maxHR * 0.8;
  
  displayText(fatBurnHRlow, textFBHRlow);
  displayText(fatBurnHRhigh, textFBHRhigh);

  previousMillis = 0;
}
/////////////////////////////////////////////////////////////////////////////// End Fat Burn Workout Page \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

//----------------------------------------------------------------------------- Heart Health Workout Page ------------------------------------------------------------------------------
void buttonStartHHPopCallback(void *ptr)
{
  dbSerialPrintln("buttonStartHHPopCallback");

  timeQS = 0;
  currentPage = 11;
  maxHR = 220 - number;
  
  heartHealthHRlow = maxHR * 0.8;
  heartHealthHRhigh = maxHR * 0.9;
  
  displayText(heartHealthHRlow, textHHHRlow);
  displayText(heartHealthHRhigh, textHHHRhigh);
 
  previousMillis = 0;
}
///////////////////////////////////////////////////////////////////////////// End Heart Health Workout Page \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

//----------------------------------------------------------------------------- Performance Workout Page -------------------------------------------------------------------------------
void buttonStartPPopCallback(void *ptr)
{
  dbSerialPrintln("buttonStartPPopCallback");

  timeQS = 0;
  currentPage = 12;

  maxHR = 220 - number;
  performanceHRlow = maxHR * 0.95;
  performanceHRhigh = maxHR;
  
  displayText(performanceHRlow, textPHRlow);
  displayText(performanceHRhigh, textPHRhigh);

  previousMillis = 0;
}
///////////////////////////////////////////////////////////////////////////// End Performance Workout Page \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

//---------------------- Quick Start Done ------------------
void buttonDoneQSPopCallback(void *ptr)
{
  page13.show();
  currentPage = 13;
  digitalWrite(motorPin, LOW);
}
//----------------------------------------------------------

//---------------------- Warm Up Done ----------------------
void buttonDoneWUPopCallback(void *ptr)
{
  dbSerialPrintln("buttonDoneWUPopCallback");
  page13.show();
  currentPage = 13;
  digitalWrite(motorPin, LOW);
}
//----------------------------------------------------------

//----------------------------------------------------------
void buttonDoneFBPopCallback(void *ptr) 
{
  page14.show();
  currentPage = 14;
  digitalWrite(motorPin, LOW);
}
//----------------------------------------------------------

//------------------- Heart Health Done --------------------
void buttonDoneHHPopCallback(void *ptr) 
{
  page14.show();
  currentPage = 14;
  digitalWrite(motorPin, LOW);
}
//----------------------------------------------------------

//------------------- Performance Done ---------------------
void buttonDonePPopCallback(void *ptr) 
{
  page14.show();
  currentPage = 14;
  digitalWrite(motorPin, LOW);
}
//----------------------------------------------------------

//------------ Quick Start/Warm Up Finish-------------------
void buttonFinishQWPopCallback(void *ptr) {  
  dbSerialPrintln("buttonFinishQWPopCallback");
  page0.show();
}
//----------------------------------------------------------

//-------- Fat Burn/Heart Health/Performace Finish ---------
void buttonFinishPHPopCallback(void *ptr) {  
  dbSerialPrintln("buttonFinishPHPopCallback");
  page0.show();
}
//----------------------------------------------------------


//---------------------------------------------------------------------------------------- SETUP ----------------------------------------------------------------------------
void setup(void) {
  wdt_disable();  
  pinMode(outputA, INPUT);
  pinMode(outputB, INPUT);
  pinMode(motorPin, OUTPUT);
    
  aLastState = digitalRead(outputA);

  pulseSensor.analogInput(PulseWire);   
  pulseSensor.setThreshold(Threshold); 
  pulseSensor.begin();

  nexInit();
  
  buttonPlus.attachPush(buttonPlusPushCallback);
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

  buttonWarmUp3.attachPop(buttonWarmUp3PopCallback);
  buttonWarmUp5.attachPop(buttonWarmUp5PopCallback);
  buttonWarmUpNT.attachPop(buttonWarmUpNTPopCallback);
  buttonDoneWU.attachPop(buttonDoneWUPopCallback);

  buttonStartFB.attachPop(buttonStartFBPopCallback);
  buttonStartHH.attachPop(buttonStartHHPopCallback);
  buttonStartP.attachPop(buttonStartPPopCallback);
  
  buttonDoneHH.attachPop(buttonDoneHHPopCallback);
  buttonDoneFB.attachPop(buttonDoneFBPopCallback);
  buttonDoneP.attachPop(buttonDonePPopCallback); 

  buttonFinishQW.attachPop(buttonFinishQWPopCallback);
  buttonFinishPH.attachPop(buttonFinishPHPopCallback);

  dbSerialPrintln("Setup done");
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------


// Begin Loop
void loop(void) {
  nexLoop(nex_listen_list);

  //=================================================================== Quick Start Page =============================================================================
  if (currentPage == 8) {
    if (timeQS/600 < 6) {
      unsigned long currentMillis = millis();
      myBPM = pulseSensor.getBeatsPerMinute(); // Calls function on our pulseSensor object that returns BPM as an "int".
        
      countRevs(textRevsQS);
  
      if (currentMillis - previousMillis >= interval) {
        timeQS += 1;
        if (pulseSensor.sawStartOfBeat()) {            // Constantly test to see if "a beat happened".   
          displayText(myBPM, textPulseQS);
          totalBPM += myBPM;
          numBPM++;
        }
  
        updateTime(timeQS, textTimeQS, textTimeQSC, textTimeQSM, textTimeQMC); // displays the time
        previousMillis = currentMillis;
      }
      aLastState = aState;
  
      if ((timeQS >= quickStartTime*60 && timeQS <= quickStartTime*60 + 3) || // vibrate for 3 seconds at time set by user
           rotations == quickStartRev ||                                      // vibrate at # of revs set by user
           (myBPM >= quickStartHR && myBPM <= quickStartHR + 10)) {           // vibrate at Heart Rate set by user
        digitalWrite(motorPin, HIGH);
      } else {
        digitalWrite(motorPin, LOW);
      }
    }
  }
  //=================================================================== Quick Start Page End =====================================================================

  //=================================================================== Warm Up Page =============================================================================
  else if (currentPage == 9 ) {
    if (timeQS/60 < warmupTime) {
      unsigned long currentMillis = millis();

      myBPM = pulseSensor.getBeatsPerMinute();

      countRevs(textRevsWU);

      if (currentMillis - previousMillis >= interval) {
        timeQS++;
        if (pulseSensor.sawStartOfBeat()) {
          displayText(myBPM, textPulseWU);
          totalBPM += myBPM;
          numBPM++;
        }

        updateTime(timeQS, textTimeWUS, textTimeWUSC, textTimeWUM, textTimeWUMC);

        previousMillis = currentMillis;
      }
      aLastState = aState;

      if (timeQS/60 == warmupTime) {
          digitalWrite(motorPin, HIGH);
      }
    }
  }
  //=================================================================== Warm Up Page End =============================================================================

  //===================================================================== Fat Burn Page ==============================================================================
  else if (currentPage == 10) {
    if (timeQS/600 < 6) {

      unsigned long currentMillis = millis();
       
      myBPM = pulseSensor.getBeatsPerMinute(); // Calls function on our pulseSensor object that returns BPM as an "int".

      if (currentMillis - previousMillis >= interval) {
        timeQS++;
        if (pulseSensor.sawStartOfBeat()) {            // Constantly test to see if "a beat happened".   
          displayText(myBPM, textPulseFB);
          totalBPM += myBPM;
          numBPM++;
        }

        updateTime(timeQS, textTimeFBS, textTimeFBSC, textTimeFBM, textTimeFBMC);

        hearRateVibe(fatBurnHRlow, fatBurnHRhigh, fatBurnZnTime);
   
        updateTime(timePSHR, textTimeFBSHR, textTimeFBSCHR, textTimeFBMHR, textTimeFBMCHR);
       
        previousMillis = currentMillis;
      }
    }
  }
//  ========================================================================== Fat Burn Page End ==================================================================================

//  ========================================================================== Heart Health Page ==================================================================================
  else if (currentPage == 11) {
  if (timeQS/600 < 6) {
    
    unsigned long currentMillis = millis();
       
    myBPM = pulseSensor.getBeatsPerMinute(); // Calls function on our pulseSensor object that returns BPM as an "int".

    if (currentMillis - previousMillis >= interval) {
       timeQS++;
       if (pulseSensor.sawStartOfBeat()) {            // Constantly test to see if "a beat happened".   
          displayText(myBPM, textPulseHH);
          totalBPM += myBPM;
          numBPM++;
        }

      updateTime(timeQS, textTimeHHS, textTimeHHSC, textTimeHHM, textTimeHHMC);

      hearRateVibe(heartHealthHRlow, heartHealthHRhigh, healthZnTime);

      updateTime(timePSHR, textTimeHHSHR, textTimeHHSCHR, textTimeHHMHR, textTimeHHMCHR);
       
      previousMillis = currentMillis;
    }
   } 
  }
  //============================================================================ Heart Health Page End ============================================================================

  //============================================================================== Performance Page ===============================================================================
  else if (currentPage == 12) {
  if (timeQS/600 < 6) {

    unsigned long currentMillis = millis();
       
    myBPM = pulseSensor.getBeatsPerMinute(); // Calls function on our pulseSensor object that returns BPM as an "int".

    if (currentMillis - previousMillis >= interval) {
      timeQS++;
      if (pulseSensor.sawStartOfBeat()) {            // Constantly test to see if "a beat happened".   
        displayText(myBPM, textPulseP);
        totalBPM += myBPM;
        numBPM++;
      }
            
      updateTime(timeQS, textTimePS, textTimePSC, textTimePM, textTimePMC);

      hearRateVibe(performanceHRlow, performanceHRhigh, performanceZnTime);
       
      updateTime(timePSHR, textTimePSHR, textTimePSCHR, textTimePMHR, textTimePMCHR);
       
      previousMillis = currentMillis;
    }
   } 
  }
   //======================================================================== Performance Page End ===========================================================================


   else if (currentPage == 13) {
    displayText(timeQS%10, totalTimeQS);
    displayText((timeQS/10)%6, totalTimeQSC);
    displayText((timeQS/60)%10, totalTimeQSM);
    displayText((timeQS/600)%10, totalTimeQMC);
    displayText(rotations, totalRevsQS);
    displayText((totalBPM/numBPM), bpmAvgQS);
    displayCals(timeQS, calsBurnQS);
    currentPage = 0;
   }

   else if (currentPage == 14) {
    displayText(timeQS%10, totalTimePS);
    displayText((timeQS/10)%6, totalTimePSC);
    displayText((timeQS/60)%10, totalTimePSM);
    displayText((timeQS/600)%10, totalTimePMC);
    displayText(timePSHR%10, totalTimePSHR);
    displayText((timePSHR/10)%6, totalTimePSCHR);
    displayText((timePSHR/60)%10, totalTimePSMHR);
    displayText((timePSHR/600)%10, totalTimePMCHR);
    displayText((totalBPM/numBPM), bpmAvgPH);
    displayCals(timeQS, calsBurnPH);
    currentPage = 0;
    inRangeWorkoutVibrate = 0;
    workout = true;
    firstBPM = false;
   }

   else if (currentPage == 0) {
    timeQS = 0;
    timePSHR = 0;
    rotations = 0;
    myBPM = 0;
    numBPM = 0;
    totalBPM = 0;
    calsMale = 0;
    calsFemale = 0;
   }
}
