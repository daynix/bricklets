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

#ifndef __DNX_ATOMIC_H__
#define __DNX_ATOMIC_H__

#include "dnx_types.h"

typedef volatile uint32_t dnx_atomic32_t;

#ifdef __linux__
#include "../src/linux/user/dnx_atomic_lnx.h"
#else
#include "../src/windows/user/dnx_atomic_win.h"
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

