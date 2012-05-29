# 
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
#

APP = leakmon
SOURCES = dnx_leakmon.c dnx_leakmon_aux.c test.c dnx_os_linux.c
CC = cc
LD = cc
CFLAGS = -c -Werror -Wall -DDNX_LM_MEM -DDNX_LM_ENABLE

OBJS = $(SOURCES:.c=.o)

all: $(APP)

-include $(SOURCES:.c=.d)

$(APP): $(OBJS)
	$(LD) $(OBJS) -o $(APP)

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@
	
clean:
	@rm -rf $(APP) *.o *.d*

%.d: %.c
	@set -e; rm -f $@; \
	$(CC) -M $(CFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

