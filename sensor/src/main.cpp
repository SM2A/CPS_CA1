#include <Arduino.h>
#include <Wire.h>

#define BAUD_RATE 9600

#define SHT25_ADDR 0x40
#define HUM_COMMAND 0xF5
#define TEMP_COMMAND 0xF3
#define MAX_SPEED 255
#define MIN_DATA_SIZE 2
#define TEMPERATURE 'T'
#define HUMIDITY 'H'

float lastTemp = 0, lastHum = 0;
const int BT_SERIAL_RX = 0, BT_SERIAL_TX = 1;

void setup()
{
  Wire.begin();
  Serial.begin(BAUD_RATE);
}

void loop()
{
  unsigned int data[2];

  Wire.beginTransmission(SHT25_ADDR);
  Wire.write(HUM_COMMAND);
  Wire.endTransmission();

  delay(500);
  Wire.requestFrom(SHT25_ADDR, MIN_DATA_SIZE);

  if (Wire.available() >= MIN_DATA_SIZE)
  {
    data[0] = Wire.read();
    data[1] = Wire.read();
    float humidity = (((data[0] * 256.0 + data[1]) * 125.0) / 65536.0) - 6;

    if (lastHum == 0 || abs(humidity - lastHum) / lastHum > 0.05)
    {
      Serial.print("H");
      Serial.println(humidity);
      lastHum = humidity;
    }
  }

  Wire.beginTransmission(SHT25_ADDR);
  Wire.write(TEMP_COMMAND);
  Wire.endTransmission();

  delay(500);
  Wire.requestFrom(SHT25_ADDR, MIN_DATA_SIZE);

  if (Wire.available() >= MIN_DATA_SIZE)
  {
    data[0] = Wire.read();
    data[1] = Wire.read();
    float cTemp = (((data[0] * 256.0 + data[1]) * 175.72) / 65536.0) - 46.85;

    if (lastTemp == 0 || abs(cTemp - lastTemp) / lastTemp > 0.05)
    {
      Serial.print("T");
      Serial.println(cTemp);
      lastTemp = cTemp;
    }
  }

  delay(300);
}