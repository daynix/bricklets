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

#include "dnx_event.h"
#include "dnx_types.h"
#include "dnx_err.h"
#include "dnx_io.h"

dnx_status_t dnx_event_init(dnx_event_t *event)
{
  int os_rc = 0;
  dnx_status_t rc = DNX_ERR_OK;

  DNX_ASSERT(NULL != event);

  event->hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
  if (NULL == event->hEvent)
  {
    rc = DNX_ERR_UNKNOWN;
    goto Exit;
  }

Exit:
  if (DNX_ERR_OK != rc && NULL != event)
    dnx_event_uninit(event);

  return rc;
}

void dnx_event_uninit(dnx_event_t *event)
{
  DNX_ASSERT(NULL != event);
  DNX_ASSERT(NULL != event->hEvent);

  CloseHandle(event->hEvent);
}

dnx_status_t dnx_event_wait(dnx_event_t *event, uint32_t msec)
{
  dnx_status_t res = DNX_ERR_UNKNOWN;
  DWORD os_rc = 0;

  DNX_ASSERT(NULL != event);
  DNX_ASSERT(NULL != event->hEvent);

  os_rc = WaitForSingleObject(event->hEvent, msec);
  switch (os_rc)
  {
  case WAIT_OBJECT_0:
    res = DNX_ERR_OK;
    break;
  case WAIT_TIMEOUT:
    res = DNX_ERR_TIMEOUT;
    break;
  default:
    break;
  }
  return res;
}

void dnx_event_set(dnx_event_t *event)
{
  BOOL res;

  DNX_ASSERT(NULL != event);
  DNX_ASSERT(NULL != event->hEvent);

  res = SetEvent(event->hEvent);
  DNX_ASSERT(res == TRUE);
}

void dnx_event_reset(dnx_event_t *event)
{
  BOOL res;
  DNX_ASSERT(NULL != event);
  DNX_ASSERT(NULL != event->hEvent);

  res = ResetEvent(event->hEvent);
  DNX_ASSERT(res == TRUE);
}

