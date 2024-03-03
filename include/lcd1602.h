#ifndef LCD_1602_H
#define LCD_1602_H

#include <stdint.h>
#include <stdbool.h>

typedef void *lcd1602_context;

/* ----------------------------------------------------------------
 * Initialization
 */

#define LCD1602_I2C_ADRESS_DEFAULT   0x27
#define LCD1602_I2C_ADRESS_ALTERNATE 0x3f

#if defined(__linux__)
typedef struct
{
   const char *device;
} lcd1602_lowlevel_config;
#else
   #error "OS type not detected"
#endif

lcd1602_context lcd1602_init(uint16_t i2c_addr, lcd1602_lowlevel_config *config);
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
int lcd1602_set_display(lcd1602_context context, bool displayEnabled, bool cursorEnabled, bool blinkEnabled);
int lcd1602_set_mode(lcd1602_context context, bool leftToRight, bool autoScroll);

int lcd1602_clear(lcd1602_context context);
int lcd1602_home(lcd1602_context context);
int lcd1602_set_cursor(lcd1602_context context, uint16_t row, uint16_t column);
int lcd1602_scroll(lcd1602_context context, eLCD1602ScrollTarget target,
   eLCD1602ScrollTarget direction);

int lcd1602_char(lcd1602_context context, char c);
int lcd1602_string(lcd1602_context context, char *s);

#endif /* LCD_1602_H */