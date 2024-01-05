# Qt Book Reader #

![Screenshot](./screenshot.png)

## Russian ##
Простая читалка электронных книг, написанная на C++ с использованием тулкита Qt5.

В настоящий момент поддерживаются следующие форматы:
* FictionBook 2.0 (fb2)
* FictionBook 3.0 (fb3)
* Comics Book Zip (cbz)
* Ancient Machine Book (amb)

### Установка ###

#### Сборка на Ubuntu 22.04: ####
1. Склонировать репозиторий
2. sudo apt install qt5-qmake qtwebengine5-dev build-essential pkg-config libzip-dev
3. mkdir -p build && cd build && qmake .. && make

#### Сборка на AltLinux p10 ####
1. sudo apt-get install git qt5-base-devel qt5-webengine-devel pkg-config libzip-devel
2. Склонировать репозиторий
3. mkdir -p build && cd build && qmake-qt5 .. && make

#### Установка из PPA (на Ubuntu 22.04): ####
1. sudo add-apt-repository ppa:bulvinkl/ppa
2. sudo apt update
3. sudo apt install qbr

## English ##
Simple book reader, written on C++ with Qt5 toolkit.

For now it support the next file formats:
* FictionBook 2.0 (fb2)
* FictionBook 3.0 (fb3)
* Comics Book Zip (cbz)
* Ancient Machine Book (amb)

### Installation ###

#### Build on Ubuntu 22.04: ####
1. Clone this repository
2. sudo apt install qt5-qmake qtwebengine5-dev libquazip5-dev pkg-config libzip-dev
3. mkdir -p build && cd build && qmake .. && make

#### Build on AltLinux p10 ####
1. sudo apt-get install git qt5-base-devel qt5-webengine-devel pkg-config libzip-devel
2. Clone this repository
3. mkdir -p build && cd build && qmake-qt5 .. && make

#### Install from PPA (Ubuntu 22.04): ####
1. sudo add-apt-repository ppa:bulvinkl/ppa
2. sudo apt update
3. sudo apt install qbr
