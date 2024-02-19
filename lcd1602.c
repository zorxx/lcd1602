#include <malloc.h>
#include <stdbool.h>
#include "lcd1602_protocol.h"
#include "lcd1602_private.h"
#include "lcd1602.h"

/* Forward function declarations */
static int lcd162_write_byte(lcd1602_t *c, uint8_t value, bool isData, uint32_t delay);

/* -----------------------------------------------------------------------------------------------------------
 * Exported Functions 
 */

lcd1602_context lcd1602_init(uint16_t i2c_addr, uint16_t rows, uint16_t columns,
   lcd1602_lowlevel_config *config)
{
   lcd1602_t *c;

   if(rows >= LCD1602_MAX_ROWS || columns >= LCD1602_MAX_COLUMNS)
      return NULL;

   c = (lcd1602_t *) malloc(sizeof(*c));
   if(NULL == c)
      return NULL;
   c->i2c_addr = i2c_addr;
   c->rows = rows;
   c->columns = columns;
   if(lcd1602_ll_init(c, config) != 0)
   {
      free(c);
      return NULL;
   }
   lcd162_reset(c);
   return (lcd1602_context) c;
}

void lcd1602_deinit(lcd1602_context context)
{
   lcd1602_t *c = (lcd1602_t *) context;
   lcd1602_ll_deinit(c);
   free(c);    
}

int lcd162_reset(lcd1602_context context)
{
   lcd1602_t *c = (lcd1602_t *) context;

   lcd1602_ll_write_byte(c, 0); // put Expander into known state - Register Select and Read/Write both low
   lcd1602_ll_delay(c, 1000);

   lcd1602_ll_write_byte(c, 0x30 | LCD1602_FLAG_ENABLE);
   lcd1602_ll_delay(c, LCD1602_DELAY_ENABLE_PULSE_WIDTH);
   lcd1602_ll_write_byte(c, 0x30);
   lcd1602_ll_delay(c, LCD1602_DELAY_INIT_1);

   lcd1602_ll_write_byte(c, 0x30 | LCD1602_FLAG_ENABLE);
   lcd1602_ll_delay(c, LCD1602_DELAY_ENABLE_PULSE_WIDTH);
   lcd1602_ll_write_byte(c, 0x30);
   lcd1602_ll_delay(c, LCD1602_DELAY_INIT_2); 

   lcd1602_ll_write_byte(c, 0x30 | LCD1602_FLAG_ENABLE);
   lcd1602_ll_delay(c, LCD1602_DELAY_ENABLE_PULSE_WIDTH);
   lcd1602_ll_write_byte(c, 0x30);
   lcd1602_ll_delay(c, LCD1602_DELAY_INIT_3); 

   lcd1602_ll_write_byte(c, 0x20 | LCD1602_FLAG_ENABLE);  // select 4-bit mode
   lcd1602_ll_delay(c, LCD1602_DELAY_ENABLE_PULSE_WIDTH);
   lcd1602_ll_write_byte(c, 0x20);
   lcd1602_ll_delay(c, LCD1602_DELAY_INIT_3); 

   lcd162_write_byte(c, LCD1602_CMD_FUNCTION_SET, false, 0);
   lcd1602_set_display(c, true, false, false);
   lcd1602_set_mode(c, true, true);
   lcd1602_clear(c);
   lcd1602_home(c);

   return 0; 
}

int lcd1602_clear(lcd1602_context context)
{
   return lcd162_write_byte((lcd1602_t *) context, LCD1602_CMD_CLEAR, false, LCD1602_DELAY_CLEAR);
}

int lcd1602_home(lcd1602_context context)
{
   return lcd162_write_byte((lcd1602_t *) context, LCD1602_CMD_HOME, false, LCD1602_DELAY_HOME);
}

int lcd1602_set_display(lcd1602_context context, bool displayEnabled, bool cursorEnabled, bool blinkEnabled)
{
   return lcd162_write_byte((lcd1602_t *) context,
      LCD1602_CMD_DISPLAY_CONTROL
      | (displayEnabled) ? LCD1602_DISPLAY_CONTROL_FLAG_DISPLAY : 0
      | (cursorEnabled)  ? LCD1602_DISPLAY_CONTROL_FLAG_CURSOR  : 0
      | (blinkEnabled)   ? LCD1602_DISPLAY_CONTROL_FLAG_BLINK   : 0, false, 0);
}

int lcd1602_set_mode(lcd1602_context context, bool leftToRight, bool autoScroll)
{
   return lcd162_write_byte((lcd1602_t *) context,
      LCD1602_CMD_ENTRY_MODE_SET
      | (leftToRight) ? LCD1602_ENTRY_MODE_SET_FLAG_INCREMENT : 0
      | (autoScroll)  ? LCD1602_ENTRY_MODE_SET_FLAG_SHIFT     : 0, false, 0);
}

