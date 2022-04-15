QT       += core gui xml webenginewidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    aboutdialog.cpp \
    format/qbrformatamb.cpp \
    format/qbrformatcbz.cpp \
    format/qbrformatfb2.cpp \
    format/qbrformatfb3.cpp \
    main.cpp \
    qbrcfg.cpp \
    qbrcommon.cpp \
    qbrformat.cpp \
    qbrmainwindow.cpp \
    qbrtemplate.cpp \
    qbrwebenginepage.cpp \
    qbrzip.cpp

HEADERS += \
    aboutdialog.h \
    format/qbrformatamb.h \
    format/qbrformatcbz.h \
    format/qbrformatfb2.h \
    format/qbrformatfb3.h \
    qbrcfg.h \
    qbrcommon.h \
    qbrformat.h \
    qbrmainwindow.h \
    qbrtemplate.h \
    qbrwebenginepage.h \
    qbrzip.h

FORMS += \
    qbrmainwindow.ui

TRANSLATIONS += \
    qbr_en_US.ts

# Default rules for deployment.
#qnx: target.path = /tmp/$${TARGET}/bin
#else: unix:!android: target.path = /opt/$${TARGET}/bin
#!isEmpty(target.path): INSTALLS += target

unix: {
    isEmpty(PREFIX) {
        PREFIX = /usr/local
    }

    SRCDIR = $${PWD}
    BINDIR = $${PREFIX}/bin
    DATADIR = $${PREFIX}/share


    target.path = $${BINDIR}

    desktop.files = $${SRCDIR}/QBR.desktop
    desktop.path = $${DATADIR}/applications

    icon.files = $${SRCDIR}/icon/512x512/qbr.png
    icon.path = $${DATADIR}/icons

    INSTALLS += target desktop icon
}

#unix: CONFIG += link_pkgconfig
#unix: PKGCONFIG += libzip

LIBS += -lquazip5

RESOURCES += \
    qbr.qrc

DISTFILES += \
    qbr.desktop \
    debian/source/format \
    debian/control \
    debian/rules \
    debian/compat \
    debian/changelog \
    debian/copyright \
    icon/16x16/qbr.png \
    icon/24x24/qbr.png \
    icon/32x32/qbr.png \
    icon/48x48/qbr.png \
    icon/64x64/qbr.png \
    icon/96x96/qbr.png \
    icon/128x128/qbr.png \
    icon/256x256/qbr.png \
    icon/512x512/qbr.png
