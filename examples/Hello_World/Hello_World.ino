/*
*	Hello_World.ino
*
*	Author: Frank Häfele
*	Date:	01.12.2023
*
*	Object: Print Hello World on LCD Display
*/

#include <LCD-I2C.h>
#include <Wire.h>

LCD_I2C lcd(0x27, 16, 2); // Default address of most PCF8574 modules, change according

void setup() {
    Wire.begin();
    lcd.begin(&Wire);
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
