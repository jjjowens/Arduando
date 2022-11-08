#include <Wire.h>
#include <ds3231.h>
 
struct ts t; 
 
void setup() {
  Serial.begin(9600);
  Wire.begin();
  DS3231_init(DS3231_CONTROL_INTCN);
  /*----------------------------------------------------------------------------
  In order to synchronise your clock module, insert timetable values below !
  ----------------------------------------------------------------------------*/
  t.hour=16; 
  t.min=16;
  t.sec=0;
  t.mday=8;
  t.mon=7;
  t.year=2021;
 
  //DS3231_set(t); 
}
 
void loop() {
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
  Serial.print(t.sec);

  Serial.print("\tTemperature : ");
  Serial.println(DS3231_get_treg());
 
  delay(1000);
}
