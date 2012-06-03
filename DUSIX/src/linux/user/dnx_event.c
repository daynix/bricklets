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

#include "dnx_event.h"
#include "dnx_mem.h"
#include "dnx_io.h"
#include "dnx_time.h"

#include <pthread.h>
#include <time.h>
#include <string.h>

#define DNX_EVENT_FLAG_MUTEX_INITIALIZED  0x1
#define DNX_EVENT_FLAG_COND_INITIALIZED   0x2

struct dnx_event {
  pthread_mutex_t mutex;
  pthread_cond_t cond;
  uint8_t flags;
  volatile bool_t wait;
};

dnx_status_t dnx_event_init(dnx_event_t **event)
{
  int os_rc = 0;
  dnx_status_t rc = DNX_ERR_OK;
  *event = dnx_malloc(sizeof **event);
  if (NULL == *event)
  {
    rc = DNX_ERR_NO_MEM;
    goto Exit;
  }

  memset(*event, 0, sizeof **event);

  os_rc = pthread_mutex_init(&((*event)->mutex), NULL);
  if (0 != os_rc)
  {
    rc = dnx_os_err_to_dnx_err(os_rc);
    goto Exit;
  }

  (*event)->flags |= DNX_EVENT_FLAG_MUTEX_INITIALIZED;

  os_rc = pthread_cond_init(&((*event)->cond), NULL);
  if (0 != os_rc)
  {
    rc = dnx_os_err_to_dnx_err(os_rc);
    goto Exit;
  }

  (*event)->flags |= DNX_EVENT_FLAG_COND_INITIALIZED;

  (*event)->wait = TRUE;

Exit:
  if (DNX_ERR_OK != rc && NULL != *event)
    dnx_event_uninit(*event);

  return rc;
}

void dnx_event_uninit(dnx_event_t *event)
{
  int os_rc;

  DNX_ASSERT(NULL != event);

  if (event->flags & DNX_EVENT_FLAG_COND_INITIALIZED)
  {
    os_rc = pthread_cond_destroy(&event->cond);
    DNX_ASSERT(0 == os_rc);
  }

  if (event->flags & DNX_EVENT_FLAG_MUTEX_INITIALIZED)
  {
    os_rc = pthread_mutex_destroy(&event->mutex);
    DNX_ASSERT(0 == os_rc);
  }

  dnx_free(event);
}

dnx_status_t dnx_event_wait(dnx_event_t *event, uint32_t msec)
{
  int os_rc = 0, mutex_rc;
  struct timespec wait_ts;
  DNX_ASSERT(NULL != event);

  if (DNX_WAIT_FOREVER != msec)
  {
    clock_gettime(CLOCK_REALTIME, &wait_ts);

    wait_ts.tv_sec += msec / DNX_MSEC_IN_SEC;
    wait_ts.tv_nsec += (msec % DNX_MSEC_IN_SEC) * DNX_NSEC_IN_MSEC;
  }

  mutex_rc = pthread_mutex_lock(&event->mutex);
  DNX_ASSERT(!mutex_rc);

  while (TRUE == event->wait &&  0 == os_rc)
  {
    if (DNX_WAIT_FOREVER == msec)
      os_rc = pthread_cond_wait(&event->cond, &event->mutex);
    else
      os_rc = pthread_cond_timedwait(&event->cond, &event->mutex, &wait_ts);
  }

  mutex_rc = pthread_mutex_unlock(&event->mutex);
  DNX_ASSERT(0 == mutex_rc);

  return (0 == os_rc ? DNX_ERR_OK : dnx_os_err_to_dnx_err(os_rc));
}

void dnx_event_set(dnx_event_t *event)
{
  int os_rc = 0;
  DNX_ASSERT(NULL != event);

  os_rc = pthread_mutex_lock(&event->mutex);
  DNX_ASSERT(0 == os_rc);

  event->wait = FALSE;

  pthread_cond_signal(&event->cond);

  os_rc = pthread_mutex_unlock(&event->mutex);
  DNX_ASSERT(0 == os_rc);
}

void dnx_event_reset(dnx_event_t *event)
{
  DNX_ASSERT(NULL != event);

  event->wait = TRUE;
}

