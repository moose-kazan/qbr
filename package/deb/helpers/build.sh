#!/bin/bash

# Test build binary
cp -av /src /source
cd /source || exit 1
chown -R root:root /source
git checkout "v$PKG_VERSION"

rm -rf build
mkdir -p build
cmake -S. -Bbuild
cmake --build build
RET=$?

if [ "$RET" != "0" ]; then
    echo "Can't build binary"
    exit 1
fi

# Build source package
cd / || exit 1

cp -av /src /source

cd /source || exit 1
chown -R root:root /source
git checkout "v$PKG_VERSION"

PKG_DISTR=$(lsb_release -cs)

echo "qbr (${PKG_VERSION}-${PKG_DISTR}${PKG_PPAVER}) ${PKG_DISTR}; urgency=medium

  * New upstream release

 -- Vadim Kalinnikov <moose@ylsoftware.com>  $(date --rfc-email)" > debian/changelog

dpkg-buildpackage --root-command=fakeroot --build=source
#dpkg-buildpackage --root-command=fakeroot --build=full

ls ..

mv -v ../qbr_${PKG_VERSION}* /release/
#mv -v ../qbr-dbgsym_${PKG_VERSION}* /release/
