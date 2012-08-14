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

#
# NOTE: Source root directory passed via SRC_ROOT shell variable
#
SRC_ROOT_PARENT_DIR=$(lastword $(subst /, , $(SRC_ROOT)))
VCXPROJ_FNAME:=VS_[$(SRC_ROOT_PARENT_DIR)]_$(USER).vcxproj
VCXPROJ_FILTERS_FNAME:=$(VCXPROJ_FNAME).filters
SLN_FNAME:=VS_[$(SRC_ROOT_PARENT_DIR)]_$(USER).sln

vcxproj:
	@echo "Building VCXPROJ ($(VCXPROJ_FNAME))..."
	@perl dnx_list_files.pl $(SRC_ROOT) dnx_list_files.cfg dnx_list_files.buildcfg $(SRC_ROOT)/$(SLN_FNAME) VS.vcxproj.cfg.template VS.vcxproj.cfg_list.template > $(SRC_ROOT)/$(VCXPROJ_FNAME) < VS.vcxproj.template
	@perl dnx_list_files.pl $(SRC_ROOT) dnx_list_files.cfg dnx_list_files.buildcfg $(SRC_ROOT)/$(SLN_FNAME) VS.vcxproj.cfg.template VS.vcxproj.cfg_list.template > $(SRC_ROOT)/$(VCXPROJ_FILTERS_FNAME) < VS.vcxproj.filters.template
	@echo "Done ($(VCXPROJ_FNAME))!"
