# Qt Book Reader #

![Screenshot](./screenshot.png)

## Russian ##
Простая читалка электронных книг, написанная на C++ с использованием тулкита Qt5.

В настоящий момент поддерживаются следующие форматы:
* FictionBook 2.0 (fb2)
* Zipped FictionBook 2.0 (fb2.zip)
* FictionBook 3.0 (fb3)
* Electronic Publication (epub)
* Comics Book Zip (cbz)
* Ancient Machine Book (amb)

### Установка ###

#### Сборка на Ubuntu 22.04 или 22.04: ####
1. Склонировать репозиторий
2. sudo apt install cmake qt6-webengine-dev libzip-dev qt6-tools-dev \
    qt6-webengine-dev-tools libgl1-mesa-dev libvulkan-dev qt6-l10n-tools \
    qt6-tools-dev-tools libqt6opengl6-dev zipcmp zipmerge ziptool
3. mkdir -p build && cd build && cmake .. && make

#### Сборка на AltLinux p10 ####
1. sudo apt-get install git cmake qt6-base-devel qt6-webengine-devel \
    libzip-devel qt6-tools-devel
2. Склонировать репозиторий
3. mkdir -p build && cd build && cmake .. && make

#### Установка из PPA (на Ubuntu 22.04): ####
1. sudo add-apt-repository ppa:bulvinkl/ppa
2. sudo apt update
3. sudo apt install qbr

## English ##
Simple book reader, written on C++ with Qt5 toolkit.

For now it support the next file formats:
* FictionBook 2.0 (fb2)
* Zipped FictionBook 2.0 (fb2.zip)
* FictionBook 3.0 (fb3)
* Electronic Publication (epub)
* Comics Book Zip (cbz)
* Ancient Machine Book (amb)

### Installation ###

#### Build on Ubuntu 22.04 or 24.04: ####
1. Clone this repository
2. sudo apt install cmake qt6-webengine-dev libzip-dev qt6-tools-dev \
    qt6-webengine-dev-tools libgl1-mesa-dev libvulkan-dev qt6-l10n-tools \
    qt6-tools-dev-tools libqt6opengl6-dev zipcmp zipmerge ziptool
3. mkdir -p build && cd build && cmake .. && make

#### Build on AltLinux p10 ####
1. sudo apt-get install git cmake qt6-base-devel qt6-webengine-devel \
    libzip-devel qt6-tools-devel
2. Clone this repository
3. mkdir -p build && cd build && cmake .. && make

#### Install from PPA (Ubuntu 22.04 or 24.04): ####
1. sudo add-apt-repository ppa:bulvinkl/ppa
2. sudo apt update
3. sudo apt install qbr
