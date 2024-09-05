/*
*	write_character_codes.ino
*
*	Author: Frank Häfele
*	Date:	03.05.2024
*
*	Object: print characters from code table
*/

#include <LCD-I2C.h>
#include <Wire.h>

LCD_I2C lcd(0x27, 16, 2); // Default address of most PCF8574 modules, change according

void setup() {
  // If you are using more I2C devices using the Wire library use lcd.begin(false)
  // this stop the library(LCD-I2C) from calling Wire.begin()
  Wire.begin();
  lcd.begin(&Wire);
  lcd.display();
  lcd.backlight();
}

void loop() {
  // write som character code from ROM code A00
  lcd.setCursor(0, 1);
  lcd.print("print characters");
  lcd.setCursor(0, 0);
  // writes a H
  lcd.writeCharCode(0b01001000);
  delay(1000);

  // writes a degree sign
  lcd.writeCharCode(0b11011111);
  delay(1000);

  // writes a question mark
  lcd.writeCharCode(0b00111111);
  delay(1000);

}