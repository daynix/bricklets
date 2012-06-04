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

#include <linux/delay.h>
#include <linux/jiffies.h>

uint32_t dnx_timestamp(void)
{
  return jiffies_to_msecs(jiffies);
}

void dnx_msleep(uint32_t msec)
{
  msleep(msec);
}

