

#include <gpio.h>
#include <serialno.h>
#include <spi.h>
#include <utils.h>

#include <Wire.h> // Enable this line if using Arduino Uno, Mega, etc.
// #include <Adafruit_GFX.h>
#include "Common.h"

Adafruit_7segment msbDisp = Adafruit_7segment();
Adafruit_7segment lsbDisp = Adafruit_7segment();
Adafruit_AlphaNum4 unitsDisp = Adafruit_AlphaNum4();

#include <eprom.h>
#include <errors.h>
#include <dmm.h>
#include "gpio.h"

DMMShield dmm;
#define ERRVAL_NODATA (1)
unsigned long exRun; // Used to schedule DMM

double val; // Most recent valid conversion
uint8_t conversionError;

double sumVal; // Averaging for the display.
uint16_t sumCount;

// dmm.cpp definition.  I need it here but wasn't declared outside of source.
uint8_t DMM_GetScaleUnit(int idxScale, double *pdScaleFact, char *szUnitPrefix, char *szUnit);

uint32_t serialBaud = 9600;

void writeUnits(const char *unit) // Expecting 4 characters
{
  int i;
  unitsDisp.clear();

  for (i = 0; i < 4 && *unit; i++, unit++)
  {
    unitsDisp.writeDigitAscii(i, (uint8_t)*unit, false);
  }
  unitsDisp.writeDisplay();
}
void writeNumber(const char *nstring) // Expecting a string of at least 8 number/spaces and possibly a decimal point
{
  uint16_t pos = 0;
  uint16_t end;
  uint8_t c;
  bool dot = false;
  static uint8_t ma[] = {0, 1, 3, 4, 0, 1, 3, 4}; // Write order for digit display

  // Start with clear buffer
  msbDisp.clear();
  lsbDisp.clear();

  // A sign, if present is written to pos 0,
  // blank otherwise

  for (end = 0; end < 8 && nstring[end] != ' '; end++)
    ;

  // Scan the input starting after the sign
  for (pos = 8 - end; pos < 8 && *nstring != '\0'; pos++)
  {
    dot = false;
    c = *nstring++; // get the next character

    if (*nstring == '.')
    {
      dot = true;
      nstring++; // skip any period
    }

    // Work the msb
    if (pos < 4)
    {
      if (c < '0' || c > '9') // Not a printable number
        msbDisp.writeDigitRaw(ma[pos], (c == '-') ? 0x40 : 0);
      else
        msbDisp.writeDigitNum(ma[pos], c - '0', dot); // Write a digit
    }
    // Work the lsb
    else
    {
      if (c < '0' || c > '9') // Not a printable number
        lsbDisp.writeDigitRaw(ma[pos], (c == '-') ? 0x40 : 0);
      else
        lsbDisp.writeDigitNum(ma[pos], c - '0', dot); // Write a digit
    }
  }
  msbDisp.writeDisplay();
  lsbDisp.writeDisplay();
}

void setup()
{
  // put your setup code here, to run once:
  lsbDisp.begin(0x71);   // For the seven segment.
  msbDisp.begin(0x70);   // For the seven segment.
  unitsDisp.begin(0x72); // For the seven segment.

  Serial.begin(serialBaud);
  Serial.println("Hello: (20250519)");
  DMM_Init();
  dmm.begin(&Serial);

  writeUnits("Helo");
  //	dmm.ProcessIndividualCmd("DMMSetScale VoltageDC5");
  dmm.SetScale(DmmDCVoltage_5e1);

  setupKnobs();
  // Try to tone
  pinMode(PIN_TONE, OUTPUT);
  digitalWrite(PIN_TONE, HIGH);

  tone(PIN_TONE, 800, 100);
  exRun = millis();
}

//******************************
bool processKnobs() // True if a knob was turned
{
  bool ret = false;
  KNOB_EVENT ke = loopKnobs();
  // Serial.print("KE: "); Serial.println(ke);
  switch (ke)
  {
  case KE_MODE_DN:
    dmm.SetScale(modeChangeHander(-1));
    ret = true;
    break;
  case KE_MODE_UP:
    dmm.SetScale(modeChangeHander(1));
    ret = true;
    break;
  case KE_RANGE_DN:
    dmm.SetScale(rangeChangeHander(-1));
    ret = true;
    break;
  case KE_RANGE_UP:
    dmm.SetScale(rangeChangeHander(1));
    ret = true;
    break;
  default:
    break;
  }
  return ret;
}

