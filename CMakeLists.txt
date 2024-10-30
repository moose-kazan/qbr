cmake_minimum_required(VERSION 3.5)

project(QBR LANGUAGES CXX)


set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Network Xml LinguistTools)
find_package(Qt6 REQUIRED COMPONENTS Core Widgets WebEngineWidgets Network Xml LinguistTools)
#find_package(libzip)

set(TS_FILES
    src/languages/qbr_en.ts
    src/languages/qbr_ru.ts)

set (ICON_FILES
    src/icon/128x128/qbr.png
    src/icon/16x16/qbr.png
    src/icon/24x24/qbr.png
    src/icon/256x256/qbr.png
    src/icon/32x32/qbr.png
    src/icon/48x48/qbr.png
    src/icon/512x512/qbr.png
    src/icon/64x64/qbr.png
    src/icon/96x96/qbr.png
    src/icon/qbr/actions/application-exit.png
    src/icon/qbr/actions/document-open.png
    src/icon/qbr/actions/document-save-as.png
    src/icon/qbr/actions/edit-find.png
    src/icon/qbr/actions/go-next.png
    src/icon/qbr/actions/go-previous.png
    src/icon/qbr/actions/help-about.png
    src/icon/qbr/actions/preferences-system.png
)

set (RES_DATA
    src/res/default_fb2.xml
    src/res/default_ru_fb2.xml
    src/res/style.css
)

set(PROJECT_SOURCES
    src/qbrcfg.cpp
    src/qbrcfg.h
    src/qbrcommon.cpp
    src/qbrcommon.h
    src/qbrmainwindow.cpp
    src/qbrmainwindow.h
    src/qbrmainwindow.ui
    src/format/qbrformatamb.cpp
    src/format/qbrformatamb.h
    src/format/qbrformatcbz.cpp
    src/format/qbrformatcbz.h
    src/format/qbrformat.cpp
    src/format/qbrformatfb2.cpp
    src/format/qbrformatfb2.h
    src/format/qbrformatfb3.cpp
    src/format/qbrformatfb3.h
    src/format/qbrformat.h
    src/libs/qbrzip.cpp
    src/libs/qbrzip.h
    src/main.cpp
    src/qbraboutdialog.cpp
    src/qbraboutdialog.h
    src/qbraboutdialog.ui
    src/qbrfileinfodialog.cpp
    src/qbrfileinfodialog.h
    src/qbrfileinfodialog.ui
    src/qbrfinddialog.cpp
    src/qbrfinddialog.h
    src/qbrfinddialog.ui
    src/qbrsettingsdialog.cpp
    src/qbrsettingsdialog.h
    src/qbrsettingsdialog.ui
    src/qbrtemplate.cpp
    src/qbrtemplate.h
    src/qbrwebenginepage.cpp
    src/qbrwebenginepage.h
    src/qbr.qrc
    qbr.desktop
    ${RES_DATA}
    ${ICON_FILES}
    ${TS_FILES}
)

qt_add_executable(qbr
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
)
qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
qt6_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})

target_link_libraries(qbr PRIVATE Qt${QT_VERSION_MAJOR}::Core)
target_link_libraries(qbr PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(qbr PRIVATE Qt${QT_VERSION_MAJOR}::Network)
target_link_libraries(qbr PRIVATE Qt${QT_VERSION_MAJOR}::Xml)
target_link_libraries(qbr PRIVATE Qt${QT_VERSION_MAJOR}::WebEngineWidgets)
target_link_libraries(qbr PRIVATE zip)

set_target_properties(qbr PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER com.ylsoftware.qbr
    #MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS qbr
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(FILES qbr.desktop DESTINATION share/applications)
install(FILES src/icon/512x512/qbr.png DESTINATION share/icons)
install(FILES src/res/style.css DESTINATION share/qbr)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(qbr)
endif()
