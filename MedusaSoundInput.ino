#include <Servo.h>

Servo 	servo;
int 	servoPin = 9;
int 	rawVolume;
float   volume;

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
  
}
void loop()
{
  rawVolume = analogRead(5); 				// read raw value from microphone
  volume = ((float)rawVolume / 1024.0f); 	// translate raw value range to (0.0f -> 1.0f)
  volume = volume - 0.5f; 					// translate raw value range to (-0.5f -> 0.5f)
  volume = fabs(volume); 					// absolute value for a (0.0f -> 0.5f) range
  volume *= 2.0f; 							// scale the value to a (0.0f -> 1.0f) range
  char buffer[256];

  snprintf(buffer, 256, "volume: %f%%", volume * 100.0f);
  // Serial.println(buffer);  			// debug the calculated value
  Serial.println(volume);  			// debug the calculated value

  //glow the LEDs depending on the ammount of sound detected by the electret
  if (volume > 0.0f && volume < 0.17f)
  { //glow first LED
    Clear(4);
    Glow(4);
    servo.write(120);
  }
  else if (volume > 0.16f && volume < 0.33f)
  { //glow 2nd LED
    Clear(5);
    Glow(5);
    servo.write(100);
  }
  else if (volume > 0.33f && volume < 0.5f)
  { //glow 3rd LED
    Clear(6);
    Glow(6);
    servo.write(180);
  }
  else if (volume > 0.5f && volume < 0.66f)
  { //glow 4th LED
    Clear(7);
    Glow(7);
    servo.write(90);
  }
  else if (volume > 0.83f && volume < 1.0f)
  { //glow 5th LED
    Clear(8);
    Glow(8);
    servo.write(50);
  }
  else if (volume < 177 || volume > 831)
  { //glow 6th LED
    Clear(9);
    Glow(9);
    servo.write(40);
  }

}

void Glow(int initial)//function to glow LEDs
{
  for (int i = 3; i < initial; i++)
    digitalWrite(i, HIGH);
}

void Clear(int initial)//function to clear LEDs
{
  for (int i = initial; i < 9; i++)
  digitalWrite(i, LOW);
}
