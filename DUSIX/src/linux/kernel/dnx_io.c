/* 
* Copyright (c) 2012, Daynix Computing LTD (www.daynix.com)
* All rights reserved.
*
* Maintained by bricklets@daynix.com
*
* This file is a part of the Bricklets library, please see the wiki page
* on https://github.com/daynix/bricklets/wiki/Bricklets-library for more.
*
* This code is licensed under standard 3-clause BSD license.
* See file LICENSE supplied with this package for the full license text.
*/

#include "dnx_io.h"
#include "dnx_logger.h"

int dnx_printf(const char *fmt, ...)
{
  int ret;
  va_list va;
  char buff[DNX_MAX_LOG_BUFF];

  va_start(va, fmt);
  ret = vsnprintf(buff, DNX_MAX_LOG_BUFF, fmt, va);
  va_end(va);

  printk("%s", buff);

  return ret;
}

int dnx_snprintf(char *str, unsigned size, const char *fmt, ...)
{
  int ret;
  va_list va;

  va_start(va, fmt);

  ret = vsnprintf(str, size, fmt, va);

  va_end(va);

  return ret;
}

int dnx_vsnprintf(char *str, unsigned size, const char *fmt, va_list va)
{
  return vsnprintf(str, size, fmt, va);
}

void dnx_assert(int cond)
{
  if (unlikely(!(cond)))
  {
    dnx_log_e("Assertion failed!\n");
    BUG();
  }
}

