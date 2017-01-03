#!/bin/bash

e="\x1b[";c=$e"39;49;00m";y=$e"93;01m";cy=$e"96;01m";r=$e"1;91m";g=$e"92;01m";

export KBUILD_BUILD_USER=rezvorck
export KBUILD_BUILD_HOST=debian
export CONFIG_DEBUG_SECTION_MISMATCH=y
export GCC_VERSION="gcc version 5.3.1 20160412 (linaro) (GCC)"

run=$(date +%s)

if [ -f arch/arm64/boot/Image.gz-dtb ]
then
    echo -e "$y >> Remove kernel... $c"
    rm arch/arm64/boot/Image*
fi

echo -e "$y >> Export toolchain... $c"
export ARCH=arm64 && export CROSS_COMPILE=$(tools/auto/toolchain.sh ..)

echo -e "$y >> Make defconfig... $c"
make s450m_4g_64_defconfig >/dev/null

echo -e "$y >> Start build... $c"
make -j4 >/dev/null 2>errors_64.log

if [ ! -f arch/arm64/boot/Image.gz-dtb ]
then
    echo -e "$r Build error! $c"
    echo "$(cat errors_64.log | grep error 2>/dev/null)"
else
    echo -e "$y >> Creating update.zip... $c"
    tools/auto/auto.sh boot recovery
    echo -e "$g Finish! Build time: $((($(date +%s) - run)/60)) min. $c"
fi

echo -e "$cy Press [y] to clean project... $c"
read -s -n1 answer; [ "$answer" = "y" ] && make ARCH=arm64 mrproper 1>/dev/null 2>/dev/null 
