/* 
* Copyright (c) 2012, Daynix Computing LTD (www.daynix.com)
* All rights reserved.
*
* Maintained by bricklets@daynix.com
*
* This file is a part of the Bricklets library, please see the wiki page
* on https://github.com/daynix/bricklets/wiki/Bricklets-library for more.
*
* This code is licensed under standard 3-clause BSD license.
* See file LICENSE supplied with this package for the full license text.
*/

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

MODULE_LICENSE("BSD");

void dnx_test_all(void);

static int __init dnx_kinit(void)
{
  printk("DUSIX Test Module Loaded\n");
  dnx_test_all();
  return 0;
}

static void __exit dnx_kuninit(void)
{
  printk("DUSIX Test Module Unloaded\n");
}

module_init(dnx_kinit);
module_exit(dnx_kuninit);

