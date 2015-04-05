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

CLIENT_NUM=$1

echo "Starting HCK client #${CLIENT_NUM}"

. `dirname $0`/hck_setup.cfg

client_ctrl_ifname()
{
  echo cc${CLIENT_NUM}_${UNIQUE_ID}
}

client_test_ifname()
{
  DEVICE_NUM=$1

  echo t${DEVICE_NUM}c${CLIENT_NUM}_${UNIQUE_ID}
}

client_ctrl_mac()
{
  echo 56:cc:cc:0${CLIENT_NUM}:cc:cc
}

client_transfer_mac()
{
  echo 56:aa:aa:0${CLIENT_NUM}:aa:aa
}

client_test_mac()
{
  DEVICE_NUM=$1

  echo 56:cc:cc:0${CLIENT_NUM}:0${DEVICE_NUM}:cc
}

client_cpus()
{
  VAR_NAME=CLIENT${CLIENT_NUM}_CPUS
  eval echo \$${VAR_NAME}
}

graphics_cmd()
{
  VAR_NAME=GRAPHICS_CLIENT${CLIENT_NUM}
  eval echo \$${VAR_NAME}
}

monitor_cmd()
{
  VAR_NAME=MONITOR_CLIENT${CLIENT_NUM}
  eval echo \$${VAR_NAME}
}

extra_cmd()
{
  VAR_NAME=CLIENT${CLIENT_NUM}_EXTRA
  eval echo \$${VAR_NAME}
}

client_mq_netdev_param()
{
  VAR_NAME=CLIENT${CLIENT_NUM}_MQ_NETDEV_PARAM
  eval echo \$${VAR_NAME}
}

client_mq_device_param()
{
  VAR_NAME=CLIENT${CLIENT_NUM}_MQ_DEVICE_PARAM
  eval echo \$${VAR_NAME}
}

image_name()
{
  VAR_NAME=CLIENT${CLIENT_NUM}_IMAGE
  eval echo \$${VAR_NAME}
}

TEST_IMAGE_NAME=$(dirname `image_name`)/client${CLIENT_NUM}_test_image.raw

prepare_test_image()
{
  test -f ${TEST_IMAGE_NAME} || \
  { echo Creating test image ${TEST_IMAGE_NAME}...; qemu-img create -f raw ${TEST_IMAGE_NAME} 20G; }
}

# This one is for clients activation only
# Commented out by default
#WORLD_NET_IFACE="-netdev tap,id=hostnet9,script=${HCK_ROOT}/hck_world_bridge_ifup.sh,downscript=no,ifname=tmp_${UNIQUE_ID}_${CLIENT_NUM}
#                  -device ${WORLD_NET_DEVICE},netdev=hostnet9,mac=22:11:11:11:0${CLIENT_NUM}:${UNIQUE_ID},bus=pci.0,id=tmp_${UNIQUE_ID}_${CLIENT_NUM}"

IDE_STORAGE_PAIR="-drive file=`image_name`,if=ide,serial=${CLIENT_NUM}110${UNIQUE_ID}${DRIVE_CACHE_OPTION}"

case $TEST_DEV_TYPE in
network)
   BOOT_STORAGE_PAIR="${IDE_STORAGE_PAIR}"
   TEST_NET_DEVICES="-netdev tap,id=hostnet2,vhost=${VHOST_STATE},script=${HCK_ROOT}/hck_test_bridge_ifup.sh,downscript=no,ifname=`client_test_ifname 1`$(client_mq_netdev_param)
                     -device virtio-net-pci,netdev=hostnet2,mac=`client_test_mac 1`,bus=pci.0,id=`client_test_ifname 1`$(client_mq_device_param)"
   ;;
bootstorage)
   BOOT_STORAGE_PAIR="-drive file=`image_name`,if=none,id=vio_block,serial=${CLIENT_NUM}110${UNIQUE_ID}${DRIVE_CACHE_OPTION}
                      -device virtio-blk-pci,bus=pci.0,addr=0x5,drive=vio_block"
   ;;
