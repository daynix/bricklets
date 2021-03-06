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

#ifndef __DNX_IO_H__
#define __DNX_IO_H__

#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "dnx_types.h"

int dnx_printf(const char *fmt, ...);
int dnx_snprintf(char *str, unsigned size, const char *fmt, ...);
int dnx_vsnprintf(char *str, unsigned size, const char *fmt, va_list va);

void dnx_assert(int cond);

#define DNX_ASSERT(x) dnx_assert((x))

#endif

