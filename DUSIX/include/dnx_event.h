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

#ifndef __DNX_EVENT_H__
#define __DNX_EVENT_H__

#include "dnx_err.h"
#include "dnx_types.h"

#define DNX_WAIT_FOREVER ~((uint32_t)0)

typedef struct dnx_event dnx_event_t;

dnx_status_t dnx_event_init(dnx_event_t **event);
void dnx_event_uninit(dnx_event_t *event);

dnx_status_t dnx_event_wait(dnx_event_t *event, uint32_t msec);
void dnx_event_set(dnx_event_t *event);
void dnx_event_reset(dnx_event_t *event);

#endif

