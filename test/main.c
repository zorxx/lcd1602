#include <stdio.h>
#include "helpers.h"
#include "lcd1602.h"

int main(int argc, char *argv[])
{
   lcd1602_context ctx;
   lcd1602_lowlevel_config config = { "/dev/i2c-0" };

   ctx = lcd1602_init(LCD1602_I2C_ADRESS_DEFAULT, &config);
   if(NULL == ctx)
   {
      LCDERR("Failed to initialize LCD1602\n");
   }

   lcd1602_clear(ctx);
   lcd1602_home(ctx);

   lcd1602_deinit(ctx);

   return 0;
}