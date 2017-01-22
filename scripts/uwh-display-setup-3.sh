#!/bin/bash -ex

if [[ $EUID -eq 0 ]]; then
  echo "This script must NOT be run as root" 2>&1
  exit 1
fi

this_script=`basename "$0"`

mkdir -p ~/.ssh/

cat << EOF > /home/pi/.ssh/uwhd_github_deploy
-----BEGIN RSA PRIVATE KEY-----
MIIEpAIBAAKCAQEArDxTUThfr1Xb7KuFpZSp9fRka7XGD2IkvE0A1VP+d3uE/B+4
+lVU3e3HnlLy4eFjBJSE0FBiVvHkZESY0nm7AiHu9jZ9GywwEhNNXJGNOquWMGMh
UQH+7bHU4gg86Rp+Lq3Jai6MRVIIxmJPPe5gwwFYIiiXvUP02IkzUS1Le29llPdU
WRXI1LfX8OosK8wEwarkocz58vSpEez5qd008ulK2ZxK7EOKhSC2ThWCiifCa9Rq
KVLdI1E3BP/LQl6Asv3JE+Oof8zVCX7s9iOQQJg9tuqBvGzFWqH1Mqja3BYx+TkZ
8sQcLZkA2jOu5RLrOVWc7yGKlBcxDallsZv7IwIDAQABAoIBAHGpSc5ypmp9/alc
s9DGn7bJNjACgBcMzHNbWa5uRKKy59q+UIMbJutiDHbGBLTd2Utfe5pBQfRgozks
L7HOVYQdwqXG5Z619W8oP8hT4WCTdYSvFoBwEbzouzUC/LdUHWtOknm5CCaC8XmB
zQtwsXhe/btlEGX9X6o6OJxciNCGaNaHcj8wOand6MFLalTxLMwZ+jZRUcH0267P
t3zv0k8S3AnDMzRkT+UZj9sOR1rkR1IS3VzCEfmP+/RqDw6MLXb/7XKi8FMN4kyj
AE9xcntoJMIYmTfIUO0Zs8vWAfJLMM9UZ5IOtn4rbTTKQfarMM/LuiTKPAyIUdvt
cU0hTjkCgYEA1MioOy/ABIvUBh+yh7jgWlKRnjOmbo2iHimSrZcsy6RnfkEes0Y3
2coAVYc232QfyqvfEU4EDlX+yPfdYJxrcUvvaCmdkOj64NHpxjuM7va7N2pLXW07
tr0KqOeHTXx1wd3aN25Fd9INelioREWoFav+PgiSjGT2MIw2M282C5UCgYEAzzdu
5cpw+/FJwa+qj2dsJlPOe+iapLv7YmR/+iKpyUsRJOOM+4ahHP2saLqFes1dDiDX
y7olVuMLtcKcifsgIohLz7mxSg/zHH6I0OLE+lgMS2GJvHFTFp8ve0htWVqNmjUK
fsaugGMiSRfKCYxPJzIlj5bIea8VFA+v1lku/dcCgYEAzszNo+g+iKyVPfZkfr/R
XDdPSyALqvqACyyAX7HDWY/I/c2wVq/x6/sE5Brr26TluQXdYGYgTwaI/yzWvOSJ
D62rWeLIPmQQH5wjxgNub3jgb3UhFgXpuSp1R77towNJN0e0o5bKcS925edwchn4
yEdGa2fNdiFRm9FJhCebsRkCgYEArKkz7qVQn53eJ1WIeRVkOEt8siKPONSqEsJL
INHhR6kVD/fLcYoslUuKbXl932VfNHaJWWgx1MQ67ajLZu2pSRzojp3b7Cp+GDAX
MHbESxUyqV0Mt8h1w1PA7Kjc9KdPdj7cD+bdYH06BE2eO3Z+Pdq7EXpCcaJBngW0
LpcBYOECgYBaTe8ChBM8Nd8XcwdIRS3QqaQeuZR0impvXAHA4TBsoQ10TxZFTE3/
6QS273eIoRbg2FdNciO7of1fmnZo7MonLONxUbGY9f3Y21UecmwJRVNsp9Q4FeIg
Y73SbGfjtbpdOkGqIfLshdRkMSp7th/abQAXC5yY1Kre4hd6+8u6Og==
-----END RSA PRIVATE KEY-----
EOF
chmod 0600 /home/pi/.ssh/uwhd_github_deploy

cat << EOF > /home/pi/.ssh/uwhd_github_deploy.pub
ssh-rsa AAAAB3NzaC1yc2EAAAADAQABAAABAQCsPFNROF+vVdvsq4WllKn19GRrtcYPYiS8TQDVU/53e4T8H7j6VVTd7ceeUvLh4WMElITQUGJW8eRkRJjSebsCIe72Nn0bLDASE01ckY06q5YwYyFRAf7tsdTiCDzpGn4urclqLoxFUgjGYk897mDDAVgiKJe9Q/TYiTNRLUt7b2WU91RZFcjUt9fw6iwrzATBquShzPny9KkR7Pmp3TTy6UrZnErsQ4qFILZOFYKKJ8Jr1GopUt0jUTcE/8tCXoCy/ckT46h/zNUJfuz2I5BAmD226oG8bMVaofUyqNrcFjH5ORnyxBwtmQDaM67lEus5VZzvIYqUFzENqWWxm/sj pi@uwh-display-01.local
EOF
chmod 0600 /home/pi/.ssh/uwhd_github_deploy.pub

cat << EOF >> ~/.ssh/config
#######################################
# Automatically added by $this_script
#
Host github.com
    IdentityFile ~/.ssh/uwhd_github_deploy
    IdentitiesOnly yes
#######################################
EOF
chmod 0600 ~/.ssh/config

mkdir -p ~/workdir
cd ~/workdir

git clone https://github.com/attie/libxbee3.git
cd libxbee3 && make configure && make all && sudo make install && cd -

git clone --recursive git@github.com:jroelofs/uwh-display.git
cd uwh-display
mkdir build
cd build
cmake ../ && make -j
