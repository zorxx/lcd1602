/* \copyright 2024 Zorxx Software. All rights reserved.
 * \license This file is released under the MIT License. See the LICENSE file for details.
 * \brief lcd1602 Library esp-idf implementation 
 */
#include "driver/i2c.h"
#include "rom/ets_sys.h"
#include "esp_timer.h"
#include "lcd1602_private.h"
#include "lcd1602_protocol.h"
#include "lcd1602.h"

typedef struct
{
   i2c_port_t port;
   uint32_t timeout;
} esp_lcd1602_t;

#define I2C_MASTER_FREQ_HZ 100000
#define I2C_RX_BUFFER_SIZE 64
#define I2C_TX_BUFFER_SIZE 64

/* ----------------------------------------------------------------------------------------------
 * lcd1602 low-level implementation for esp-idf 
 */

int lcd1602_ll_init(lcd1602_t *ctx, lcd1602_lowlevel_config *config)
{
   bool success = false;

   esp_lcd1602_t *l = (esp_lcd1602_t *) calloc(1, sizeof(*l));
   if(NULL == l)
      return -1; 

   l->port = config->port;
   l->timeout = pdMS_TO_TICKS(LCD1602_I2C_TRANSFER_TIMEOUT);
   i2c_config_t conf = {
      .mode = I2C_MODE_MASTER,
      .sda_io_num = config->pin_sda,
      .sda_pullup_en = GPIO_PULLUP_ENABLE,
      .scl_io_num = config->pin_scl,
      .scl_pullup_en = GPIO_PULLUP_ENABLE,
      .master.clk_speed = I2C_MASTER_FREQ_HZ,
      .clk_flags = 0,
   };
   if(i2c_param_config(config->port, &conf) == ESP_OK
   && i2c_driver_install(config->port, I2C_MODE_MASTER, I2C_RX_BUFFER_SIZE, I2C_TX_BUFFER_SIZE, 0) == ESP_OK)
   {
      ctx->lowlevel = l; 
      success = true;
   }

   return (success) ? 0  : -1;
}

int lcd1602_ll_deinit(lcd1602_t *ctx)
{
   esp_lcd1602_t *l = (esp_lcd1602_t *) ctx->lowlevel;
   i2c_driver_delete(l->port);
   free(l);
   return 0;
}

int lcd1602_ll_write_byte(lcd1602_t *ctx, uint8_t data)
{
   esp_lcd1602_t *l = (esp_lcd1602_t *) ctx->lowlevel;
   i2c_cmd_handle_t cmd = i2c_cmd_link_create();
   esp_err_t result = ESP_FAIL;

   if(i2c_master_start(cmd) == ESP_OK
   && i2c_master_write_byte(cmd, (ctx->i2cAddress << 1) | I2C_MASTER_WRITE, true) == ESP_OK
   && i2c_master_write_byte(cmd, data, true) == ESP_OK
   && i2c_master_stop(cmd) == ESP_OK
   && i2c_master_cmd_begin(l->port, cmd, l->timeout) == ESP_OK)
   {
      result = ESP_OK;
   }
   i2c_cmd_link_delete(cmd);
   return result; 
}

int lcd1602_ll_delay(lcd1602_t *ctx, uint32_t microseconds)
{
   /* This is a busy wait. TODO: convert to task delay */
   ets_delay_us(microseconds);
   return 0;
}

int lcd1602_ll_mutex_lock(lcd1602_t *ctx)
{
   /* TODO */
   return 0;
}

int lcd1602_ll_mutex_unlock(lcd1602_t *ctx)
{
   /* TODO */
   return 0;
}

uint64_t lcd1602_ll_microsecond_tick(lcd1602_t *ctx)
{
   return esp_timer_get_time(); /* microseconds since boot */
}