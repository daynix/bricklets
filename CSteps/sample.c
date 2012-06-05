
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#ifdef WIN32
typedef unsigned long      uint32_t;
#else
#include <stdint.h>
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#define DNX_ASSERT assert

#define DNX_CS_LOG_E(fmt, ...) printf("ERR: " fmt "\n", ## __VA_ARGS__)
#define DNX_CS_LOG_I(fmt, ...) printf("INF: " fmt "\n", ## __VA_ARGS__)
#define DNX_CS_LOG_X(fmt, ...) printf("INX: " fmt "\n", ## __VA_ARGS__)
#define __DNX_CS_ERROR_SIMULATION_ON

#include "dnx_csteps.h"

#ifndef DNX_UNREFERENCED_PARAM
#define DNX_UNREFERENCED_PARAM(x) (x) = (x)
#endif

#define STEP_TO_FAIL 2

DNX_CS_DECLARE_BUILD_STEP_COUNT;

#define TEST_ITER_STEPS
#define TEST_LOCAL_STEPS

DNX_CS_SET_BEGIN(module1_test1)
#ifdef TEST_ITER_STEPS
  DNX_CS_STEP(module1_test1, iter_step_first)
#endif
  DNX_CS_STEP(module1_test1, step_simple)
  DNX_CS_STEP(module1_test1, step_void)
  DNX_CS_STEP(module1_test1, step)
#ifdef TEST_ITER_STEPS
  DNX_CS_STEP(module1_test1, iter_step_mid)
#endif
  DNX_CS_STEP(module1_test1, step_simple_cond)
  DNX_CS_STEP(module1_test1, step_void_cond)
  DNX_CS_STEP(module1_test1, step_cond)
#ifdef TEST_ITER_STEPS
  DNX_CS_STEP(module1_test1, iter_step_last)
#endif
DNX_CS_SET_LOCAL(module1_test1)
#ifdef TEST_LOCAL_STEPS
  DNX_CS_STEP(module1_test1, local_step1)
  DNX_CS_STEP(module1_test1, local_step2)
#endif
DNX_CS_SET_END(module1_test1);

struct module1
{
  DNX_CS_DECLARE_SET(module1_test1);
#ifdef TEST_ITER_STEPS
  DNX_CS_DECLARE_LOOP_STEP(module1_test1, iter_step_first);
  DNX_CS_DECLARE_LOOP_STEP(module1_test1, iter_step_mid);
  DNX_CS_DECLARE_LOOP_STEP(module1_test1, iter_step_last);
#endif
  void *step_ptr;
  void *step_cond_ptr;
};


static int test1_simple_f(struct module1 *m, int p)
{
  DNX_UNREFERENCED_PARAM(m);
  DNX_UNREFERENCED_PARAM(p);
  return 0;
}

struct module1 module1_test_obj;

static void DoCStepsTestsCleanup(void)
{
  struct module1 *m = &module1_test_obj;

  DNX_CS_RUIN_BLOCK_BEGIN(m, module1_test1)
#ifdef TEST_ITER_STEPS
    while (DNX_CS_LOOP_STEP_CNT(m, module1_test1, iter_step_last))
    {
      DNX_CS_RUIN_STEP_ITER(m, module1_test1, iter_step_last,
                            dnx_cs_do_nothing())
    }
#endif
    DNX_CS_RUIN_STEP(m, module1_test1, step_cond,
                     free(m->step_cond_ptr))
    DNX_CS_RUIN_STEP(m, module1_test1, step_void_cond,
                     dnx_cs_do_nothing())
    DNX_CS_RUIN_STEP(m, module1_test1, step_simple_cond,
                      dnx_cs_do_nothing())
#ifdef TEST_ITER_STEPS
    while (DNX_CS_LOOP_STEP_CNT(m, module1_test1, iter_step_mid))
    {
      DNX_CS_RUIN_STEP_ITER(m, module1_test1, iter_step_mid,
                            dnx_cs_do_nothing())
    }
#endif
    DNX_CS_RUIN_STEP(m, module1_test1, step,
                     free(m->step_ptr))
    DNX_CS_RUIN_STEP(m, module1_test1, step_void,
                     dnx_cs_do_nothing())
    DNX_CS_RUIN_STEP(m, module1_test1, step_simple,
                     dnx_cs_do_nothing())
#ifdef TEST_ITER_STEPS
    while (DNX_CS_LOOP_STEP_CNT(m, module1_test1, iter_step_first))
    {
      DNX_CS_RUIN_STEP_ITER(m, module1_test1, iter_step_first,
                            dnx_cs_do_nothing())
    }
#endif

  DNX_CS_RUIN_BLOCK_END(m, module1_test1)
}

