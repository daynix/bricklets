# 
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
#

APP = dusix

SLIB = dusix.a

DUSIX_SOURCES = src/std/dnx_mem.c \
					src/std/dnx_io.c \
					src/linux/user/dnx_logger.c \
					src/linux/user/dnx_time.c \
					src/linux/user/dnx_mutex.c \
					src/linux/shared/dnx_err.c \
					src/linux/user/dnx_event.c \
					src/shared/dnx_rmlock.c \
					src/linux/user/dnx_thread.c

APP_SOURCES = src/test/dnx_test.c src/test/dnx_main.c

SOURCES = $(DUSIX_SOURCES) $(APP_SOURCES)

INCLUDES = -Iinclude

BUILD_DIRS=src/test \
					 src/std \
					 src/linux/user \
					 src/linux/shared \
					 src/shared

CC = cc
LD = cc
AR = ar
CFLAGS = -c -Werror -Wall
LFLAGS = -lrt -lpthread
ARFLAGS = rcs

DUSIX_OBJS = $(DUSIX_SOURCES:.c=.o)
APP_OBJS = $(APP_SOURCES:.c=.o)

all: $(APP)

slib: $(SLIB)

$(SLIB): $(DUSIX_OBJS)
	$(AR) $(ARFLAGS) $(SLIB) $(DUSIX_OBJS)

$(APP): $(SLIB) $(APP_OBJS)
	$(LD) $(APP_OBJS) $(SLIB) $(LFLAGS) -o $(APP)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) $< -o $@
	
%.d: %.c
	@set -e; rm -f $@; \
	$(CC) -M $(CFLAGS) $(INCLUDES) $< > $@.$$$$; \
	sed 's,$(patsubst %.c,%.o,$(<F)), $(dir $@)\$(patsubst %.c,%.o,$(<F)),' \
	< $@.$$$$ > $@; rm -f $@.$$$$

-include $(SOURCES:.c=.d)

clean:
	@rm -rf $(APP) $(SLIB) $(addsuffix /*.o, $(BUILD_DIRS)) \
		$(addsuffix /*.d*, $(BUILD_DIRS))


