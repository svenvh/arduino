#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);

void setup() {
  lcd.init();
  lcd.backlight();
}

void loop() {
  for (int i = 10; i >= 0; i--) {
    lcd.clear();
    lcd.print("Selfdestruct in ");
    lcd.setCursor(7,1);
    lcd.print(i);
    delay(1000);
  }  
}
