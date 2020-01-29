set(DJVU_TARGET qpdfview_djvu)

set(DJVU_TARGET_SHORT qpdfdjvu)

set(DJVU_OBJECTS_DIR objects-djvu)

set(DJVU_MOC_DIR moc-djvu)

set(DJVU_PLUGIN_HEADERS
    ${QPDFVIEW_SOURCE_DIR}/global.h
    ${QPDFVIEW_SOURCE_DIR}/model.h
    ${QPDFVIEW_SOURCE_DIR}/djvumodel.h)

set(DJVU_PLUGIN_SOURCES
    ${QPDFVIEW_SOURCE_DIR}/djvumodel.cpp)

set(DJVU_PLUGIN_MIMETYPES
    image/vnd.djvu
    image/x-djvu
    image/vnd.djvu+multipage)

set(DJVU_PLUGIN_DEFINITIONS "")

find_package(Qt${QT_MAJOR} ${QT_EXACT_VERSION} COMPONENTS Gui REQUIRED)

set(DJVU_PLUGIN_INCLUDEPATH
    ${QPDFVIEW_SOURCE_DIR})

set(DJVU_PLUGIN_LIBS
    Qt${QT_MAJOR}::Core
    Qt${QT_MAJOR}::Gui)

if(${QT_MAJOR} GREATER 4)
    list(APPEND DJVU_PLUGIN_LIBS Qt${QT_MAJOR}::Widgets)
endif()

# include library usign pkgconfig
if(NOT ${WITHOUT_PKGCONFIG})
    pkg_check_modules(DJVULIBRE ddjvuapi QUIET)
    if(${DJVULIBRE_FOUND})
        set(REQUIRED_FOUND TRUE)
        list(APPEND QPDFVIEW_DEFINITIONS    -DDJVULIBRE_VERSION="${DJVULIBRE_VERSION}")
        list(APPEND DJVU_PLUGIN_INCLUDEPATH ${DJVULIBRE_INCLUDE_DIRS})
        list(APPEND DJVU_PLUGIN_LIBS        ${DJVULIBRE_LIBRARIES})
    else()
        set(REQUIRED_FOUND FALSE)
    endif()
endif()

if(${REQUIRED_FOUND})
    if(${STATIC_DJVU_PLUGIN})
        list(APPEND QPDFVIEW_DEFINITIONS
             -DWITH_DJVU
             -DSTATIC_DJVU_PLUGIN
             -DDJVU_PLUGIN_NAME="${STATIC_LIB_PREFIX}${DJVU_TARGET}${STATIC_LIB_SUFFIX}")

        add_library(${DJVU_TARGET}
                    STATIC MODULE
                    ${DJVU_PLUGIN_HEADERS}
                    ${DJVU_PLUGIN_SOURCES})
    else()
        list(APPEND QPDFVIEW_DEFINITIONS
             -DWITH_DJVU
             -DDJVU_PLUGIN_NAME="${SHARED_LIB_PREFIX}${DJVU_TARGET}${SHARED_LIB_SUFFIX}")

        add_library(${DJVU_TARGET}
                    SHARED MODULE
                    ${DJVU_PLUGIN_HEADERS}
                    ${DJVU_PLUGIN_SOURCES})
    endif()

    target_compile_definitions(${DJVU_TARGET} PRIVATE ${DJVU_PLUGIN_DEFINITIONS})
    target_include_directories(${DJVU_TARGET} PUBLIC ${DJVU_PLUGIN_INCLUDEPATH})
    target_link_libraries(${DJVU_TARGET} PUBLIC ${DJVU_PLUGIN_LIBS})
    set_target_properties(${DJVU_TARGET}
                          PROPERTIES
                          AUTOGEN_BUILD_DIR ${CMAKE_BINARY_DIR}/${DJVU_MOC_DIR})

    list(APPEND ADDITIONAL_MAKE_CLEAN_FILES
         ${CMAKE_BINARY_DIR}/${DJVU_MOC_DIR})

    install(TARGETS ${DJVU_TARGET} LIBRARY
            DESTINATION ${PLUGIN_INSTALL_PATH})

    if(${STATIC_DJVU_PLUGIN})
        qp_status("Building with static djvu plugin using djvulibre, version ${DJVULIBRE_VERSION}")
    else()
        qp_status("Building with djvu plugin using djvulibre, version ${DJVULIBRE_VERSION}")
    endif()
else()
    unset(DJVU_TARGET)
    unset(DJVU_PLUGIN_DEFINITIONS)
    unset(DJVU_PLUGIN_MIMETYPES)
    qp_status("Library libdjvulibre not found. Building without djvu plugin")
endif()
