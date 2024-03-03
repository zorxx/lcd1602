/* \copyright 2024 Zorxx Software. All rights reserved.
 * \license This file is released under the MIT License. See the LICENSE file for details.
 * \brief lcd1602 I2C protocol definitions
 */
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

#endif /* LCD1602_PROTOCOL_H */