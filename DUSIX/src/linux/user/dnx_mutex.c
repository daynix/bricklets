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
#include "dnx_types.h"
#include "dnx_io.h"
#include "dnx_mem.h"

#include <pthread.h>
#include <string.h>

struct dnx_mutex {
  pthread_mutex_t os_mutex;
  bool_t created;
};

static void _dnx_mutex_uninit(dnx_mutex_t *mutex)
{
  if (mutex)
  {
    if (TRUE == mutex->created)
    {
      int os_rc = pthread_mutex_destroy(&mutex->os_mutex);
      DNX_ASSERT(0 == os_rc);
    }

    dnx_free(mutex);
  }
}

dnx_status_t dnx_mutex_init(dnx_mutex_t **mutex)
{
  int os_rc = 0;
  dnx_status_t rc = DNX_ERR_OK;

  *mutex = dnx_malloc(sizeof **mutex);
  if (NULL == *mutex)
  {
    rc = DNX_ERR_NO_MEM;
    goto Exit;
  }

  memset(*mutex, 0, sizeof **mutex);

  os_rc = pthread_mutex_init(&(*mutex)->os_mutex, NULL);
  if (0 != os_rc)
  {
    rc = dnx_os_err_to_dnx_err(os_rc);
    goto Exit;
  }

  (*mutex)->created = TRUE;

Exit:
  if (DNX_ERR_OK != rc)
    _dnx_mutex_uninit(*mutex);

  return rc;
}

void dnx_mutex_uninit(dnx_mutex_t *mutex)
{
  DNX_ASSERT(NULL != mutex);
  DNX_ASSERT(TRUE == mutex->created);

  _dnx_mutex_uninit(mutex);
}

static void dnx_mutex_operation(dnx_mutex_t *mutex, bool_t lock)
{ 
  int os_rc = 0;

  DNX_ASSERT(NULL != mutex);

  os_rc = lock ? pthread_mutex_lock(&mutex->os_mutex) : 
                 pthread_mutex_unlock(&mutex->os_mutex);
  DNX_ASSERT (0 == os_rc);
}

void dnx_mutex_lock(dnx_mutex_t *mutex)
{  
  dnx_mutex_operation(mutex, TRUE);
}

void dnx_mutex_unlock(dnx_mutex_t *mutex)
{
  dnx_mutex_operation(mutex, FALSE);
}

