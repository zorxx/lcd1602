/*! \copyright 2024 Zorxx Software. All rights reserved.
 *  \license This file is released under the MIT License. See the LICENSE file for details.
 *  \brief lcd1602 library esp-idf example application
 */
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "nvs_flash.h"
#include "esp_event.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "lcd1602/lcd1602.h"

#define LCD1602_LIBRARY_I2C_BUS_INIT 0
#define TAG "lcd1602"

/* The following definitions may change, based on the ESP device,
   LCD1602 configuration, and wiring between them. */
#define ESP_I2C_PORT    I2C_NUM_0
#define ESP_I2C_SDA     GPIO_NUM_21
#define ESP_I2C_SCL     GPIO_NUM_22
#define ESP_I2C_ADDRESS LCD1602_I2C_ADDRESS_ALTERNATE
static i2c_master_bus_handle_t i2c_bus;

void app_main(void)
{
   i2c_lowlevel_config config = {0}; /* ensure initialize to zero */

   ESP_ERROR_CHECK(nvs_flash_init());
   ESP_ERROR_CHECK(esp_event_loop_create_default() );

   #if LCD1602_LIBRARY_I2C_BUS_INIT
   /* Allow the rtci2c library to initialize the i2c bus */
   config.port = ESP_I2C_PORT;
   config.pin_sda = ESP_I2C_SDA;
   config.pin_scl = ESP_I2C_SCL;

   #else
   /* Initialize the i2c bus the way we want. Optionally keep the "bus"
      pointer to allow the use of the same bus instance for other i2c
      device communication. */
   i2c_master_bus_config_t bus_cfg = {
      .clk_source = I2C_CLK_SRC_DEFAULT,
      .i2c_port = ESP_I2C_PORT, 
      .sda_io_num = ESP_I2C_SDA,
      .scl_io_num = ESP_I2C_SCL,
      .glitch_ignore_cnt = 7,
      .flags.enable_internal_pullup = true,      
   };
   if(i2c_new_master_bus(&bus_cfg, &i2c_bus) != ESP_OK)
   {
      ESP_LOGE(TAG, "Failed to initialize I2C bus");
   }
   else
      config.bus = &i2c_bus;
   #endif

   lcd1602_context *ctx = lcd1602_init(ESP_I2C_ADDRESS, true, &config);
   if(NULL != ctx)
   {
      lcd1602_string(ctx, "Zorxx Software");
      lcd1602_set_cursor(ctx, 1, 0);
      lcd1602_string(ctx, "LCD1602 Library");
      lcd1602_set_display(ctx, true, true, true);
      lcd1602_deinit(ctx);
   }

   for(;;)
      vTaskDelay(portMAX_DELAY);
}
