#include <Arduino.h>
#include <DMMShield.h>

#include "Knobs.h"
volatile int16_t knobCount=0; // Modified in isr
int16_t knobCountFolo=0; // follows knobCount in loop()
int16_t subMode=0; // Follows knobCount but is limited by range
int16_t lastDmmModeScale=0;
extern DMMShield dmm;

#define dacScale (1023)
#define L1 (dacScale/6) // 1/6
#define L2 (dacScale/2) // 3/6
#define L3 ((dacScale*5)/6) // 5/6


#define APIN 18
#define BPIN 19

#define OPEN HIGH
#define CLOSED LOW
//==========================================================================


DMMRange_t resistanceRanges[]={    DmmResistance_5e7, DmmResistance_5e6, DmmResistance_5e5,
    DmmResistance_5e4, DmmResistance_5e3, DmmResistance_5e2, DmmResistance_5e1 };

DMMRange_t dcvRanges[]={ DmmDCVoltage_5e1, DmmDCVoltage_5e0, DmmDCVoltage_5em1,   DmmDCVoltage_5em2 };

DMMRange_t dccRanges[]= { DmmDCCurrent_5e0, DmmDCLowCurrent_5em1,
    DmmDCLowCurrent_5em2, DmmDCLowCurrent_5em3, DmmDCLowCurrent_5em4 };

DMMRange_t acvRanges[]={DmmACVoltage_5e1, DmmACVoltage_5e0, DmmACVoltage_5em1, DmmACVoltage_5em2};
DMMRange_t accRanges[]={DmmACCurrent_5e0, DmmACLowCurrent_5em1,DmmACLowCurrent_5em2,
    DmmACLowCurrent_5em3, DmmACLowCurrent_5em4 };

DMMRange_t sfRanges[]={DmmContinuity, DmmDiode};



//==========================================================================
typedef enum ENCODER_STATE {E_START, AFIRST, BSECOND, BFIRST,ASECOND } encoder_state_t;

encoder_state_t es=E_START;

void ISR_A();
void ISR_B();

void ISR_A()
{

    if (digitalRead(APIN)==CLOSED)
    {
        if (E_START==es) es=AFIRST;  // NULL->>AFIRST
        else if (BFIRST==es)
        {
            es=ASECOND; // BFIRST->>ASECOND
            knobCount--;
        }
    }
    else if (digitalRead(BPIN)==OPEN) // Both open
        es=E_START;
}
void ISR_B()
{

    if (digitalRead(BPIN)==CLOSED)
    {
        if (E_START==es) es=BFIRST;
        else if (AFIRST==es)
        {
            es=BSECOND;
            knobCount++;
        }
    }
    else if (digitalRead(APIN)==OPEN)
        es=E_START;
}

void setupKnobs()
{
    //uint16_t ia,ib;

    analogReference(DEFAULT);
    pinMode(APIN,INPUT_PULLUP);
    pinMode(BPIN,INPUT_PULLUP);
    //ia=digitalPinToInterrupt(APIN);
    //ib=digitalPinToInterrupt(BPIN);
    //Serial.print("isrs: ");
    //Serial.print(ia);
    //Serial.print(" ");
    //Serial.println(ib);
    attachInterrupt(digitalPinToInterrupt(APIN),ISR_A,CHANGE);
    attachInterrupt(digitalPinToInterrupt(BPIN),ISR_B,CHANGE);
    lastDmmModeScale=-1;
}

void loopKnobs()
{
    uint16_t ai;
    int16_t newDmmMode;

    int16_t newDmmModeScale;

    DMMRange_t * rngt;
    uint16_t rngxx;

    interrupts();


  ai = analogRead(A8);

    if (ai<L1)
    {
        rngt=&dcvRanges[0];
        rngxx=sizeof(dcvRanges)/sizeof(dcvRanges[0]);

        newDmmMode=0;
    }
    else if  (ai<L2)
    {
        newDmmMode=1;
        rngt=&dccRanges[0];
         rngxx=sizeof(dccRanges)/sizeof(dccRanges[0]);
    }
    else if (ai<L3)
    {
        newDmmMode=2;
        rngt=&resistanceRanges[0];
         rngxx=sizeof(resistanceRanges)/sizeof(resistanceRanges[0]);
    }
    else
    {
        newDmmMode=3;
        rngt=&sfRanges[0];
         rngxx=sizeof(sfRanges)/sizeof(sfRanges[0]);
    }

 //Serial.write(String(ai));
 //Serial.print("DAC: "); Serial.print(ai); Serial.print(" ");
 // Serial.println(mode);
//============== Now go get the knob encoder command part
int16_t diff=knobCount-knobCountFolo;
knobCountFolo+=diff;
subMode+=diff;
//Serial.print("Knobc: ");
//Serial.print(knobCount);
//Serial.print(" KnobcF: ");
//Serial.print(knobCountFolo);
//Serial.print(" Diff: ");
//Serial.print(diff);
//Serial.print(" rngxx: ");
//Serial.print(rngxx);
// Limit the submode to the number of ranges in the mode
if (subMode<0) subMode=0;
if (subMode>=rngxx) subMode=rngxx-1;
newDmmModeScale=rngt[subMode];
if (newDmmModeScale!= lastDmmModeScale)
{
     dmm.SetScale(newDmmModeScale);
     lastDmmModeScale=newDmmModeScale;
}

//Serial.print(" Mode: ");
//Serial.print(newDmmMode);
//Serial.print("/");
///Serial.print(subMode);
//Serial.print("/");
//Serial.print(newDmmModeScale);
//Serial.print(" === ");

}
