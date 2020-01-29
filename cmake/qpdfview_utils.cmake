macro(qp_check_environment_variables)
    foreach(_var ${ARGN})
        if("${${_var}}" STREQUAL " " AND DEFINED ENV{${_var}})
            set(__value "$ENV{${_var}}")
            file(TO_CMAKE_PATH "${__value}" __value)
            set(${_var} "${__value}")
            qp_status("Update variable ${_var} from environment: ${${_var}}")
        endif()
    endforeach()
endmacro()

macro(__qp_configure_plugin_vars config_name dependencies mimetypes)
    string(TOLOWER ${config_name} __plugin_config)
    string(FIND ${config_name} ".cmake" __cmake_config_ext)
    if(${__cmake_config_ext} EQUAL -1)
        include(${QPDFVIEW_CMAKE_DIR}/${__plugin_config}.cmake)
    else()
        include(${QPDFVIEW_CMAKE_DIR}/${__plugin_config})
    endif()

    set(${__plugin}_MIMETYPES "")

    string(TOUPPER ${__plugin_config} __plugin)
    string(REPLACE "PLUGIN" "TARGET" __target ${__plugin})

    set(${dependencies} ${${__target}})
    set(${mimetypes} ${${__plugin}_MIMETYPES})
endmacro()

macro(qp_check_plugin plugin_name)
    set(__depend_key "DEPENDS")
    set(__mimes_key "MIMES")

    set(__active_value "")
    set(__deps "")
    set(__mimes "")

    foreach(__arg ${ARGN})
        if(${__arg} STREQUAL ${__mimes_key})
            set(__active_value __mimes)
        elseif(${__arg} STREQUAL ${__depend_key})
            set(__active_value __deps)
        else()
            list(APPEND ${__active_value} ${__arg})
        endif()
    endforeach()

    unset(__active_value)
    unset(__depend_key)
    unset(__mimes_key)

    __qp_configure_plugin_vars(${plugin_name} __depend_key __mimes_key)

    list(APPEND ${__mimes} ${__mimes_key})
    list(APPEND ${__deps} ${__depend_key})
endmacro()


# Provides an option that the user can optionally select.
# Can accept condition to control when option is available for user.
# Usage:
#   option(<option_variable>
#          "help string describing the option"
#          <initial value or boolean expression>)
macro(qp_dependent_option variable description value)
    set(__value ${value})
    set(__condition "")
    set(__varname "__value")
    foreach(arg ${ARGN})
        if(${arg} STREQUAL "IF" OR ${arg} STREQUAL "if")
            set(__varname "__condition")
        else()
            list(APPEND ${__varname} ${arg})
        endif()
    endforeach()
    unset(__varname)
    if(__condition STREQUAL "")
        set(__condition 2 GREATER 1)
    endif()

    if(${__condition})
        if(__value MATCHES ";")
            if(${__value})
                option(${variable} "${description}" ON)
            else()
                option(${variable} "${description}" OFF)
            endif()
        elseif(DEFINED ${__value})
            if(${__value})
                option(${variable} "${description}" ON)
            else()
                option(${variable} "${description}" OFF)
            endif()
        else()
            option(${variable} "${description}" ${__value})
        endif()
    else()
        if(DEFINED ${variable} AND "${${variable}}")
            message(WARNING "Unexpected option: ${variable} (=${${variable}})\nCondition: IF (${__condition})")
        endif()
        if(UNSET_UNSUPPORTED_OPTION)
            unset(${variable} CACHE)
        endif()
    endif()
    unset(__condition)
    unset(__value)
endmacro()


macro(qp_find_library_from_system)
    set(options CHECK_VERSION)
    set(oneValueArgs
        PREFIX
        COMMAND
        LIBRARY
        TARGET
        DEFINITIONS)
    set(multiValueArgs NONE)

    cmake_parse_arguments(qp "${options}" "${oneValueArgs}"
                          "${multiValueArgs}" ${ARGN})

    set(__libraries ${qp_LIBRARIES})
    if("${qp_COMMAND}" STREQUAL "pkg-config")
        set(__version_command "--modversion")
    else()
        set(__version_command "--version")
    endif()
    set(__command ${qp_COMMAND})
    set(__pkg_prefix ${qp_PREFIX})

    if(${qp_CHECK_VERSION})
        execute_process(COMMAND ${__command} ${qp_LIBRARY}
                        ${__version_command}
                        OUTPUT_VARIABLE __version
                        OUTPUT_STRIP_TRAILING_WHITESPACE)
    endif()

    execute_process(COMMAND ${__command} --libs ${qp_LIBRARY}
                    OUTPUT_VARIABLE __libraries
                    OUTPUT_STRIP_TRAILING_WHITESPACE)

    string(REGEX REPLACE "-l" "" __libraries ${__libraries})

    set(qp_VERSION ${__version})
    list(APPEND qp_TARGET ${__libraries})
    list(APPEND qp_DEFINITIONS
         -D${__pkg_prefix}_VERSION="${__version}"
         )
    message("Version ${__libraries}")
endmacro()

macro(qp_find_package_if_matches)
    # qp_find_package_if_matches(Qt EXACT_VERSION 5
    #                               COMPONENTS Core Gui
    #                               VERSION GREATER 4 DEPENDENT_COMPONENTS Widgets)
    set(options
        GREATER
        LESS
        EQUAL
        GREATER_EQUAL
        LESS_EQUAL)
    set(oneValueArgs
        LIBRARY_TARGET  # library to append result
        INCLUDE_TARGET  # include_dirs to append result
        VERSION         # find optional component that matches condition
        EXACT_VERSION   # find exactly matching this version in find_package
        )
    set(multiValueArgs
        COMPONENTS      # components to find with find_package
        DEPENDENT_COMPONENTS # if VERSION
        )

    cmake_parse_arguments(qp "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if(${qp_VERSION} STREQUAL "")
        list(APPEND qp_LIBRARY_TARGET
             ${qp_COMPONENTS})

        foreach(component ${qp_COMPONENTS})
            qp_status(${component})
        endforeach()
    endif()

    qp_status(${qp_EXACT_VERSION})
    qp_status(${qp_VERSION})

    foreach(component ${qp_COMPONENTS})
        qp_status(${component})
    endforeach()

    if(NOT ${qp_EXACT_VERSION} STREQUAL " ")
    endif()
endmacro()

function(qp_status message)
    if("${DISABLE_STATUS}" STREQUAL "")
        set(DISABLE_STATUS OFF)
    endif()
    if (NOT ${DISABLE_STATUS})
        message(STATUS ${message})
    endif()
endfunction()

function(qp_write_file)
    set(options "")
    set(oneValueArgs INPUT OUTPUT)
    set(multiValueArgs MATCH REPLACE)

    cmake_parse_arguments(qp_write_file "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
    set(__output "")

    file(READ "${qp_write_file_INPUT}" __contents)

    foreach(match ${qp_write_file_MATCH})
        foreach(replace ${qp_write_file_REPLACE})
            foreach(repl ${${replace}})
                string(APPEND __output "${repl};")
            endforeach()
#            string(APPEND __output "${replace};")

#            string(REPLACE "${match}" "${__output}" __file_contents "${__contents}")
        endforeach()
    endforeach()
message(STATUS ${__output})
#    foreach(var ${qp_write_file_REPLACE})
#        string(APPEND __output "${var};")
#    endforeach()
#    string(REPLACE "${qp_write_file_MATCH}" "${__output}" __file_contents "${__contents}")
#    file(WRITE ${qp_write_file_OUTPUT} "${__file_contents}")

endfunction()
