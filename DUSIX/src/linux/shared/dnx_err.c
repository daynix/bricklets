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

#include "dnx_err.h"

#include <errno.h>

dnx_status_t dnx_os_err_to_dnx_err(int rc)
{
  switch (rc) 
  {
  case EAGAIN: 
    return DNX_ERR_AGAIN;
  case ENOMEM: 
    return DNX_ERR_NO_MEM;
  case EPERM: 
    return DNX_ERR_PERM;
  case EBUSY: 
    return DNX_ERR_BUSY;
  case EINVAL: 
    return DNX_ERR_INVAL;
  default:
    return DNX_ERR_UNKNOWN;
  }
}