//**************************
uint8_t loopADC(double *newVal)
{
  uint16_t stat = 1; // Default value for no-reading available
  uint8_t err = ERRVAL_NODATA;
  double val = NAN;

  stat = digitalRead(PIN_SPI_MISO);
  if (stat == HIGH)
  {
    val = DMM_DGetValue(&err);
    if (err == ERRVAL_SUCCESS)
    {
      // Modification to limit reading to a value in range.
      // No need to check if there is already an error.
      // A value can still be out of range so we really dont want it in that case.

      double range = DMM_GetScaleRange(0);

      range *= 1.1;
      if (val > range)
      {
        val = INFINITY;
        err = ERRVAL_CALIB_NANDOUBLE;
      }
      if (val < -range)
      {
        val = -INFINITY;
        err = ERRVAL_CALIB_NANDOUBLE;
      }
    }
  }
  if (err == ERRVAL_SUCCESS)
  {
    *newVal = val; // Only if its good
  }
  return err;
}

//******************************
void loopDisplay(uint8_t err, double val)
{
  char *cp;
  static char displayBuffer[16]; // Used in loopDisplay()
  // Serial.print(err);
  // Serial.print(" ");

  if ((err == ERRVAL_SUCCESS) && (val != INFINITY) && (val != -INFINITY) && !DMM_IsNotANumber(val))
  {
    DMM_FormatValue(val, displayBuffer, 1); // Format the value into displayBuffer and add the units.

    // Serial.println(displayBuffer);

    writeNumber(displayBuffer); // TO LED
    cp = displayBuffer + 4;
    for (; *cp && *cp < 'A'; cp++)
      ; // Find the first character that is not a number
    writeUnits(cp);
    // Serial.print("Display Buffer: ");
    // Serial.println(displayBuffer);
  }
  else
  {
    // Serial.print("Display Buffer: ");
    // Serial.println(displayBuffer);

    writeNumber("------ ");
    writeUnits("OPEN");
  }
}