static int DoCStepsTestsInit(void)
{
  struct module1 *m = &module1_test_obj;

  int res_var = -1, i;

#ifdef STEP_TO_FAIL
  DNX_CS_SET_BUILD_STEP_TO_FAIL(STEP_TO_FAIL);
#endif

  DNX_CS_BUILD_BLOCK_BEGIN(m, module1_test1, int, 0, -1)

#ifdef TEST_ITER_STEPS
    for (i = 0; i < 10; i++)
    {
      DNX_CS_BUILD_STEP_ITER(TRUE,
                             m, module1_test1, iter_step_first,
                             res_var = test1_simple_f(m, i), res_var == 0, res_var)
    }
#endif

    DNX_CS_BUILD_STEP(TRUE,
                      m, module1_test1, step_simple,
                      res_var = test1_simple_f(m, 10), res_var == 0, res_var)
    DNX_CS_BUILD_STEP(TRUE,
                      m, module1_test1, step_void,
                      test1_simple_f(m, 11), TRUE, -1)

#ifdef TEST_LOCAL_STEPS
    DNX_CS_BUILD_STEP(TRUE,
                      m, module1_test1, local_step1,
                      test1_simple_f(m, 22), TRUE, -1);
#endif

    DNX_CS_BUILD_STEP(TRUE,
                      m, module1_test1, step,
                      m->step_ptr = malloc(10), m->step_ptr != NULL, -1)

#ifdef TEST_ITER_STEPS
    for (i = 0; i < 10; i++)
    {
      DNX_CS_BUILD_STEP_ITER(TRUE,
                             m, module1_test1, iter_step_mid,
                             res_var = test1_simple_f(m, i), res_var == 0, res_var)
    }
#endif

    DNX_CS_BUILD_STEP(m->step_cond_ptr != NULL,
                      m, module1_test1, step_simple_cond,
                      res_var = test1_simple_f(m, 10), res_var == 0, res_var)
    DNX_CS_BUILD_STEP(FALSE,
                      m, module1_test1, step_void_cond,
                      test1_simple_f(m, 11), TRUE, -1)
    DNX_CS_BUILD_STEP(m->step_ptr != NULL,
                      m, module1_test1, step_cond,
                      m->step_cond_ptr = malloc(10), m->step_cond_ptr != NULL, -1)

#ifdef TEST_LOCAL_STEPS
    DNX_CS_BUILD_STEP(TRUE,
                      m, module1_test1, local_step2,
                      test1_simple_f(m, 23), TRUE, -1);
#endif

#ifdef TEST_ITER_STEPS
    for (i = 0; i < 10; i++)
    {
      DNX_CS_BUILD_STEP_ITER(TRUE,
                             m, module1_test1, iter_step_last,
                             res_var = test1_simple_f(m, i), res_var == 0, res_var)
    }
#endif

  DNX_CS_BUILD_BLOCK_ALWAYS(m, module1_test1)
#ifdef TEST_LOCAL_STEPS
    DNX_CS_RUIN_STEP(m, module1_test1, local_step2,
                     dnx_cs_do_nothing());
    DNX_CS_RUIN_STEP(m, module1_test1, local_step1,
                     dnx_cs_do_nothing());
#endif
  DNX_CS_BUILD_BLOCK_END(m, module1_test1, DoCStepsTestsCleanup())
}

void main(void)
{
  if (DoCStepsTestsInit() == 0)
    DoCStepsTestsCleanup();
}
