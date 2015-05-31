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

#ifndef __DNX_MUTEX_H__
#define __DNX_MUTEX_H__

#include "dnx_err.h"

#ifdef __linux__
#ifdef __KERNEL__
#include "../src/linux/kernel/dnx_mutex.in"
#else
#include "../src/linux/user/dnx_mutex.in"
#endif
#else
#include "../src/windows/user/dnx_mutex.in"
#endif

typedef struct dnx_mutex dnx_mutex_t;

dnx_status_t dnx_mutex_init(dnx_mutex_t *mutex);
void         dnx_mutex_uninit(dnx_mutex_t *mutex);

void         dnx_mutex_lock(dnx_mutex_t *mutex);
void         dnx_mutex_unlock(dnx_mutex_t *mutex);

#endif
