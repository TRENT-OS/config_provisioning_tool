#
# Config Provisioning Tool library
#
# Copyright (C) 2019-2020, Hensoldt Cyber GmbH
#

cmake_minimum_required(VERSION 3.13.0)

# enable new behavior introduced in CMake 3.13, where relative paths will be
# converted into absolute paths in target_sources(INTERFACE ....)
cmake_policy(SET CMP0076 NEW)


#-------------------------------------------------------------------------------
project(cpt_lib C)

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME}
    INTERFACE
        include
)

target_sources(${PROJECT_NAME}
    INTERFACE
        src/ConfigTool_Backend.c
        src/ConfigTool_ConfigService.c
        src/ConfigTool_HostFs.c
        src/ConfigTool_HostFsFile.c
        src/ConfigTool_Util.c
        src/ConfigTool_XmlParser.c
)
