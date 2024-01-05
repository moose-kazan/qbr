#!/bin/bash

cd $(dirname $0)

for sc_file in `ls ../../release/*.changes`; do
	echo "Processing ${sc_file}:"
	dput ppa:bulvinkl/ppa $sc_file
	echo ""
	echo ""
done
