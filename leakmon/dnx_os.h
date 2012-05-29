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

#ifndef _DNX_OS_H_
#define _DNX_OS_H_

#ifndef NULL
#define NULL '\0'
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

void dnx_assert(int cond);

#define DNX_ASSERT(x) dnx_assert((x))

/* memory allocation interface */
void *dnx_malloc(unsigned size);
void dnx_free(void *ptr);

/* mutex interface */
typedef struct dnx_mutex dnx_mutex_t;

int dnx_mutex_init(dnx_mutex_t **mutex);
void dnx_mutex_uninit(dnx_mutex_t *mutex);

void dnx_mutex_lock(dnx_mutex_t *mutex);
void dnx_mutex_unlock(dnx_mutex_t *mutex);

/* log interface */
int dnx_log(const char *fmt, ...);

/* String interface */
int dnx_strncmp(const char *str1, const char *str2, unsigned num);

/* stdio interface */
int dnx_snprintf(char *str, unsigned size, const char *fmt, ...);

#endif

