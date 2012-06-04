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
#include "dnx_err.h"
#include "dnx_io.h"
#include "dnx_mem.h"
#include "dnx_string.h"

#include <linux/sched.h>
#include <linux/wait.h>

#define DNX_EVENT_FLAG_MUTEX_INITIALIZED  0x1
#define DNX_EVENT_FLAG_COND_INITIALIZED   0x2

struct dnx_event
{
  wait_queue_head_t wait_queue;
  volatile int      wait_flag;
};

dnx_status_t dnx_event_init(dnx_event_t **event)
{
  dnx_status_t rc = DNX_ERR_OK;
  *event = dnx_malloc(sizeof **event);
  if (NULL == *event)
  {
    rc = DNX_ERR_NO_MEM;
    goto Exit;
  }

  dnx_memset(*event, 0, sizeof **event);

  (*event)->wait_flag = 0;
  init_waitqueue_head(&(*event)->wait_queue);

Exit:
  if (DNX_ERR_OK != rc && NULL != *event)
    dnx_event_uninit(*event);

  return rc;
}

void dnx_event_uninit(dnx_event_t *event)
{
  DNX_ASSERT(NULL != event);

  dnx_free(event);
}

dnx_status_t dnx_event_wait(dnx_event_t *event, uint32_t msec)
{
  int res;

  DNX_ASSERT(NULL != event);

  res = msec?wait_event_timeout(event->wait_queue,
                                event->wait_flag,
                                msecs_to_jiffies(msec)):event->wait_flag;

  if (event->wait_flag)
    res = DNX_ERR_OK;
  else if (res == 0)
    res = DNX_ERR_TIMEOUT;
  else {
    // make sure we cover all cases
    printk(KERN_ALERT "wait_event_timeout returned %d", res);
    DNX_ASSERT(FALSE);
  }

  return res;
}

void dnx_event_set(dnx_event_t *event)
{
  DNX_ASSERT(NULL != event);

  event->wait_flag = 1;
  wake_up(&event->wait_queue);
}

void dnx_event_reset(dnx_event_t *event)
{
  DNX_ASSERT(NULL != event);

  event->wait_flag = 0;
}

