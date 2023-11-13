#!/bin/bash

cd `dirname $0`

which convert > /dev/null 2>&1
if [ "$?" != "0" ]; then
	echo "Please install ImageMagick first!"
	exit 1
fi


DESTDIR="../../src/icon/qbr"
ICONS=$(find . -type f -name '*.svg')

for iconsrc in ${ICONS}; do
	DSTICON=${iconsrc/.svg/.png}
	mkdir -p $(dirname "${DESTDIR}/${DSTICON}")
	convert -adaptive-resize 128x128 ${iconsrc} ${DESTDIR}/${DSTICON}
done
