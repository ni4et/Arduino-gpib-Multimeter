#include "Common.h"


#define APIN 18
#define BPIN 19
#define CPIN 14
#define OPEN HIGH
#define CLOSED LOW

#define KNOB_Q_DEPTH 8 // Power of 2
#define KNOB_Q_MASK (KNOB_Q_DEPTH-1)
KNOB_EVENT knobEvents[KNOB_Q_DEPTH];
uint8_t Q_in=0,Q_out=0;
// Q_in==Q_Q_out queue empty.
// post then incrememt or read then increment pointer


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
            knobEvents[Q_in]=(digitalRead(CPIN)==OPEN)?KE_RANGE_DN:KE_MODE_UP; 
            Q_in=(Q_in+1) & KNOB_Q_MASK;
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
            knobEvents[Q_in]=(digitalRead(CPIN)==OPEN)?KE_RANGE_UP:KE_MODE_DN; 
            Q_in=(Q_in+1) & KNOB_Q_MASK;

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

// Returns a queued knob event;
KNOB_EVENT loopKnobs()
{
    KNOB_EVENT rv=KE_NONE;

    if (Q_in!=Q_out)
    {
       rv=knobEvents[Q_out];
       Q_out=(Q_out+1) & KNOB_Q_MASK;
    }
    return rv;
}
