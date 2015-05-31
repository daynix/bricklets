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

#include "dnx_leakmon.h"
#include "dnx_os.h"
#include "queue.h"

#define DNX_MAX_F_NAME_LEN 32
#define DNX_MAX_ENUM_BUFF 1024

#ifdef DNX_LM_ENABLE

typedef struct dnx_res {
  void *res;
  const char *f;
  unsigned line;
  unsigned id;
  LIST_ENTRY(dnx_res) next;
} dnx_res_t;

static LIST_HEAD(lm_list_s, dnx_res) lm_list;
static dnx_mutex_t *lm_mutex = NULL;
static int lm_count = 0;

int dnx_lm_start(void)
{
  int rc = dnx_mutex_init(&lm_mutex);
  if (0 != rc)
  {
    dnx_log("Failed to initialize leakmon mutex\n");
    goto Exit;
  }

  LIST_INIT(&lm_list);

Exit:
  return rc;
}

void dnx_lm_end(void)
{
  char str[DNX_MAX_ENUM_BUFF];

  if (0 == lm_count)
    goto Exit;

  dnx_lm_enum(str, DNX_MAX_ENUM_BUFF, 0);

  dnx_log("ASSERTION: Existing entries in Leak Monitor List. Enumerating:\n");
  dnx_log("%s", str);
  DNX_ASSERT(0);

Exit:
  dnx_mutex_uninit(lm_mutex);
}

static void dnx_lm_add_res_after(dnx_res_t *prev, dnx_res_t *new)
{
  if (NULL == prev)
    LIST_INSERT_HEAD(&lm_list, new, next);
  else
    LIST_INSERT_AFTER(prev, new, next);
}

int dnx_lm_add_res(void *res, const char *f, unsigned line, unsigned id)
{
  int rc = 0;
  dnx_res_t *cur = NULL, *prev = NULL;
  dnx_res_t *new = (dnx_res_t *)dnx_malloc(sizeof *new);
  if (NULL == new)
  {
    dnx_log("Failed to allocate memory for leak monitor resource\n");
    rc = -1;
    goto Exit;
  }

  new->res = res;
  new->f = f;
  new->line = line;
  new->id = id;

  dnx_mutex_lock(lm_mutex);
  LIST_FOREACH(cur, &lm_list, next) 
  {
    int result = dnx_strncmp(cur->f, new->f, DNX_MAX_F_NAME_LEN);
    if (result > 0)
    {
      break;
    } 
    else if (0 == result)
    {
      if (cur->line >= new->line)
        break;
    }

    prev = cur;
  }

  dnx_lm_add_res_after(prev, new);
  lm_count++;
  dnx_mutex_unlock(lm_mutex);

Exit:
  if (0 != rc && NULL != new)
    dnx_free(new);

  return rc;
}

void dnx_lm_rem_res(void *res)
{
  dnx_res_t *cur = NULL;

  dnx_mutex_lock(lm_mutex);
  LIST_FOREACH(cur, &lm_list, next)
  {
    if (cur->res != res)
      continue;

    LIST_REMOVE(cur, next);
    dnx_free(cur);
    break;
  }
  lm_count--;
  dnx_mutex_unlock(lm_mutex);
}

int dnx_lm_enum(char *str, unsigned len, unsigned id)
{
  dnx_res_t *cur = NULL;
  int written = 0;

  written = dnx_snprintf(str, len, "Start Resource Enumeration:\n");

  if (written >= len)
    goto Exit;

  dnx_mutex_lock(lm_mutex);
  LIST_FOREACH(cur, &lm_list, next)
  {
    if (0 == id || id == cur->id)
    {
      written += dnx_snprintf(str + written, len - written, 
        "%s(%d): Res Addr: %p, ID: %u\n", cur->f, cur->line, cur->res,
        cur->id);
      if (written >= len)
        break;
    }
  }

  dnx_mutex_unlock(lm_mutex);
  if (written < len)
    written += dnx_snprintf(str + written, len - written, 
      "End Resource Enumeration.\n");

Exit:
  return written;
}

#endif

