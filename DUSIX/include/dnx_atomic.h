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

#ifndef __DNX_ATOMIC_H__
#define __DNX_ATOMIC_H__

#include "dnx_types.h"

#if defined(__linux__) && defined (__KERNEL__)
typedef atomic_t dnx_atomic32_t;
#else
typedef volatile uint32_t dnx_atomic32_t;
#endif

#ifdef __linux__ 
#ifdef __KERNEL__
#include "../src/linux/kernel/dnx_atomic.in"
#else
#include "../src/linux/user/dnx_atomic.in"
#endif
#else
#include "../src/windows/user/dnx_atomic.in"
#endif

/* Atomic Ops Interface */
/*
 * void     dnx_atomic32_set(dnx_atomic32_t *a);
 * uint32_t dnx_atomic32_get(dnx_atomic32_t *a);
 * uint32_t dnx_atomic32_add(dnx_atomic32_t *a, uint32_t val);
 * uint32_t dnx_atomic32_sub(dnx_atomic32_t *a, uint32_t val);
 * uint32_t dnx_atomic32_inc(dnx_atomic32_t *a);
 * uint32_t dnx_atomic32_dec(dnx_atomic32_t *a);
 *
 */

#endif

