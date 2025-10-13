#!/bin/bash

cd "$(dirname "$(readlink -f $0)")" || exit

for s in 512x512 256x256 128x128 96x96 64x64 48x48 32x32 24x24 16x16; do
	echo $s
	mkdir -pv ../../src/icon/$s
	convert icon-2-src.png -resize $s ../../src/icon/$s/qbr.png
done
