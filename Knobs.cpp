#include <Arduino.h>


#include "Knobs.h"
// Locals that need to be static
volatile int16_t knobCount=0; // Modified in isr
int16_t knobCountFollow=0; // follows knobCount in loop()

// Shared with main loop:
int16_t rangeSteps=0;
int16_t modeSteps=0;


#define APIN 18
#define BPIN 19
#define CPIN 14
#define OPEN HIGH
#define CLOSED LOW


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
    pinMode(CPIN,INPUT_PULLUP);

    attachInterrupt(digitalPinToInterrupt(APIN),ISR_A,CHANGE);
    attachInterrupt(digitalPinToInterrupt(BPIN),ISR_B,CHANGE);
}

void loopKnobs()
{
    //============== Now go get the knob encoder command part
    int16_t diff=knobCount-knobCountFollow;
    knobCountFollow+=diff;

    if (digitalRead(CPIN))
    {
        modeSteps=diff;
    }
    else
    {
        rangeSteps=diff;
    }

}
