

//#include <gpio.h>
//#include <serialno.h>
//#include <spi.h>
//#include <utils.h>

#include <Wire.h> // Enable this line if using Arduino Uno, Mega, etc.
//#include <Adafruit_GFX.h>
#include "Common.h"

Adafruit_7segment msbDisp = Adafruit_7segment();
Adafruit_7segment lsbDisp = Adafruit_7segment();
Adafruit_AlphaNum4 unitsDisp = Adafruit_AlphaNum4();


#include <eprom.h>
#include <errors.h>
#include <dmm.h>
#include "gpio.h"

DMMShield dmm;

unsigned long exRun; // Used to schedule DMM

typedef enum RUN_MODE {RM_DMM=0,RM_REMOTE,RM_CMD } RUN_MODE_t;
RUN_MODE_t runMode=RM_DMM;


uint8_t ma[]={0,1,3,4,0,1,3,4};

void writeUnits(const char * unit) // Expecting 4 characters
{
  int i;
  unitsDisp.clear();

  for (i=0;i<4  && *unit;i++,unit++)
  {
    unitsDisp.writeDigitAscii(i,(uint8_t) *unit,false);
  }
  unitsDisp.writeDisplay();

}
void writeNumber(const char * nstring) // Expecting a string of at least 8 number/spaces and possibly a decimal point
{
  uint16_t pos=0;
  uint16_t end;
  uint8_t c;
  bool dot=false;

  // Start with clear buffer
  msbDisp.clear();
  lsbDisp.clear();

  // A sign, if present is written to pos 0,
  // blank otherwise

  for (end=0;end<8 && nstring[end]!=' ';end++);

  // Scan the input starting after the sign
  for (pos=8-end;pos<8 && *nstring!='\0';pos++)
  {
    dot=false;
    c=*nstring++; // get the next character

    if (*nstring=='.')
    {
      dot=true;
      nstring++; // skip any period
    }

    // Work the msb
    if (pos<4)
    {
      if (c<'0' || c>'9') // Not a printable number
        msbDisp.writeDigitRaw(ma[pos],(c=='-')?0x40:0);
      else
        msbDisp.writeDigitNum(ma[pos],c-'0',dot); // Write a digit
    }
    // Work the lsb
    else
    {
      if (c<'0' || c>'9') // Not a printable number
        lsbDisp.writeDigitRaw(ma[pos],(c=='-')?0x40:0);
      else
        lsbDisp.writeDigitNum(ma[pos],c-'0',dot); // Write a digit
    }
  }
  msbDisp.writeDisplay();
  lsbDisp.writeDisplay();
}


void setup()
{
    // put your setup code here, to run once:
      lsbDisp.begin(0x71); // For the seven segment.
      msbDisp.begin(0x70); // For the seven segment.
      unitsDisp.begin(0x72); // For the seven segment.

      Serial.begin(9600);
      Serial.println("Hello:");
      DMM_Init();
  dmm.begin(&Serial);

  writeUnits("Helo");
  //	dmm.ProcessIndividualCmd("DMMSetScale VoltageDC5");
  dmm.SetScale(DmmDCVoltage_5e1);

  setupKnobs();
  // Try to tone
  pinMode(PIN_TONE,OUTPUT);
  digitalWrite(PIN_TONE,HIGH);
  
  tone(PIN_TONE,2500,1000);
  exRun=millis();
  runMode=RM_DMM;

}

char buf[16];

void loop()
{
  int ai;
uint16_t stat;
  char * cp;


  double val;
  uint8_t err;
  
  KNOB_EVENT ke=loopKnobs();
  //Serial.print("KE: "); Serial.println(ke);
  switch( ke )
  {
    case KE_MODE_DN:
        dmm.SetScale(modeChangeHander(-1));
      break;
    case KE_MODE_UP:
         dmm.SetScale(modeChangeHander(1));
      break;
   case KE_RANGE_DN:
         dmm.SetScale(rangeChangeHander(-1));
      break;
   case KE_RANGE_UP:
        dmm.SetScale(rangeChangeHander(1));
      break;
    default:
      break;
  }
  
  //dmm.CheckForCommand();
  stat=digitalRead(PIN_SPI_MISO);
  if (stat==HIGH)
  {
    val=DMM_DGetValue(&err);
    if (err==ERRVAL_SUCCESS)
    {
      // Modification to limit reading to a value in range.
      // No need to check if there is already an error.
      // A value can still be out of range so we really dont want it in that case.

      double range=DMM_GetScaleRange(0);

      range*=1.1;
      if (val>range) val=INFINITY;
      if (val< -range) val=-INFINITY;
    }
    //Serial.print(range);Serial.println(val);

    //Serial.print(err);
    //Serial.print(" ");
    if ((err == ERRVAL_SUCCESS) && (val != INFINITY) && (val != -INFINITY )&& !DMM_IsNotANumber(val))
    {
      DMM_FormatValue(val,buf,1);
      //Serial.println(buf);

      writeNumber(buf);  // TO LED
      cp=buf+4;
      for (;*cp && *cp<'A';cp++);
      writeUnits(cp);
    }
    else
    {
      writeNumber("--------");
      writeUnits(" Err");
    }
  }
  {
    // Limit how fast we can loop:
    unsigned long exNow=millis();
    unsigned long diff=exNow-exRun; // Et from last time
    // Diff should always be positive here
    unsigned long tWait=500-diff;  // tWait cant be negative.

    if (tWait>1000) tWait=1000;
   
    exRun=exNow;
    delay(100);

  }
}

#ifdef NEVER
void loop()
{
  switch (runMode)
  {
  case RM_CMD:
    /* code */
    break;
  case RM_REMOTE:
    dmm.CheckForCommand();
    break;
  case RM_DMM:
  default:
    dmmloop();
    if (Serial.available())
      int rc=Serial.read();
      if (rc='A')
        runMode=RM_REMOTE;

    break;
  }
}; //RM_DMM=0,RM_REMOTE,RM_CMD */
#endif
