/*
*	Hello_World.ino
*
*	Author: Frank Häfele
*	Date:	01.12.2023
*
*	Object: Print Hello World on LCD Display
*/

#include <LCD-I2C.h>


LCD_I2C lcd(0x27, 16, 2); // Default address of most PCF8574 modules, change according

void setup() {
    // If you are using more I2C devices using the Wire library use lcd.begin(false)
    // this stop the library(LCD-I2C) from calling Wire.begin()
    lcd.begin();
    lcd.display();
    lcd.backlight();
}

void loop()
{
    lcd.print("     Hello"); // You can make spaces using well... spaces
    lcd.setCursor(5, 1); // Or setting the cursor in the desired position.
    lcd.print("World!");
    delay(500);

    // Flashing the backlight
    for (int i = 0; i < 5; ++i)
    {
        lcd.backlight();
        delay(50);
        lcd.backlightOff();
        delay(50);
    }

    lcd.backlight();
    lcd.clear();
    delay(500);
}
