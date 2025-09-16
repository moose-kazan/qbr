cmake_minimum_required(VERSION 3.18)

project(QBR LANGUAGES CXX)


set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Network Xml LinguistTools)
find_package(Qt6 REQUIRED COMPONENTS Core Widgets WebEngineWidgets Network Xml LinguistTools)
find_package(libzip REQUIRED)

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
    src/mainwindow.cpp
    src/mainwindow.h
    src/mainwindow.ui
    src/libs/helpers.cpp
    src/libs/helpers.h
    src/libs/qbrzip.cpp
    src/libs/qbrzip.h
    src/libs/qbrunzip.h
    src/libs/qbrunzip.cpp
    src/libs/settings.cpp
    src/libs/settings.h
    src/libs/qbrbookinfo.cpp
    src/libs/qbrbookinfo.h
    src/format/format.h
    src/format/format.cpp
    src/format/formatamb.h
    src/format/formatamb.cpp
    src/format/formatcbz.h
    src/format/formatcbz.cpp
    src/format/formatfb2.h
    src/format/formatfb2.cpp
    src/format/formatfb2zip.h
    src/format/formatfb2zip.cpp
    src/format/formatfb3.h
    src/format/formatfb3.cpp
    src/format/formatepub.h
    src/format/formatepub.cpp
    src/main.cpp
    src/aboutdialog.cpp
    src/aboutdialog.h
    src/aboutdialog.ui
    src/fileinfodialog.cpp
    src/fileinfodialog.h
    src/fileinfodialog.ui
    src/finddialog.cpp
    src/finddialog.h
    src/finddialog.ui
    src/settingsdialog.cpp
    src/settingsdialog.h
    src/settingsdialog.ui
    src/bookloader.h
    src/bookloader.cpp
    src/qbrwebenginepage.cpp
    src/qbrwebenginepage.h
    src/booksaver.h
    src/booksaver.cpp
    src/export/export.h
    src/export/export.cpp
    src/export/exporthtml.h
    src/export/exporthtml.cpp
    src/export/exporttext.h
    src/export/exporttext.cpp
    src/export/exportepub.cpp
    src/export/exportepub.h
    src/qbr.qrc
    qbr.desktop
    ${RES_DATA}
    ${ICON_FILES}
    ${TS_FILES}
)

if (Qt6Core_VERSION VERSION_GREATER_EQUAL 6.4.0)
set(PROJECT_SOURCES
    src/export/exportmarkdown.h
    src/export/exportmarkdown.cpp
	${PROJECT_SOURCES}
)
endif()

qt_add_executable(qbr
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
)

#qt6_standard_project_setup()

qt_add_translations(
    qbr
    TS_FILES ${TS_FILES}
    RESOURCE_PREFIX "languages"
)


target_link_libraries(qbr PRIVATE Qt${QT_VERSION_MAJOR}::Core)
target_link_libraries(qbr PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(qbr PRIVATE Qt${QT_VERSION_MAJOR}::Network)
target_link_libraries(qbr PRIVATE Qt${QT_VERSION_MAJOR}::Xml)
target_link_libraries(qbr PRIVATE Qt${QT_VERSION_MAJOR}::WebEngineWidgets)
target_link_libraries(qbr PRIVATE libzip::zip)

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
install(FILES src/res/style.css DESTINATION share/qbr)
install(FILES src/icon/512x512/qbr.png DESTINATION share/icons)
foreach(SIZE 16x16 24x24 32x32 48x48 64x64 96x96 128x128 256x256 512x512)
    install(FILES src/icon/${SIZE}/qbr.png DESTINATION share/icons/hicolor/${SIZE}/apps)
endforeach()

qt_finalize_executable(qbr)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(Packing)

