#include <Servo.h>

Servo 	servo;

int     microphonePin = 5;
int     ledPinMin     = 3;
int     ledCount      = 6;
int 	  servoPin      = 9;

int 	  rawVolume;
float   volume;

char* ledState = NULL;
char* desiredLedState = NULL;

char debugBuffer[256];

#define ON HIGH
#define OFF LOW

void setup()
{

  Serial.begin(9600); //set baud rate
  //set pins attached to LEDs as outputs
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  servo.attach(servoPin);
  
  ledState = (char*)malloc(ledCount * sizeof(char));
  desiredLedState = (char*)malloc(ledCount * sizeof(char));
  memset(ledState, OFF, ledCount * sizeof(char));
  memset(desiredLedState, OFF, ledCount * sizeof(char));
  ClearAllLEDs();
}

void DebugLedCode()
{
  LightLEDRange(0);
  FlushLEDState();
  delay(1000);
  LightLEDRange(1);
  FlushLEDState();
  delay(1000);
  LightLEDRange(2);
  FlushLEDState();
  delay(1000);
  LightLEDRange(3);
  FlushLEDState();
  delay(1000);
  LightLEDRange(4);
  FlushLEDState();
  delay(1000);
  LightLEDRange(5);
  FlushLEDState();
  delay(1000);
  LightLEDRange(6);
  FlushLEDState();
  delay(2000);
  LightLEDRange(3);
  FlushLEDState();
  delay(2000);
}

void loop()
{
  volume = ReadVolume();
  Serial.println(volume);       // debug the calculated value
  if (volume > 0.2f)
  {
    LightLEDRange(4);
  }
  else
  {
    LightLEDRange(0);
  }
  FlushLEDState();

  //glow the LEDs depending on the ammount of sound detected by the electret
  // if (volume > 0.0f && volume < 0.17f)
  // { //glow first LED
  //   Clear(4);
  //   Glow(4);
  //   servo.write(120);
  // }
  // else if (volume > 0.16f && volume < 0.33f)
  // { //glow 2nd LED
  //   Clear(5);
  //   Glow(5);
  //   servo.write(100);
  // }
  // else if (volume > 0.33f && volume < 0.5f)
  // { //glow 3rd LED
  //   Clear(6);
  //   Glow(6);
  //   servo.write(180);
  // }
  // else if (volume > 0.5f && volume < 0.66f)
  // { //glow 4th LED
  //   Clear(7);
  //   Glow(7);
  //   servo.write(90);
  // }
  // else if (volume > 0.83f && volume < 1.0f)
  // { //glow 5th LED
  //   Clear(8);
  //   Glow(8);
  //   servo.write(50);
  // }
  // else if (volume < 177 || volume > 831)
  // { //glow 6th LED
  //   Clear(9);
  //   Glow(9);
  //   servo.write(40);
  // }

}

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

void ClearAllLEDs()
{
  for (int i = ledPinMin; i <= ledPinMin + ledCount; i++)
  {
    digitalWrite(i, LOW);
  }
}
void TurnLedOn(int offset)
{
  if (offset > ledCount || offset < 0)
  {
    Serial.println("Invalid offset in TurnLedOn");
    return;
  }
  desiredLedState[offset] = ON;
}

void TurnLedOff(int offset)
{
  if (offset > ledCount || offset < 0)
  {
    Serial.println("Invalid offset in TurnLedOff");
    return;
  }
  desiredLedState[offset] = OFF;
}

void LightLEDRange(int count)
{
  if (count > ledCount || count < 0)
  {
    Serial.println("Invalid count in LightLEDRange");
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
// void Glow(int initial)//function to glow LEDs
// {
//   for (int i = 3; i < initial; i++)
//     digitalWrite(i, HIGH);
// }

// void Clear(int initial)//function to clear LEDs
// {
//   for (int i = initial; i < 9; i++)
//   digitalWrite(i, LOW);
// }
