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

#include "dnx_mutex.h"
#include "dnx_io.h"

static void _dnx_mutex_uninit(dnx_mutex_t *mutex)
{
  if (NULL != mutex)
  {
    if (NULL != mutex->os_mutex)
    {
      BOOL os_rc = CloseHandle(mutex->os_mutex);
      DNX_ASSERT(TRUE == os_rc);
    }
  }
}

dnx_status_t dnx_mutex_init(dnx_mutex_t *mutex)
{
  int os_rc = 0;
  dnx_status_t rc = DNX_ERR_OK;

  mutex->os_mutex = CreateMutex(NULL, FALSE, NULL);

  if (NULL == mutex->os_mutex)
  {
    rc = DNX_ERR_UNKNOWN;
    goto Exit;
  }

Exit:
  if (DNX_ERR_OK != rc)
    (void)_dnx_mutex_uninit(mutex);

  return rc;
}

void dnx_mutex_uninit(dnx_mutex_t *mutex)
{
  int os_rc = 0;

  DNX_ASSERT(NULL != mutex);
  DNX_ASSERT(NULL != mutex->os_mutex);

  _dnx_mutex_uninit(mutex);
}

void dnx_mutex_lock(dnx_mutex_t *mutex)
{
  DWORD waitResult;
  DNX_ASSERT(NULL != mutex);
  DNX_ASSERT(NULL != mutex->os_mutex);

  waitResult = WaitForSingleObject(mutex->os_mutex, INFINITE);

  DNX_ASSERT(WAIT_OBJECT_0 == waitResult);
}

void dnx_mutex_unlock(dnx_mutex_t *mutex)
{ 
  BOOL os_rc = ReleaseMutex(mutex->os_mutex);
  DNX_ASSERT(TRUE == os_rc);
}

