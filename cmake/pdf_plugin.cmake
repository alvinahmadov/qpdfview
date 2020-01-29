set(PDF_TARGET qpdfview_pdf)

set(PDF_TARGET_SHORT qpdfpdf)

set(PDF_OBJECTS_DIR objects-pdf)

set(PDF_MOC_DIR moc-pdf)

set(PDF_PLUGIN_HEADERS
    ${QPDFVIEW_SOURCE_DIR}/global.h
    ${QPDFVIEW_SOURCE_DIR}/model.h
    ${QPDFVIEW_SOURCE_DIR}/pdfmodel.h
    ${QPDFVIEW_SOURCE_DIR}/annotationwidgets.h
    ${QPDFVIEW_SOURCE_DIR}/formfieldwidgets.h)

set(PDF_PLUGIN_SOURCES
    ${QPDFVIEW_SOURCE_DIR}/pdfmodel.cpp
    ${QPDFVIEW_SOURCE_DIR}/annotationwidgets.cpp
    ${QPDFVIEW_SOURCE_DIR}/formfieldwidgets.cpp)

set(PDF_PLUGIN_MIMETYPES
    application/pdf
    application/x-pdf
    text/pdf
    text/x-pdf
    image/pdf
    image/x-pdf)

find_package(Qt${QT_MAJOR} ${QT_EXACT_VERSION} COMPONENTS Core Xml Gui REQUIRED)

set(PDF_PLUGIN_DEFINITIONS)

set(PDF_PLUGIN_INCLUDEPATH
    ${QPDFVIEW_SOURCE_DIR})

set(PDF_PLUGIN_LIBS
    Qt${QT_MAJOR}::Core
    Qt${QT_MAJOR}::Xml
    Qt${QT_MAJOR}::Gui)

if(${QT_MAJOR} GREATER 4)
    list(APPEND PDF_PLUGIN_LIBS Qt${QT_MAJOR}::Widgets)
endif()

# include library using pkgconfig
if(NOT ${WITHOUT_PKGCONFIG})
    pkg_check_modules(POPPLER poppler-qt${QT_MAJOR} QUIET)
    if(${POPPLER_FOUND})
        set(REQUIRED_FOUND TRUE)
        list(APPEND PDF_PLUGIN_INCLUDEPATH ${POPPLER_INCLUDE_DIRS})
        list(APPEND QPDFVIEW_DEFINITIONS    -DPOPPLER_VERSION="${POPPLER_VERSION}")
        list(APPEND PDF_PLUGIN_LIBS         ${POPPLER_LIBRARIES})
        if(${POPPLER_VERSION} VERSION_GREATER_EQUAL 0.14)
            list(APPEND PDF_PLUGIN_DEFINITIONS -DHAS_POPPLER_14)
        endif()
        if(${POPPLER_VERSION} VERSION_GREATER_EQUAL 0.18)
            list(APPEND PDF_PLUGIN_DEFINITIONS -DHAS_POPPLER_18)
        endif()
        if(${POPPLER_VERSION} VERSION_GREATER_EQUAL 0.20.1)
            list(APPEND PDF_PLUGIN_DEFINITIONS -DHAS_POPPLER_20)
        endif()
        if(${POPPLER_VERSION} VERSION_GREATER_EQUAL 0.22)
            list(APPEND PDF_PLUGIN_DEFINITIONS -DHAS_POPPLER_22)
        endif()
        if(${POPPLER_VERSION} VERSION_GREATER_EQUAL 0.24)
            list(APPEND PDF_PLUGIN_DEFINITIONS -DHAS_POPPLER_24)
        endif()
        if(${POPPLER_VERSION} VERSION_GREATER_EQUAL 0.26)
            list(APPEND PDF_PLUGIN_DEFINITIONS -DHAS_POPPLER_26)
        endif()
        if(${POPPLER_VERSION} VERSION_GREATER_EQUAL 0.31)
            list(APPEND PDF_PLUGIN_DEFINITIONS -DHAS_POPPLER_31)
        endif()
        if(${POPPLER_VERSION} VERSION_GREATER_EQUAL 0.35)
            list(APPEND PDF_PLUGIN_DEFINITIONS -DHAS_POPPLER_35)
        endif()
    else()
        set(REQUIRED_FOUND FALSE)
    endif()
endif()

if(${REQUIRED_FOUND})
    if(${STATIC_PDF_PLUGIN})
        list(APPEND QPDFVIEW_DEFINITIONS
             -DWITH_PDF
             -DSTATIC_PDF_PLUGIN
             -DPDF_PLUGIN_NAME="${STATIC_LIB_PREFIX}${PDF_TARGET}${STATIC_LIB_SUFFIX}")
        add_library(${PDF_TARGET}
                    STATIC MODULE
                    ${PDF_PLUGIN_HEADERS}
                    ${PDF_PLUGIN_SOURCES})
    else()
        list(APPEND QPDFVIEW_DEFINITIONS
             -DWITH_PDF
             -DPDF_PLUGIN_NAME="${SHARED_LIB_PREFIX}${PDF_TARGET}${SHARED_LIB_SUFFIX}")
        add_library(${PDF_TARGET}
                    SHARED MODULE
                    ${PDF_PLUGIN_HEADERS}
                    ${PDF_PLUGIN_SOURCES})
    endif()

    target_compile_definitions(${PDF_TARGET} PUBLIC ${PDF_PLUGIN_DEFINITIONS})
    target_include_directories(${PDF_TARGET} PUBLIC ${PDF_PLUGIN_INCLUDEPATH})
    target_link_libraries(${PDF_TARGET} PUBLIC ${PDF_PLUGIN_LIBS})
    set_target_properties(${PDF_TARGET}
                          PROPERTIES
                          AUTOGEN_BUILD_DIR ${CMAKE_BINARY_DIR}/${PDF_MOC_DIR})

    list(APPEND ADDITIONAL_MAKE_CLEAN_FILES
         ${CMAKE_BINARY_DIR}/${PDF_MOC_DIR})

    install(TARGETS ${PDF_TARGET} LIBRARY
            DESTINATION ${PLUGIN_INSTALL_PATH})

    if(${STATIC_PDF_PLUGIN})
        qp_status("Building with static pdf plugin using poppler-qt${QT_MAJOR}, version ${POPPLER_VERSION}")
    else()
        qp_status("Building with pdf plugin using poppler-qt${QT_MAJOR}, version ${POPPLER_VERSION}")
    endif()
else()
    unset(PDF_TARGET)
    unset(PDF_PLUGIN_DEFINITIONS)
    unset(PDF_PLUGIN_MIMETYPES)
    qp_status("Library poppler-qt${QT_MAJOR} not found. Building without pdf plugin")
endif()
