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

#ifndef __DNX_ATOMIC_LNX_H__
#define __DNX_ATOMIC_LNX_H__

#include <linux/slab.h>

/* do not include this file directly. use dnx_atomic.h instead */

static inline uint32_t dnx_atomic32_get(dnx_atomic32_t *a)
{
  return atomic_read(a);
}

static inline void dnx_atomic32_set(dnx_atomic32_t *a, uint32_t val)
{
  (void)atomic_set(a, val);
}

static inline uint32_t dnx_atomic32_add(dnx_atomic32_t *a, uint32_t val)
{
  return atomic_add_return(val, a);
}

static inline uint32_t dnx_atomic32_sub(dnx_atomic32_t *a, uint32_t val)
{
  return atomic_sub_return(val, a);
}

static inline uint32_t dnx_atomic32_inc(dnx_atomic32_t *a)
{
  return atomic_inc_return(a);
}

static inline uint32_t dnx_atomic32_dec(dnx_atomic32_t *a)
{
  return atomic_dec_return(a);
}

#endif

