#include "LCD-I2C.h"
#include "Wire.h"

/**
 * @brief function begin 
 * 
 * @param beginWire if true start I²C wire
 */
void LCD_I2C::begin(bool beginWire) {
    if (beginWire)
        Wire.begin();
    
    // Clear i2c adapter
    I2C_Write(0b00000000);
    // Wait more than 40 ms after powerOn
    delay(50);
    InitializeLCD();
}

/**
 * @brief switch backlight on
 * 
 */
void LCD_I2C::backlight() {
    _output.Led = 1;
    // Led pin is independent from LCD data and control lines.
    I2C_Write(0b00000000 | _output.Led << 3);
}

/**
 * @brief switch backlight off
 * 
 */
void LCD_I2C::backlightOff() {
    _output.Led = 0;
    // Led pin is independent from LCD data and control lines.
    I2C_Write(0b00000000 | _output.Led << 3); 
}

/**
 * @brief clear display
 * 
 */
void LCD_I2C::clear() {
    _output.rs = 0;
    _output.rw = 0;

    LCD_Write(HD44780_CLEAR_DISPLAY);
    delayMicroseconds(1550);
}

/**
 * @brief set home position
 * 
 */
void LCD_I2C::home() {
    _output.rs = 0;
    _output.rw = 0;

    LCD_Write(HD44780_CURSOR_HOME);
    delayMicroseconds(1550);
}


/**
 * @brief set left to right
 * 
 */
void LCD_I2C::leftToRight() {
    _output.rs = 0;
    _output.rw = 0;

    _entryState |= (1 << 1);

    LCD_Write(HD44780_ENTRY_MODE_SET | _entryState);
    delayMicroseconds(37);
}


/**
 * @brief set right to left
 * 
 */
void LCD_I2C::rightToLeft() {
    _output.rs = 0;
    _output.rw = 0;

    _entryState &= ~(1 << 1);

    LCD_Write(HD44780_ENTRY_MODE_SET | _entryState);
    delayMicroseconds(37);
}


/**
 * @brief set autoscroll
 * 
 */
void LCD_I2C::autoscroll() {
    _output.rs = 0;
    _output.rw = 0;

    _entryState |= 1;

    LCD_Write(HD44780_ENTRY_MODE_SET | _entryState);
    delayMicroseconds(37);
}


/**
 * @brief set no autoscroll
 * 
 */
void LCD_I2C::autoscrollOff() {
    _output.rs = 0;
    _output.rw = 0;

    _entryState &= ~1;

    LCD_Write(HD44780_ENTRY_MODE_SET | _entryState);
    delayMicroseconds(37);
}


/**
 * @brief show display
 * 
 */
void LCD_I2C::display() {
    _output.rs = 0;
    _output.rw = 0;

    _displayState |= (1 << 2);

    LCD_Write(HD44780_DISPLAY_CONTROL | _displayState);
    delayMicroseconds(37);
}


/**
 * @brief display off
 * 
 */
void LCD_I2C::displayOff() {
    _output.rs = 0;
    _output.rw = 0;

    _displayState &= ~(1 << 2);

    LCD_Write(HD44780_DISPLAY_CONTROL | _displayState);
    delayMicroseconds(37);
}


/**
 * @brief show cursor
 * 
 */
void LCD_I2C::cursor() {
    _output.rs = 0;
    _output.rw = 0;

    _displayState |= (1 << 1);

    LCD_Write(HD44780_DISPLAY_CONTROL | _displayState);
    delayMicroseconds(37);
}


/**
 * @brief set display => no cursor
 * 
 */
void LCD_I2C::cursorOff() {
    _output.rs = 0;
    _output.rw = 0;

    _displayState &= ~(1 << 1);

    LCD_Write(HD44780_DISPLAY_CONTROL | _displayState);
    delayMicroseconds(37);
}


/**
 * @brief Set cursor blinkin on 
 * 
 */
void LCD_I2C::blink() {
    _output.rs = 0;
    _output.rw = 0;

    _displayState |= 1;

    LCD_Write(HD44780_DISPLAY_CONTROL | _displayState);
    delayMicroseconds(37);
}


/**
 * @brief Set cursor blinking off
 * 
 */
void LCD_I2C::blinkOff() {
    _output.rs = 0;
    _output.rw = 0;

    _displayState &= ~1;

    LCD_Write(HD44780_DISPLAY_CONTROL | _displayState);
    delayMicroseconds(37);
}

/**
 * @brief scroll display left
 * 
 */
void LCD_I2C::scrollDisplayLeft() {
    _output.rs = 0;
    _output.rw = 0;

    LCD_Write(0b00011000);
    delayMicroseconds(37);
}


/**
 * @brief scroll display right
 * 
 */
void LCD_I2C::scrollDisplayRight() {
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
void LCD_I2C::createChar(uint8_t memory_location, uint8_t charmap[]) {
    _output.rs = 0;
    _output.rw = 0;

    memory_location %= 8;

    LCD_Write(HD44780_SET_CGRAM_ADDR | (memory_location << 3));
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
void LCD_I2C::setCursor(uint8_t col, uint8_t row) {
    static const uint8_t row_offsets[] = {0x00, 0x40, 0x14, 0x54};
    _output.rs = 0;
    _output.rw = 0;
     // sanity limits
    if (col > _columnMax) { col = _columnMax; }
    // sanity limits
    if (row > _rowMax) { row = _rowMax; }

    uint8_t newAddress = row_offsets[row] + col;

    LCD_Write(HD44780_SET_DDRRAM_ADDR | newAddress);
    delayMicroseconds(37);
}

/**
 * @brief write function 
 * 
 * @param character to write 
 * @return size_t written bytes
 */
size_t LCD_I2C::write(uint8_t character) {
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
void LCD_I2C::InitializeLCD() {
    // See HD44780U datasheet "Initializing by Instruction" Figure 24 (4-Bit Interface)
    // SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
	// according to datasheet, we need at least 40 ms after power rises above 2.7 V
	// before sending commands. Arduino can turn on way before 4.5 V so we'll wait 50
    _output.rs = 0;
    _output.rw = 0;

    // wait mor than 40 ms after Vcc = 2.7 V
    delay(50);

    // first
    LCD_Write(0b00110000, true);
    delayMicroseconds(4200);
    // second
    LCD_Write(0b00110000, true);
    delayMicroseconds(150);
    // third
    LCD_Write(0b00110000, true);
    delayMicroseconds(37);
    
    // Function Set - 4 bits mode
    LCD_Write(0b00100000, true);
    delayMicroseconds(37);
    // Function Set  - 4 bit Interface, 1 = 2 lines, 0 = 5x8 font
    LCD_Write(0b00101000);
    delayMicroseconds(37);

    displayOff();
    clear();
    leftToRight();
}

/**
 * @brief I²C write funtion
 * 
 * @param output data to write
 */
void LCD_I2C::I2C_Write(uint8_t output) {
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
void LCD_I2C::LCD_Write(uint8_t output, bool initialization) {
    _output.data = output;

    _output.E = true;
    I2C_Write(_output.GetHighData());
    // High part of enable should be > 450 ns
    delayMicroseconds(1);

    _output.E = false;
    I2C_Write(_output.GetHighData());

    // During initialization we only send half a byte
    if (!initialization) {
        // I think we need a delay between half byte writes, but no sure how long it needs to be.
        delayMicroseconds(37);

        _output.E = true;
        I2C_Write(_output.GetLowData());
        // High part of enable should be > 450 ns
        delayMicroseconds(1); 

        _output.E = false;
        I2C_Write(_output.GetLowData());
    }
}
