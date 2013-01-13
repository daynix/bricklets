# 
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
#
#!/bin/sh

VM_START_TIMEOUT=10

if [ "$1" = "-h" ] || [ "$1" = "--help" ] 
then
  echo This script starts VirtHCK Studio with two Clients
  echo if executed without parameters or you can  specify 
  echo VMs to run by following parameters:
  echo st - Start HCK Studio VM
  echo c1 - Start HCK Client 1 VM
  echo c2 - Start HCK Client 2 VM 
  echo $0" [st | c1 | c2] [ c1 | c2 ] [c2]"
  exit
fi

if test x`whoami` != xroot
then
  echo This script must be run as superuser
  exit 1
fi

SCRIPTS_DIR=`dirname $0`
. ${SCRIPTS_DIR}/hck_setup.cfg

remove_bridges() {
 ifconfig ${TEST_BR_NAME} down
 brctl delbr ${TEST_BR_NAME}
 ifconfig ${CTRL_BR_NAME} down
 brctl delbr ${CTRL_BR_NAME}
}

create_bridges() {
 brctl addbr ${TEST_BR_NAME} 2>&1 | grep -v "already exists"
 ifconfig ${TEST_BR_NAME} up
 brctl addbr ${CTRL_BR_NAME} 2>&1 | grep -v "already exists"
 ifconfig ${CTRL_BR_NAME} up
}

kill_jobs() {
  jobs -p > /tmp/.jobs_$$
  kill `cat /tmp/.jobs_$$ | tr '\n' ' '`
  rm -f /tmp/.jobs_$$
}

echo
dump_config
echo

trap "kill_jobs; loop_run_reset; remove_bridges; exit 0" INT

echo Creating bridges...
create_bridges

loop_run_reset
if [ "$#" -eq 0 ] || [ "$1" = "st" ]; then
  loop_run_vm ${SCRIPTS_DIR}/run_hck_studio.sh &
  sleep $VM_START_TIMEOUT
fi
if [ "$#" -eq 0 ] || [ "$1" = "c1" ] || [ "$2" = "c1" ]; then
  loop_run_vm ${SCRIPTS_DIR}/run_hck_client.sh 1 &
  sleep $VM_START_TIMEOUT
fi
if [ "$#" -eq 0 ] || [ "$1" = "c2" ] || [ "$2" = "c2" ] || [ "$3" = "c2" ]; then
  loop_run_vm ${SCRIPTS_DIR}/run_hck_client.sh 2 &
fi
sleep 2

read -p "Press any key to disable VMs respawn..." NOT_NEEDED_VAR
loop_run_stop
echo VMs won\'t respawn anymore.
wait

sleep 2
remove_bridges
loop_run_reset

