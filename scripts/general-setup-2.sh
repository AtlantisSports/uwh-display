#!/bin/bash -ex

if [[ $EUID -ne 0 ]]; then
  echo "This script must be run as root" 2>&1
  exit 1
fi

this_script=`basename "$0"`

echo "Set up the Chronodot"
modprobe rtc-ds1307
echo ds3231 0x68 > /sys/class/i2c-adapter/i2c-1/new_device
hwclock -s
update-rc.d -f fake-hwclock remove
update-rc.d -f ntp remove
hwclock -r
rm -f /etc/localtime
ln -sf /usr/share/zoneinfo/America/Denver /etc/localtime
ntpdate 0.pool.ntp.org
hwclock -w

echo "Update the system clock on boot"
sed -i 's/^exit 0//g' /etc/rc.local
cat << EOF >> /etc/rc.local
#######################################
# Automatically added by $this_script
#
# Update the system clock on boot:
echo ds3231 0x68 > /sys/class/i2c-adapter/i2c-1/new_device
(sudo ntpdate -s 0.north-america.pool.ntp.org &&
 sudo hwclock -w &&
 echo "Time set via ntp") ||
(sudo hwclock -s &&
 echo "Time set via Chronodot") ||
(echo "Time setup failed")
#######################################

exit 0
EOF

echo "Blacklist the sound drivers"
cat << EOF >> /etc/modprobe.d/raspi-blacklist.conf
#######################################
# Automatically added by $this_script
#
# Cripple the sound drivers so they don't mess with the hardware PWMs:
blacklist soundcore
blacklist snd
blacklist snd_pcm
blacklist snd_pcsp
blacklist snd_bcm2835
blacklist snd_timer
blacklist pcspkr
#######################################
EOF
update-initramfs -u

echo "Reboot in 3s"
sleep 3
reboot
