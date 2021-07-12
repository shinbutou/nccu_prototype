#include <Wire.h>
#include "RTClib.h"

RTC_DS1307 rtc;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

const int buzzer = 13;

void setup () 
{
  Serial.begin(9600);
  delay(3000); // wait for console opening

  // Buzzer Setup
  pinMode(buzzer, OUTPUT);

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (!rtc.isrunning()) {
    Serial.println("RTC lost power, lets set the time!");
  
  // Comment out below lines once you set the date & time.
    // Following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  
    // Following line sets the RTC with an explicit date & time
    // for example to set January 27 2017 at 12:56 you would call:
    // rtc.adjust(DateTime(2017, 1, 27, 12, 56, 0));
  }
}

void loop () 
{
    // RTC initialization
    DateTime now = rtc.now();

    // for display
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();

    String cur = String(now.hour()) + ":" + String(now.minute());
    String tar = "20:3";

    // Get Time
    // Serial.println(cur);

    if (cur == tar)
    {
      tone(buzzer, 1000);
      Serial.println("beep!");
    } else {
      Serial.println("silent!");
    }

    // Operating at Every Second
    delay(1000);   
}
