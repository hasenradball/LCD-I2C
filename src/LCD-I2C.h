#ifndef _LCD_I2C_H_
#define _LCD_I2C_H_

#include "Arduino.h"
#include <LCD_Constants.h>

/*
   This struct helps us constructing the I2C output based on data and control outputs.
   Because the LCD is set to 4-bit mode, 4 bits of the I2C output are for the control outputs
   while the other 4 bits are for the 8 bits of data which are send in parts using the enable output.
*/
struct OutputState {
    uint8_t rs = 0;
    uint8_t rw = 0;
    uint8_t E = 0;
    uint8_t Led = 0;
    uint8_t data = 0;

    uint8_t GetLowData() {
        uint8_t buffer = rs;
        buffer |= rw << 1;
        buffer |= E << 2;
        buffer |= Led << 3;
        buffer |= (data & 0x0F) << 4;

        return buffer;
    }

    uint8_t GetHighData() {
        uint8_t buffer = rs;
        buffer |= rw << 1;
        buffer |= E << 2;
        buffer |= Led << 3;
        buffer |= (data & 0xF0);
        return buffer;
    }
};

class LCD_I2C : public Print {
    public:
        LCD_I2C(uint8_t address, uint8_t columns = 16, uint8_t rows = 2)
        : _address(address), _columnMax(--columns), _rowMax(--rows) {}

        void begin(bool beginWire = true);
        void backlight();
        void backlightOff();

        void clear();
        void home();
        void leftToRight();
        void rightToLeft();
        void autoscroll();
        void autoscrollOff();
        void display();
        void displayOff();
        void cursor();
        void cursorOff();
        void blink();
        void blinkOff();
        void scrollDisplayLeft();
        void scrollDisplayRight();
        void createChar(uint8_t memory_location, uint8_t charmap[]);
        void setCursor(uint8_t column, uint8_t row);
        // Method used by the Arduino class "Print" which is the one that provides the .print(string) method
        virtual size_t write(uint8_t character);


    private:
        uint8_t _address;
        uint8_t _columnMax;
        uint8_t _rowMax;
        OutputState _output;
        uint8_t _displayState = 0x00;
        uint8_t _entryState = 0x00;

        void InitializeLCD();
        void I2C_Write(uint8_t output);
        void LCD_Write(uint8_t output, bool initialization = false);
};

#endif