//******************************
bool processCommand(String &cmdBuf) // True if range change
{
  bool rangeChanged = false;

  cmdBuf.trim();

  if (cmdBuf.equalsIgnoreCase(String("*idn?")))
    Serial.println("Homebrew GPIB DMM");
  else if (cmdBuf.equalsIgnoreCase(String("val?")))
  {
    char formatBuf[16];
    DMM_FormatValue(val, formatBuf, 1);
    Serial.println(formatBuf);
  }
  else if (cmdBuf.equalsIgnoreCase(String("mode?")))
  {
    int idxScale = DMM_GetCurrentScale(); /// Gets current scale
    double sf;
    double rng;
    char prefix[4];
    char unit[8];
    DMM_GetScaleUnit(idxScale, &sf, prefix, unit);
    rng = DMM_GetScaleRange(idxScale);
    // Serial.print(sf * rng);
    // Serial.print(" ");
    // Serial.print(prefix);
    // Serial.println(unit);
  }
  else if (cmdBuf.substring(0).equalsIgnoreCase(String("mode ")))
  {
    rangeChanged = true;
    // Serial.print("Processing mode command: ");Serial.println( cmdBuf);
    String dmmCommand;
    cmdBuf = cmdBuf.substring(5); // "mode "
    cmdBuf.trim();                // Leading spaces
    dmmCommand.reserve(20);
    dmmCommand = "DMMSetScale "; // Start building

    // Find the voltage/current/resistance range
    int subRange = cmdBuf.toInt(); // 5,50,500 or 0
    switch (subRange)
    {
    case 500:
      cmdBuf = cmdBuf.substring(3);
      break;
    case 50:
      cmdBuf = cmdBuf.substring(2);
      break;
    case 5:
      cmdBuf = cmdBuf.substring(1);
      break;
    default:
      break;
    } // We can encode the # directly into the command;

    // we could have M, k or k, m, u or nothing
    // If the command includes a scaling character figure out its meaning, ie 500m

    char sf = cmdBuf[0];
    switch (sf)
    {
    case 'M':
      cmdBuf = cmdBuf.substring(1);
      break;
    case 'K':
    case 'k':
      sf = 'k';
      cmdBuf = cmdBuf.substring(1);
      break;
    case 'm':
      cmdBuf = cmdBuf.substring(1);
      break;
    case 'u':
    case 'U':
      cmdBuf = cmdBuf.substring(1);
      break;
    default:
      sf = '\0';
      break;
    }
    // Serial.println("cmdBuf so far:"+cmdBuf);

    // We could have v, vac, i, iac, ohms, diode, or cont.

    cmdBuf.toLowerCase();

    // Serial.println(String("Post sr:")+String(subRange,10)+String( "sf:")+String(sf)+String(" remaining:")+cmdBuf+String(":"));

    if (cmdBuf == "v")
      dmmCommand += "VoltageDC";
    else if (cmdBuf == "vdc")
      dmmCommand += "VoltageDC";
    else if (cmdBuf == "vac")
      dmmCommand += "VoltageAC";
    else if (cmdBuf == "a")
      dmmCommand += "CurrentDC";
    else if (cmdBuf == "adc")
      dmmCommand += "CurrentDC";
    else if (cmdBuf == "aac")
      dmmCommand += "CurrentAC";
    else if (cmdBuf == "ohm")
      dmmCommand += "Resistance";
    else if (cmdBuf == "diode")
      dmmCommand += "Diode";
    else if (cmdBuf == "cont")
      dmmCommand += "Continuity";
    else
    {
      Serial.println("unrecognized:" + cmdBuf);
      subRange = 0;
    }

    // Serial.println("Built so far:", dmmCommand)
    //  Finish the range:
    if (subRange > 0)
    {
      dmmCommand += String(subRange);
      if (sf)
        dmmCommand += String(sf);
    }

    // Serial.println(String("generated:")+dmmCommand);
    Serial.end();
    dmm.ProcessIndividualCmd(dmmCommand.c_str());
    rangeChanged = true;

    Serial.begin(serialBaud);
  }
  else
  {
    // Serial.println("Passing direct to dmm:"+cmdBuf);
    dmm.ProcessIndividualCmd(cmdBuf.c_str());
    rangeChanged = true;
  }
  return rangeChanged;
}
//******************************
bool checkSerialCommand() // Return true if s range change happened/
{
  bool rv = false;
  static String cmdBuf;

  if (Serial.available() > 0)
  {
    char key = Serial.read();

    switch (key)
    {
    case '\n':
    case '\r':
      // Serial.println("");
      // Serial.print(cmdBuf.length());Serial.println(" ");Serial.println(cmdBuf);
      // if (echo)
      //  Serial.println();
      if (cmdBuf.length() > 0)
      {
        rv = processCommand(cmdBuf);
        cmdBuf = "";
      }
    case '\b':
    case '\x7f':
      if (cmdBuf.length() > 0)
      {
        cmdBuf.remove(cmdBuf.length() - 1);
        Serial.print("\b \b");
      }
      break;
    default:
      cmdBuf += key;
      break;
    }
    return rv; // Indicating no range change
  }
  return false; // TODO - Broken apparently
}
//******************************
void loop()
{
  static unsigned long nextReportDue = millis() + 3000; // When to finish averaging and display it.

  if (processKnobs()) // True if a knob changed range
  {

    double fullScaleOfRange = DMM_GetScaleRange(DMM_GetCurrentScale());

    loopDisplay(ERRVAL_SUCCESS, fullScaleOfRange);
    sumVal = 0.0;
    sumCount = 0;
    // Serial.println("Process Knobs");
    nextReportDue = millis() + 1000;
  }
  if (checkSerialCommand()) // Got one from serial
  {
    // Need the full scale value here
    double fullScaleOfRange = DMM_GetScaleRange(DMM_GetCurrentScale());

    // Same as above:
    loopDisplay(ERRVAL_SUCCESS, fullScaleOfRange);
    sumVal = 0.0;
    sumCount = 0;
    nextReportDue = millis() + 1000;
    // Serial.println("Check serial command");
  }

  // See if new data:
  double valBuf; // Hold the last reading.
  if (loopADC(&valBuf) != ERRVAL_NODATA)
  { // IOW we have new data

    sumVal += valBuf;
    sumCount++;
    if (millis() > (nextReportDue)) // has one second elapsed?
    {
      // Serial.print("Conversion results: ");
      // Serial.print(sumVal);
      // Serial.print("/");
      // Serial.print(sumCount);
      // Serial.print(" ");
      // Serial.print(nextReportDue);
      // Serial.print(" ");
      // Serial.println(millis());

      loopDisplay(ERRVAL_SUCCESS, (sumCount == 0) ? val : sumVal / sumCount);
      sumVal = 0.0;
      sumCount = 0;
      nextReportDue = millis() + 1000;
    }
  }
  else
  {
    // some sort of error`
  }
}
