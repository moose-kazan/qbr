find_package(Git QUIET)
if (GIT_FOUND AND EXISTS "${CMAKE_SOURCE_DIR}/.git")
    execute_process(
            COMMAND ${GIT_EXECUTABLE} describe --tags --always --dirty --match "v*"
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            OUTPUT_VARIABLE GIT_VERSION_STRING
            OUTPUT_STRIP_TRAILING_WHITESPACE
    )
else()
    set(GIT_VERSION_STRING "0.0.0-unknown")
endif()

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
