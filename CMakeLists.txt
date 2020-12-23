#
# Config Provisioning Tool
#
# Copyright (C) 2019-2020, Hensoldt Cyber GmbH
#

cmake_minimum_required(VERSION 3.10)

#-------------------------------------------------------------------------------
# configure and use the SDK

set(OS_SDK_PASSIVE_CMAKE ON)
if (NOT OS_SDK_PATH)
    message(FATAL_ERROR "OS_SDK_PATH not set")
endif()
if (NOT EXISTS "${OS_SDK_PATH}")
    message(FATAL_ERROR "OS_SDK_PATH not found: ${OS_SDK_PATH}")
endif()
include("${OS_SDK_PATH}/CMakeLists.txt")


os_set_config_file("config.h" CREATE_CONFIG_PROJECT)

add_subdirectory("${OS_SDK_DIR}/os_core_api" "os_core_api")
add_os_libs_subdirectories()
add_subdirectory(src/lib)


#-------------------------------------------------------------------------------
project(cpt C)

add_executable(${PROJECT_NAME}
    src/ConfigTool.c
)

find_package(LibXml2 REQUIRED)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        src/include
        ${LIBXML2_INCLUDE_DIR}
)

target_compile_options(${PROJECT_NAME}
    PUBLIC
        -Wall
        -Werror
)

target_compile_definitions(${PROJECT_NAME}
    PRIVATE
        OS_CONFIG_SERVICE_BACKEND_FILESYSTEM
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        os_core_api
        lib_debug
        lib_host
        os_configuration
        os_filesystem
        cpt_lib
        ${LIBXML2_LIBRARIES}
)
