/*
 Name:		TestInterrupts.ino
 Created:	5/11/2022 12:55:42
 Author:	James Jethro Owens
*/

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <Wire.h>
#include <ds3231.h>

#define LED 10
#define DHTPIN 11
#define DHTTYPE DHT11   // DHT 11

struct ts t;

// Initialize objects
DHT dht(DHTPIN, DHTTYPE);

// the setup function runs once when you press reset or power the board
void setup() {
	pinMode(LED, OUTPUT);
	Serial.begin(9600);
	Serial.println(F("DHTxx test!"));
	dht.begin();
	Wire.begin();
	DS3231_init(DS3231_CONTROL_INTCN);
	t.hour = 12;
	t.min = 30;
	t.sec = 0;
	t.mday = 25;
	t.mon = 12;
	t.year = 2019;
	//DS3231_set(t);
}

// the loop function runs over and over again until power down or reset
void loop() {

	//digitalWrite(LED, HIGH);

	float h = dht.readHumidity();
	float t = dht.readTemperature();
	float f = dht.readTemperature(true);
	if (isnan(h) || isnan(t) || isnan(f)) {
		Serial.println(F("Failed to read from DHT sensor!"));
		return;
	}
	float hif = dht.computeHeatIndex(f, h);
	float hic = dht.computeHeatIndex(t, h, false);
	Serial.print(F(" Humidity: "));
	Serial.print(h);
	Serial.print(F("%  Temperature: "));
	Serial.print(t);
	Serial.print(F("C "));
	Serial.print(f);
	Serial.print(F("F  Heat index: "));
	Serial.print(hic);
	Serial.print(F("C "));
	Serial.print(hif);
	Serial.println(F("F"));

	DS3231_get(&t);
	Serial.print("Date : ");
	Serial.print(t.mday);
	Serial.print("/");
	Serial.print(t.mon);
	Serial.print("/");
	Serial.print(t.year);
	Serial.print("\t Hour : ");
	Serial.print(t.hour);
	Serial.print(":");
	Serial.print(t.min);
	Serial.print(".");
	Serial.println(t.sec);
	delay(500);

	
	
	//digitalWrite(LED, LOW);
	delay(500);
}
