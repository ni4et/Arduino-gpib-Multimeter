void setupKnobs();

void loopKnobs();


typedef enum DMM_RANGE
{ // THIS MUST MATCH DMMCFG dmmcfg[]  in dmm.c!!!!
    DmmResistance_5e7,
    DmmResistance_5e6,
    DmmResistance_5e5,
    DmmResistance_5e4,
    DmmResistance_5e3,
    DmmResistance_5e2,
    DmmResistance_5e1,
    DmmDCVoltage_5e1,
    DmmDCVoltage_5e0,
    DmmDCVoltage_5em1,
    DmmDCVoltage_5em2,
    DmmACVoltage_5e1,
    DmmACVoltage_5e0,
    DmmACVoltage_5em1,
    DmmACVoltage_5em2,
    DmmDCCurrent_5e0,
    DmmACCurrent_5e0,
    DmmContinuity,
    DmmDiode,
    DmmDCLowCurrent_5em1,
    DmmDCLowCurrent_5em2,
    DmmDCLowCurrent_5em3,
    DmmDCLowCurrent_5em4,
    DmmACLowCurrent_5em1,
    DmmACLowCurrent_5em2,
    DmmACLowCurrent_5em3,
    DmmACLowCurrent_5em4
} DMMRange_t;
