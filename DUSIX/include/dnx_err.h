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

#ifndef __DNX_ERR_H__
#define __DNX_ERR_H__

typedef enum {
  DNX_ERR_OK = 0,
  DNX_ERR_NO_MEM = -1,
  DNX_ERR_UNKNOWN = -2,
  DNX_ERR_NO_ENTRY = -3,
  DNX_ERR_AGAIN = -4,
  DNX_ERR_PERM = -5,
  DNX_ERR_INVAL = -6,
  DNX_ERR_BUSY = -7,
  DNX_ERR_TIMEOUT = -8,
  DNX_ERR_LAST
} dnx_status_t;

dnx_status_t dnx_os_err_to_dnx_err(int rc);

#endif /* __DNX_ERR_H__ */

