/* 
# Copyright (c) 2012, Daynix Computing LTD (www.daynix.com)
# All rights reserved.
#
# Maintained by bricklets@daynix.com
#
# This file is a part of Bricklets library, see "Bricklets" page
# on www.daynix.com for more.
#
# This code is licensed under standard 3-clause BSD license.
# See file LICENSE supplied with this package for the full license text.
*/

/* Extended leakmon implementation file.
 * 1. undef your alloc/free functions
 * 2. include .h file with your alloc/free function prototypes.
 * 3. implement wrappers for your functions. should call your original function + add_res/rem_res.
 * */

#include "dnx_leakmon_aux.h"
#include "dnx_leakmon.h"

#ifdef DNX_LM_MEM
#undef dnx_malloc
#undef dnx_free

#include "dnx_os.h"

void *dnx_lm_malloc(unsigned size, char *file, unsigned line)
{
  void *ptr = dnx_malloc(size);
  if (NULL != ptr)
    dnx_lm_add_res(ptr, file, line, DNX_LM_MEM_ID);

  return ptr;
}

void dnx_lm_free(void *ptr)
{
  dnx_lm_rem_res(ptr);

  dnx_free(ptr);
}

#endif

