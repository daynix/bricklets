/* 
# Copyright (c) 2012, Daynix Computing LTD (www.daynix.com)
# All rights reserved.
#
# Maintained by bricklets@daynix.com
#
# This file is a part of the Bricklets library, please see the wiki page
# on https://github.com/daynix/bricklets/wiki/Bricklets-library for more.
#
# This code is licensed under standard 3-clause BSD license.
# See file LICENSE supplied with this package for the full license text.
*/

/* os abstruction interface. */
#include "dnx_os.h"

/* leakmon core interface */
#include "dnx_leakmon.h"

/* leakmon extended interface */
#include "dnx_leakmon_aux.h"

int main(int argc, char **argv)
{
  char *x = NULL, *y = NULL, *z = NULL;
  char str[1024];

  /* start monitoring from here */
  dnx_lm_start();

  x = (char *)dnx_malloc(10);
  if (NULL == x)
    goto Exit;

  y = (char *)dnx_malloc(10);
  if (NULL == y)
    goto Exit;

  z = (char *)dnx_malloc(10);
  if (NULL == z)
    goto Exit;

  dnx_lm_enum(str, 1024, 0);
  dnx_log("%s", str);

Exit:
  if (NULL != x)
    dnx_free(x);

  /* this is commented out on purpose to demonstrate assert in leakmon */ 
#if 0 
  if (NULL != y)
    dnx_free(y);
#endif
  if (NULL != z)
    dnx_free(z);

  /* end monitor. This will assert! */
  dnx_lm_end();
  return 0;
}

