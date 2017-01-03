#!/bin/bash

mkbootfs=$(dirname $0)/bin/mkbootfs
minigzip=$(dirname $0)/bin/minigzip
mkbootimg=$(dirname $0)/bin/mkbootimg

[[ -f $(dirname $0)/$1.img ]] && rm -f $(dirname $0)/$1.img

[ ! "$(dirname $0)/$1/$ARCH" ] && echo "No folder selected. ERROR!" && exit 1
dir="$(dirname $0)/$1/$ARCH"
kernel=$dir/$1.img-kernel

compress=$(cat $dir/$1.img-ramdisk-compress);

if [ "$compress" == "gz" ]; then
	$mkbootfs $dir/ramdisk | $minigzip -c -9 > $(dirname $0)/$1.img-ramdisk.gz
	ramdisk=$(dirname $0)/$1.img-ramdisk.gz
	[ ! $? -eq "0" ] && exit 1
fi

if [ "$compress" == "lzma" ]; then
	$mkbootfs $dir/ramdisk | xz --format=lzma -1zv > $(dirname $0)/$1.img-ramdisk.lzma
	ramdisk=$(dirname $0)/$1.img-ramdisk.lzma
	[ ! $? -eq "0" ] && exit 1
fi

if [ "$compress" == "xz" ]; then
	$mkbootfs $dir/ramdisk | xz -1zv -Ccrc32 > $(dirname $0)/$1.img-ramdisk.xz
	ramdisk=$(dirname $0)/$1.img-ramdisk.xz
	[ ! $? -eq "0" ] && exit 1
fi

if [ "$compress" == "bz2" ]; then
	$mkbootfs $dir/ramdisk | bzip2 -v > $(dirname $0)/$1.img-ramdisk.bz2
	ramdisk=$(dirname $0)/$1.img-ramdisk.bz2
	[ ! $? -eq "0" ] && exit 1
fi

if [ "$compress" == "lzo" ]; then
	$mkbootfs $dir/ramdisk | lzop -v > $(dirname $0)/$1.img-ramdisk.lzo
	ramdisk=$(dirname $0)/$1.img-ramdisk.lzo
	[ ! $? -eq "0" ] && exit 1
fi

if [ "$compress" == "lz4" ]; then
	$mkbootfs $dir/ramdisk | lz4 -l stdin stdout > $(dirname $0)/$1.img-ramdisk.lz4
	ramdisk=$(dirname $0)/$1.img-ramdisk.lz4
	[ ! $? -eq "0" ] && exit 1
fi

[[ -f $dir/$1.img-board ]] && bname=$(cat $dir/$1.img-board)
[[ -f $dir/$1.img-base ]] && base=$(cat $dir/$1.img-base);
[[ -f $dir/$1.img-pagesize ]] && pagesize=$(cat $dir/$1.img-pagesize)
[[ -f $dir/$1.img-cmdline ]] && lcmd=$(cat $dir/$1.img-cmdline)
[[ -f $dir/$1.img-kernel_offset ]] && keoff=$(cat $dir/$1.img-kernel_offset)
[[ -f $dir/$1.img-ramdisk_offset ]] && ramoff=$(cat $dir/$1.img-ramdisk_offset)
[[ -f $dir/$1.img-second_offset ]] && flsec=$(cat $dir/$1.img-second_offset) && losec=" --second_offset $flsec"	
[[ -f $dir/$1.img-second ]] && secboot=$dir/$1.img-second && second=" --second $secboot"
[[ -f $dir/$1.img-tags_offset ]] && tagoff=$(cat $dir/$1.img-tags_offset)
[[ -f $dir/$1.img-dt ]] && ldtb=$dir/$1.img-dt && dtb=" --dt $ldtb"

if [[ -f $dir/$1.img-mtk ]]; then
	$mkbootimg --kernel "$kernel" --ramdisk "$ramdisk" --pagesize "$pagesize" --cmdline "$lcmd" --board "$bname" --base "$base" --kernel_offset "$keoff" --ramdisk_offset "$ramoff" --tags_offset "$tagoff"$losec$second$dtb --mtk 1 -o $(dirname $0)/$1.img 1>/dev/null
	[ ! $? -eq "0" ] && exit 1
else
	$mkbootimg --kernel "$kernel" --ramdisk "$ramdisk" --pagesize "$pagesize" --cmdline "$lcmd" --board "$bname" --base "$base" --kernel_offset "$keoff" --ramdisk_offset "$ramoff" --tags_offset "$tagoff"$losec$second$dtb -o $(dirname $0)/$1.img 1>/dev/null
	[ ! $? -eq "0" ] && exit 1
fi

rm -f $ramdisk
