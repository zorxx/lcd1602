#ifndef LCD_1602_H
#define LCD_1602_H

#include <stdint.h>

#define LCD1602_I2C_ADRESS_DEFAULT   0x27
#define LCD1602_I2C_ADRESS_ALTERNATE 0x3f

typedef void *lcd1602_context;

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

int lcd1602_clear(lcd1602_context context);
int lcd1602_home(lcd1602_context context);

#endif /* LCD_1602_H */