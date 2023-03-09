/*
 Name:		007_Validar_DS3231_OK.ino
 Created:	21/2/2023 15:05:04
 Author:	James Owens
*/


#include <DS3231.h>

DS3231 RTC;
DateTime dt;


void printTimeNow();

/*
* Program that will check the current situation of the time stored in RTC DS3231. If the RTC suffered a
* hard restart due to power failure or crash the application will say so and ask user to input new
* date/time using the serial port. To set new date/time send string in the following format:
* DD/MM/YY HH:MM:SS
* 
* Requires library DS3231 from NorthenWidget available from 
* https://github.com/NorthernWidget/DS3231
* 
*/
void setup() 
{
	Serial.begin(9600);
	while (!Serial);
	Wire.begin();

	if (RTC.oscillatorCheck())
	{
		Serial.println(F("RTC OK!"));
		Serial.print(F("\nCurrent time: "));
		printTimeNow();
		Serial.println();
	}
	else {
		Serial.println(F("RTC ERROR!"));
	}
	Serial.println(F("\nSend string in format DD/MM/YY HH:MM:SS to adjust or set the date/time on RTC"));
}

// the loop function runs over and over again until power down or reset
int loopCounter = 0;

void loop() {

	if (Serial.available() >= 17) {
		// read the time from the serial port in format "DD/MM/YY HH:MM:SS"
		String timeStr = Serial.readStringUntil('\n');
		Serial.print(F("\nNew time: ")); Serial.println(timeStr);

		int day = timeStr.substring(0, 2).toInt();
		int month = timeStr.substring(3, 5).toInt();
		int year = timeStr.substring(6, 8).toInt();
		int hour = timeStr.substring(9, 11).toInt();
		int minute = timeStr.substring(12, 14).toInt();
		int second = timeStr.substring(15, 17).toInt();
		RTC.setDate(day);
		RTC.setMonth(month);
		RTC.setYear(year);
		RTC.setHour(hour);
		RTC.setMinute(minute);
		RTC.setSecond(second);
		Serial.print(F("\nTime is now set to: "));
		printTimeNow();
		Serial.println();
	}
	 
}


void printTimeNow()
{
	dt = RTClib::now();
	Serial.print(dt.day()); Serial.print("/");
	Serial.print(dt.month()); Serial.print("/");
	Serial.print(dt.year());
	Serial.print(" ");
	Serial.print(dt.hour()); Serial.print(":");
	if (dt.minute() < 10) Serial.print("0");
	Serial.print(dt.minute()); Serial.print(":");
	if (dt.second() < 10) Serial.print("0");
	Serial.print(dt.second()); Serial.print(" ");
}