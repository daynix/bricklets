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

#include "dnx_os.h"

#include <pthread.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

void dnx_assert(int cond)
{
  assert(cond);
}

void *dnx_malloc(unsigned size)
{
  return malloc(size);
}

void dnx_free(void *ptr)
{
  free(ptr);
}

struct dnx_mutex {
  pthread_mutex_t os_mutex;
  int created;
};

static void _dnx_mutex_uninit(dnx_mutex_t *mutex)
{
  if (mutex)
  {
    if (TRUE == mutex->created)
    {
      int os_rc = pthread_mutex_destroy(&mutex->os_mutex);
      DNX_ASSERT(0 == os_rc);
    }

    dnx_free(mutex);
  }
}

int dnx_mutex_init(dnx_mutex_t **mutex)
{
  int rc = 0;

  *mutex = (dnx_mutex_t *)dnx_malloc(sizeof **mutex);
  if (NULL == *mutex)
  {
    rc = ENOMEM;
    goto Exit;
  }

  memset(*mutex, 0, sizeof **mutex);

  rc = pthread_mutex_init(&(*mutex)->os_mutex, NULL);
  if (0 != rc)
    goto Exit;

  (*mutex)->created = TRUE;

Exit:
  if (0 != rc)
    _dnx_mutex_uninit(*mutex);

  return rc;
}

void dnx_mutex_uninit(dnx_mutex_t *mutex)
{
  DNX_ASSERT(NULL != mutex);
  DNX_ASSERT(TRUE == mutex->created);

  _dnx_mutex_uninit(mutex);
}

static void dnx_mutex_operation(dnx_mutex_t *mutex, int lock)
{ 
  int rc = 0;

  DNX_ASSERT(NULL != mutex);

  rc = lock ? pthread_mutex_lock(&mutex->os_mutex) : 
    pthread_mutex_unlock(&mutex->os_mutex);
  DNX_ASSERT (0 == rc);
}

void dnx_mutex_lock(dnx_mutex_t *mutex)
{  
  dnx_mutex_operation(mutex, TRUE);
}

void dnx_mutex_unlock(dnx_mutex_t *mutex)
{
  dnx_mutex_operation(mutex, FALSE);
}

int dnx_log(const char *fmt, ...)
{
  int ret;
  va_list va;

  va_start(va, fmt);

  ret = vprintf(fmt, va);

  va_end(va);

  return ret;
}

int dnx_strncmp(const char *str1, const char *str2, unsigned num)
{
  return strncmp(str1, str2, num);
}

int dnx_snprintf(char *str, unsigned size, const char *fmt, ...)
{
  int ret;
  va_list va;

  va_start(va, fmt);

  ret = vsnprintf(str, size, fmt, va);

  va_end(va);

  return ret;
}

