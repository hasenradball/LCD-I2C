/*
*	Functions.ino
*
*	Author: Frank Häfele
*	Date:	01.12.2023
*
*	Object: Print User defined characters on LCD Display
*/

#include <LCD-I2C.h>

// Default address of most PCF8574 modules, change according

LCD_I2C lcd(0x27, 16, 2);

/*
* When using lcd.print() (and almost everywhere you use string literals),
* is a good idea to use the macro F(String literal).
* This tells the compiler to store the string array in the flash memory
* instead of the ram memory. Usually you have more spare flash than ram.
* More info: https://www.arduino.cc/reference/en/language/variables/utilities/progmem/
*/

void setup() {
    lcd.begin();
    lcd.display();
    lcd.backlight();
}

void loop()
{
    //Autoscroll
    lcd.setCursor(5, 0);
    lcd.print(F("Autoscrolling"));
    lcd.setCursor(10, 1);
    lcd.autoscroll();

    for (int i = 0; i < 10; i++)
    {
        lcd.print(i);
        delay(200);
    }

    lcd.autoscrollOff();
    lcd.clear();

    // Scroll left and right
    lcd.setCursor(10, 0);
    lcd.print(F("To the left!"));
    for (int i = 0; i < 10; i++)
    {
        lcd.scrollDisplayLeft();
        delay(200);
    }
    lcd.clear();
    lcd.print(F("To the right!"));
    for (int i = 0; i < 10; i++)
    {
        lcd.scrollDisplayRight();
        delay(200);
    }
    lcd.clear();

    //Cursor
    lcd.setCursor(0, 0);
    lcd.cursor();
    lcd.print(F("Cursor"));
    delay(3000);
    lcd.clear();

    //Cursor blink
    lcd.setCursor(0, 0);
    lcd.blink();
    lcd.print(F("Cursor blink"));
    delay(3000);
    lcd.clear();

    //Blink without cursor
    lcd.setCursor(0, 0);
    lcd.cursorOff();
    lcd.print(F("Just blink"));
    delay(3000);
    lcd.blinkOff();
    lcd.clear();
}
