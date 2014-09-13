#!/bin/bash

#
# Setup evironement to compile for OpenWrt
# You have probably to change the settings for STAGING_DIR and INCLUDE_DIR
#

export STAGING_DIR=`readlink -m ../openwrt/staging_dir/toolchain-mips_r2_gcc-4.6-linaro_uClibc-0.9.33.2`
export INCLUDE_DIR=`readlink -m ../openwrt/staging_dir/target-mips_r2_uClibc-0.9.33.2/usr/include`
export LIB_DIR=`readlink -m ../openwrt/staging_dir/target-mips_r2_uClibc-0.9.33.2/usr/lib`

export PATH=$PATH:$STAGING_DIR/bin
export CC=mips-openwrt-linux-uclibc-gcc
export LD=mips-openwrt-linux-uclibc-ld
export AR=mips-openwrt-linux-uclibc-ar


#bash --rcfile <(cat ~/.bashrc; echo 'PS1="OpenWrtEnv:\w\$"')
echo 'export PS1="OpenWrtEnv:\w\$ "' > /tmp/brc
bash --rcfile /tmp/brc
