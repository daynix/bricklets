#!/bin/sh
 
# Copyright (c) 2013, Daynix Computing LTD (www.daynix.com)
# All rights reserved.
#
# Maintained by bricklets@daynix.com
#
# This file is a part of Bricklets library, see "Bricklets" page
# on www.daynix.com for more.
#
# This code is licensed under standard 3-clause BSD license.
# See file LICENSE supplied with this package for the full license text.

echo "Starting HCK studio..."

. `dirname $0`/hck_setup.cfg

STUDIO_CONTROL_IFNAME=cs_${UNIQUE_ID}
STUDIO_CONTROL_MAC=56:cc:cc:ff:cc:cc
STUDIO_WORLD_IFNAME=ws_${UNIQUE_ID}
STUDIO_WORLD_MAC=56:${UNIQUE_ID}:${UNIQUE_ID}:${UNIQUE_ID}:${UNIQUE_ID}:${UNIQUE_ID}

WORLD_NET_DEVICE="-netdev tap,id=hostnet0,script=${HCK_ROOT}/hck_world_bridge_ifup.sh,downscript=no,ifname=${STUDIO_WORLD_IFNAME}
                 -device ${WORLD_NET_DEVICE},netdev=hostnet0,mac=${STUDIO_WORLD_MAC},bus=pci.0,id=${STUDIO_WORLD_IFNAME}"

CTRL_NET_DEVICE="-netdev tap,id=hostnet1,script=${HCK_ROOT}/hck_ctrl_bridge_ifup.sh,downscript=no,ifname=${STUDIO_CONTROL_IFNAME}
                -device ${CTRL_NET_DEVICE},netdev=hostnet1,mac=${STUDIO_CONTROL_MAC},bus=pci.0,id=${STUDIO_CONTROL_IFNAME}"

${QEMU_BIN} \
    -drive file=${STUDIO_IMAGE},if=ide${DRIVE_CACHE_OPTION} \
    ${WORLD_NET_DEVICE} \
    ${CTRL_NET_DEVICE} \
    -m 512M -smp 1 -enable-kvm -cpu qemu64,+x2apic -usbdevice tablet \
    -uuid 9999127c-8795-4e67-95da-8dd0a8891cd1 \
    -name HCK-Studio_${UNIQUE_ID}_`hostname`${_TITLE_POSTFIX} \
    ${GRAPHICS_STUDIO} ${SNAPSHOT_OPTION} ${STUDIO_EXTRA} $1

