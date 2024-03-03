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

void app_main(void)
{
   ESP_ERROR_CHECK(nvs_flash_init());
   ESP_ERROR_CHECK(esp_netif_init());
   ESP_ERROR_CHECK(esp_event_loop_create_default() );

   lcd1602_lowlevel_config config;
   config.port = I2C_NUM_0;
   config.pin_sda = GPIO_NUM_21;
   config.pin_scl = GPIO_NUM_22;
   lcd1602_context *ctx = lcd1602_init(LCD1602_I2C_ADRESS_ALTERNATE, true, &config);
   if(NULL != ctx)
   {
      lcd1602_string(ctx, "Hello!");
      lcd1602_set_cursor(ctx, 1, 0);
      lcd1602_string(ctx, "ESP-IDF");
      lcd1602_set_display(ctx, true, true, true);
      lcd1602_deinit(ctx);
   }

   for(;;)
      vTaskDelay(portMAX_DELAY);
}
