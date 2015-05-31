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

#ifndef __DNX_TIME_H_
#define __DNX_TIME_H_

#include "dnx_types.h"

#define DNX_MSEC_IN_SEC 1000
#define DNX_USEC_IN_MSEC 1000
#define DNX_NSEC_IN_USEC 1000
#define DNX_USEC_IN_SEC (DNX_MSEC_IN_SEC * DNX_USEC_IN_MSEC)
#define DNX_NSEC_IN_MSEC (DNX_NSEC_IN_USEC * DNX_USEC_IN_MSEC)

uint32_t dnx_timestamp(void);

void dnx_msleep(uint32_t msec);

#endif

