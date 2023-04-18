#include <Arduino.h>

#define BAUD_RATE 9600

#define MAX_SPEED 255
#define MIN_DATA_SIZE 4 
#define TEMPERATURE 'T'
#define HUMIDITY 'H'

const int BT_SERIAL_RX = 0, BT_SERIAL_TX = 1;

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(BAUD_RATE);
}

void loop()
{
  delay(1000);
  Serial.println("T30.0");
  Serial.println("H21.0");
}