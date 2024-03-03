/* \copyright 2024 Zorxx Software. All rights reserved.
 * \license This file is released under the MIT License. See the LICENSE file for details.
 * \brief lcd1602 library Linux example application
 */
#include <stdio.h>
#include "helpers.h"
#include "lcd1602.h"

int main(int argc, char *argv[])
{
   lcd1602_context ctx;
   lcd1602_lowlevel_config config = { "/dev/i2c-0" };

   ctx = lcd1602_init(LCD1602_I2C_ADRESS_ALTERNATE, true, &config);
   if(NULL == ctx)
   {
      LCDERR("Failed to initialize LCD1602\n");
      return -1;
   }

   lcd1602_string(ctx, "Hello!");
   lcd1602_set_cursor(ctx, 1, 5);
   lcd1602_string(ctx, "There");
   lcd1602_set_display(ctx, true, true, true);

   lcd1602_deinit(ctx);

   return 0;
}