QT       += core gui xml webenginewidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11 link_pkgconfig

PKGCONFIG += libzip

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
    src/format/qbrformatamb.cpp \
    src/format/qbrformatcbz.cpp \
    src/format/qbrformatfb2.cpp \
    src/format/qbrformatfb3.cpp \
    src/main.cpp \
    src/qbraboutdialog.cpp \
    src/qbrcfg.cpp \
    src/qbrcommon.cpp \
    src/qbrformat.cpp \
    src/qbrmainwindow.cpp \
    src/qbrsettingsdialog.cpp \
    src/qbrtemplate.cpp \
    src/qbrwebenginepage.cpp \
    src/libs/qbrzip.cpp

HEADERS += \
    src/format/qbrformatamb.h \
    src/format/qbrformatcbz.h \
    src/format/qbrformatfb2.h \
    src/format/qbrformatfb3.h \
    src/qbraboutdialog.h \
    src/qbrcfg.h \
    src/qbrcommon.h \
    src/qbrformat.h \
    src/qbrmainwindow.h \
    src/qbrsettingsdialog.h \
    src/qbrtemplate.h \
    src/qbrwebenginepage.h \
    src/libs/qbrzip.h

FORMS += \
    src/qbraboutdialog.ui \
    src/qbrmainwindow.ui \
    src/qbrsettingsdialog.ui

TRANSLATIONS += \
    src/languages/qbr_en.ts \
    src/languages/qbr_ru.ts

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

    desktop.files = $${SRCDIR}/qbr.desktop
    desktop.path = $${DATADIR}/applications

    icon.files = $${SRCDIR}/src/icon/512x512/qbr.png
    icon.path = $${DATADIR}/icons

    INSTALLS += target desktop icon
}

#unix: CONFIG += link_pkgconfig
#unix: PKGCONFIG += libzip

#LIBS += -lquazip5

RESOURCES += \
    src/qbr.qrc

DISTFILES += \
    qbr.desktop \
    debian/source/format \
    debian/control \
    debian/rules \
    debian/compat \
    debian/changelog \
    debian/copyright \
    src/icon/16x16/qbr.png \
    src/icon/24x24/qbr.png \
    src/icon/32x32/qbr.png \
    src/icon/48x48/qbr.png \
    src/icon/64x64/qbr.png \
    src/icon/96x96/qbr.png \
    src/icon/128x128/qbr.png \
    src/icon/256x256/qbr.png \
    src/icon/512x512/qbr.png \
    src/res/default_fb2.xml \
    src/res/default_ru_fb2.xml \
    NEWS.md \
    README.md

