#!/bin/bash

FULLPATH=$(readlink -f "$0")

BASEPATH=$(dirname "$FULLPATH")

cd "$BASEPATH" || exit

# Point to root dir of source code
ROOTPATH=$(readlink -f "./../../")

# Output dir
OUTPUTDIR=$(readlink -f "./../../release")

PKG_VER="$1"
PPA_VER="$2"


if [ -z "$PPA_VER" ]; then
	echo "Usage:"
	echo "  $0 VERSION BUILD"
	echo " "
	echo "Example:"
	echo "  $0 0.0.6 1"
	exit
fi

# Cleanup
rm -rf "$OUTPUTDIR"
mkdir -p "$OUTPUTDIR"

mkdir -p "$ROOTPATH/.cache/apt"

for dockerfile in dockerfiles/*.Dockerfile; do
	IMGNAME=$(basename ${dockerfile/.Dockerfile//})
	echo "$IMGNAME"

	docker build -f "$dockerfile" -t "$IMGNAME" "$ROOTPATH"
	docker run --rm -it \
		-e PKG_VERSION="${PKG_VER}" \
		-e PKG_PPAVER="${PPA_VER}" \
		-v "$OUTPUTDIR:/release" \
		-v "$ROOTPATH:/src" \
		-t "$IMGNAME"
done
