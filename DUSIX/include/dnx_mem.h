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

#ifndef __DNX_MEM_H__
#define __DNX_MEM_H__

#include "dnx_types.h"

void *dnx_malloc(size_t size);
void dnx_free(void *ptr);


#endif

