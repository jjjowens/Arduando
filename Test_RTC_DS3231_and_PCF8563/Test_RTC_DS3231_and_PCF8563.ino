// Date and time functions using a PCF8563 RTC connected via I2C and Wire lib
#include "RTClib.h"

//[UPDATE ME!!!]
// Select which RTC you are testing, values are:
// RTC_PCF8563 
// RTC_DS3231

RTC_PCF8563 rtc_pcf8563;
RTC_DS3231 rtc_ds3231;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

void setup () {
  Serial.begin(9600);

  if (! rtc_pcf8563.begin()) {
    Serial.println("Couldn't find RTC PCF8563");

    abort();
  }
  if (! rtc_ds3231.begin()) {
    Serial.println("Couldn't find RTC DS3231");

    abort();
  }
  // Delay of 2 seconds to give the RTC enoug time to initialize
  delay(2000);
  
  if (rtc_pcf8563.lostPower()) 
  {
    // Initialize the RTC if it not already initialized
    Serial.println("RTC PCF8563 is NOT initialized, let's set the time!");
    rtc_pcf8563.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
    if (rtc_ds3231.lostPower()) 
  {
    // Initialize the RTC if it not already initialized
    Serial.println("RTC DS3231 is NOT initialized, let's set the time!");
    rtc_ds3231.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  rtc_pcf8563.start();
}

void loop () {

    Serial.println("******PCF8563*****");
    DateTime now = rtc_pcf8563.now();

    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(" (");
    Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
    Serial.print(") ");
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.print(" since midnight 1/1/1970 = ");
    Serial.print(now.unixtime());
    Serial.print("s = ");
    Serial.print(now.unixtime() / 86400L);
    Serial.println("d");

    // calculate a date which is 7 days, 12 hours and 30 seconds into the future
    DateTime future_pcf8563 (now + TimeSpan(7,12,30,6));

    Serial.print(" now + 7d + 12h + 30m + 6s: ");
    Serial.print(future_pcf8563.year(), DEC);
    Serial.print('/');
    Serial.print(future_pcf8563.month(), DEC);
    Serial.print('/');
    Serial.print(future_pcf8563.day(), DEC);
    Serial.print(' ');
    Serial.print(future_pcf8563.hour(), DEC);
    Serial.print(':');
    Serial.print(future_pcf8563.minute(), DEC);
    Serial.print(':');
    Serial.print(future_pcf8563.second(), DEC);
    Serial.println();

    Serial.println();

    Serial.println("******DS3231*****");

    now = rtc_ds3231.now();

    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(" (");
    Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
    Serial.print(") ");
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.print(" since midnight 1/1/1970 = ");
    Serial.print(now.unixtime());
    Serial.print("s = ");
    Serial.print(now.unixtime() / 86400L);
    Serial.println("d");

    // calculate a date which is 7 days, 12 hours and 30 seconds into the future
    DateTime future_ds3231 (now + TimeSpan(7,12,30,6));

    Serial.print(" now + 7d + 12h + 30m + 6s: ");
    Serial.print(future_ds3231.year(), DEC);
    Serial.print('/');
    Serial.print(future_ds3231.month(), DEC);
    Serial.print('/');
    Serial.print(future_ds3231.day(), DEC);
    Serial.print(' ');
    Serial.print(future_ds3231.hour(), DEC);
    Serial.print(':');
    Serial.print(future_ds3231.minute(), DEC);
    Serial.print(':');
    Serial.print(future_ds3231.second(), DEC);
    Serial.println();

    Serial.println();    
    delay(3000);
}
