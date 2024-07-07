/*! \copyright 2024 Zorxx Software. All rights reserved.
 *  \license This file is released under the MIT License. See the LICENSE file for details.
 *  \brief lcd1602 library Linux example application
 */
#include <stdio.h>
#include "lcd1602/lcd1602.h"

#define MSG(...) fprintf(stderr, __VA_ARGS__)

int main(int argc, char *argv[])
{
   lcd1602_context ctx;
   i2c_lowlevel_config config = { "/dev/i2c-0" };

   ctx = lcd1602_init(LCD1602_I2C_ADDRESS_ALTERNATE, true, &config);
   if(NULL == ctx)
   {
      MSG("Failed to initialize LCD1602\n");
      return -1;
   }

   lcd1602_string(ctx, "Zorxx Software");
   lcd1602_set_cursor(ctx, 1, 0);
   lcd1602_string(ctx, "LCD1602 Library");
   lcd1602_set_display(ctx, true, true, true);

   lcd1602_deinit(ctx);

   return 0;
}
