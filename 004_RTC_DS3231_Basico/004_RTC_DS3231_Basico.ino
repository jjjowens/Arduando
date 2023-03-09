/**
 * Copyright (c) 2022 - James Owens <jjo(at)arduando.com.br>
 * 
 * Arquivo:     _004_RTC_DS3231_Basico.ino
 * Arquivo:     12/12/2022 09:00:22
 * Versão:      
 * Fonte:       
 * Website:     https://arduando.com.br
 *
 * Descrição: 
 *
 * DISCLAIMER:
 * The author is in no way responsible for any problems or damage caused by
 * using this code. Use at your own risk.
 *
 * LICENSE:
 * This code is distributed under the GNU Public License
 * as published by the Free Software Foundation; either version 3
 * of the License, or (at your option) any later version.
 * More details can be found at http://www.gnu.org/licenses/gpl.txt
 */

#include <Wire.h>
#include <DS3231.h>

DS3231 RTC;
DateTime dt;

void setup() 
{

	Serial.begin(9600);
	while (!Serial);
	Serial.println("Starting..");
	Wire.begin();
	/* UNCOMMENT THIS BLOCK IF YOU NEED TO SET TIME ON YOUR DS3231
	* YOU SHOULD RUN THIS ONLY ONCE, SET INITIAL VALUES AS NEEDED */
	/*
	//delay(1000);
	RTC.setYear(22);
	RTC.setMonth(12);
	RTC.setDate(19);
	RTC.setDoW(0);
	RTC.setHour(19);
	RTC.setMinute(50);
	RTC.setSecond(0);
	RTC.setClockMode(false);
	*/



	Serial.print("The time now is:  ");
	printTimeNow();
	Serial.println();



}


void loop() 
{
  
}

// Prints the current timestamp as set in DS3231 RTC
void printTimeNow()
{
	dt = RTClib::now();
	Serial.print(dt.month()); Serial.print("/");
	Serial.print(dt.day()); Serial.print("/");
	Serial.print(dt.year());
	Serial.print(" ");
	Serial.print(dt.hour()); Serial.print(":");
	if (dt.minute() < 10) Serial.print("0");
	Serial.print(dt.minute()); Serial.print(":");
	if (dt.second() < 10) Serial.print("0");
	Serial.print(dt.second()); Serial.print(" ");
}