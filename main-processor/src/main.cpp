#include <Arduino.h>
#include <LiquidCrystal.h>
#include <SoftwareSerial.h>

#define BAUD_RATE 9600

#define MAX_SPEED 255
#define MIN_DATA_SIZE 4 
#define TEMPERATURE 'T'
#define HUMIDITY 'H'

#define LCD_COLUMNS 16
#define LCD_ROWS 4

const int rs = 7, en = 6, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal LCD(rs, en, d4, d5, d6, d7);

const int tx = 13, rx = 12;
SoftwareSerial Sender(rx, tx);

int irrigationSpeed, irrigationRate;
float humidity, temperature;
bool isNewData;
bool speedFromTempAndHum;

void readNewTemperaturAndHumidity();
void updateMotorSpeed();
void updateBasedOnTemperature();
void updateBasedOnHumidity();
void monitorHandler();


void setup()
{
  // put your setup code here, to run once:
  irrigationRate = 0;
  irrigationSpeed = 0;
  isNewData = false;
  speedFromTempAndHum = false;
  LCD.begin(LCD_COLUMNS, LCD_ROWS);
  Serial.begin(BAUD_RATE);
  Sender.begin(BAUD_RATE);
}

void loop()
{
  if (Serial.available() > MIN_DATA_SIZE)
  {
    readNewTemperaturAndHumidity();
  }
  if (isNewData)
  {
    isNewData = false;
    monitorHandler();
    updateMotorSpeed();
  }
  if (speedFromTempAndHum)
  {
    Sender.print(irrigationSpeed);
    Sender.print("$");
    speedFromTempAndHum = false;
  }
}

void updateMotorSpeed()
{
  if (humidity >= 20 && humidity <= 30)
  {
    updateBasedOnTemperature();
  }
  else
  {
    updateBasedOnHumidity();
  }
}

void updateBasedOnTemperature()
{
  if (temperature < 25)
  {
    irrigationRate = 0;
    irrigationSpeed = 0;
  }
  else
  {
    irrigationRate = 10;
    irrigationSpeed = MAX_SPEED / 10;
  }
}

void updateBasedOnHumidity()
{
  if (humidity > 30)
  {
    irrigationRate = 0;
    irrigationSpeed = 0;
  }
  else if (humidity >= 10 && humidity < 20)
  {
    irrigationRate = 15;
    irrigationSpeed = MAX_SPEED / 5;
  }
  else if (humidity < 10)
  {
    irrigationRate = 20;
    irrigationSpeed = MAX_SPEED / 4;
  }
}

void readNewTemperaturAndHumidity()
{
  char messageType = Serial.read();
  if (messageType == HUMIDITY)
  {
    float newHumidity = Serial.parseFloat();
    isNewData = (newHumidity != humidity);
    if (isNewData)
    {
      humidity = newHumidity;
    }
    speedFromTempAndHum = true;
  }
  else if (messageType == TEMPERATURE)
  {
    float newTemperature = Serial.parseFloat();
    isNewData = (newTemperature != temperature);
    if (isNewData)
    {
      temperature = newTemperature;
    }
  }
}

void monitorHandler()
{
  LCD.clear();
  LCD.setCursor(0, 0);
  LCD.print("Temp=");
  LCD.println(String(temperature).c_str());
  LCD.setCursor(0, 1);
  LCD.print("Hum=");
  LCD.println(String(humidity).c_str());
  LCD.setCursor(0, 2);
  LCD.println("IrrRate=");
  LCD.print(String(irrigationRate));
  LCD.println("CCPM");
}