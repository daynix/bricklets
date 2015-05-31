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

#include "dnx_rmlock.h"
#include "dnx_string.h"
#include "dnx_io.h"

dnx_status_t dnx_rmlock_init(dnx_rmlock_t *lock)
{
  dnx_status_t rc = DNX_ERR_OK;

  DNX_ASSERT(NULL != lock);
  lock->event_initialized = FALSE;

  rc = dnx_event_init(&lock->event);
  if (DNX_ERR_OK != rc)
    goto Exit;

  lock->event_initialized = TRUE;

  dnx_atomic32_set(&lock->cnt, 0);

Exit:
  if (DNX_ERR_OK != rc && NULL != lock)
    dnx_rmlock_uninit(lock);

  return rc;
}

void dnx_rmlock_uninit(dnx_rmlock_t *lock)
{
  DNX_ASSERT(NULL != lock);

  if (TRUE == lock->event_initialized)
    dnx_event_uninit(&lock->event);
}

dnx_status_t dnx_rmlock_add_ex(dnx_rmlock_t *lock, uint32_t count)
{
  DNX_ASSERT(NULL != lock);

  dnx_atomic32_add(&lock->cnt, count);
  return DNX_ERR_OK;
}

dnx_status_t dnx_rmlock_rem_ex(dnx_rmlock_t *lock, uint32_t count)
{  
  DNX_ASSERT(NULL != lock);

  dnx_atomic32_sub(&lock->cnt, count);

  if (0 == dnx_atomic32_get(&lock->cnt))
    dnx_event_set(&lock->event);

  return DNX_ERR_OK;
}

void dnx_rmlock_reset(dnx_rmlock_t *lock)
{
  DNX_ASSERT(NULL != lock);

  dnx_event_reset(&lock->event);
}

dnx_status_t dnx_rmlock_wait(dnx_rmlock_t *lock)
{
  DNX_ASSERT(NULL != lock);

  return dnx_event_wait(&lock->event, DNX_WAIT_FOREVER);
}

