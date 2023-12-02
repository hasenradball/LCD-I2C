/*
*	Custom_Chars.ino
*
*	Author: Frank Häfele
*	Date:	01.12.2023
*
*	Object: Print User defined characters on LCD Display
*/

#include <LCD-I2C.h>

// Default address of most PCF8574 modules, change according

LCD_I2C lcd(0x27, 16, 2);

uint8_t happy[8] =
{
    0b00000,
    0b10001,
    0b00000,
    0b00000,
    0b10001,
    0b01110,
    0b00000,
    0b00000,
};

uint8_t wow[8] =
{
    0b00000,
    0b10001,
    0b00000,
    0b01110,
    0b10001,
    0b01110,
    0b00000,
    0b00000,
};

uint8_t anchor[8] =
{
    0b01110,
    0b01010,
    0b01110,
    0b00100,
    0b10101,
    0b10101,
    0b01110,
    0b00100
};

uint8_t snow[8] =
{
    0b01000,
    0b11101,
    0b01011,
    0b00001,
    0b00100,
    0b01110,
    0b00100,
    0b10000
};

void setup()
{
    lcd.begin();
    lcd.display();
    lcd.backlight();

    lcd.createChar(0, happy);
    lcd.createChar(1, wow);
    lcd.createChar(2, anchor);
    lcd.createChar(3, snow);


    lcd.write(0);
    lcd.write(1);
    lcd.write(2);
    lcd.write(3);
}

void loop()
{
}
