#ifndef _LCD1602_PRIVATE_H
#define _LCD1602_PRIVATE_H

#include <stdint.h>
#include "lcd1602.h"

typedef struct lcd1602_s
{
    uint16_t i2c_addr;
    void *lowlevel;
} lcd1602_t;

int lcd1602_ll_init(lcd1602_t *ctx, lcd1602_lowlevel_config *config);
int lcd1602_ll_deinit(lcd1602_t *ctx);
int lcd1602_ll_write_byte(lcd1602_t *ctx, uint8_t byte);

#endif /* _LCD1602_PRIVATE_H */