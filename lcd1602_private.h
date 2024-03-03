#ifndef _LCD1602_PRIVATE_H
#define _LCD1602_PRIVATE_H

#include <stdbool.h>
#include <stdint.h>
#include "lcd1602.h"

typedef struct lcd1602_s
{
    uint16_t i2c_addr;
    bool backlight_on;
    uint64_t nextCommand; /* tick count when next command may begin */
    void *lowlevel;
} lcd1602_t;

int lcd1602_ll_init(lcd1602_t *ctx, lcd1602_lowlevel_config *config);
int lcd1602_ll_deinit(lcd1602_t *ctx);
int lcd1602_ll_write_byte(lcd1602_t *ctx, uint8_t data);
int lcd1602_ll_delay(lcd1602_t *ctx, uint32_t microseconds);
int lcd1602_ll_mutex_lock(lcd1602_t *ctx);
int lcd1602_ll_mutex_unlock(lcd1602_t *ctx);
uint64_t lcd1602_ll_microsecond_tick(lcd1602_t *ctx);

#endif /* _LCD1602_PRIVATE_H */