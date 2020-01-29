set(IMAGE_TARGET qpdfview_image)

set(IMAGE_TARGET_SHORT qpdfimg)

set(IMAGE_OBJECTS_DIR objects-image)

set(IMAGE_MOC_DIR moc-image)

set(IMAGE_PLUGIN_HEADERS
    ${QPDFVIEW_SOURCE_DIR}/global.h
    ${QPDFVIEW_SOURCE_DIR}/model.h
    ${QPDFVIEW_SOURCE_DIR}/imagemodel.h)

set(IMAGE_PLUGIN_SOURCES
    ${QPDFVIEW_SOURCE_DIR}/imagemodel.cpp)

set(IMAGE_PLUGIN_MIMETYPES)

find_package(Qt${QT_MAJOR} ${QT_EXACT_VERSION} COMPONENTS Core Gui REQUIRED)

set(IMAGE_PLUGIN_DEFINITIONS)

set(IMAGE_PLUGIN_INCLUDEPATH
    ${QPDFVIEW_SOURCE_DIR})

set(IMAGE_PLUGIN_LIBS
    Qt${QT_MAJOR}::Core
    Qt${QT_MAJOR}::Gui)

if(${QT_MAJOR} GREATER 4)
    list(APPEND IMAGE_PLUGIN_LIBS Qt${QT_MAJOR}::Widgets)
endif()

    if(${STATIC_IMAGE_PLUGIN})
        list(APPEND QPDFVIEW_DEFINITIONS
             -DWITH_IMAGE
             -DSTATIC_IMAGE_PLUGIN
             -DIMAGE_PLUGIN_NAME="${STATIC_LIB_PREFIX}${IMAGE_TARGET}${STATIC_LIB_SUFFIX}")
        add_library(${IMAGE_TARGET}
                    STATIC MODULE
                    ${IMAGE_PLUGIN_SOURCES}
                    ${IMAGE_PLUGIN_HEADERS})
    else()
        list(APPEND QPDFVIEW_DEFINITIONS
             -DWITH_IMAGE
             -DIMAGE_PLUGIN_NAME="${SHARED_LIB_PREFIX}${IMAGE_TARGET}${SHARED_LIB_SUFFIX}")
        add_library(${IMAGE_TARGET}
                    SHARED MODULE
                    ${IMAGE_PLUGIN_SOURCES}
                    ${IMAGE_PLUGIN_HEADERS})
    endif()

    target_compile_definitions(${IMAGE_TARGET} PUBLIC ${IMAGE_PLUGIN_DEFINITIONS})
    target_include_directories(${IMAGE_TARGET} PUBLIC ${IMAGE_PLUGIN_INCLUDEPATH})
    target_link_libraries(${IMAGE_TARGET} PUBLIC ${IMAGE_PLUGIN_LIBS})
    set_target_properties(${IMAGE_TARGET}
                          PROPERTIES
                          AUTOGEN_BUILD_DIR ${CMAKE_BINARY_DIR}/${IMAGE_MOC_DIR})

    list(APPEND ADDITIONAL_MAKE_CLEAN_FILES
         ${CMAKE_BINARY_DIR}/${IMAGE_MOC_DIR})
    install(TARGETS ${IMAGE_TARGET} LIBRARY DESTINATION ${PLUGIN_INSTALL_PATH})

if(${STATIC_IMAGE_PLUGIN})
    qp_status("Building with static image plugin")
else()
    qp_status("Building with image plugin")
endif()
