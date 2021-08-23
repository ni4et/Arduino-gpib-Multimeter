
#include "Common.h"

// Imported from Knob.cpp
int16_t rangeSteps;
int16_t modeSteps;


DMM_RANGE dmmRangeTable[][8]=
{      
    { DmmDCVoltage_5e1, DmmDCVoltage_5e0, DmmDCVoltage_5em1,   DmmDCVoltage_5em2 ,
        DMMInvalidSetting,DMMInvalidSetting,DMMInvalidSetting,DMMInvalidSetting },
    { DmmACVoltage_5e1, DmmACVoltage_5e0, DmmACVoltage_5em1, DmmACVoltage_5em2,
        DMMInvalidSetting,DMMInvalidSetting,DMMInvalidSetting,DMMInvalidSetting } , 
    { DmmResistance_5e7, DmmResistance_5e6, DmmResistance_5e5, DmmResistance_5e4,
         DmmResistance_5e3, DmmResistance_5e2, DmmResistance_5e1 ,DMMInvalidSetting},  
    { DmmACCurrent_5e0, DmmACLowCurrent_5em1,DmmACLowCurrent_5em2,   DmmACLowCurrent_5em3,
         DmmACLowCurrent_5em4 ,DMMInvalidSetting,DMMInvalidSetting,DMMInvalidSetting},
    { DmmDCCurrent_5e0, DmmDCLowCurrent_5em1, DmmDCLowCurrent_5em2, DmmDCLowCurrent_5em3, 
        DmmDCLowCurrent_5em4 ,DMMInvalidSetting,DMMInvalidSetting,DMMInvalidSetting},
    { DmmContinuityRange, DmmDiodeRange,DMMInvalidSetting,DMMInvalidSetting,
        DMMInvalidSetting,DMMInvalidSetting,DMMInvalidSetting,DMMInvalidSetting }
};

#define N_MODES sizeof(dmmRangeTable)/sizeof(dmmRangeTable[0])
#define N_RANGES sizeof(dmmRangeTable[0])/sizeof(dmmRangeTable[0][0])

// Call to get the DMM_RANGE
// passed parameter cmd can be either -1 or +1


uint16_t rangeChangeHander(int8_t cmd)
{

    if (cmd<0)
    {
        if(rangeSteps>0)
        {
            rangeSteps--;
            tone(15,2000,50);
        }
    }
    else if  (rangeSteps<N_RANGES-1)
    {
        if (dmmRangeTable[modeSteps][rangeSteps+1]>=0)
        { // Test for valid range
            rangeSteps++;
            tone(15,2000,50);
        }
    }
    return dmmRangeTable[modeSteps][rangeSteps];
}
uint16_t modeChangeHander(int8_t cmd)
{

    if (cmd<0)
    {
        if(modeSteps>0)
        {
            modeSteps--;
            rangeSteps=0;
            tone(15,2800,50);
        }
    }
    else if  (modeSteps<N_MODES-1)
    {
        modeSteps++;
        rangeSteps=0;
        tone(15,2800,50);
    }
    return dmmRangeTable[modeSteps][rangeSteps];
}

