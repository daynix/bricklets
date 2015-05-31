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

#ifndef __DNX_THREAD_H__
#define __DNX_THREAD_H__

#include "dnx_err.h"
#include "dnx_types.h"

#ifdef __linux__
#ifdef __KERNEL__
#include "../src/linux/kernel/dnx_thread.in"
#else
#include "../src/linux/user/dnx_thread.in"
#endif
#else
#include "../src/windows/user/dnx_thread.in"
#endif


typedef struct dnx_thread dnx_thread_t;

typedef void (*dnx_thread_cb_t)(void *ctx);

dnx_status_t dnx_thread_create(dnx_thread_t *thread, dnx_thread_cb_t cb,
  void *ctx);

void dnx_thread_exit(dnx_thread_t *thread);

bool_t dnx_thread_should_exit(dnx_thread_t *thread);

#endif

