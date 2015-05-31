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

/* Extended leakmon interface file.
 * Add prototypes of wrapper alloc/free functions. */

#ifndef _DNX_LEAKMON_AUX_H_
#define _DNX_LEAKMON_AUX_H_

/* add optional resource ID's 
 * 0 is used for all resources. */
#define DNX_LM_MEM_ID 1

#ifdef DNX_LM_MEM
#ifndef DNX_LM_ENABLE
#error "LeakMonitor module not enabled."
#else
#define dnx_malloc(size) dnx_lm_malloc((size), (char *)__FILE__, __LINE__)
#define dnx_free dnx_lm_free

void *dnx_lm_malloc(unsigned size, char *file, unsigned line);
void dnx_lm_free(void *ptr);

#endif
#endif

#endif

