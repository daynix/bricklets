#ifndef __DNX_CSTEPS_H__
#define __DNX_CSTEPS_H__

/******************************************************************************
 * External definitions - to be defined outside the module
 *****************************************************************************/
/* DNX_ASSERT   - [mandatory] Used by CSteps module to assert in case of an
*                 error detected
*  DNX_CS_LOG_E - [optional] Highly recommended. Used by CSteps module to
*                 printout additional info in case of an error detected
*  DNX_CS_LOG_I - [optional] Recommended while debugging build/ruin related
*                 issues. Used by CSteps module to printout verbose info
*                 while executing build and ruin steps.
*  DNX_CS_LOG_X - [optional] Recommended while debugging build/ruin related
*                 issues. Used by CSteps module to printout even more verbose
*                 info while executing build and ruin steps.
*/
#ifndef DNX_ASSERT
#error DNX_ASSERT has to be defined
#endif

#ifndef DNX_CS_LOG_E
#define DNX_CS_LOG_E
#endif

#ifndef DNX_CS_LOG_I
#define DNX_CS_LOG_I
#endif

#ifndef DNX_CS_LOG_X
#define DNX_CS_LOG_X
#endif

/******************************************************************************
 * Internal definitions - MUST NOT TO BE USED DIRECTLY
 *****************************************************************************/
typedef uint32_t dnx_cs_bfield_t;
typedef uint32_t dnx_cs_loop_cnt_t;

#define __DNX_CS_FULL_SET_NAME(set_name) \
  dnx_cs_##set_name##_e

#define __DNX_CS_FULL_STEP_NAME(set_name, step_name) \
  DNC_SC_STEP_##set_name##_##step_name

#define __DNX_CS_BFIELD_NAME(set_name, extra) \
  __dnx_cs_bfield_##set_name##_##extra

#define __DNX_CS_ALL_BFIELD_NAME(set_name) \
  __DNX_CS_BFIELD_NAME(set_name, __ALL)

#define __DNX_CS_SKP_BFIELD_NAME(set_name) \
  __DNX_CS_BFIELD_NAME(set_name, __SKIPPED)

#define __DNX_CS_LOOP_NAME(set_name, step_name) \
  __dnx_cs_loop_##set_name##_##step_name

#define __DNX_CS_BUILD_GUARD_NAME(set_name) \
  __dnx_cs_guard_build_##set_name

#define __DNX_CS_RUIN_GUARD_NAME(set_name) \
  __dnx_cs_guard_ruin_##set_name

#define __DNX_CS_CHECK_BUILD_GUARD(set_name) \
  __DNX_CS_BUILD_GUARD_NAME(set_name) = __DNX_CS_BUILD_GUARD_NAME(set_name)

#define __DNX_CS_CHECK_RUIN_GUARD(set_name) \
  __DNX_CS_RUIN_GUARD_NAME(set_name) = __DNX_CS_RUIN_GUARD_NAME(set_name)

#define __DNX_CS_LAST_GLOBAL_STEP_NAME(set_name) \
  __DNX_CS_FULL_STEP_NAME(set_name, __DNX_CS_LAST_GLOBAL)

#define __DNX_CS_LAST_LOCAL_STEP_NAME(set_name) \
  __DNX_CS_FULL_STEP_NAME(set_name, __DNX_CS_LAST_LOCAL)

#define __DNX_CS_BFIELD_BIT_VAL(bitno) \
  (((dnx_cs_bfield_t)1) << (bitno))

#define __DNX_CS_BFIELD_BIT_SET(bfield, bitno) \
  (bfield) |= __DNX_CS_BFIELD_BIT_VAL(bitno)

#define __DNX_CS_BFIELD_BIT_RESET(bfield, bitno) \
  (bfield) &= ~__DNX_CS_BFIELD_BIT_VAL(bitno)

#define __DNX_CS_BFIELD_BIT_GET(bfield, bitno) \
  ((bfield) & __DNX_CS_BFIELD_BIT_VAL(bitno))?1:0

