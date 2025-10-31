find_package(Git QUIET)
if (GIT_FOUND AND EXISTS "${CMAKE_SOURCE_DIR}/.git")
    execute_process(
            COMMAND ${GIT_EXECUTABLE} describe --tags --always --dirty --match "v*"
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            OUTPUT_VARIABLE GIT_VERSION_STRING_SRC
            OUTPUT_STRIP_TRAILING_WHITESPACE
    )
else()
    set(GIT_VERSION_STRING_SRC "v0.0.0-unknown")
endif()

string(REGEX REPLACE "^v" "" GIT_VERSION_STRING ${GIT_VERSION_STRING_SRC})
set(CPACK_PACKAGE_VERSION ${GIT_VERSION_STRING})
#set(CPACK_PACKAGE_VERSION_MAJOR, "12")

configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/src/version.h.in"
        "${CMAKE_CURRENT_BINARY_DIR}/src/version.h"
        @ONLY
)

set(PROJECT_SOURCES
        "${CMAKE_CURRENT_BINARY_DIR}/src/version.h"
        ${PROJECT_SOURCES}
)

target_include_directories(qbr PUBLIC "${CMAKE_CURRENT_BINARY_DIR}/src")
