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

#include "dnx_mutex.h"
#include "dnx_mem.h"
#include "dnx_string.h"
#include "dnx_io.h"

#include <linux/semaphore.h>

struct dnx_mutex
{
  struct semaphore sem;
};

dnx_status_t dnx_mutex_init(dnx_mutex_t **mutex)
{
  dnx_status_t rc = DNX_ERR_OK;

  *mutex = dnx_malloc(sizeof **mutex);
  if (NULL == *mutex)
  {
    rc = DNX_ERR_NO_MEM;
    goto Exit;
  }

  dnx_memset(*mutex, 0, sizeof **mutex);

  sema_init(&(*mutex)->sem, 1);

Exit:
  if (DNX_ERR_OK != rc && NULL != *mutex)
    (void)dnx_mutex_uninit(*mutex);

  return rc;
}

void dnx_mutex_uninit(dnx_mutex_t *mutex)
{
  DNX_ASSERT(NULL != mutex);

  dnx_free(mutex);
}

void dnx_mutex_lock(dnx_mutex_t *mutex)
{
  DNX_ASSERT(NULL != mutex);
  down(&mutex->sem);
}

void dnx_mutex_unlock(dnx_mutex_t *mutex)
{
  DNX_ASSERT(NULL != mutex);
  up(&mutex->sem);
}

