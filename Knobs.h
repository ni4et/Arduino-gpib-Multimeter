// Interface to the knobs ISR routines. 

// call loopKnobs to get knob events that may be pending.

enum KNOB_EVENT { KE_NONE,KE_RANGE_UP,KE_RANGE_DN,KE_MODE_UP, KE_MODE_DN};
void setupKnobs();
KNOB_EVENT loopKnobs();

