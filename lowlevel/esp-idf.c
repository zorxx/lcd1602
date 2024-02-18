#include "driver/i2c.h"
#include "lcd1602_protocol.h"
#include "lcd1602.h"

typedef struct esp_lcd1602_s
{
    i2c_port_t port;
    ssize_t timeout;
} esp_lcd1602_t;

/* ----------------------------------------------------------------------------------------------
 * Exported Functions
 */

int lcd1602_ll_init(lcd1602_t *ctx, lcd1602_lowlevel_config *config)
{
    esp_lcd1602_t *l = (esp_lcd1602_t *) calloc(1, sizeof(*l));
    if(NULL == l)
      return -1; 
    l->port = port;
    l->timeout = pdMS_TO_TICKS(LCD1602_I2C_TRANSFER_TIMEOUT);
    ctx->lowlevel = l; 
    return 0;
}

void lcd1602_ll_deinit(lcd1602_t *ctx)
{
   esp_lcd1602_t *l = (esp_lcd1602_t *) ctx->lowlevel;
   free(l);
}

/* ----------------------------------------------------------------------------------------------
 * Helper Functions
 */

int lcd1602_ll_write_byte(const lcd1602_t *c, const uint8_t byte)
{
   esp_lcd1602_t *l = (esp_lcd1602_t *) ctx->lowlevel;
   i2c_cmd_handle_t cmd = i2c_cmd_link_create();
   esp_err_t ret;

   ret = i2c_master_start(cmd);
   assert(ESP_OK == ret);
   ret = i2c_master_write_byte(cmd, c->dev_addr | I2C_MASTER_WRITE, true);
   assert(ESP_OK == ret);
   ret = i2c_master_write_byte(cmd, byte, true);
   assert(ESP_OK == ret);
   ret = i2c_master_stop(cmd);
   assert(ESP_OK == ret);
   ret = i2c_master_cmd_begin(l->port, cmd, l->timeout); 
   i2c_cmd_link_delete(cmd);

   return ret;
}