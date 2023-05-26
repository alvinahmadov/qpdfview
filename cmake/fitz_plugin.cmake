set(FITZ_TARGET qpdfview_fitz)

set(FITZ_TARGET_SHORT qpdffitz)

set(FITZ_OBJECTS_DIR objects-fitz)

set(FITZ_MOC_DIR moc-fitz)

set(FITZ_PLUGIN_HEADERS
    ${QPDFVIEW_SOURCE_DIR}/model.h
    ${QPDFVIEW_SOURCE_DIR}/fitzmodel.h)

set(FITZ_PLUGIN_SOURCES
    ${QPDFVIEW_SOURCE_DIR}/fitzmodel.cpp)

set(FITZ_PLUGIN_MIMETYPES
    application/epub+zip
    application/oxps
    application/xps
    application/x-fictionbook+xml
    application/vnd.comicbook+zip)

find_package(Qt${QT_MAJOR} ${QT_EXACT_VERSION} COMPONENTS Core Gui REQUIRED)

set(FITZ_PLUGIN_DEFINITIONS)

set(FITZ_PLUGIN_INCLUDEPATH
    ${QPDFVIEW_SOURCE_DIR})

set(FITZ_PLUGIN_LIBS
    Qt${QT_MAJOR}::Core
    Qt${QT_MAJOR}::Gui)

if(${QT_MAJOR} GREATER 4)
    list(APPEND FITZ_PLUGIN_LIBS Qt${QT_MAJOR}::Widgets)
endif()

# include library usign pkgconfig
if(NOT ${WITHOUT_PKGCONFIG})
    pkg_check_modules(MUPDF mupdf)
    pkg_check_modules(JPEG libjpeg)

    if(${MUPDF_FOUND})
        set(REQUIRED_FOUND TRUE)
        list(APPEND FITZ_PLUGIN_INCLUDEPATH ${MUPDF_INCLUDE_DIRS})
        list(APPEND QPDFVIEW_DEFINITIONS    -DFITZ_VERSION="${MUPDF_VERSION}")
        list(APPEND FITZ_PLUGIN_LIBS        ${MUPDF_LIBRARIES} ${JPEG_LIBRARIES})
    else()
        set(REQUIRED_FOUND FALSE)
    endif()
endif()

if(${REQUIRED_FOUND})
    if(${STATIC_FITZ_PLUGIN})
        list(APPEND QPDFVIEW_DEFINITIONS
             -DWITH_FITZ
             -DSTATIC_FITZ_PLUGIN
             -DFITZ_PLUGIN_NAME="${STATIC_LIB_PREFIX}${FITZ_TARGET}${STATIC_LIB_SUFFIX}")
        add_library(${FITZ_TARGET}
                    STATIC MODULE
                    ${FITZ_PLUGIN_HEADERS}
                    ${FITZ_PLUGIN_SOURCES})
    else()
        list(APPEND QPDFVIEW_DEFINITIONS
             -DWITH_FITZ
             -DFITZ_PLUGIN_NAME="${SHARED_LIB_PREFIX}${FITZ_TARGET}${SHARED_LIB_SUFFIX}")
        add_library(${FITZ_TARGET}
                    SHARED MODULE
                    ${FITZ_PLUGIN_HEADERS}
                    ${FITZ_PLUGIN_SOURCES})
    endif()

    target_include_directories(${FITZ_TARGET} PUBLIC ${FITZ_PLUGIN_INCLUDEPATH})
    target_link_libraries(${FITZ_TARGET} PUBLIC ${FITZ_PLUGIN_LIBS})
    set_target_properties(${FITZ_TARGET}
                          PROPERTIES
                          AUTOGEN_BUILD_DIR ${CMAKE_BINARY_DIR}/${FITZ_MOC_DIR})

    list(APPEND ADDITIONAL_MAKE_CLEAN_FILES
         ${CMAKE_BINARY_DIR}/${FITZ_MOC_DIR})

    install(TARGETS ${FITZ_TARGET} LIBRARY
            DESTINATION ${PLUGIN_INSTALL_PATH})

    if(${STATIC_FITZ_PLUGIN})
        qp_status("Building with static fitz plugin using mupdf, version ${MUPDF_VERSION}")
    else()
        qp_status("Building with fitz plugin using mupdf, version ${MUPDF_VERSION}")
    endif()
else()
    unset(FITZ_TARGET)
    unset(FITZ_PLUGIN_DEFINITIONS)
    unset(FITZ_PLUGIN_MIMETYPES)
    qp_status("Library mupdf not found. Building without fitz plugin")
endif()

