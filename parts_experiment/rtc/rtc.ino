#include <Wire.h>
#include "RTClib.h"

RTC_DS1307 rtc;

void setup () 
{
  Serial.begin(9600);

  if (!rtc.begin())
  {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (!rtc.isrunning())
  {
    Serial.println("RTC lost power, lets set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
}

void loop () 
{
    DateTime now = rtc.now();

    String cur = String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second());
    String tar = "20:10:30";
    String tar2 = "20:10:40";

    if (cur == tar)
    {
      Serial.println("beep!");
    } 
    if (cur == tar2)
    {
      Serial.println("silent!");
    }

    delay(1000);   
}
