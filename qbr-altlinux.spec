Name:           qbr
Version:        0.1.1
Release:        1%{?dist}
Summary:        Simple book reader, written on C++ with Qt6 toolkit.
Group:          Office
License:        LGPL-2.1
URL:            https://github.com/moose-kazan/qbr
Source0:        https://github.com/moose-kazan/qbr/archive/refs/tags/v%{version}.zip
BuildRequires:  build-essential cmake libzip-devel qt6-base-devel qt6-webengine-devel qt6-tools-devel

%description
Simple book reader, written on C++ with Qt6 toolkit.

For now it support the next file formats:

* FictionBook 2.0 (fb2)
* Zipped FictionBook 2.0 (fb2.zip)
* FictionBook 3.0 (fb3)
* Electronic Publication (epub)
* Comics Book Zip (cbz)
* Ancient Machine Book (amb)

%prep
%setup -q

%build
%cmake
%cmake_build

%install
%cmake_install

%files
%_bindir/*
%_desktopdir/*
%_datadir/*

%changelog
* Wed Jul 29 2026 Vadim Kalinnikov <moose@ylsoftware.com> - 0.1.1
- Initial build
