#!/bin/sh

dir=$PWD

case "$ARCH" in
arm64) 
 [ -f "arch/arm64/boot/Image.gz-dtb" ] && [ "$(echo $@ | grep boot)" ] && cp arch/arm64/boot/Image.gz-dtb $(dirname $0)/boot/arm64/boot.img-kernel && $(dirname $0)/repack.sh boot
 [ -f "arch/arm64/boot/Image.gz-dtb" ] && [ "$(echo $@ | grep recovery)" ] && cp arch/arm64/boot/Image.gz-dtb $(dirname $0)/recovery/arm64/recovery.img-kernel && $(dirname $0)/repack.sh recovery
;;
arm)
 [ -f "arch/arm/boot/zImage-dtb" ] && [ "$(echo $@ | grep boot)" ] && cp arch/arm/boot/zImage-dtb $(dirname $0)/boot/arm/boot.img-kernel && $(dirname $0)/repack.sh boot
 [ -f "arch/arm/boot/zImage-dtb" ] && [ "$(echo $@ | grep recovery)" ] && cp arch/arm/boot/zImage-dtb $(dirname $0)/recovery/arm/recovery.img-kernel && $(dirname $0)/repack.sh recovery
;;
esac

[ -f "$(dirname $0)/boot.img" ] && mv $(dirname $0)/boot.img $(dirname $0)/update-zip/boot.img && echo 'package_extract_file("boot.img", "/dev/block/platform/mtk-msdc.0/by-name/boot");' >> $(dirname $0)/update-zip/META-INF/com/google/android/updater-script
[ -f "$(dirname $0)/recovery.img" ] && mv $(dirname $0)/recovery.img $(dirname $0)/update-zip/recovery.img && echo 'package_extract_file("recovery.img", "/dev/block/platform/mtk-msdc.0/by-name/recovery");' >> $(dirname $0)/update-zip/META-INF/com/google/android/updater-script

cd $(dirname $0)/update-zip
zip -r $dir/update.zip .
rm -f *.img
rm -f META-INF/com/google/android/updater-script
