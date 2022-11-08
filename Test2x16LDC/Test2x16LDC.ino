//www.diyusthad.com
#include <LiquidCrystal.h>

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
int i = 0;
void setup() {
  lcd.begin(16, 2);
  lcd.print("Initializing...");
  lcd.setCursor(0,1);
  lcd.print("                ");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print ("Counter:");
}

void loop() {
  lcd.setCursor(0,1);
  lcd.print ( i++ );
  delay (1000);
}
