#include <Servo.h>

Servo 	servo;

// pins
int     microphonePin = A5;
int     ledPinMin     = 3;
int     ledCount      = 6;
int 	  servoPin      = 9;

// volume
int 	  rawVolume;
float   volume;

// arrays containing the desired and actual states of LEDs used by FlushLEDState
char*   ledState = NULL;
char*   desiredLedState = NULL;

// heigh value smoothed from the volume input using heightIncrement and heightDecrement
float   height = 0.0f;
float   heightIncrement = 0.1;
float   heightDecrement = 0.01;

char debugBuffer[256];

#define ON HIGH
#define OFF LOW

void setup()
{

  Serial.begin(9600);

  //set pins attached to LEDs as outputs
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  servo.attach(servoPin);

  // Initializing ledState arrays  
  ledState = (char*)malloc(ledCount * sizeof(char));
  desiredLedState = (char*)malloc(ledCount * sizeof(char));
  memset(ledState, OFF, ledCount * sizeof(char));
  memset(desiredLedState, OFF, ledCount * sizeof(char));

  ClearAllLEDs();
}

void loop()
{
  volume = ReadVolume();

  if (volume > 0.2f)
  {
    height += heightIncrement;
  }
  else
  {
    height -= heightDecrement;
  }

  if (height < 0.0f)
    height = 0.0f;
  if (height > 1.0f)
    height = 1.0f;

  int litLedCount = (int)(height * (float)ledCount);
  Serial.println(litLedCount);
  TurnLedRangeOn(litLedCount);
  FlushLEDState();
}

void DebugLedCode()
{
  TurnLedRangeOn(0);
  FlushLEDState();
  delay(1000);
  TurnLedRangeOn(1);
  FlushLEDState();
  delay(1000);
  TurnLedRangeOn(2);
  FlushLEDState();
  delay(1000);
  TurnLedRangeOn(3);
  FlushLEDState();
  delay(1000);
  TurnLedRangeOn(4);
  FlushLEDState();
  delay(1000);
  TurnLedRangeOn(5);
  FlushLEDState();
  delay(1000);
  TurnLedRangeOn(6);
  FlushLEDState();
  delay(2000);
  TurnLedRangeOn(3);
  FlushLEDState();
  delay(2000);
}

// Read the volume from microphonePin and normalize it in a (0.0f -> 1.0f) range
float ReadVolume()
{
  rawVolume = analogRead(microphonePin);        // read raw value from microphone
  // Serial.println(rawVolume);       // debug the calculated value
  volume = ((float)rawVolume / 1024.0f);  // translate raw value range to (0.0f -> 1.0f)
  volume = volume - 0.5f;           // translate raw value range to (-0.5f -> 0.5f)
  volume = fabs(volume);          // absolute value for a (0.0f -> 0.5f) range
  volume *= 2.0f;               // scale the value to a (0.0f -> 1.0f) range
  // Serial.println(volume);       // debug the calculated value

  snprintf(debugBuffer, 256, "volume: %.2f", volume);
  // Serial.println(debugBuffer);        // debug the calculated value
  return volume;
}

// Turn Off All LEDs
void ClearAllLEDs()
{
  for (int i = ledPinMin; i <= ledPinMin + ledCount; i++)
  {
    digitalWrite(i, OFF);
  }
}

// Change the desired state for a LED index to On
void TurnLedOn(int offset)
{
  if (offset > ledCount || offset < 0)
  {
    Serial.println("Invalid offset in TurnLedOn");
    return;
  }
  desiredLedState[offset] = ON;
}

// Change the desired state for a LED index to Off
void TurnLedOff(int offset)
{
  if (offset > ledCount || offset < 0)
  {
    Serial.println("Invalid offset in TurnLedOff");
    return;
  }
  desiredLedState[offset] = OFF;
}


// Change the desired state for a LED range to On
void TurnLedRangeOn(int count)
{
  if (count > ledCount || count < 0)
  {
    Serial.println("Invalid count in TurnLedRangeOn");
    return;
  }
  for (int i = 0; i < ledCount; i++)
  {
    if (i < count)
    {
      TurnLedOn(i);
    }
    else
    {
      TurnLedOff(i);
    }
  }
}

// Compare ledState array with desiredLedState array to actually turn the LEDs On or Off
void FlushLEDState()
{
  for (int i = 0; i < ledCount; i++)
  {
    if (desiredLedState[i] != ledState[i])
    {
      if (desiredLedState[i] == ON)
      {
        Serial.println("Turning on LED ");
        Serial.println(i);
        digitalWrite(ledPinMin + i, ON);
      }
      else
      {
        Serial.println("Turning off LED ");
        Serial.println(i);
        digitalWrite(ledPinMin + i, OFF);
      }
      ledState[i] = desiredLedState[i];
    }
  }
}

