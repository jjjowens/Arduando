/**
 * Copyright (c) 2022 - James Owens <jjo(at)arduando.com.br>
 * 
 * Arquivo:     001_DS3231_Test_Alarms
 * Arquivo:     27/11/2022 12:03:23
 * Versão:      
 * Fonte:       
 * Website:     https://arduando.com.br
 *
 * Descrição: Se aproveita dos dois alarmes integrados no RTC DS3231 para
 * acionar interrupção de modo a executar ações especificas e diferenciadas
 * dependendo de qual alarme foi disparado.
 * 
 * Dependências: Bibliotecas Wire (padrão) e DS3231 disponível em 
 * https://github.com/NorthernWidget/DS3231. 
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


// Time intervals the alarms will ring, set in sub-multiples of 60, valid values are therefore:
// 1, 2, 3, 5, 10.
// I added dependency between the second and the first alarms for convinience.
// In the default setting alarm 1 will ring every 5 minutes and alarm 2 therefore every 15 minutes
#define ALARM1_TIMEOUT 5     // In minutes
#define ALARM2_TIMEOUT (ALARM1_TIMEOUT * 3) 

#define ALARM1 1
#define ALARM2 2

#define ALARM_INTERRUPT_PIN 3


// Global objects 
DS3231 RTC;
DateTime dt;
DateTime *alarmtime;

// Variables accessed from ISR
volatile bool alarmEventFlag = false;

// Function prototype declaration
void resetAlarm(uint8_t, uint8_t = 0);
uint8_t calculateAlarmMinute(uint8_t, uint8_t = 0);


void setup()
{
    Serial.begin(57600);
    while (!Serial);
    Wire.begin();

    pinMode(ALARM_INTERRUPT_PIN, INPUT);
    attachInterrupt(digitalPinToInterrupt(ALARM_INTERRUPT_PIN), alarmISR, FALLING);

    printTimeNow();
    Serial.println("Begin the test");

    /* UNCOMMENT THIS BLOCK IF YOU NEED TO SET TIME ON YOUR DS3231
     * YOU SHOULD RUN THIS ONLY ONCE, SET INITIAL VALUES AS NEEDED */
    /*
    RTC.setYear(2022);
    RTC.setMonth(11);
    RTC.setDate(27);
    RTC.setDoW(0);
    RTC.setHour(12);
    RTC.setMinute(8);
    RTC.setSecond(0);
    RTC.setClockMode(clockMode);
    */


    // Alarm 1: Add extra minute in the chance minute will change while alarm is being set
    resetAlarm(ALARM1, 1);
    
    // Alarm 2: Ensure Alarm1 was triggered at least once before Alarm2 
    resetAlarm(ALARM2, ALARM1_TIMEOUT + 2);
} // end of setup()

void loop() {
    if (alarmEventFlag == true) 
    {
        printTimeNow();
        if (RTC.checkIfAlarm(ALARM1))
        {
            handleAlarm1Event();
            resetAlarm(ALARM1);
        }
        if (RTC.checkIfAlarm(ALARM2))
        {
            handleAlarm2Event();
            resetAlarm(ALARM2);
        }
        alarmEventFlag = false;
    }
}  // end of loop()


//////////////////////////////////////////////

// All there is to do when Alarm 1 rings
void handleAlarm1Event(void)
{
    Serial.println("Alarm1 event occurred!");
}

// All there is to do when Alarm 2 rings
void handleAlarm2Event(void)
{
    Serial.println("Alarm2 event occurred!");
}


//RTC Alarm interrupt handler, sets flag to true to indicate alarm interrupt
void alarmISR(void)
{
    alarmEventFlag = true;
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


// Will re-set the alarm based on the next calculated time interval
// Takes the alarm number (1 or 2) and an optional minutes parameter
// to push the interval to the next slot depending on what the current
// time is.
void resetAlarm(uint8_t alarm, uint8_t minute = 0)
{
    minute = calculateAlarmMinute(alarm, minute);
    DateTime alarmtime(0, 0, 0, 0, minute, 0);
    printTimeNow();
    if (alarm == ALARM1)
    {
        setAlarm1(alarmtime);
        Serial.print("Alarm1 now set to ring at h:");
        Serial.print(minute); Serial.println("m");

    }
    else {
        setAlarm2(alarmtime);
        Serial.print("Alarm2 now set to ring at h:");
        Serial.print(minute); Serial.println("m");
    }
}

// Returns an 8 bit unsigned number between 0 and 59 corresponding to
// the minute at which a given alarm should be set to based on the 
// Alarm timout constants and the current time. This ensures the alarm
// will ring only at pre-defined times within the hour regardless of 
// the time the program started. It accepts a sencod optional parameter
// to allow for additional offset to push time to the next interval. Used
// to ensure enough time is given for the microcontroller to startup
// after reset.
uint8_t calculateAlarmMinute(uint8_t alarm, uint8_t minute = 0)
{
    uint8_t interval;
    minute += RTClib::now().minute();

    if (alarm == ALARM1) interval = ALARM1_TIMEOUT;
    else interval = ALARM2_TIMEOUT;

    if (minute < (60 - interval))
    {
        minute /= interval;     // discard fraction
        minute *= interval;
        minute += interval;
    }
    else minute = 0;            // rollover to next hour
    return minute;
}


// Sets the trigger time for Alarm 1. Will reset alarm flags to enable 
// the alarm in case it was triggered before. 
// Expects a DateTime object set with the time alrm shoudl trigger.
void setAlarm1(DateTime alarmTime) 
{
    RTC.turnOffAlarm(ALARM1);
    RTC.checkIfAlarm(ALARM1);
    // day, and hour are ignored, but included to use DS3231 library
    RTC.setA1Time(
        alarmTime.day(),        
        alarmTime.hour(),
        alarmTime.minute(),
        alarmTime.second(),
        0b00001100,  // Alarm when minutes and seconds match
        false, false, false
    );
    RTC.turnOnAlarm(ALARM1);
}

// Sets the trigger time for Alarm 2. Will reset alarm flags to enable 
// the alarm in case it was triggered before. 
// Expects a DateTime object set with the time alrm shoudl trigger.
void setAlarm2(DateTime alarmTime)
{
    RTC.turnOffAlarm(ALARM2);
    RTC.checkIfAlarm(ALARM2);
    //day, and hour are ignored, but included to use DS3231 library
    RTC.setA2Time(
        alarmTime.day(),
        alarmTime.hour(),
        alarmTime.minute(),
        0b01100000, // Alarm when minutes match
        false, false, false
    );
    RTC.turnOnAlarm(ALARM2);
}