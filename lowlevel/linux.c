#include <unistd.h>
#include <malloc.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <inttypes.h>
#include <pthread.h>
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
    pthread_mutex_t lock;
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
      LCDERR("[%s] Memory allocation error\n", __func__);
   }
   else
   {
      l->handle = open(l->device, O_RDONLY);
      if(l->handle < 0)
      {
         LCDERR("[%s] Failed to open device '%s'\n", __func__, l->device);
      }
      else if(ioctl(l->handle, I2C_SLAVE, ctx->i2cAddress) < 0)
      {
         LCDERR("[%s] Failed to set I2C slave address to 0x%02x\n", __func__, ctx->i2cAddress);
      }
      else
         result = 0;
   }

   if(0 == result)
   {
      ctx->lowlevel = l;
      pthread_mutex_init(&l->lock, NULL);
   }
   else
   {
      if(l->handle >= 0)
         close(l->handle);
      free(l);
   }

   LCDERR("[%s] result %d\n", __func__, result);
   return result;
}

int lcd1602_ll_deinit(lcd1602_t *ctx)
{
   linux_lcd1602_t *l = (linux_lcd1602_t *) ctx->lowlevel; 

   if(NULL == l)
      return 0;

   if(l->handle >= 0)
      close(l->handle);
   pthread_mutex_destroy(&l->lock);
   if(NULL != l->device)
      free(l->device);
   free(l);

   return 0;
}

int lcd1602_ll_write_byte(lcd1602_t *ctx, uint8_t data)
{
   linux_lcd1602_t *l = (linux_lcd1602_t *) ctx->lowlevel;
   struct i2c_smbus_ioctl_data args;
   union i2c_smbus_data smdata;
   uint64_t time = lcd1602_ll_microsecond_tick(ctx);
   int result;

   smdata.byte = data;
   args.read_write = I2C_SMBUS_WRITE;
   args.command = 0; 
   args.size = I2C_SMBUS_BYTE_DATA;  
   args.data = &smdata; 
   result = ioctl(l->handle, I2C_SMBUS, &args);
   if(0 != result)
   {
      LCDERR("[%s] Failed (result %d, errno %d)\n", __func__, result, errno);
      return result;
   }

   LCDDBG("[%s %016" PRIu64 "] 0x%02x\n", __func__, time, data);
   return 0; 
}

int lcd1602_ll_delay(lcd1602_t *ctx, uint32_t microseconds)
{
   uint64_t time = lcd1602_ll_microsecond_tick(ctx);
   LCDDBG("[%s %016" PRIu64 "] delay %" PRIu32 "\n", __func__, time, microseconds);
   usleep(microseconds);
   return 0;
}

int lcd1602_ll_mutex_lock(lcd1602_t *ctx)
{
   linux_lcd1602_t *l = (linux_lcd1602_t *) ctx->lowlevel;
   pthread_mutex_lock(&l->lock);
   return 0;
}

int lcd1602_ll_mutex_unlock(lcd1602_t *ctx)
{
   linux_lcd1602_t *l = (linux_lcd1602_t *) ctx->lowlevel;
   pthread_mutex_unlock(&l->lock);
   return 0;
}

uint64_t lcd1602_ll_microsecond_tick(lcd1602_t *ctx)
{
   struct timespec ts;
   if(clock_gettime(CLOCK_MONOTONIC, &ts) != 0)
   {
      LCDERR("[%s] Failed to query time (errno %d)\n", __func__, errno);
      memset(&ts, 0, sizeof(ts)); /* no reasonable recourse */
   }
   return ((uint64_t)ts.tv_nsec) / 1000 + (((uint64_t)ts.tv_sec) * 1000000UL);
}