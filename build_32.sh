#!/bin/bash

export KBUILD_BUILD_USER=rezvorck
export KBUILD_BUILD_HOST=ubuntu
export CONFIG_DEBUG_SECTION_MISMATCH=y
export GCC_VERSION="gcc version 5.2.1 20151005 (linaro) (GCC)"

run=$(date +%s)

if [ -f arch/arm/boot/zImage-dtb ]
then
    echo "Remove kernel..."
    rm arch/arm/boot/zImage*
fi

echo "Export toolchains..."
export ARCH=arm CROSS_COMPILE=../*5.2*/bin/arm-cortex-linux-gnueabi-

echo "Make defconfig..."
make s450m_4g_defconfig >/dev/null

echo "Start build..."
make -j4 >/dev/null 2>errors_3.10_32.log

if [ ! -f arch/arm/boot/zImage-dtb ]
then
    echo "BUILD ERRORS!"
    echo "$(cat errors_3.10_32.log | grep error 2>/dev/null)"
else
    echo "Moving..."
    mv arch/arm/boot/zImage-dtb boot_3.10_32.img-kernel
    echo "Finish! Build time: $((($(date +%s) - run)/60)) min."
fi

echo "Press [y] to clean project:"
while read answer; do [ "$answer" = "y" ] && make ARCH=arm mrproper; break; done
