/* 
* Copyright (c) 2012, Daynix Computing LTD (www.daynix.com)
* All rights reserved.
*
* Maintained by bricklets@daynix.com
*
* This file is a part of Bricklets library, see "Bricklets" page
* on www.daynix.com for more.
*
* This code is licensed under standard 3-clause BSD license.
* See file LICENSE supplied with this package for the full license text.
*/

#include "dnx_logger.h"
#include "dnx_err.h"
#include "dnx_io.h"
#include "dnx_time.h"

static dnx_log_level_t dnx_max_dlevel = DNX_LOG_LEVEL_EXTRA;
static uint8_t log_flags = 0;

dnx_status_t dnx_logger_init(const char *ident, uint8_t flags,
  dnx_log_level_t max_level)
{
  dnx_max_dlevel = max_level;

  log_flags = flags;

  return DNX_ERR_OK;
}

void dnx_logger_uninit(void)
{

}

void dnx_log(const char *func, uint32_t line,
  dnx_log_level_t level, const char *fmt, ...)
{
  char buff[DNX_MAX_LOG_BUFF] = {0};
  int n = 0, offset = 0;
  va_list va;

  if (level > dnx_max_dlevel)
    goto Exit;

  if (log_flags & DNX_LOG_FLAG_TIMESTAMP)
  {
    n = dnx_snprintf(buff + offset, DNX_MAX_LOG_BUFF - offset, "[%010u] ",
      dnx_timestamp());
    if (n >= DNX_MAX_LOG_BUFF - offset)
    {
      goto Exit;
    }
    offset += n;
  }

  n = dnx_snprintf(buff + offset, DNX_MAX_LOG_BUFF - offset, "[%s: (%d)]: ", func,
    line);
  if (n >= DNX_MAX_LOG_BUFF - offset)
  {
    goto Exit;
  }
  offset += n;

  va_start(va, fmt);
  dnx_vsnprintf(buff + offset, DNX_MAX_LOG_BUFF - offset, fmt, va);
  va_end(va);

Exit:
  switch (level)
  {
  case DNX_LOG_LEVEL_ERROR:
    printk(KERN_ERR "%s", buff);
    return;
  case DNX_LOG_LEVEL_WARNING:
    printk(KERN_WARNING "%s", buff);
    return;
  case DNX_LOG_LEVEL_INFO:
    printk(KERN_INFO "%s", buff);
    return;
  case DNX_LOG_LEVEL_EXTRA:
    printk(KERN_DEBUG "%s", buff);
    return;
  default:
    DNX_ASSERT(0);
    break;
  }
}


