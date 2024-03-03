#ifndef _LCD1602_HELPERS_H
#define _LCD1602_HELPERS_H

#include <stdio.h>

#if defined(LCD_DEBUG_ENABLE)
#define LCDERR(...) fprintf(stderr, __VA_ARGS__)
#define LCDDBG(...) fprintf(stderr, __VA_ARGS__)
#else
#define LCDERR(...)
#define LCDDBG(...)
#endif

#endif /* _LCD1602_HELPERS_H */