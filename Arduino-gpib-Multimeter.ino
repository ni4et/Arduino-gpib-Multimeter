

//#include <gpio.h>
//#include <serialno.h>
//#include <spi.h>
//#include <utils.h>

#include <Wire.h> // Enable this line if using Arduino Uno, Mega, etc.
//#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"
#include "Knobs.h"

Adafruit_7segment msbDisp = Adafruit_7segment();
Adafruit_7segment lsbDisp = Adafruit_7segment();
Adafruit_AlphaNum4 unitsDisp = Adafruit_AlphaNum4();

#define PIN_TONE 15

#include <DMMShield.h>
#include <eprom.h>
#include <errors.h>
#include <dmm.h>
#include "gpio.h"

DMMShield dmm;
extern int knobCount;

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
  setupKnobs();
  // Try to tone
  pinMode(PIN_TONE,OUTPUT);
  digitalWrite(PIN_TONE,HIGH);
  
  tone(PIN_TONE,2500,1000);
}

char buf[16];

void loop()
{
  int ai;
uint16_t stat;
  char * cp;


  double val;
  uint8_t err;
  
  loopKnobs();
  dmm.CheckForCommand();
  stat=digitalRead(PIN_SPI_MISO);
  if (stat==HIGH)
  {
    val=DMM_DGetValue(&err);


    DMM_FormatValue(val,buf,1);
    //Serial.print(err);
    //Serial.print(" ");
    //Serial.println(buf);
    if ((err == ERRVAL_SUCCESS) && (val != INFINITY) && (val != -INFINITY )&& !DMM_IsNotANumber(val))
    {

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
  delay(100);
}
