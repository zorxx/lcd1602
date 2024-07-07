/*! \copyright 2024 Zorxx Software. All rights reserved.
 *  \license This file is released under the MIT License. See the LICENSE file for details.
 *  \brief lcd1602 library API
 */
#ifndef LCD_1602_H
#define LCD_1602_H

#include <stdint.h>
#include <stdbool.h>  /* Requires C99 */

#if defined(__linux__)
   #include "lcd1602/sys_linux.h"
#elif defined(ESP_PLATFORM)
   #include "lcd1602/sys_esp.h"
#else
   #error "Supported OS type not detected"
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef void *lcd1602_context;

/* ----------------------------------------------------------------
 * Initialization
 */

#define LCD1602_I2C_ADDRESS_DEFAULT   0x27
#define LCD1602_I2C_ADDRESS_ALTERNATE 0x3f

lcd1602_context lcd1602_init(uint8_t i2cAddress, bool backlightOn, i2c_lowlevel_config *config);
void lcd1602_deinit(lcd1602_context context);

/* ----------------------------------------------------------------
 * Functions 
 */

typedef enum
{
   LCD1602_SCROLL_DISPLAY,
   LCD1602_SCROLL_CURSOR
} eLCD1602ScrollTarget;

typedef enum
{
   LCD1602_SCROLL_LEFT,
   LCD1602_SCROLL_RIGHT,
} eLCD1602ScrollDirection;

int lcd1602_reset(lcd1602_context context);
int lcd1602_set_backlight(lcd1602_context context, bool enable);
int lcd1602_set_display(lcd1602_context context, bool displayEnabled,
   bool cursorEnabled, bool blinkEnabled);
int lcd1602_set_mode(lcd1602_context context, bool leftToRight, bool autoScroll);

int lcd1602_clear(lcd1602_context context);
int lcd1602_home(lcd1602_context context);
int lcd1602_set_cursor(lcd1602_context context, uint16_t row, uint16_t column);
int lcd1602_scroll(lcd1602_context context, eLCD1602ScrollTarget target,
   eLCD1602ScrollDirection direction);

int lcd1602_char(lcd1602_context context, char c);
int lcd1602_string(lcd1602_context context, char *s);

#ifdef __cplusplus
}
#endif

#endif /* LCD_1602_H */
