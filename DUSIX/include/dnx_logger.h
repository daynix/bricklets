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

#ifndef __DNX_LOGGER_H__
#define __DNX_LOGGER_H__

#include "dnx_err.h"
#include "dnx_types.h"

typedef enum dnx_log_level {
  DNX_LOG_LEVEL_ERROR,
  DNX_LOG_LEVEL_WARNING,
  DNX_LOG_LEVEL_INFO,
  DNX_LOG_LEVEL_EXTRA
} dnx_log_level_t;

#define DNX_MAX_LOG_BUFF 512

#define DNX_LOG_FLAG_TO_STDERR  0x01 /* also print to stderr */
#define DNX_LOG_FLAG_TIMESTAMP  0x02 /* print timestamp */

dnx_status_t dnx_logger_init(const char *ident, uint8_t flags,
  dnx_log_level_t max_level);

void dnx_logger_uninit(void);

void dnx_log(const char *func, uint32_t line,
  dnx_log_level_t level, const char *fmt, ...);

#define dnx_log_e(fmt, ...) dnx_log(__func__, __LINE__, DNX_LOG_LEVEL_ERROR, \
  (fmt), ## __VA_ARGS__)

#define dnx_log_w(fmt, ...) dnx_log(__func__, __LINE__, \
  DNX_LOG_LEVEL_WARNING, (fmt), ## __VA_ARGS__)

#define dnx_log_i(fmt, ...) dnx_log(__func__, __LINE__, DNX_LOG_LEVEL_INFO, \
  (fmt), ## __VA_ARGS__)

#define dnx_log_x(fmt, ...) dnx_log(__func__, __LINE__, DNX_LOG_LEVEL_EXTRA, \
  (fmt), ## __VA_ARGS__)

#endif

