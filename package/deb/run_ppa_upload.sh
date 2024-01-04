#!/bin/bash

cd $(dirname $0)

for sc_file in `ls ../../release/*.changes`; do
	echo dput ppa:bulvinkl/ppa $sc_file
done
