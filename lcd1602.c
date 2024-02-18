#include <malloc.h>
#include "lcd1602_protocol.h"
#include "lcd1602_private.h"
#include "lcd1602.h"

lcd1602_context lcd1602_init(uint16_t i2c_addr, lcd1602_lowlevel_config *config)
{
    lcd1602_t *c = (lcd1602_t *) malloc(sizeof(*c));
    if(NULL == c)
      return NULL;
    c->i2c_addr = i2c_addr;
    if(lcd1602_ll_init(c, config) != 0)
    {
        free(c);
        return NULL;
    }
    return (lcd1602_context) c;
}

void lcd1602_deinit(lcd1602_context context)
{
   lcd1602_t *c = (lcd1602_t *) context;
   lcd1602_ll_deinit(c);
   free(c);    
}

int lcd1602_clear(lcd1602_context context)
{
   lcd1602_t *c = (lcd1602_t *) context;
   return lcd1602_ll_write_byte(c, LCD1602_CMD_CLEAR);
}

int lcd1602_home(lcd1602_context context)
{
   lcd1602_t *c = (lcd1602_t *) context;
   return lcd1602_ll_write_byte(c, LCD1602_CMD_HOME);
}
