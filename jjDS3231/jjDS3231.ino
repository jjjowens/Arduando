/* ----------------------------------------------------------------
 * Copyright (C) 2022, James J. Owens. Unless you have purchased
 * a separate commercial license for this software code, the
 * following terms and conditions apply:
 *
 * This program is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 3 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see http://www.gnu.org/licenses.
 *
 * For more information see https://referenciando.com
 * ----------------------------------------------------------------
 * jjDS3231
 * --------
 * Handles basic functions regarding a DS3231 RTC on i2C bus and arduino
 * 
 */

// Define User Types below here or use a .h file ----------
//

#include <Wire.h> 

#define DS3231_ADDRESS     0x68      //=104 dec
#define DS3231_STATUS_REG  0x0F
#define DS3231_CONTROL_REG 0x0E
#define DS3231_TMP_UP_REG  0x11

char CycleTimeStamp[] = "0000/00/00,00:00"; //16 characters without seconds!
uint8_t t_second;   //= sec;  used in RTC_DS3231_getTime()
uint8_t t_minute;   //= min;
uint8_t t_hour;     //= hour;
uint8_t t_day;      //= day;
uint8_t t_month;    //= month;
uint16_t t_year;    //= year //yOff = raw year; //need to add 2000
uint8_t Alarmday = 1;
uint8_t Alarmhour = 1;
uint8_t Alarmminute = 1;
uint8_t Alarmsecond = 1;
boolean midnightRollover = false;
int rtc_TEMP_Raw = 0;
float rtc_TEMP_degC = 0.0;



static uint8_t bcd2bin(uint8_t val) 
{
	return val - 6 * (val >> 4);
}
static uint8_t bin2bcd(uint8_t val) 
{
	return val + 6 * (val / 10);
}

void RTC_DS3231_getTime()
{
	Wire.beginTransmission(DS3231_ADDRESS);
	Wire.write(0);
	Wire.endTransmission();
	Wire.requestFrom(DS3231_ADDRESS, 7);
	t_second = bcd2bin(Wire.read() & 0x7F);
	t_minute = bcd2bin(Wire.read());
	t_hour = bcd2bin(Wire.read());
	Wire.read();
	t_day = bcd2bin(Wire.read());
	t_month = bcd2bin(Wire.read());
	t_year = bcd2bin(Wire.read()) + 2000;
	return;
}

void RTC_DS3231_setTime()
{
	Wire.beginTransmission(DS3231_ADDRESS);
	Wire.write((byte)0);
	Wire.write(bin2bcd(t_second));
	Wire.write(bin2bcd(t_minute));
	Wire.write(bin2bcd(t_hour));
	Wire.write(bin2bcd(0));
	Wire.write(bin2bcd(t_day));
	Wire.write(bin2bcd(t_month));
	Wire.write(bin2bcd(t_year - 2000));
	Wire.endTransmission();
}

// Define Function Prototypes that use User Types below here or use a .h file----------
//




// Define Functions below here or use other .ino or cpp files----------
//

// The setup() function runs once each time the micro-controller starts----------
void setup()
{
	Wire.begin();
	Serial.begin(9600);
	/*
	t_year = 2022;
	t_month = 11;
	t_day = 6;
	t_hour = 18;
	t_minute = 58;
	t_second = 59;
	RTC_DS3231_setTime();
	*/
}

// Add the main program code into the continuous loop() function
void loop()
{
	RTC_DS3231_getTime();
	//delay(20);
	Serial.print("Today is day ");
	Serial.print(t_day);
	Serial.print(" of the month ");
	Serial.print(t_month);
	Serial.print(" of year ");
	Serial.print(t_year);
	Serial.print(". And now is ");
	Serial.print(t_hour);
	Serial.print(":");
	Serial.print(t_minute);
	Serial.print(":");
	Serial.println(t_second);
	delay(5000);

}