storage)
   BOOT_STORAGE_PAIR="${IDE_STORAGE_PAIR}"
   TEST_STORAGE_PAIR="-drive file=${TEST_IMAGE_NAME},if=none,id=virtio_blk,serial=${CLIENT_NUM}000${UNIQUE_ID}${DRIVE_CACHE_OPTION}
                      -device virtio-blk-pci,bus=pci.0,addr=0x5,drive=virtio_blk"
   prepare_test_image
   ;;
serial)
   BOOT_STORAGE_PAIR="${IDE_STORAGE_PAIR}"
   TEST_SERIAL_DEVICES="-device virtio-serial-pci,id=virtio_serial_pci0,addr=0x07"
   ;;
balloon)
   BOOT_STORAGE_PAIR="${IDE_STORAGE_PAIR}"
   TEST_BALLOON_DEVICE="-device virtio-balloon-pci,bus=pci.0,addr=0x8"
   ;;
usb)
   BOOT_STORAGE_PAIR="${IDE_STORAGE_PAIR}"
   TEST_STORAGE_PAIR="
    -device usb-ehci,id=vhck_ehci
    -drive if=none,id=usbdisk,serial=${CLIENT_NUM}000${UNIQUE_ID},file=${TEST_IMAGE_NAME}
    -device usb-storage,bus=vhck_ehci.0,drive=usbdisk,id=vhck_usbdisk "

    prepare_test_image
    ;;
video)
   BOOT_STORAGE_PAIR="${IDE_STORAGE_PAIR}"
    ;;

  * )
   echo "NOT IMPLEMENTED"
   exit 1
    ;;
esac

if [ ${SHARE_ON_HOST} != "false" ]; then
  FILE_TRANSFER_SETUP="-netdev user,id=filenet0,net=192.168.101.0/24,dhcpstart=192.168.101.${CLIENT_NUM}00,smb=${SHARE_ON_HOST},smbserver=192.168.101.1,restrict=on \
                       -device ${FILE_TRANSFER_DEVICE},netdev=filenet0,mac=`client_transfer_mac`"
fi

CTRL_NET_DEVICE="-netdev tap,id=hostnet0,script=${HCK_ROOT}/hck_ctrl_bridge_ifup.sh,downscript=no,ifname=`client_ctrl_ifname`
                 -device ${CTRL_NET_DEVICE},netdev=hostnet0,mac=`client_ctrl_mac`,bus=pci.0,id=`client_ctrl_ifname`"

${QEMU_BIN} \
        ${BOOT_STORAGE_PAIR} \
        ${TEST_STORAGE_PAIR} \
        ${CTRL_NET_DEVICE} \
        ${TEST_NET_DEVICES} \
        ${FILE_TRANSFER_SETUP} \
        ${TEST_SERIAL_DEVICES} \
        ${TEST_BALLOON_DEVICE} \
        ${WORLD_NET_IFACE} \
        -m ${CLIENT_MEMORY} -smp `client_cpus`,cores=`client_cpus` -enable-kvm -cpu qemu64,+x2apic,model=13${ENLIGHTENMENTS_OPTION} -usbdevice tablet -boot d \
	-M pc -rtc-td-hack -global kvm-pit.lost_tick_policy=discard -rtc base=localtime,clock=host,driftfix=slew \
        -global PIIX4_PM.disable_s3=${S3_DISABLE_OPTION} -global PIIX4_PM.disable_s4=${S4_DISABLE_OPTION} \
        -uuid CDEF127c-8795-4e67-95da-8dd0a889100${CLIENT_NUM} \
        -name HCK-Client${CLIENT_NUM}_${UNIQUE_ID}_`hostname`${_TITLE_POSTFIX} \
        `graphics_cmd` `monitor_cmd` ${SNAPSHOT_OPTION} `extra_cmd`
