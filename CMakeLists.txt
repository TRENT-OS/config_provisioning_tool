#
# Config Provisioning Tool
#
# Copyright (C) 2019-2024, HENSOLDT Cyber GmbH
# 
# SPDX-License-Identifier: GPL-2.0-or-later
#
# For commercial licensing, contact: info.cyber@hensoldt.net
#

cmake_minimum_required(VERSION 3.10)

#-------------------------------------------------------------------------------
# use the SDK
find_package("os-sdk" REQUIRED)
os_sdk_create_config_project("os_sdk_config" "config.h")
os_sdk_import_core_api()
os_sdk_import_libs()


#-------------------------------------------------------------------------------
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
        os_sdk_config
        os_core_api
        lib_debug
        lib_host
        os_configuration
        os_filesystem
        cpt_lib
        ${LIBXML2_LIBRARIES}
)
