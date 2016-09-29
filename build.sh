#!/bin/bash

export KBUILD_BUILD_USER=rezvorck
export KBUILD_BUILD_HOST=ubuntu
export CONFIG_DEBUG_SECTION_MISMATCH=y

run=$(date +%s)

if [ -f arch/arm/boot/zImage-dtb ]
then
    echo "Remove kernel..."
    rm arch/arm/boot/zImage*
fi

echo "Export toolchains..."
export ARCH=arm CROSS_COMPILE=../*5.2*/bin/arm-cortex-linux-gnueabi-
# export ARCH=arm64 CROSS_COMPILE=../*4.9*/bin/aarch64-linux-android-

echo "Make defconfig..."
make s450m_4g_defconfig >/dev/null
# make s450m_4g_64_defconfig >/dev/null

echo "Start build..."
make -j4 >/dev/null 2>errors.log

if [ ! -f arch/arm/boot/zImage-dtb ]
then
    echo "BUILD ERRORS!"
    echo "$(cat errors.log | grep error 2>/dev/null)"
else
    echo "Moving..."
    mv arch/arm/boot/zImage-dtb boot.img-kernel
    echo "Finish! Build time: $((($(date +%s) - run)/60)) min."
fi

echo "Press [y] to clean project:"
while read answer; do [ "$answer" = "y" ] && make ARCH=arm clean; break; done



