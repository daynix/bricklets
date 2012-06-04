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
#include "dnx_io.h"

void dnx_thread_exit(dnx_thread_t *thread)
{
  DNX_ASSERT(NULL != thread);

  thread->should_exit = TRUE;

  WaitForSingleObject(thread->hthread, INFINITE);
}

static DWORD dnx_thread_run(LPVOID arg)
{
  dnx_thread_t *thread = (dnx_thread_t *)arg;
  dnx_log_x("Running New Thread\n");

  thread->cb(thread->ctx);

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

  thread->hthread = CreateThread(NULL, 0, 
    (LPTHREAD_START_ROUTINE)dnx_thread_run, thread, 0, NULL);

  if (NULL == thread->hthread)
    rc = DNX_ERR_UNKNOWN;

  return rc;
}

bool_t dnx_thread_should_exit(dnx_thread_t *thread)
{
  DNX_ASSERT(NULL != thread);

  return thread->should_exit;
}


