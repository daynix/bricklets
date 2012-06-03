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

#include "dusix.h"

#define DNX_TEMP_BUF_SIZE 256

void dnx_test_io(void)
{
  char buff[DNX_TEMP_BUF_SIZE] = {0};
  int ret;
  ret = dnx_printf("DNX_OS_IO_TEST: dnx_printf %s is working. \n", "function");
  dnx_printf("DNX_OS_IO_TEST: Previous line has %d characters.\n", ret);

  ret = dnx_snprintf(buff, DNX_TEMP_BUF_SIZE,
    "DNX_OS_IO_TEST: dnx_snprintf %s is working.\n", "function");
  dnx_snprintf(buff + ret, DNX_TEMP_BUF_SIZE - ret, "DNX_OS_IO_TEST: Previous "
    "line has %d characters.\n", ret);

  dnx_printf("%s", buff);

  dnx_printf("DNX_OS_IO_TEST: Finished Successfully.\n");
}

dnx_status_t dnx_test_mem(void)
{
  char *buff = NULL;
  dnx_status_t rc = DNX_ERR_OK;

  buff = (char *)dnx_malloc(DNX_TEMP_BUF_SIZE);
  if (NULL == buff)
  {
    dnx_printf("DNX_OS_MEM_TEST: Failed to allocate temp buffer.\n");
    rc = DNX_ERR_NO_MEM;
    goto Exit;
  }

  dnx_snprintf(buff, DNX_TEMP_BUF_SIZE, "DNX_OS_MEM_TEST: This is a string "
    "inside the allocated buffer.\n");

  dnx_printf("%s", buff);

  dnx_free(buff);

  dnx_printf("DNX_OS_MEM_TEST: Finished Successfully.\n");
Exit:
  return rc;
}

void dnx_test_logger(void)
{
	const char *str = "This is an info line. Should be printed";
	dnx_logger_init("DNX_OS_LOGGER_TEST", DNX_LOG_FLAG_TO_STDERR | 
    DNX_LOG_FLAG_TIMESTAMP,	DNX_LOG_LEVEL_INFO);

	dnx_log_e("This is an error line. Should be printed\n");

	dnx_log_i("%s\n", str);

	dnx_log_x("This is an extra info line. Should not be printed\n");

	dnx_logger_uninit();

  dnx_printf("DNX_OS_LOGGER_TEST: Finished Successfully.\n");
}

dnx_status_t dnx_test_time(void)
{
  uint32_t timestamp1, timestamp2, counter = 10000;
  dnx_status_t rc = DNX_ERR_OK;

  timestamp1 = dnx_timestamp();
  if (0 == timestamp1)
  {
    dnx_printf("DNX_OS_TIME_TEST: Failed to get first timestamp.\n");
    rc = DNX_ERR_UNKNOWN;
    goto Exit;
  }

  while (counter--);

  timestamp2 = dnx_timestamp();
  if (0 == timestamp2)
  {
    dnx_printf("DNX_OS_TIME_TEST: Failed to get second timestamp.\n");
    rc = DNX_ERR_UNKNOWN;
    goto Exit;
  }

  if (timestamp1 == timestamp2)
  {
    dnx_printf("DNX_OS_TIME_TEST: Both timestamps have the same value: %u.\n",
      timestamp1);
    rc = DNX_ERR_UNKNOWN;
    goto Exit;
  }

  dnx_printf("DNX_OS_TIME_TEST: First timestamp: %u, second timestamp %u.\n",
    timestamp1, timestamp2);


  dnx_printf("DNX_OS_TIME_TEST: Finished Successfully.\n");

Exit:
  return rc;
}

void dnx_test_thread(void *ctx)
{
  dnx_rmlock_t *lock = (dnx_rmlock_t *)ctx;

  dnx_printf("DNX_OS_SYNC_TEST: Test thread is runnung.\n");

  dnx_rmlock_rem(lock);
}

dnx_status_t dnx_test_sync()
{
  dnx_rmlock_t *lock = NULL;
  dnx_thread_t *thread = NULL;
  dnx_status_t rc;

  rc = dnx_rmlock_init(&lock);
  if (DNX_ERR_OK != rc)
  {
    dnx_printf("DNX_OS_SYNC_TEST: Failed to init rmlock.\n");
    goto Exit;
  }

  dnx_rmlock_add(lock);

  rc = dnx_thread_create(&thread, dnx_test_thread, lock);
  if (DNX_ERR_OK != rc)
  {
    dnx_printf("DNX_OS_SYNC_TEST: Failed to create test thread.\n");
    goto Exit;
  }

  rc = dnx_rmlock_wait(lock);
  if (DNX_ERR_OK != rc)
  {
    dnx_printf("DNX_OS_SYNC_TEST: rmlock wait returned with error = %d.\n",
      rc);
    goto Exit;
  }

  dnx_printf("DNX_OS_SYNC_TEST: Finished Successfully.\n");

Exit:
  if (NULL != lock)
    dnx_rmlock_uninit(lock);

  if (NULL != thread)
    dnx_thread_exit(thread);

  return rc;
}

/* add test function for every implemented module. */
int main(int argc, char **argv)
{
  dnx_status_t rc;

  /* does not return error code, as this usually prints to console/syslog/whatever 
   * we just have to make sure we see the prints. */
  dnx_test_io();

  rc = dnx_test_mem();
  if (DNX_ERR_OK != rc)
  {
    dnx_printf("ERROR in mem tests!\n");
    goto Exit;
  }

  /* does not return error code.  */
  dnx_test_logger();

  rc = dnx_test_time();
  if (DNX_ERR_OK != rc)
  {
    dnx_printf("ERROR in time tests!\n");
    goto Exit;
  }

  /* test mutex, event, thread, and rmlock modules. */
  rc = dnx_test_sync();
  if (DNX_ERR_OK != rc)
  {
    dnx_printf("ERROR in sync tests\n");
    goto Exit;
  }

  dnx_printf("DNX OS Abstraction Layer tests finished successfully.\n");

Exit:
  if (DNX_ERR_OK != rc)
    dnx_printf("DNX OS Abstraction Layer tests failed.\n");

  return 0;
}

