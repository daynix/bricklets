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

#ifndef __DNX_TYPES_H__
#define __DNX_TYPES_H__

#if (defined(__linux__) && defined(__KERNEL__))
#include <linux/types.h>
#else
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <stdio.h>
#endif

typedef int bool_t;

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifdef _WIN32
#define __func__ __FUNCTION__
#endif

#endif
