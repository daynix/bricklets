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

#include "dnx_time.h"

#include <windows.h>

uint32_t dnx_timestamp(void)
{
  LARGE_INTEGER lint;

  (void)QueryPerformanceCounter(&lint);
  return (uint32_t)lint.LowPart;
}

void dnx_msleep(uint32_t msec)
{
  Sleep(msec);
}

