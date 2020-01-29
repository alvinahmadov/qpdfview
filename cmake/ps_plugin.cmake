set(PS_TARGET qpdfview_ps)

set(PS_TARGET_SHORT qpdfps)

set(PS_OBJECTS_DIR objects-ps)

set(PS_MOC_DIR moc-ps)

set(PS_PLUGIN_HEADERS
    ${QPDFVIEW_SOURCE_DIR}/global.h
    ${QPDFVIEW_SOURCE_DIR}/model.h
    ${QPDFVIEW_SOURCE_DIR}/psmodel.h)

set(PS_PLUGIN_SOURCES
    ${QPDFVIEW_SOURCE_DIR}/psmodel.cpp)

set(PS_PLUGIN_DEFINITIONS)

set(PS_PLUGIN_MIMETYPES
    application/postscript)

find_package(Qt${QT_MAJOR}Gui ${QT_EXACT_VERSION} REQUIRED)

if(${QT_MAJOR} GREATER 4)
    find_package(Qt${QT_MAJOR}Widgets ${QT_EXACT_VERSION} REQUIRED)
endif()

set(PS_PLUGIN_INCLUDEPATH ${QPDFVIEW_SOURCE_DIR})

set(PS_PLUGIN_LIBS
    Qt${QT_MAJOR}::Core
    Qt${QT_MAJOR}::Gui)

if(${QT_MAJOR} GREATER 4)
    list(APPEND PS_PLUGIN_LIBS Qt${QT_MAJOR}::Widgets)
endif()

# include library using pkgconfig
if(NOT ${WITHOUT_PKGCONFIG})
    pkg_check_modules(LIBSPECTRE libspectre QUIET)

    if(${LIBSPECTRE_FOUND})
        set(REQUIRED_FOUND TRUE)
        list(APPEND PS_PLUGIN_INCLUDEPATH   ${LIBSPECTRE_INCLUDE_DIRS})
        list(APPEND PS_PLUGIN_LIBS          ${LIBSPECTRE_LIBRARIES})
        list(APPEND QPDFVIEW_DEFINITIONS    -DLIBSPECTRE_VERSION="${LIBSPECTRE_VERSION}")
    else()
        set(REQUIRED_FOUND FALSE)
    endif()
else()

endif()

if(${REQUIRED_FOUND})
    if(${STATIC_PS_PLUGIN})
        list(APPEND QPDFVIEW_DEFINITIONS
             -DWITH_PS
             -DSTATIC_PS_PLUGIN
             -DPS_PLUGIN_NAME="${STATIC_LIB_PREFIX}${PS_TARGET}${STATIC_LIB_SUFFIX}")
        add_library(${PS_TARGET}
                    STATIC MODULE
                    ${PS_PLUGIN_HEADERS}
                    ${PS_PLUGIN_SOURCES})
    else()
        list(APPEND QPDFVIEW_DEFINITIONS
             -DWITH_PS
             -DPS_PLUGIN_NAME="${SHARED_LIB_PREFIX}${PS_TARGET}${SHARED_LIB_SUFFIX}")
        add_library(${PS_TARGET}
                    SHARED MODULE
                    ${PS_PLUGIN_HEADERS}
                    ${PS_PLUGIN_SOURCES})
    endif()

    target_compile_definitions(${PS_TARGET} PUBLIC ${PS_PLUGIN_DEFINITIONS})
    target_include_directories(${PS_TARGET} PUBLIC ${PS_PLUGIN_INCLUDEPATH})
    target_link_libraries(${PS_TARGET} PUBLIC ${PS_PLUGIN_LIBS})
    set_target_properties(${PS_TARGET}
                          PROPERTIES
                          AUTOGEN_BUILD_DIR ${CMAKE_BINARY_DIR}/${PS_MOC_DIR})

    list(APPEND ADDITIONAL_MAKE_CLEAN_FILES
         ${CMAKE_BINARY_DIR}/${PS_MOC_DIR})

    install(TARGETS ${PS_TARGET} LIBRARY
            DESTINATION ${PLUGIN_INSTALL_PATH})

    if(${STATIC_PS_PLUGIN})
        qp_status("Building with static ps plugin using libspectre, version ${LIBSPECTRE_VERSION}")
    else()
        qp_status("Building with ps plugin using libspectre, version ${LIBSPECTRE_VERSION}")
    endif()
else()
    unset(PS_TARGET)
    unset(PS_PLUGIN_DEFINITIONS)
    unset(PS_PLUGIN_MIMETYPES)
    qp_status("Library libspectre not found. Building without ps plugin")
endif()

