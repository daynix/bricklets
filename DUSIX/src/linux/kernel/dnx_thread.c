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
#include "dnx_io.h"
#include "dnx_logger.h"
#include "dnx_string.h"

void dnx_thread_exit(dnx_thread_t *thread)
{ 
  dnx_status_t rc = DNX_ERR_OK;
  DNX_ASSERT(NULL != thread);
  DNX_ASSERT(FALSE == thread->should_exit);

  thread->should_exit = TRUE;

  kthread_stop(thread->kthread);

  rc = dnx_event_wait(&thread->event, DNX_WAIT_FOREVER);

  DNX_ASSERT(DNX_ERR_OK == rc);

  dnx_event_uninit(&thread->event);
}

static int dnx_thread_run(void *arg)
{
  dnx_thread_t *thread = (dnx_thread_t *)arg;
  dnx_log_x("Running New Thread\n");

  thread->cb(thread->ctx);

  dnx_event_set(&thread->event);

  return 0;
}

dnx_status_t dnx_thread_create(dnx_thread_t *thread, dnx_thread_cb_t cb,
  void *ctx)
{
  dnx_status_t rc = DNX_ERR_OK;
  int os_rc = 0;

  DNX_ASSERT(NULL != thread);
  DNX_ASSERT(NULL != cb);

  thread->cb = cb;
  thread->ctx =ctx;
  thread->event_initialized = FALSE;

  rc = dnx_event_init(&thread->event); 
  if (DNX_ERR_OK != rc)
  {
    dnx_log_e("Failed to initialize event object\n");
    goto Exit;
  }

  thread->event_initialized = TRUE;
  thread->kthread = kthread_run(dnx_thread_run, thread,
    "dnx_tread");

  if (NULL == thread->kthread)
    rc = dnx_os_err_to_dnx_err(os_rc);

Exit:
  if (DNX_ERR_OK != rc && NULL != thread)
  {
    if (TRUE == thread->event_initialized)
      dnx_event_uninit(&thread->event);
  }

  return rc;
}

bool_t dnx_thread_should_exit(dnx_thread_t *thread)
{
  DNX_ASSERT(NULL != thread);

  return thread->should_exit;
}

