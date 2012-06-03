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

#include "dnx_thread.h"
#include "dnx_logger.h"
#include "dnx_event.h"
#include "dnx_string.h"
#include "dnx_io.h"
#include "dnx_mem.h"

#include <pthread.h>

struct dnx_thread {
  pthread_t pthread;
  dnx_thread_cb_t cb;
  dnx_event_t *event;
  void *ctx;
  volatile bool_t should_exit;
};

void dnx_thread_exit(dnx_thread_t *thread)
{ 
  dnx_status_t rc = DNX_ERR_OK;
  DNX_ASSERT(NULL != thread);
  DNX_ASSERT(FALSE == thread->should_exit);

  thread->should_exit = TRUE;

  rc = dnx_event_wait(thread->event, DNX_WAIT_FOREVER);

  DNX_ASSERT(DNX_ERR_OK == rc);

  dnx_event_uninit(thread->event);
  dnx_free(thread);
}

static void *dnx_thread_run(void *arg)
{
  dnx_thread_t *thread = (dnx_thread_t *)arg;
  dnx_log_x("Running New Thread\n");

  thread->cb(thread->ctx);

  dnx_event_set(thread->event);

  return NULL;
}

dnx_status_t dnx_thread_create(dnx_thread_t **thread, dnx_thread_cb_t cb,
  void *ctx)
{
  dnx_status_t rc = DNX_ERR_OK;
  int os_rc = 0;

  DNX_ASSERT(NULL != cb);

  *thread = dnx_malloc(sizeof **thread);
  if (NULL == *thread)
  {
    dnx_log_e("Failed to allocate thread context\n");
    rc = DNX_ERR_NO_MEM;
    goto Exit;
  }

  dnx_memset(*thread, 0, sizeof **thread);

  (*thread)->cb = cb;
  (*thread)->ctx =ctx;

  rc = dnx_event_init(&(*thread)->event); 
  if (DNX_ERR_OK != rc)
  {
    dnx_log_e("Failed to initialize event object\n");
    goto Exit;
  }

  os_rc = pthread_create(&(*thread)->pthread, NULL, dnx_thread_run, *thread);

  if (0 != os_rc)
    rc = dnx_os_err_to_dnx_err(os_rc);

Exit:
  if (DNX_ERR_OK != rc && NULL != *thread)
  {
    if (NULL != (*thread)->event)
      dnx_event_uninit((*thread)->event);

    dnx_free(*thread);
  }

  return rc;
}

bool_t dnx_thread_should_exit(dnx_thread_t *thread)
{
  DNX_ASSERT(NULL != thread);

  return thread->should_exit;
}