int lcd1602_char(lcd1602_context context, char c)
{
   return lcd162_write_byte((lcd1602_t *) context, c, true, 0); 
}

int lcd1602_string(lcd1602_context context, char *s)
{
   uint32_t count;
   int result;

   for(count = 0; count < LCD1602_MAX_CHAR_WRITE_COUNT && s[count] != '\0'; ++count)
   { 
      result = lcd162_write_byte((lcd1602_t *) context, s[count], true, 0);
      if(0 != result)
         return result;
   }
   return 0;
}

int lcd1602_scroll(lcd1602_context context, eLCD1602ScrollTarget target,
   eLCD1602ScrollTarget direction)
{
   return lcd162_write_byte((lcd1602_t *) context,
      LCD1602_CMD_SHIFT
      | (LCD1602_SCROLL_DISPLAY == target) ? LCD1602_SHIFT_FLAG_DISPLAY : 0
      | (LCD1602_SCROLL_LEFT == direction) ? LCD1602_SHIFT_FLAG_LEFT : 0, false, 0);
}

int lcd1602_set_backlight(lcd1602_context context, bool enable)
{
   lcd1602_t *c = (lcd1602_t *) context;
   c->backlight_on = enable;
   return lcd1602_ll_write_byte(c, (c->backlight_on) ? LCD1602_FLAG_BACKLIGHT_ON : 0); 
}

int lcd1602_set_cursor(lcd1602_context context, uint16_t row, uint16_t column)
{
   lcd1602_t *c = (lcd1602_t *) context;

   if(row >= c->rows || column >= c->columns)
      return -1;

   return lcd162_write_byte((lcd1602_t *) context,
      LCD1602_CMD_SET_DDRAM_ADDR
      | column + LCD1602_ROW_OFFSET[row], false, 0);
}

/* -----------------------------------------------------------------------------------------------------------
 * Private Helper Functions
 */

/* Byte is clocked-in on the falling edge of LCD1602_FLAG_ENABLE */
static int lcd162_write_byte(lcd1602_t *c, uint8_t value, bool isData, uint32_t finalDelay)
{
   uint64_t currentTime = lcd1602_ll_microsecond_tick(c); 
   uint8_t flags = ((c->backlight_on) ? LCD1602_FLAG_BACKLIGHT_ON : 0)
                 | (isData) ? LCD1602_FLAG_RS_DATA : 0;   /* if not isData, then control */

   lcd1602_ll_mutex_lock(c);

   if(c->nextCommand > currentTime)
      lcd1602_ll_delay(c, c->nextCommand - currentTime);

   // Upper nibble of "value"
   lcd1602_ll_write_byte(c, (value & 0xF0) | flags | LCD1602_FLAG_ENABLE);
   lcd1602_ll_delay(c, LCD1602_DELAY_ENABLE_PULSE_WIDTH);
   lcd1602_ll_write_byte(c, (value & 0xF0) | flags & ~LCD1602_FLAG_ENABLE);
   lcd1602_ll_delay(c, LCD1602_DELAY_ENABLE_PULSE_SETTLE);

   // Lower nibble of "value"
   lcd1602_ll_write_byte(c, ((value & 0x0F) << 4) | flags | LCD1602_FLAG_ENABLE);
   lcd1602_ll_delay(c, LCD1602_DELAY_ENABLE_PULSE_WIDTH);
   lcd1602_ll_write_byte(c, ((value & 0x0F) << 4) | flags & ~LCD1602_FLAG_ENABLE);

   c->nextCommand = lcd1602_ll_microsecond_tick(c)
                  + (finalDelay < LCD1602_DELAY_ENABLE_PULSE_SETTLE) ? LCD1602_DELAY_ENABLE_PULSE_SETTLE : finalDelay;

   lcd1602_ll_mutex_unlock(c);

   return 0;
}

// ----------------------------------------------------------------------------------------------------------

#if 0

esp_err_t i2c_lcd1602_define_char(const i2c_lcd1602_info_t * i2c_lcd1602_info, i2c_lcd1602_custom_index_t index, const uint8_t pixelmap[])
{
    esp_err_t err = ESP_FAIL;
    if (_is_init(i2c_lcd1602_info))
    {
        index &= 0x07;  // only the first 8 indexes can be used for custom characters
        err = _write_command(i2c_lcd1602_info, COMMAND_SET_CGRAM_ADDR | (index << 3));
        for (int i = 0; err == ESP_OK && i < 8; ++i)
        {
            err = _write_data(i2c_lcd1602_info, pixelmap[i]);
        }
    }
    return err;
}

#endif