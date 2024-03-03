/* \copyright 2024 Zorxx Software. All rights reserved.
 * \license This file is released under the MIT License. See the LICENSE file for details.
 * \brief lcd1602 library esp-idf example application
 */
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "nvs_flash.h"
#include "esp_netif.h"
#include "esp_event.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "lcd1602.h"

/* The following definitions may change, based on the ESP device,
   LCD1602 configuration, and wiring between them. */
#define ESP_I2C_PORT    I2C_NUM_0
#define ESP_I2C_SDA     GPIO_NUM_21
#define ESP_I2C_SCL     GPIO_NUM_22
#define ESP_I2C_ADDRESS LCD1602_I2C_ADRESS_ALTERNATE

void app_main(void)
{
   ESP_ERROR_CHECK(nvs_flash_init());
   ESP_ERROR_CHECK(esp_netif_init());
   ESP_ERROR_CHECK(esp_event_loop_create_default() );

   lcd1602_lowlevel_config config;
   config.port = ESP_I2C_PORT;
   config.pin_sda = ESP_I2C_SDA;
   config.pin_scl = ESP_I2C_SCL;
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
