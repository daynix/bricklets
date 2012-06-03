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

#ifndef __DNX_RMLOCK_H__
#define __DNX_RMLOCK_H__

#include "dnx_err.h"
#include "dnx_types.h"

typedef struct dnx_rmlock dnx_rmlock_t;

dnx_status_t dnx_rmlock_init(dnx_rmlock_t **lock);
void dnx_rmlock_uninit(dnx_rmlock_t *lock);

dnx_status_t dnx_rmlock_add_ex(dnx_rmlock_t *lock, uint32_t count);
dnx_status_t dnx_rmlock_rem_ex(dnx_rmlock_t *lock, uint32_t count);

void dnx_rmlock_reset(dnx_rmlock_t *lock);
dnx_status_t dnx_rmlock_wait(dnx_rmlock_t *lock);

#define dnx_rmlock_add(lock) dnx_rmlock_add_ex(lock, 1)
#define dnx_rmlock_rem(lock) dnx_rmlock_rem_ex(lock, 1)

#endif

