/*! \copyright 2024 Zorxx Software. All rights reserved.
 *  \license This file is released under the MIT License. See the LICENSE file for details.
 *  \brief lcd1602 Library internal interface
 */
#ifndef _LCD1602_PRIVATE_H
#define _LCD1602_PRIVATE_H

#include <stdbool.h>
#include <stdint.h>
#include "lcd1602.h"
#include "sys.h"

typedef struct lcd1602_s
{
    uint8_t i2cAddress;
    bool backlightOn;
    uint64_t nextCommand; /* microsecond tick count when next command may begin */
    i2c_lowlevel_context i2c;
    mutex_lowlevel mutex;
} lcd1602_t;

int lcd1602_ll_init(lcd1602_t *ctx, i2c_lowlevel_config *config);
int lcd1602_ll_deinit(lcd1602_t *ctx);
int lcd1602_ll_mutex_lock(lcd1602_t *ctx);
int lcd1602_ll_mutex_unlock(lcd1602_t *ctx);
uint64_t lcd1602_ll_microsecond_tick(lcd1602_t *ctx);

#endif /* _LCD1602_PRIVATE_H */
