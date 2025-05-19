
#include "Common.h"

// Imported from Knob.cpp
int16_t rangeSteps;
int16_t modeSteps;

DMM_RANGE dmmRangeTable[][8] =
    {
        {DmmDCVoltage_5e1, DmmDCVoltage_5e0, DmmDCVoltage_5em1, DmmDCVoltage_5em2,
         DMMInvalidSetting, DMMInvalidSetting, DMMInvalidSetting, DMMInvalidSetting},
        {DmmACVoltage_5e1, DmmACVoltage_5e0, DmmACVoltage_5em1, DmmACVoltage_5em2,
         DMMInvalidSetting, DMMInvalidSetting, DMMInvalidSetting, DMMInvalidSetting},
        {DmmResistance_5e7, DmmResistance_5e6, DmmResistance_5e5, DmmResistance_5e4,
         DmmResistance_5e3, DmmResistance_5e2, DmmResistance_5e1, DMMInvalidSetting},

        {DmmDCCurrent_5e0, DmmDCLowCurrent_5em1, DmmDCLowCurrent_5em2, DmmDCLowCurrent_5em3,
         DmmDCLowCurrent_5em4, DMMInvalidSetting, DMMInvalidSetting, DMMInvalidSetting},
        {DmmACCurrent_5e0, DmmACLowCurrent_5em1, DmmACLowCurrent_5em2, DmmACLowCurrent_5em3,
         DmmACLowCurrent_5em4, DMMInvalidSetting, DMMInvalidSetting, DMMInvalidSetting},
        {DmmContinuityRange, DmmDiodeRange, DMMInvalidSetting, DMMInvalidSetting,
         DMMInvalidSetting, DMMInvalidSetting, DMMInvalidSetting, DMMInvalidSetting}};

#define N_MODES sizeof(dmmRangeTable) / sizeof(dmmRangeTable[0])
#define N_RANGES sizeof(dmmRangeTable[0]) / sizeof(dmmRangeTable[0][0])

// Call to get the DMM_RANGE
// passed parameter cmd can be either -1 or +1

uint16_t rangeChangeHander(int8_t cmd)
{

    if (cmd < 0)
    {
        if (rangeSteps > 0)
        {
            rangeSteps--;
            tone(PIN_TONE, 1600, 50);
        }
        else
        {
            tone(PIN_TONE, 300, 100);
        }
    }
    else if (cmd > 0)
    { // Test for valid range
        if (dmmRangeTable[modeSteps][rangeSteps + 1] > 0 && (rangeSteps + 1) < N_RANGES)
        {
            rangeSteps++;
            tone(PIN_TONE, 800, 50);
        }
        else
        {
            tone(PIN_TONE, 300, 100);
        }
    }

    return dmmRangeTable[modeSteps][rangeSteps];
}
uint16_t modeChangeHander(int8_t cmd)
{

    if (cmd < 0)
    {
        if (modeSteps > 0) //  can go down
        {
            modeSteps--;
            rangeSteps = 0;
            tone(PIN_TONE, 2800, 50);
        }
        else
        {
            tone(PIN_TONE, 300, 100);
        }
    }
    else if ((modeSteps + 1) < N_MODES)
    {
        modeSteps++;
        rangeSteps = 0;
        tone(PIN_TONE, 1400, 50);
    }
    else
    {
        tone(PIN_TONE, 300, 100);
    }

    return dmmRangeTable[modeSteps][rangeSteps];
}
