/*
    LCD-I2C - Arduino library to control a 16x2 LCD via an I2C adapter based on PCF8574
    * 2021-11-18 Brewmanz: make changes to also work for 20x4 LCD2004

    Copyright(C) 2020 Blackhack <davidaristi.0504@gmail.com>

    This program is free software : you can redistribute it and /or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.If not, see < https://www.gnu.org/licenses/>.
*/

#include "LCD-I2C.h"
#include "Wire.h"

/**
 * @brief function begin 
 * 
 * @param beginWire if true start I²C wire
 */
void LCD-I2C::begin(bool beginWire) {
    if (beginWire)
        Wire.begin();

    I2C_Write(0b00000000); // Clear i2c adapter
    delay(50); //Wait more than 40ms after powerOn.

    InitializeLCD();
}

/**
 * @brief switch backlight on
 * 
 */
void LCD-I2C::backlight() {
    _output.Led = 1;
    I2C_Write(0b00000000 | _output.Led << 3); // Led pin is independent from LCD data and control lines.
}

/**
 * @brief switch backlight off
 * 
 */
void LCD-I2C::noBacklight() {
    _output.Led = 0;
    I2C_Write(0b00000000 | _output.Led << 3); // Led pin is independent from LCD data and control lines.
}

/**
 * @brief clear display
 * 
 */
void LCD-I2C::clear() {
    _output.rs = 0;
    _output.rw = 0;

    LCD_Write(LCD_CLEAR);
    delayMicroseconds(1600);
}

/**
 * @brief set home position
 * 
 */
void LCD-I2C::home() {
    _output.rs = 0;
    _output.rw = 0;

    LCD_Write(LCD_HOME);
    delayMicroseconds(1600);
}


/**
 * @brief set left to right
 * 
 */
void LCD-I2C::leftToRight() {
    _output.rs = 0;
    _output.rw = 0;

    _entryState |= 1 << 1;

    LCD_Write(LCD_ENTRY_MODE_SET | _entryState);
    delayMicroseconds(37);
}


/**
 * @brief set right to left
 * 
 */
void LCD-I2C::rightToLeft() {
    _output.rs = 0;
    _output.rw = 0;

    _entryState &= ~(1 << 1);

    LCD_Write(LCD_ENTRY_MODE_SET | _entryState);
    delayMicroseconds(37);
}


/**
 * @brief set autoscroll
 * 
 */
void LCD-I2C::autoscroll() {
    _output.rs = 0;
    _output.rw = 0;

    _entryState |= 1;

    LCD_Write(LCD_ENTRY_MODE_SET | _entryState);
    delayMicroseconds(37);
}


/**
 * @brief set no autoscroll
 * 
 */
void LCD-I2C::noAutoscroll() {
    _output.rs = 0;
    _output.rw = 0;

    _entryState &= ~1;

    LCD_Write(LCD_ENTRY_MODE_SET | _entryState);
    delayMicroseconds(37);
}


/**
 * @brief show display
 * 
 */
void LCD-I2C::display() {
    _output.rs = 0;
    _output.rw = 0;

    _displayState |= 1 << 2;

    LCD_Write(LCD_DISPLAY_CONTROL | _displayState);
    delayMicroseconds(37);
}


/**
 * @brief show no display
 * 
 */
void LCD-I2C::noDisplay() {
    _output.rs = 0;
    _output.rw = 0;

    _displayState &= ~(1 << 2);

    LCD_Write(LCD_DISPLAY_CONTROL | _displayState);
    delayMicroseconds(37);
}


/**
 * @brief show cursor
 * 
 */
void LCD-I2C::cursor() {
    _output.rs = 0;
    _output.rw = 0;

    _displayState |= 1 << 1;

    LCD_Write(LCD_DISPLAY_CONTROL | _displayState);
    delayMicroseconds(37);
}


/**
 * @brief set display => no cursor
 * 
 */
void LCD-I2C::noCursor() {
    _output.rs = 0;
    _output.rw = 0;

    _displayState &= ~(1 << 1);

    LCD_Write(LCD_DISPLAY_CONTROL | _displayState);
    delayMicroseconds(37);
}


/**
 * @brief Set Cursor blink
 * 
 */
