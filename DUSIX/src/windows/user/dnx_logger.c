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

#include "dnx_logger.h"
#include "dnx_err.h"
#include "dnx_types.h"
#include "dnx_io.h"
#include "dnx_time.h"

#include <windows.h>
#include <stdio.h>

static int duplicate_to_stderr = 0;
static const char *os_ident = 0;
static dnx_log_level_t os_max_level = DNX_LOG_LEVEL_EXTRA;
static uint32_t log_flags = 0;

dnx_status_t dnx_logger_init(const char *ident, uint8_t flags,
  dnx_log_level_t max_level)
{

  log_flags = flags;
  os_ident = ident;
  duplicate_to_stderr = flags & DNX_LOG_FLAG_TO_STDERR;
  os_max_level = max_level;

  return DNX_ERR_OK;
}

void dnx_logger_uninit(void)
{
}

void dnx_log(const char *func, uint32_t line,
  dnx_log_level_t level, const char *fmt, ...)
{
  char buff[DNX_MAX_LOG_BUFF] = {0};
  int offset = 0, n = 0;
  va_list va;

  if (level > os_max_level)
    return;

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

  switch (level) {
  case DNX_LOG_LEVEL_ERROR:
    n = dnx_snprintf(buff + offset, DNX_MAX_LOG_BUFF - offset, "Error: ");
    break;
  case DNX_LOG_LEVEL_WARNING:
    n = dnx_snprintf(buff + offset, DNX_MAX_LOG_BUFF - offset, "Warning: ");
    break;
  case DNX_LOG_LEVEL_INFO:
    n = dnx_snprintf(buff + offset, DNX_MAX_LOG_BUFF - offset, "Info: ");
    break;
  case DNX_LOG_LEVEL_EXTRA:
    n = dnx_snprintf(buff + offset, DNX_MAX_LOG_BUFF - offset, "Extra: ");
    break;
  default:
    DNX_ASSERT(0);
  } 
  if (n >= DNX_MAX_LOG_BUFF - offset)
    {
      goto Exit;
    }

  offset += n;

  n = dnx_snprintf(buff + offset, DNX_MAX_LOG_BUFF - offset, "[%s: (%d)]: ", func,
    line);
  if (n >= DNX_MAX_LOG_BUFF - offset)
  {
    goto Exit;
  }
  offset += n;

  if (os_ident)
  {
    n = dnx_snprintf(buff + offset, DNX_MAX_LOG_BUFF - offset, "%s: ",
      os_ident);
    /* if ident too long. skip it. */
    if (n < DNX_MAX_LOG_BUFF - offset)
    {
      offset +=n;
    }
  }

  va_start(va, fmt);
  n = dnx_vsnprintf(buff + offset, DNX_MAX_LOG_BUFF - offset, fmt, va);
  va_end(va);

  if (n >= DNX_MAX_LOG_BUFF - offset)
  {
    buff[DNX_MAX_LOG_BUFF - 1] = 0;
  }

Exit:
  OutputDebugStringA(buff);

  if (duplicate_to_stderr)
    fprintf(stderr, "%s\n", buff);
}

