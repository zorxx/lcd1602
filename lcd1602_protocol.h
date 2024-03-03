#ifndef LCD1602_PROTOCOL_H
#define LCD1602_PROTOCOL_H

#define LCD1602_I2C_TRANSFER_TIMEOUT       50 /* (milliseconds) give up on i2c transaction after this timeout */
#define LCD1602_MAX_DELAY                  10000  /* (microseconds) never need to wait longer than 10ms between i2c transactions */
#define LCD1602_DELAY_ENABLE_PULSE_WIDTH   1  /* (microseconds) enable pulse must be at least 450ns wide */
#define LCD1602_DELAY_ENABLE_PULSE_SETTLE  38 /* (microseconds) command requires > 37us to settle */

#define LCD1602_MAX_CHAR_WRITE_COUNT 256 
#define LCD1602_MAX_ROWS      4
#define LCD1602_MAX_COLUMNS   20

/* ------------------------------------------------------------------------------
 * Commands
 */

#define LCD1602_CMD_CLEAR           (1 << 0)
   #define LCD1602_DELAY_CLEAR    1640 /* microseconds */

#define LCD1602_CMD_HOME            (1 << 1)
   #define LCD1602_DELAY_HOME     1640 /* microseconds */

#define LCD1602_CMD_ENTRY_MODE_SET  (1 << 2)
   #define LCD1602_ENTRY_MODE_SET_FLAG_INCREMENT  0x02 /* left-to-right, if set; right-to-left if not */
   #define LCD1602_ENTRY_MODE_SET_FLAG_SHIFT      0x01 /* auto-scroll if set */

#define LCD1602_CMD_DISPLAY_CONTROL (1 << 3)
   #define LCD1602_DISPLAY_CONTROL_FLAG_DISPLAY  0x04
   #define LCD1602_DISPLAY_CONTROL_FLAG_CURSOR   0x02
   #define LCD1602_DISPLAY_CONTROL_FLAG_BLINK    0x01

#define LCD1602_CMD_SHIFT           (1 << 4)
   #define LCD1602_SHIFT_FLAG_DISPLAY   0x08 /* shift display if set; cursor if not */
   #define LCD1602_SHIFT_FLAG_LEFT      0x04 /* shift left if set, right if not*/

#define LCD1602_CMD_FUNCTION_SET    (1 << 5)
   #define FLAG_FUNCTION_SET_MODE_8BIT      0x10 /* disabled: 4-bit */
   #define FLAG_FUNCTION_SET_LINES_2        0x08 /* disabled: 1 line */
   #define FLAG_FUNCTION_SET_DOTS_5X10      0x04 /* disabled: 5x8 */

#define LCD1602_CMD_SET_CGRAM_ADDR  (1 << 6)
#define LCD1602_CMD_SET_DDRAM_ADDR  (1 << 7)
#define LCD1602_ROW_OFFSET "\x00\x40\0x14\0x54"

/* Control flags (low nibble of each i2c byte) */
#define LCD1602_FLAG_BACKLIGHT_ON    0b00001000   /* backlight enabled (disabled if clear) */
#define LCD1602_FLAG_ENABLE          0b00000100   /* Data clock enable (falling edge clocks data) */
#define LCD1602_FLAG_READ            0b00000010   /* read (write if clear) */
#define LCD1602_FLAG_RS_DATA         0b00000001   /* data (command if clear) */

// Use the second set (0bxxxx1xxx) to avoid placing the null character within a string
#define I2C_LCD1602_CHARACTER_CUSTOM_0     0b00001000   ///< User-defined custom symbol in index 0
#define I2C_LCD1602_CHARACTER_CUSTOM_1     0b00001001   ///< User-defined custom symbol in index 1
#define I2C_LCD1602_CHARACTER_CUSTOM_2     0b00001010   ///< User-defined custom symbol in index 2
#define I2C_LCD1602_CHARACTER_CUSTOM_3     0b00001011   ///< User-defined custom symbol in index 3
#define I2C_LCD1602_CHARACTER_CUSTOM_4     0b00001100   ///< User-defined custom symbol in index 4
#define I2C_LCD1602_CHARACTER_CUSTOM_5     0b00001101   ///< User-defined custom symbol in index 5
#define I2C_LCD1602_CHARACTER_CUSTOM_6     0b00001110   ///< User-defined custom symbol in index 6
#define I2C_LCD1602_CHARACTER_CUSTOM_7     0b00001111   ///< User-defined custom symbol in index 7

#define I2C_LCD1602_CHARACTER_ALPHA        0b11100000   ///< Lower-case alpha symbol
#define I2C_LCD1602_CHARACTER_BETA         0b11100010   ///< Lower-case beta symbol
#define I2C_LCD1602_CHARACTER_THETA        0b11110010   ///< Lower-case theta symbol
#define I2C_LCD1602_CHARACTER_PI           0b11110111   ///< Lower-case pi symbol
#define I2C_LCD1602_CHARACTER_OMEGA        0b11110100   ///< Upper-case omega symbol
#define I2C_LCD1602_CHARACTER_SIGMA        0b11110110   ///< Upper-case sigma symbol
#define I2C_LCD1602_CHARACTER_INFINITY     0b11110011   ///< Infinity symbol
#define I2C_LCD1602_CHARACTER_DEGREE       0b11011111   ///< Degree symbol
#define I2C_LCD1602_CHARACTER_ARROW_RIGHT  0b01111110   ///< Arrow pointing right symbol
#define I2C_LCD1602_CHARACTER_ARROW_LEFT   0b01111111   ///< Arrow pointing left symbol
#define I2C_LCD1602_CHARACTER_SQUARE       0b11011011   ///< Square outline symbol
#define I2C_LCD1602_CHARACTER_DOT          0b10100101   ///< Centred dot symbol
#define I2C_LCD1602_CHARACTER_DIVIDE       0b11111101   ///< Division sign symbol
#define I2C_LCD1602_CHARACTER_BLOCK        0b11111111   ///< 5x8 filled block

#endif /* LCD1602_PROTOCOL_H */