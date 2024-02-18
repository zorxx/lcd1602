#include <unistd.h>
#include <malloc.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include "helpers.h"
#include "lcd1602_private.h"
#include "lcd1602.h"

typedef struct esp_lcd1602_s
{
    char *device;
    int handle;
} linux_lcd1602_t;

int lcd1602_ll_init(lcd1602_t *ctx, lcd1602_lowlevel_config *config)
{
   linux_lcd1602_t *l;
   int result = -1;

   l = (linux_lcd1602_t *) malloc(sizeof(*l));
   if(NULL == l)
   {
      LCDERR("[%s] Failed to allocate low-level structure\n", __func__);
      return -1;
   }

   l->handle = -1;
   l->device = strdup(config->device);
   if(NULL == l->device)
   {
      LCDERR("[%s] Failed to open device '%s'\n", __func__, l->device);
   }
   else
   {
      l->handle = open(l->device, O_RDONLY);
      if(l->handle < 0)
      {
         LCDERR("[%s] Failed to open device '%s'\n", __func__, l->device);
      }
      else if(ioctl(l->handle, I2C_SLAVE, ctx->i2c_addr) < 0)
      {
         LCDERR("[%s] Failed to set I2C slave address to 0x%02x\n", __func__, ctx->i2c_addr);
      }
      else
         result = 0;
   }

   if(0 == result)
   {
      ctx->lowlevel = l;
   }
   else
   {
      if(l->handle >= 0)
         close(l->handle);
      free(l);
   }
   return result;
}

int lcd1602_ll_deinit(lcd1602_t *ctx)
{
   linux_lcd1602_t *l = (linux_lcd1602_t *) ctx->lowlevel; 
   if(NULL != l->device)
      free(l->device);
   free(l);
   return 0;
}

int lcd1602_ll_write_byte(lcd1602_t *ctx, uint8_t byte)
{
   linux_lcd1602_t *l = (linux_lcd1602_t *) ctx->lowlevel;
   struct i2c_smbus_ioctl_data args;
   union i2c_smbus_data data;

   args.read_write = I2C_SMBUS_WRITE;
   args.command = 0; 
   args.size = sizeof(byte); 
   args.data = &data; 
   return ioctl(l->handle, I2C_SMBUS, &args);
}