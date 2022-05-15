#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include "RTClib.h"
#include <OneWire.h>
#include <DallasTemperature.h>

const int chipSelect = 4;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

RTC_DS1307 rtc;

OneWire oneWire(2);
DallasTemperature sensors(&oneWire);


void setup() {
  Serial.begin(9600);
  sensors.begin();

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    while (1);
  }
  Serial.println("card initialized.");
}

void loop() {
  DateTime now = rtc.now();
  String date = String(String(now.day()) + "." + String(now.month()) + "." + now.year()) + " " + String(daysOfTheWeek[now.dayOfTheWeek()]) + " " + String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second());
  sensors.requestTemperatures();
  float tmp = sensors.getTempCByIndex(0);

  String dataString = (date + " Temperature: " + String (tmp) + " Celsium");

  File dataFile = SD.open("datalog.txt", FILE_WRITE);
  if(dataFile){
    dataFile.println(dataString);
    dataFile.close();
    Serial.println(dataString);
  }
  else
    Serial.println("error opening datalog.txt");
  delay(60000);
}