#define __DNX_CS_BFIELD_BITS_ALL_VAL(nofbits) \
  (__DNX_CS_BFIELD_BIT_VAL(nofbits) - 1)

#define __DNX_CS_ALL_BFIELD(container, set_name) \
  (container)->__DNX_CS_ALL_BFIELD_NAME(set_name)

#define __DNX_CS_SKP_BFIELD(container, set_name) \
  (container)->__DNX_CS_SKP_BFIELD_NAME(set_name)

#define __DNX_CS_ALL_GLOBAL_BITS_VAL(set_name) \
  __DNX_CS_BFIELD_BITS_ALL_VAL(__DNX_CS_LAST_GLOBAL_STEP_NAME(set_name) + 1)

#define __DNX_CS_ASSERT(cond, desc) \
  { \
    if (!(cond)) \
    { \
      DNX_CS_LOG_E("DNX CS ASSERTION: %s", desc); \
      DNX_ASSERT(0); \
    } \
  }

#define __DNX_CS_ASSERT_PREVIOUS_STEPS_DONE(container, set_name, full_step_name, desc) \
  { \
    dnx_cs_bfield_t bits_to_check = __DNX_CS_ALL_BFIELD(container, set_name); \
    if (full_step_name <= __DNX_CS_LAST_GLOBAL_STEP_NAME(set_name)) \
    { \
        bits_to_check &= __DNX_CS_ALL_GLOBAL_BITS_VAL(set_name); \
    } \
    else \
    { \
        bits_to_check |= __DNX_CS_ALL_GLOBAL_BITS_VAL(set_name); \
    } \
    __DNX_CS_ASSERT(bits_to_check == __DNX_CS_BFIELD_BITS_ALL_VAL(full_step_name), \
                    desc); \
  }

#define __DNX_CS_LOOP_STEP_CNT(container, set_name, step_name) \
  (container)->__DNX_CS_LOOP_NAME(set_name, step_name)

#ifdef __DNX_CS_ERROR_SIMULATION_ON
#define __DNX_CS_HANDLE_BUILD_STEP_ERROR_DECLARE(ret_value_type, set_name, \
                                                 ret_fail_step_value) \
   ret_value_type __ds_cs_res_fail_##set_name = ret_fail_step_value

#define __DNX_CS_HANDLE_BUILD_STEP_ERROR_SIMULATION(set_name, step_name) \
  for (;;) \
  { \
    ++__dnx_cs_build_step_current; \
    if (__dnx_cs_build_step_current == __dnx_cs_build_step_to_fail) \
    { \
      __ds_cs_res_##set_name = __ds_cs_res_fail_##set_name; \
       DNX_CS_LOG_I("  Build Step " #step_name " of " #set_name " failure simulated! step#%u" , \
                    __dnx_cs_build_step_current); \
       goto _DNX_CS_ON_ERROR_##set_name; \
    } \
    break; \
  }
#else
#define __DNX_CS_HANDLE_BUILD_STEP_ERROR_DECLARE(ret_value_type, set_name, ret_fail_step_value)
#define __DNX_CS_HANDLE_BUILD_STEP_ERROR_SIMULATION(set_name, step_name)
#endif

