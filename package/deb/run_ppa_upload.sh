#!/bin/bash

FULLPATH=$(readlink -f "$0")

BASEPATH=$(dirname "$FULLPATH")

cd "$BASEPATH" || exit

for sc_file in  ../../release/*.changes; do
	echo "Processing ${sc_file}:"
	dput ppa:bulvinkl/ppa "${sc_file}"
	echo ""
	echo ""
done
