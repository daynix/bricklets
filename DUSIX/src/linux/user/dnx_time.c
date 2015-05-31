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

#include "dnx_time.h"

#include <sys/time.h>
#include <unistd.h>

uint32_t dnx_timestamp(void)
{
  struct timeval ts;
  uint32_t rc, timestamp = 0;
  rc = gettimeofday(&ts, 0);
  if (0 != rc)
    goto Exit;

    timestamp = ts.tv_usec + (ts.tv_sec * DNX_USEC_IN_SEC);
Exit:
  return timestamp;
}

void dnx_msleep(uint32_t msec)
{
  (void)usleep(DNX_USEC_IN_MSEC * msec);
}

