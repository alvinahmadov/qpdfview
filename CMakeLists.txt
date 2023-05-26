cmake_minimum_required(VERSION 3.15)
project(qpdfview)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CONFIGURATION_TYPES Debug;Release)
#set(CMAKE_COLOR_MAKEFILE OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Globally enable automoc
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# build/(win32|linux)/(release|debug)
string(TOLOWER "${CMAKE_BUILD_TYPE}" BUILD_TYPE)
set(BUILD_PATH ${CMAKE_SOURCE_DIR}/build/${BUILD_TYPE})

set(CMAKE_BINARY_DIR ${BUILD_PATH})

if(WIN32)
    set(CXX_FLAGS "/EHsc")
    set(CXX_FLAGS_DEBUG "/Wall /Od")
    # Warning level 2, Optimization level 2
    set(CXX_FLAGS_RELEASE "/O2 /W2")
    set(NATIVE WIN32)
else()
    set(CXX_FLAGS "-Wall")
    if(NOT ${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
        set(CXX_FLAGS_DEBUG "-Wextra -g -O0 ")
        set(CXX_FLAGS_RELEASE "-O3 -ffp-contract=off -march=native -fno-expensive-optimizations -Wno-deprecated-declarations")
    else()
        set(CXX_FLAGS_DEBUG "-Wextra -g -O0 -fstandalone-debug")
        set(CXX_FLAGS_RELEASE "-O3 -ffp-contract=off")
    endif()
    # On linux target exe file doesn't need to be build as Windows' native executable
    set(NATIVE)
endif(WIN32)

set(APPLICATION_VERSION     0.4.18)
set(TARGET_INSTALL_PATH     ${CMAKE_INSTALL_PREFIX}/bin)
set(PLUGIN_INSTALL_PATH     ${CMAKE_INSTALL_PREFIX}/lib/qpdfview)
set(DATA_INSTALL_PATH       ${CMAKE_INSTALL_PREFIX}/share/qpdfview)
set(MANUAL_INSTALL_PATH     ${CMAKE_INSTALL_PREFIX}/share/man/man1)
set(ICON_INSTALL_PATH       ${CMAKE_INSTALL_PREFIX}/share/icons/hicolor/scalable/apps)
set(LAUNCHER_INSTALL_PATH   ${CMAKE_INSTALL_PREFIX}/share/applications)
set(APPDATA_INSTALL_PATH    ${CMAKE_INSTALL_PREFIX}/share/appdata)
set(DESKTOP_FILE            ${CMAKE_CURRENT_SOURCE_DIR}/miscellaneous/qpdfview.desktop)
set(APPDATA_FILE            ${CMAKE_CURRENT_SOURCE_DIR}/miscellaneous/qpdfview.appdata.xml)
set(APP_DIR_DATA_PATH       "data")
#set(MANUAL_FILE)
set(RESOURCES "")
set(QPDFVIEW_MIME_TYPES)
set(QPDFVIEW_PLUGINS)

# Global variables
set(SHARED_LIB_PREFIX       ${CMAKE_SHARED_LIBRARY_PREFIX})
set(SHARED_LIB_SUFFIX       ${CMAKE_SHARED_LIBRARY_SUFFIX})
set(STATIC_LIB_PREFIX       ${CMAKE_STATIC_LIBRARY_PREFIX})
set(STATIC_LIB_SUFFIX       ${CMAKE_STATIC_LIBRARY_SUFFIX})
set(QPDFVIEW_CMAKE_DIR      ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(QPDFVIEW_SOURCE_DIR     ${CMAKE_CURRENT_SOURCE_DIR}/sources)
set(QPDFVIEW_INCLUDE_DIR    ${CMAKE_CURRENT_SOURCE_DIR}/sources)
set(QPDFVIEW_DEPENDENCIES)
set(QPDFVIEW_LIBRARIES)
set(QPDFVIEW_SOURCES
    ${QPDFVIEW_SOURCE_DIR}/global.h
    ${QPDFVIEW_SOURCE_DIR}/model.h
    ${QPDFVIEW_SOURCE_DIR}/settings.cpp
    ${QPDFVIEW_SOURCE_DIR}/pluginhandler.cpp
    ${QPDFVIEW_SOURCE_DIR}/shortcuthandler.cpp
    ${QPDFVIEW_SOURCE_DIR}/rendertask.cpp
    ${QPDFVIEW_SOURCE_DIR}/tileitem.cpp
    ${QPDFVIEW_SOURCE_DIR}/pageitem.cpp
    ${QPDFVIEW_SOURCE_DIR}/thumbnailitem.cpp
    ${QPDFVIEW_SOURCE_DIR}/presentationview.cpp
    ${QPDFVIEW_SOURCE_DIR}/searchmodel.cpp
    ${QPDFVIEW_SOURCE_DIR}/searchtask.cpp
    ${QPDFVIEW_SOURCE_DIR}/miscellaneous.cpp
    ${QPDFVIEW_SOURCE_DIR}/documentlayout.cpp
    ${QPDFVIEW_SOURCE_DIR}/documentview.cpp
    ${QPDFVIEW_SOURCE_DIR}/printdialog.cpp
    ${QPDFVIEW_SOURCE_DIR}/settingsdialog.cpp
    ${QPDFVIEW_SOURCE_DIR}/searchitemdelegate.cpp
    ${QPDFVIEW_SOURCE_DIR}/fontsdialog.cpp
    ${QPDFVIEW_SOURCE_DIR}/helpdialog.cpp
    ${QPDFVIEW_SOURCE_DIR}/recentlyusedmenu.cpp
    ${QPDFVIEW_SOURCE_DIR}/recentlyclosedmenu.cpp
    ${QPDFVIEW_SOURCE_DIR}/bookmarkmenu.cpp
    ${QPDFVIEW_SOURCE_DIR}/bookmarkdialog.cpp
    ${QPDFVIEW_SOURCE_DIR}/bookmarkmodel.cpp
    ${QPDFVIEW_SOURCE_DIR}/database.cpp
    ${QPDFVIEW_SOURCE_DIR}/mainwindow.cpp
    ${QPDFVIEW_SOURCE_DIR}/main.cpp)
set(QPDFVIEW_DEFINITIONS
     -DAPPLICATION_VERSION="${APPLICATION_VERSION}"
     -DTARGET_INSTALL_PATH="${TARGET_INSTALL_PATH}"
     -DPLUGIN_INSTALL_PATH="${PLUGIN_INSTALL_PATH}"
     -DDATA_INSTALL_PATH="${DATA_INSTALL_PATH}"
     -DMANUAL_INSTALL_PATH="${MANUAL_INSTALL_PATH}"
     -DICON_INSTALL_PATH="${ICON_INSTALL_PATH}"
     -DLAUNCHER_INSTALL_PATH="${LAUNCHER_INSTALL_PATH}"
     -DAPP_DIR_DATA_PATH="${APP_DIR_DATA_PATH}"
     -DAPPDATA_INSTALL_PATH="${APPDATA_INSTALL_PATH}")
set(DISABLE_STATUS OFF)

include(${QPDFVIEW_CMAKE_DIR}/qpdfview_utils.cmake)

set(QT_MAJOR "5")
set(QT_EXACT_VERSION "5.12.4")

qp_dependent_option(WITH_LTO
                    "Disables PDF support, i.e. the PDF plug-in using Poppler will not be built."
                    ON)

qp_dependent_option(WITHOUT_PDF
                    "Disables PDF support, i.e. the PDF plug-in using Poppler will not be built."
                    OFF)

qp_dependent_option(WITHOUT_PS
                    "Disables PS support, i.e. the PS plug-in using libspectre will not be built."
                    OFF)

qp_dependent_option(WITHOUT_DJVU
                    "Disables DjVu support, i.e. the DjVu plug-in using DjVuLibre will not be built."
                    OFF)

qp_dependent_option(WITH_FITZ
                    "Enables Fitz support, i.e. the Fitz plug-in using MuPDF will be built."
                    ON)

qp_dependent_option(WITHOUT_IMAGE
                    "Disable image support, i.e. the plug-in using Qt's built-in image I/O will not be built."
                    OFF)

qp_dependent_option(STATIC_PDF_PLUGIN
                    "Links the PDF plug-in statically (This could lead to linker dependency collisions.)"
                    OFF)

qp_dependent_option(STATIC_PS_PLUGIN
                    "Links the PS plug-in statically. (This could lead to linker dependency collisions.)"
                    OFF)

qp_dependent_option(STATIC_DJVU_PLUGIN
                    "Links the PS plug-in statically. (This could lead to linker dependency collisions.)"
                    OFF)

qp_dependent_option(STATIC_IMAGE_PLUGIN
                    "Links the image plug-in statically."
                    OFF)

qp_dependent_option(STATIC_FITZ_PLUGIN
                    "Links the Fitz plug-in statically. (This could lead to linker dependency collisions.)"
                    OFF)

qp_dependent_option(PLUGIN_RESOLVE_ALL
                    "Disables PDF support, i.e. the PDF plug-in using Poppler will not be built."
                    OFF)

qp_dependent_option(WITHOUT_SVG
                    "Disables SVG support, i.e. fallback and application-specific icons will not be available."
                    OFF)

qp_dependent_option(WITHOUT_SQL
                    "Disables SQL support, i.e. restoring tabs, bookmarks and per-file settings will not be available."
                    OFF)

qp_dependent_option(WITHOUT_DBUS
                    "Disables D-Bus support, i.e. the '--unique' command-line option will not be available."
                    OFF)

qp_dependent_option(WITHOUT_PKGCONFIG
                    "Disables the use of pkg-config, i.e. compiler and linker options have to be configured manually"
                    OFF
                    IF UNIX AND NOT WIN32)

qp_dependent_option(WITHOUT_CUPS
                    "Disables CUPS support, i.e. the program will attempt to rasterize the document instead of requesting CUPS to print the document file."
                    OFF
                    IF UNIX AND NOT WIN32)

qp_dependent_option(WITHOUT_SYNCTEX
                    "Disables SyncTeX support, i.e. the program will not perform forward and inverse search for sources."
                    ON)

qp_dependent_option(WITHOUT_SIGNALS "Disables support for UNIX signals, i.e. the program will not save bookmarks, tabs and per-file settings on receiving SIGINT or SIGTERM."
                    OFF
                    IF UNIX AND NOT WIN32)

# Set CXX flags
set(CMAKE_CXX_FLAGS ${CXX_FLAGS})
set(CMAKE_CXX_FLAGS_DEBUG ${CXX_FLAGS_DEBUG})
set(CMAKE_CXX_FLAGS_RELEASE ${CXX_FLAGS_RELEASE})

# Needed to include here to preoperly run AUTOMOC precompiling
# Reason: All needed Qt libraries are linked against project's
# base library and not visible for module libraries, make it global
find_package(Qt${QT_MAJOR}Core ${QT_EXACT_VERSION} REQUIRED)

if(${QT_MAJOR} GREATER 4)
    find_package(Qt${QT_MAJOR}Concurrent ${QT_EXACT_VERSION} REQUIRED)
    find_package(Qt${QT_MAJOR}PrintSupport ${QT_EXACT_VERSION} REQUIRED)
    find_package(Qt${QT_MAJOR}Widgets ${QT_EXACT_VERSION} REQUIRED)

    list(APPEND QPDFVIEW_LIBRARIES
         Qt${QT_MAJOR}::Concurrent
         Qt${QT_MAJOR}::PrintSupport
         Qt${QT_MAJOR}::Widgets)
endif()

# WITH_LTO
if(${WITH_LTO})
endif()

# WITHOUT_SVG
if(NOT ${WITHOUT_SVG})
    find_package(Qt${QT_MAJOR}Svg ${QT_EXACT_VERSION} REQUIRED)
    list(APPEND QPDFVIEW_LIBRARIES Qt${QT_MAJOR}::Svg)
    list(APPEND QPDFVIEW_DEFINITIONS -DWITH_SVG)
    list(APPEND RESOURCES icons.qrc)
    message(STATUS "Building with svg")
else()
    message(STATUS "Building without svg")
endif()

# WITHOUT_SQL
if(NOT ${WITHOUT_SQL})
    find_package(Qt${QT_MAJOR}Sql ${QT_EXACT_VERSION} REQUIRED)
    list(APPEND QPDFVIEW_LIBRARIES Qt${QT_MAJOR}::Sql)
    list(APPEND QPDFVIEW_DEFINITIONS -DWITH_SQL)
    message(STATUS "Building with sql")
else()
    message(STATUS "Building without sql")
endif()

# WITHOUT_DBUS
if(NOT ${WITHOUT_DBUS})
    find_package(Qt${QT_MAJOR}DBus ${QT_EXACT_VERSION} REQUIRED)
    list(APPEND QPDFVIEW_LIBRARIES Qt${QT_MAJOR}::DBus)
    list(APPEND QPDFVIEW_DEFINITIONS -DWITH_DBUS)
    message(STATUS "Building with dbus")
else()
    message(STATUS "Building without dbus")
endif()

# WITHOUT_CUPS
if(NOT ${WITHOUT_CUPS})
    add_definitions("-DWITH_CUPS")
    execute_process(COMMAND cups-config --version
                    OUTPUT_VARIABLE CUPS_VERSION
                    OUTPUT_STRIP_TRAILING_WHITESPACE)
    execute_process(COMMAND cups-config --libs
                    OUTPUT_VARIABLE CUPS_LIBRARIES
                    OUTPUT_STRIP_TRAILING_WHITESPACE)
    string(REGEX REPLACE "-l" "" CUPS_LIBRARIES ${CUPS_LIBRARIES})

    list(APPEND QPDFVIEW_LIBRARIES ${CUPS_LIBRARIES})
    list(APPEND QPDFVIEW_DEFINITIONS -DWITH_DBUS -DCUPS_VERSION="${CUPS_VERSION}")
    message(STATUS "Building with cups")
else()
    message(STATUS "Building without cups")
endif()

# WITHOUT_SYNCTEX
if(NOT ${WITHOUT_SYNCTEX})
    list(APPEND QPDFVIEW_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/synctex")
    list(APPEND QPDFVIEW_LIBRARIES "z")
    list(PREPEND QPDFVIEW_SOURCES
         synctex/synctex_parser.c
         synctex/synctex_parser_utils.c)
    list(APPEND QPDFVIEW_DEFINITIONS -DWITH_SYNCTEX)
    qp_status("Building with synctex")
else()
    qp_status("Building without synctex")
endif()

# WITHOUT_SIGNALS
if(NOT ${WITHOUT_SIGNALS})
    list(PREPEND QPDFVIEW_SOURCES
         ${QPDFVIEW_SOURCE_DIR}/signalhandler.cpp)
    list(APPEND QPDFVIEW_DEFINITIONS -DWITH_SIGNALS)
    qp_status("Building with signals")
else()
    qp_status("Building without signals")
endif()

if(NOT ${WITHOUT_PKGCONFIG})
    include(FindPkgConfig)
    qp_status("Building with pkgconfig")
else()
    qp_status("Building without pkgconfig")
endif()

# PLUGIN_RESOLVE_ALL
if(${PLUGIN_RESOLVE_ALL})
    list(APPEND QPDFVIEW_DEFINITIONS -DPLUGIN_RESOLVE_ALL)
endif()

## Plugins
# WITHOUT_PDF
if(NOT ${WITHOUT_PDF})
    qp_check_plugin(pdf_plugin
                    MIMES QPDFVIEW_MIME_TYPES
                    DEPENDS QPDFVIEW_DEPENDENCIES)
    list(APPEND QPDFVIEW_PLUGINS "pdf")
else()
    qp_status("Building without pdf")
endif()

# WITHOUT_PS
if(NOT ${WITHOUT_PS})
    qp_check_plugin(ps_plugin
                    MIMES QPDFVIEW_MIME_TYPES
                    DEPENDS QPDFVIEW_DEPENDENCIES)
    list(APPEND QPDFVIEW_PLUGINS "ps")
else()
    qp_status("Building without ps")
endif()

# WITHOUT_DJVU
if(NOT ${WITHOUT_DJVU})
    qp_check_plugin(djvu_plugin
                    MIMES QPDFVIEW_MIME_TYPES
                    DEPENDS QPDFVIEW_DEPENDENCIES)
    list(APPEND QPDFVIEW_PLUGINS "djvu")
else()
    qp_status("Building without djvu")
endif()

# WITH_FITZ
if(${WITH_FITZ})
    qp_check_plugin(fitz_plugin
                    MIMES QPDFVIEW_MIME_TYPES
                    DEPENDS QPDFVIEW_DEPENDENCIES)
    list(APPEND QPDFVIEW_PLUGINS "epub" "fb2" "cbz")

else()
    qp_status("Building without fitz")
endif()

# WITHOUT_IMAGE
if(NOT ${WITHOUT_IMAGE})
    qp_check_plugin(image_plugin
                    MIMES QPDFVIEW_MIME_TYPES
                    DEPENDS QPDFVIEW_DEPENDENCIES)

else()
    qp_status("Building without image")
endif()

add_executable(qpdfview ${QPDFVIEW_SOURCES} ${RESOURCES})
target_compile_definitions(qpdfview PUBLIC ${QPDFVIEW_DEFINITIONS})
target_include_directories(qpdfview PUBLIC ${QPDFVIEW_INCLUDE_DIR})
target_link_libraries(qpdfview PUBLIC ${QPDFVIEW_LIBRARIES})
set_target_properties(qpdfview
                      PROPERTIES
                      POSITION_INDEPENDENT_CODE OFF
                      INSTALL_RPATH ${PLUGIN_INSTALL_PATH}
                      AUTOGEN_BUILD_DIR ${CMAKE_BINARY_DIR}/moc)

foreach(dependency ${QPDFVIEW_DEPENDENCIES})
    add_dependencies(qpdfview ${dependency})
endforeach()

file(READ "${DESKTOP_FILE}.in" CONTENTS)

foreach(var "${QPDFVIEW_PLUGINS}")
    string(APPEND __output "${var};")
endforeach()
string(REPLACE "PLUGINS" "${__output}" CONTENTS "${CONTENTS}")
unset(__output)

foreach(var "${QPDFVIEW_MIME_TYPES}")
    string(APPEND __output "${var};")
endforeach()
string(REPLACE "MIME_TYPES" "${__output}" CONTENTS "${CONTENTS}")
file(WRITE "${DESKTOP_FILE}" "${CONTENTS}")

install(TARGETS qpdfview RUNTIME
        DESTINATION ${TARGET_INSTALL_PATH}
        PERMISSIONS
        OWNER_READ OWNER_WRITE
        GROUP_READ GROUP_EXECUTE
        WORLD_READ WORLD_EXECUTE
        INCLUDES ${PLUGIN_INSTALL_PATH})

install(DIRECTORY
        ${CMAKE_CURRENT_LIST_DIR}/help
        ${CMAKE_CURRENT_LIST_DIR}/translations
        DESTINATION ${DATA_INSTALL_PATH})

install(FILES
        ${CMAKE_CURRENT_LIST_DIR}/icons/qpdfview.svg
        DESTINATION ${ICON_INSTALL_PATH})

install(FILES
        ${DESKTOP_FILE}
        DESTINATION ${LAUNCHER_INSTALL_PATH}
        PERMISSIONS
        OWNER_READ OWNER_WRITE
        GROUP_READ
        WORLD_READ
        )

if(${MANUAL_FILE})
install(FILES
        ${MANUAL_FILE}
        DESTINATION ${MANUAL_INSTALL_PATH}
        PERMISSIONS WORLD_READ)
endif()

install(FILES
        ${APPDATA_FILE}
        DESTINATION ${DATA_INSTALL_PATH}
        PERMISSIONS WORLD_READ)
