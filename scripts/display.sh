#!/bin/bash -ex

if [[ $EUID -ne 0 ]]; then
  echo "This script must be run as root" 2>&1
  exit 1
fi

this_script=`basename "$0"`

sed -i 's/^exit 0//g' /etc/rc.local
cat << EOF >> /etc/rc.local
#######################################
# Automatically added by $this_script
#
# Start the uwhdd daemon:
/home/pi/workdir/uwhd-display/build/bin/uwhdd --daemon
#######################################

exit 0
EOF

