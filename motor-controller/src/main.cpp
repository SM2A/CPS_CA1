#include <Arduino.h>

#define BAUD_RATE 9600

#define MAX_SPEED 255
#define MIN_DATA_SIZE 2
#define MOTOR_PIN 9

int dutyCycle;

void setup()
{
  dutyCycle = 0;
  Serial.begin(BAUD_RATE);
}

void loop()
{
  if (Serial.available() > MIN_DATA_SIZE){
    auto data = Serial.readStringUntil('$');
    dutyCycle = atoi(data.c_str());
  }

  analogWrite(MOTOR_PIN, dutyCycle);
}