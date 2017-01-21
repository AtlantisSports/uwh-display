#!/bin/sh

if [[ $EUID -ne 0 ]]; then
  echo "This script must be run as root" 2>&1
  exit 1
fi

this_script=`basename "$0"`

apt-get install git cmake vim swig i2c-tools libi2c-dev ntpdate minicom --assume-yes

# Initial setup of i2c for the Chronodot
cat << EOF >> /boot/config.txt
#######################################
# Automatically added by $this_script
dtparam=i2c_arm=on
dtoverlay=ds1307-rtc
# Force display resolution to that of the touchscreen:
hdmi_force_hotplug=1
hdmi_group=2
hdmi_mode=1
hdmi_mode=87
hdmi_cvt=800 480 60 6 0 0 0
max_usb_current=1
#######################################
EOF

# More i2c setup for the Chronodot
cat << EOF >> /etc/modules
#######################################
# Automatically added by $this_script
snd-bcm2835
8192cu
i2c-bcm2708
i2c-dev
rtc-ds1307
#######################################
EOF

# Default to the US keyboard layout
sed -i 's/XKBLAYOUT="gb"/XKBLAYOUT="us"/g' /etc/default/keyboard

reboot
