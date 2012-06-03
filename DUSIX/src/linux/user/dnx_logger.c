
#include "dnx_logger.h"
#include "dnx_err.h"
#include "dnx_io.h"
#include "dnx_time.h"

#include <syslog.h>
#include <stdarg.h>
#include <stdio.h>

static uint32_t log_flags = 0;

static int dnx_convert_prioroty(dnx_log_level_t level)
{  
  switch (level)
  {
  case DNX_LOG_LEVEL_ERROR:
    return LOG_ERR;
  case DNX_LOG_LEVEL_WARNING:
    return LOG_WARNING;
  case DNX_LOG_LEVEL_INFO:
    return LOG_INFO;
  case DNX_LOG_LEVEL_EXTRA:
    return LOG_DEBUG;
  default:
    DNX_ASSERT(0);
    return -1;
  }
}

dnx_status_t dnx_logger_init(const char *ident, uint8_t flags,
  dnx_log_level_t max_level)
{
  log_flags = flags;
  openlog(ident, flags & DNX_LOG_FLAG_TO_STDERR ? LOG_PERROR : 0, LOG_USER);

  setlogmask(LOG_UPTO(dnx_convert_prioroty(max_level)));

  return DNX_ERR_OK;
}

void dnx_logger_uninit(void)
{
  closelog();
}

void dnx_log(const char *func, uint32_t line,
  dnx_log_level_t level, const char *fmt, ...)
{
  char buff[DNX_MAX_LOG_BUFF] = {0};
  int n = 0, offset = 0;
  va_list va;

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

  va_start(va, fmt);
  vsnprintf(buff + offset, DNX_MAX_LOG_BUFF - offset, fmt, va);
  va_end(va);

Exit:
  syslog(dnx_convert_prioroty(level), "%s", buff);
}

