
#ifndef _LCD_CONSTANTS_H_
#define _LCD_CONSTANTS_H_

/**
 * @brief Define Instructions Constants of HD44780U
 * 
 */
constexpr int HD44780_CLEAR_DISPLAY {0b00000001};
constexpr int HD44780_CURSOR_HOME  {0b00000010};
constexpr int HD44780_ENTRY_MODE_SET  {0b00000100};

constexpr int HD44780_DISPLAY_CONTROL  {0b00001000};

constexpr int HD44780_CURSOR_OR_DISPLAY_SHIFT  {0b00010000};
constexpr int HD44780_FUNCTION_SET  {0b00100000};
constexpr int HD44780_SET_CGRAM_ADDR  {0b01000000};
constexpr int HD44780_SET_DDRRAM_ADDR  {0b10000000};

#endif