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

#ifndef _DNX_LEAKMON_H_
#define _DNX_LEAKMON_H_

#ifdef DNX_LM_ENABLE

int dnx_lm_start(void);

void dnx_lm_end(void);

int dnx_lm_add_res(void *res, const char *f, unsigned line, unsigned id);

void dnx_lm_rem_res(void *res);

int dnx_lm_enum(char *str, unsigned len, unsigned id);

#else

#define dnx_lm_start()

#define dnx_lm_end()

#define dnx_lm_dump()

#define dnx_lm_enum()

#endif

#endif