#define __DNX_CS_BUILD_STEP(exec_condition, container, set_name, step_name, \
                            build_call, success_condition, error_code, iter_ctr_ptr) \
  for (;;) \
  { \
    dnx_cs_loop_cnt_t *iter_ctr = (dnx_cs_loop_cnt_t *)iter_ctr_ptr; \
    __DNX_CS_CHECK_BUILD_GUARD(set_name); \
    DNX_CS_LOG_X("  Entering Build Step " #step_name " of " #set_name); \
    __DNX_CS_HANDLE_BUILD_STEP_ERROR_SIMULATION(set_name, step_name); \
    __ds_cs_res_##set_name = __ds_cs_ref_##set_name; \
    if (exec_condition) \
    { \
      (build_call); \
      if (success_condition) \
      { \
        __ds_cs_res_##set_name = __ds_cs_ref_##set_name; \
        DNX_CS_LOG_I("  Build Step " #step_name " of " #set_name " SUCCEEDED! step#%u" , __dnx_cs_build_step_current); \
      } \
      else \
      { \
        __ds_cs_res_##set_name = error_code; \
        DNX_CS_LOG_E("  Build Step " #step_name " of " #set_name "FAILED! step#%u" , __dnx_cs_build_step_current); \
        goto _DNX_CS_ON_ERROR_##set_name; \
      } \
    } \
    else \
    { \
      DNX_CS_LOG_I("  Build Step " #step_name " of " #set_name " SKIPPED because its precondition is false"); \
      __DNX_CS_BFIELD_BIT_SET(__DNX_CS_SKP_BFIELD(container, set_name), __DNX_CS_FULL_STEP_NAME(set_name, step_name)); \
    } \
    if (!iter_ctr || *iter_ctr == 0) \
    { \
      __DNX_CS_ASSERT_PREVIOUS_STEPS_DONE(container, set_name, __DNX_CS_FULL_STEP_NAME(set_name, step_name), \
                                     "Build Step " #step_name " of " #set_name ": not all the previous Build steps have been done"); \
      __DNX_CS_BFIELD_BIT_SET(__DNX_CS_ALL_BFIELD(container, set_name), __DNX_CS_FULL_STEP_NAME(set_name, step_name)); \
    } \
    if (iter_ctr) \
    { \
      ++(*iter_ctr); \
    } \
    break; \
  }

#define __DNX_CS_RUIN_STEP(container, set_name, step_name, ruin_call, iter_ctr_ptr) \
  for (;;) \
  { \
    __DNX_CS_CHECK_RUIN_GUARD(set_name); \
    DNX_CS_LOG_X("  Entering Ruin Step " #step_name " of " #set_name); \
    if (__DNX_CS_BFIELD_BIT_GET(__DNX_CS_SKP_BFIELD(container, set_name), \
                                __DNX_CS_FULL_STEP_NAME(set_name, step_name))) \
    { \
      DNX_CS_LOG_I("  Ruin Step " #step_name " of " #set_name " SKIPPED as it was skipped by build"); \
    } \
    else if (__DNX_CS_BFIELD_BIT_GET(__DNX_CS_ALL_BFIELD(container, set_name), \
                                     __DNX_CS_FULL_STEP_NAME(set_name, step_name))) \
    { \
      ruin_call; \
      DNX_CS_LOG_I("  Ruin Step " #step_name " of " #set_name " DONE"); \
    } \
    else \
    { \
      DNX_CS_LOG_I("  Ruin Step " #step_name " of " #set_name " SKIPPED as it was not built"); \
    } \
    if (__DNX_CS_BFIELD_BIT_GET(__DNX_CS_ALL_BFIELD(container, set_name), \
                                __DNX_CS_FULL_STEP_NAME(set_name, step_name))) \
    { \
      dnx_cs_loop_cnt_t *iter_ctr = (dnx_cs_loop_cnt_t *)iter_ctr_ptr; \
      if (iter_ctr && *iter_ctr) \
      { \
        --(*iter_ctr); \
      } \
      if (!iter_ctr || *iter_ctr == 0) \
      { \
        __DNX_CS_BFIELD_BIT_RESET(__DNX_CS_ALL_BFIELD(container, set_name), \
                                  __DNX_CS_FULL_STEP_NAME(set_name, step_name)); \
        __DNX_CS_BFIELD_BIT_RESET(__DNX_CS_SKP_BFIELD(container, set_name), \
                                  __DNX_CS_FULL_STEP_NAME(set_name, step_name)); \
        __DNX_CS_ASSERT_PREVIOUS_STEPS_DONE(container, set_name, __DNX_CS_FULL_STEP_NAME(set_name, step_name), \
                                       "Ruin Step " #step_name " of " #set_name ": not all the previous Ruin steps have been done"); \
      } \
    } \
    break; \
  }

/******************************************************************************
 * Public API
 *****************************************************************************/
#define DNX_CS_STEP(set_name, step_name) \
  __DNX_CS_FULL_STEP_NAME(set_name, step_name),

#define DNX_CS_SET_BEGIN(set_name) \
  enum __DNX_CS_FULL_SET_NAME(set_name) {

#define DNX_CS_SET_LOCAL(set_name) \
    __DNX_CS_LAST_GLOBAL_STEP_NAME(set_name),

#define DNX_CS_SET_END(set_name) \
    __DNX_CS_LAST_LOCAL_STEP_NAME(set_name) \
  }

#define DNX_CS_DECLARE_SET(set_name) \
  dnx_cs_bfield_t __DNX_CS_ALL_BFIELD_NAME(set_name); \
  dnx_cs_bfield_t __DNX_CS_SKP_BFIELD_NAME(set_name)


#define DNX_CS_DECLARE_LOOP_STEP(set_name, step_name) \
  dnx_cs_loop_cnt_t __DNX_CS_LOOP_NAME(set_name, step_name)

#define DNX_CS_BUILD_BLOCK_BEGIN(container, set_name, \
                                 ret_value_type, ret_success_value, \
                                 ret_fail_step_value) \
  __DNX_CS_ASSERT(__DNX_CS_LAST_LOCAL_STEP_NAME(set_name) < \
                  (sizeof(dnx_cs_bfield_t) * 8), \
                  "Too many steps defined for dnx_cs_bfield_t"); \
  __DNX_CS_ASSERT(ret_success_value != ret_fail_step_value, \
                  "Success value must not be same as Fail step value"); \
  for(;;) { \
    ret_value_type __ds_cs_ref_##set_name = ret_success_value; \
    ret_value_type __ds_cs_res_##set_name = ret_success_value; \
    __DNX_CS_HANDLE_BUILD_STEP_ERROR_DECLARE(ret_value_type, set_name, ret_fail_step_value); \
    int __DNX_CS_BUILD_GUARD_NAME(set_name) = 0; \
    DNX_CS_LOG_I("Entering Build block begin of " #set_name); \
    __DNX_CS_ALL_BFIELD(container, set_name) = 0; \
    __DNX_CS_SKP_BFIELD(container, set_name) = 0;

#define DNX_CS_BUILD_BLOCK_ALWAYS(container, set_name) \
    __DNX_CS_CHECK_BUILD_GUARD(set_name); \
    __DNX_CS_ASSERT_PREVIOUS_STEPS_DONE(container, set_name, __DNX_CS_LAST_GLOBAL_STEP_NAME(set_name), \
                                   "Set " #set_name ": not all the global Build steps have been done"); \
    __DNX_CS_BFIELD_BIT_SET(__DNX_CS_ALL_BFIELD(container, set_name), __DNX_CS_LAST_GLOBAL_STEP_NAME(set_name)); \
    __DNX_CS_ASSERT_PREVIOUS_STEPS_DONE(container, set_name, __DNX_CS_LAST_LOCAL_STEP_NAME(set_name), \
                                   "Set " #set_name ": not all the local Build steps have been done"); \
_DNX_CS_ON_ERROR_##set_name: \
    DNX_CS_LOG_I("Entering Build block always of " #set_name); \
    { \
      int __DNX_CS_RUIN_GUARD_NAME(set_name) = 0;

#define DNX_CS_BUILD_BLOCK_END(container, set_name, ruin_call) \
      __DNX_CS_CHECK_RUIN_GUARD(set_name); \
    } \
    DNX_CS_LOG_I("Entering Build block end of " #set_name); \
    __DNX_CS_CHECK_BUILD_GUARD(set_name); \
    __DNX_CS_ASSERT((__DNX_CS_ALL_BFIELD(container, set_name) & ~__DNX_CS_ALL_GLOBAL_BITS_VAL(set_name)) == 0, \
                   "A local ruin step missed"); \
    if(__ds_cs_ref_##set_name == __ds_cs_res_##set_name) { \
      DNX_CS_LOG_I("Build block of " #set_name " succeeded"); \
       __DNX_CS_BFIELD_BIT_RESET(__DNX_CS_ALL_BFIELD(container, set_name), __DNX_CS_LAST_GLOBAL_STEP_NAME(set_name)); \
      __DNX_CS_ASSERT_PREVIOUS_STEPS_DONE(container, set_name, __DNX_CS_LAST_GLOBAL_STEP_NAME(set_name), \
                                          "Set " #set_name ": not all the Build steps have been done"); \
      return __ds_cs_res_##set_name; \
      /* NOTE: goto to avoid "defined but not used" warning when \
               there are no steps defined */ \
      goto _DNX_CS_ON_ERROR_##set_name; \
    } \
    DNX_CS_LOG_E("Build block of " #set_name " FAILED! Ruining..."); \
    ruin_call; \
    return __ds_cs_res_##set_name; \
  }

#define DNX_CS_RUIN_BLOCK_BEGIN(container, set_name) \
  { \
    int __DNX_CS_RUIN_GUARD_NAME(set_name) = 0; \
    DNX_CS_LOG_I("Entering Ruin block begin of " #set_name " set"); \
    __DNX_CS_BFIELD_BIT_RESET(__DNX_CS_ALL_BFIELD(container, set_name), \
                              __DNX_CS_LAST_GLOBAL_STEP_NAME(set_name));

#define DNX_CS_RUIN_BLOCK_END(container, set_name) \
    __DNX_CS_CHECK_RUIN_GUARD(set_name); \
    __DNX_CS_ASSERT(__DNX_CS_ALL_BFIELD(container, set_name) == 0, \
                    "A ruin step missed"); \
    __DNX_CS_ASSERT(__DNX_CS_SKP_BFIELD(container, set_name) == 0, \
                    "A ruin skp step missed"); \
      DNX_CS_LOG_I("Ruin block of " #set_name " done"); \
   }

#define DNX_CS_LOOP_STEP_CNT(container, set_name, step_name) \
  __DNX_CS_LOOP_STEP_CNT(container, set_name, step_name)

#define DNX_CS_BUILD_STEP(exec_condition, container, set_name, step_name, \
                          build_step_call, success_condition, error_code) \
  __DNX_CS_BUILD_STEP(exec_condition, container, set_name, step_name, \
                      build_step_call, success_condition, error_code, \
                      NULL)

#define DNX_CS_BUILD_STEP_ITER(exec_condition, container, set_name, step_name, \
                               build_step_call, success_condition, error_code) \
  __DNX_CS_BUILD_STEP(exec_condition, container, set_name, step_name, \
                      build_step_call, success_condition, error_code, \
                      &__DNX_CS_LOOP_STEP_CNT(container, set_name, step_name))


#define DNX_CS_RUIN_STEP(container, set_name, step_name, ruin_call) \
  __DNX_CS_RUIN_STEP(container, set_name, step_name, ruin_call, NULL)

#define DNX_CS_RUIN_STEP_ITER(container, set_name, step_name, ruin_call) \
  __DNX_CS_RUIN_STEP(container, set_name, step_name, ruin_call, \
                     &__DNX_CS_LOOP_STEP_CNT(container, set_name, step_name))

extern dnx_cs_loop_cnt_t __dnx_cs_build_step_current;
extern dnx_cs_loop_cnt_t __dnx_cs_build_step_to_fail;

#define DNX_CS_DECLARE_BUILD_STEP_COUNT \
  dnx_cs_loop_cnt_t __dnx_cs_build_step_current = 0; \
  dnx_cs_loop_cnt_t __dnx_cs_build_step_to_fail = 0; /* disable */

#define DNX_CS_SET_BUILD_STEP_TO_FAIL(n) \
  __dnx_cs_build_step_to_fail = (dnx_cs_loop_cnt_t)(n)

#define DNX_CS_GET_BUILD_STEP_TO_FAIL(n, type) \
  (type)__dnx_cs_build_step_to_fail

static __inline void
dnx_cs_do_nothing(void)
{

}

#endif /* __DNX_CSTEPS_H__ */

