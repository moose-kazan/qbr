#!/bin/bash

cp -av /src /source

cd /source
chown -R root:root /source
git checkout v$PKG_VERSION

PKG_DISTR=$(lsb_release -c | grep Codename | awk '{print $2;}')

echo "qbr (${PKG_VERSION}-${PKG_DISTR}${PKG_PPAVER}) ${PKG_DISTR}; urgency=medium

  * New upstream release

 -- Vadim Kalinnikov <moose@ylsoftware.com>  $(date --rfc-email)" > debian/changelog

#dpkg-buildpackage --root-command=fakeroot --build=source
dpkg-buildpackage --root-command=fakeroot --build=full

ls ..

mv -v ../qbr_${PKG_VERSION}* /release/
