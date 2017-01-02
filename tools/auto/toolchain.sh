#!/bin/sh

toolchain_arm64='https://releases.linaro.org/components/toolchain/binaries/5.3-2016.05/aarch64-linux-gnu/gcc-linaro-5.3.1-2016.05-x86_64_aarch64-linux-gnu.tar.xz'
toolchain_arm='https://releases.linaro.org/components/toolchain/binaries/5.3-2016.05/arm-linux-gnueabi/gcc-linaro-5.3.1-2016.05-x86_64_arm-linux-gnueabi.tar.xz'

if [ "$@" ]; then dir=$@; else dir=$(dirname $0); fi

case "$ARCH" in
arm64) 
 if ! [ -d $dir/$(basename $toolchain_arm64 .tar.xz) ]; then
	wget -qP $dir $toolchain_arm64 && tar -xf $dir/$(basename $toolchain_arm64) -C $dir && rm $dir/$(basename $toolchain_arm64) && echo "$dir/$(basename $toolchain_arm64 .tar.xz)/bin/aarch64-linux-gnu-"
 else 
	echo "$dir/$(basename $toolchain_arm64 .tar.xz)/bin/aarch64-linux-gnu-"
 fi
;;
arm)
 if ! [ -d $dir/$(basename $toolchain_arm .tar.xz) ]; then
	wget -qP $dir $toolchain_arm && tar -xf $dir/$(basename $toolchain_arm) -C $dir && rm $dir/$(basename $toolchain_arm) && echo "$dir/$(basename $toolchain_arm .tar.xz)/bin/arm-linux-gnueabi-"
 else 
	echo "$dir/$(basename $toolchain_arm .tar.xz)/bin/arm-linux-gnueabi-"
 fi
;;
esac
