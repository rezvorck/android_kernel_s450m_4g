#!/bin/bash

export KBUILD_BUILD_USER=rezvorck
export KBUILD_BUILD_HOST=debian
export CONFIG_DEBUG_SECTION_MISMATCH=y
export GCC_VERSION="gcc version 5.3.1 20160412 (linaro) (GCC)"

run=$(date +%s)

if [ -f arch/arm64/boot/Image.gz-dtb ]
then
    echo "Remove kernel..."
    rm arch/arm64/boot/Image*
fi

echo "Export toolchains..."
export ARCH=arm64 && export CROSS_COMPILE=$(tools/auto/toolchain.sh ..)

echo "Make defconfig..."
make s450m_4g_64_defconfig >/dev/null

echo "Start build..."
make -j4 >/dev/null 2>errors_64.log

if [ ! -f arch/arm64/boot/Image.gz-dtb ]
then
    echo "BUILD ERRORS!"
    echo "$(cat errors_64.log | grep error 2>/dev/null)"
else
    echo "Creating update zip..."
    tools/auto/auto.sh boot recovery
    echo "Finish! Build time: $((($(date +%s) - run)/60)) min."
fi

echo "Press [y] to clean project:"
while read answer; do [ "$answer" = "y" ] && make ARCH=arm64 mrproper; break; done