void LCD-I2C::blink() {
    _output.rs = 0;
    _output.rw = 0;

    _displayState |= 1;

    LCD_Write(LCD_DISPLAY_CONTROL | _displayState);
    delayMicroseconds(37);
}


/**
 * @brief Set Cursor no blink
 * 
 */
void LCD-I2C::noBlink() {
    _output.rs = 0;
    _output.rw = 0;

    _displayState &= ~1;

    LCD_Write(LCD_DISPLAY_CONTROL | _displayState);
    delayMicroseconds(37);
}

/**
 * @brief scroll display left
 * 
 */
void LCD-I2C::scrollDisplayLeft() {
    _output.rs = 0;
    _output.rw = 0;

    LCD_Write(0b00011000);
    delayMicroseconds(37);
}


/**
 * @brief scroll display right
 * 
 */
void LCD-I2C::scrollDisplayRight() {
    _output.rs = 0;
    _output.rw = 0;

    LCD_Write(0b00011100);
    delayMicroseconds(37);
}

/**
 * @brief Function to create character
 * 
 * @param memory_location memory location where char is saved
 * @param charmap defined character map
 */
void LCD-I2C::createChar(uint8_t memory_location, uint8_t charmap[]) {
    _output.rs = 0;
    _output.rw = 0;

    location %= 8;

    LCD_Write(LCD_SET_CGRAM_ADDR | (memory_location << 3));
    delayMicroseconds(37);

    for (int i = 0; i < 8; i++)
        write(charmap[i]);

    setCursor(0, 0); // Set the address pointer back to the DDRAM
}


/**
 * @brief Set Cursor to col and row
 * 
 * @param col 
 * @param row 
 */
void LCD-I2C::setCursor(uint8_t col, uint8_t row) {
    static const uint8_t row_offsets[] = {0x00, 0x40, 0x14, 0x54};
    _output.rs = 0;
    _output.rw = 0;
     // sanity limits
    if (col > _columnMax) { col = _columnMax; }
    // sanity limits
    if (row > _rowMax) { row = _rowMax; }

    uint8_t newAddress = row_offsets[row] + col;

    LCD_Write(LCD_SET_DDRRAM_ADDR | newAddress);
    delayMicroseconds(37);
}

/**
 * @brief write function 
 * 
 * @param character to write 
 * @return size_t written bytes
 */
size_t LCD-I2C::write(uint8_t character) {
    _output.rs = 1;
    _output.rw = 0;

    LCD_Write(character);
    delayMicroseconds(41);

    return 1;
}

/**
 * @brief Funtion to initialize LCD
 * 
 */
void LCD-I2C::InitializeLCD() {
    // See HD44780U datasheet "Initializing by Instruction" Figure 24 (4-Bit Interface)
    _output.rs = 0;
    _output.rw = 0;

    LCD_Write(0b00110000, true);
    delayMicroseconds(4200);
    LCD_Write(0b00110000, true);
    delayMicroseconds(150);
    LCD_Write(0b00110000, true);
    delayMicroseconds(37);
    // Function Set - 4 bits mode
    LCD_Write(LCD_FUNCTION_SET, true);
    delayMicroseconds(37);
    // Function Set - 4 bits(Still), 2 lines, 5x8 font
    LCD_Write(0b00101000);
    delayMicroseconds(37);

    display();
    clear();
    leftToRight();
}

/**
 * @brief I²C write funtion
 * 
 * @param output data to write
 */
void LCD-I2C::I2C_Write(uint8_t output) {
    Wire.beginTransmission(_address);
    Wire.write(output);
    Wire.endTransmission();
}

/**
 * @brief LCD_Write function
 * 
 * @param output data to write
 * @param initialization initialization
 */
void LCD-I2C::LCD_Write(uint8_t output, bool initialization) {
    _output.data = output;

    _output.E = true;
    I2C_Write(_output.GetHighData());
    // High part of enable should be >450 nS
    delayMicroseconds(1);

    _output.E = false;
    I2C_Write(_output.GetHighData());

    // During initialization we only send half a byte
    if (!initialization) {
        // I think we need a delay between half byte writes, but no sure how long it needs to be.
        delayMicroseconds(37);

        _output.E = true;
        I2C_Write(_output.GetLowData());
        // High part of enable should be >450 nS
        delayMicroseconds(1); 

        _output.E = false;
        I2C_Write(_output.GetLowData());
    }
}
