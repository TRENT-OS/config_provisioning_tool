/*
 *  Module to initialize the configuration service library
 *
 *  Copyright (C) 2019-2020, Hensoldt Cyber GmbH
 *
 */

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "LibDebug/Debug.h"
#include "ConfigTool_ConfigService.h"
#include "ConfigTool_Util.h"


/* Private functions ---------------------------------------------------------*/
// Create the file backends before initializing them
static
OS_Error_t ConfigTool_ConfigServiceCreateFileBackends(
    OS_FileSystem_Handle_t hFs,
    ConfigTool_ConfigServiceCounter_t* configCounter)
{
    OS_ConfigServiceBackend_FileName_t name = {0};

    ConfigTool_UtilInitializeName(
        name.buffer,
        OS_CONFIG_BACKEND_MAX_FILE_NAME_LEN,
        DOMAIN_FILE);
    OS_Error_t err = OS_ConfigServiceBackend_createFileBackend(
                         name,
                         hFs,
                         configCounter->domain_count,
                         sizeof(OS_ConfigServiceLibTypes_Domain_t));

    if (err != OS_SUCCESS)
    {
        Debug_LOG_ERROR("Failed to create %s file", DOMAIN_FILE);
        return err;
    }

    ConfigTool_UtilInitializeName(
        name.buffer,
        OS_CONFIG_BACKEND_MAX_FILE_NAME_LEN,
        PARAMETER_FILE);
    err = OS_ConfigServiceBackend_createFileBackend(
              name,
              hFs,
              configCounter->param_count,
              sizeof(OS_ConfigServiceLibTypes_Parameter_t));
    if (err != OS_SUCCESS)
    {
        Debug_LOG_ERROR("Failed to create %s file", PARAMETER_FILE);
        return err;
    }

    ConfigTool_UtilInitializeName(
        name.buffer,
        OS_CONFIG_BACKEND_MAX_FILE_NAME_LEN,
        STRING_FILE);
    err = OS_ConfigServiceBackend_createFileBackend(
              name,
              hFs,
              configCounter->string_count,
              OS_CONFIG_LIB_PARAMETER_MAX_STRING_LENGTH);
    if (err != OS_SUCCESS)
    {
        Debug_LOG_ERROR("Failed to create %s file", STRING_FILE);
        return err;
    }

    ConfigTool_UtilInitializeName(
        name.buffer,
        OS_CONFIG_BACKEND_MAX_FILE_NAME_LEN,
        BLOB_FILE);
    err = OS_ConfigServiceBackend_createFileBackend(
              name,
              hFs,
              configCounter->blob_count,
              OS_CONFIG_LIB_PARAMETER_MAX_BLOB_BLOCK_LENGTH);
    if (err != OS_SUCCESS)
    {
        Debug_LOG_ERROR("Failed to create %s file", BLOB_FILE);
        return err;
    }

    Debug_LOG_DEBUG("Config Backend Files successfully created");
    return OS_SUCCESS;
}

static
OS_Error_t ConfigTool_ConfigServiceInitBackends(
    OS_ConfigServiceLib_t* configLib,
    OS_FileSystem_Handle_t hFs)
{
    OS_ConfigServiceBackend_t parameterBackend;
    OS_ConfigServiceBackend_t domainBackend;
    OS_ConfigServiceBackend_t stringBackend;
    OS_ConfigServiceBackend_t blobBackend;
    OS_ConfigServiceBackend_FileName_t name;

    // Initialize the backends in the config library object.
    ConfigTool_UtilInitializeName(
        name.buffer,
        OS_CONFIG_BACKEND_MAX_FILE_NAME_LEN,
        DOMAIN_FILE);
    OS_Error_t err = OS_ConfigServiceBackend_initializeFileBackend(
                         &domainBackend,
                         name,
                         hFs);
    if (err != OS_SUCCESS)
    {
        Debug_LOG_ERROR("Failed to initialize %s", DOMAIN_FILE);
        return err;
    }

    ConfigTool_UtilInitializeName(
        name.buffer,
        OS_CONFIG_BACKEND_MAX_FILE_NAME_LEN,
        PARAMETER_FILE);
    err = OS_ConfigServiceBackend_initializeFileBackend(
              &parameterBackend,
              name,
              hFs);
    if (err != OS_SUCCESS)
    {
        Debug_LOG_ERROR("Failed to initialize %s", PARAMETER_FILE);
        return err;
    }

    ConfigTool_UtilInitializeName(
        name.buffer,
        OS_CONFIG_BACKEND_MAX_FILE_NAME_LEN,
        STRING_FILE);
    err = OS_ConfigServiceBackend_initializeFileBackend(
              &stringBackend,
              name,
              hFs);
    if (err != OS_SUCCESS)
    {
        Debug_LOG_ERROR("Failed to initialize %s", STRING_FILE);
        return err;
    }

    ConfigTool_UtilInitializeName(
        name.buffer,
        OS_CONFIG_BACKEND_MAX_FILE_NAME_LEN,
        BLOB_FILE);
    err = OS_ConfigServiceBackend_initializeFileBackend(
              &blobBackend,
              name,
              hFs);
    if (err != OS_SUCCESS)
    {
        Debug_LOG_ERROR("Failed to initialize %s", BLOB_FILE);
        return err;
    }

    err = OS_ConfigServiceLib_Init(
              configLib,
              &parameterBackend,
              &domainBackend,
              &stringBackend,
              &blobBackend);
    if (err != OS_SUCCESS)
    {
        Debug_LOG_ERROR("OS_ConfigServiceLib_Init() failed with %d", err);
        return err;
    }


    Debug_LOG_DEBUG("Config Backend initialized successfully");
    return OS_SUCCESS;
}


/* Exported functions --------------------------------------------------------*/
OS_Error_t ConfigTool_ConfigServiceInit(
    OS_ConfigServiceLib_t* configLib,
    OS_FileSystem_Handle_t hFs,
    ConfigTool_ConfigServiceCounter_t* configCounter)
{
    OS_Error_t err = ConfigTool_ConfigServiceCreateFileBackends(
                         hFs,
                         configCounter);
    if (err != OS_SUCCESS)
    {
        Debug_LOG_ERROR("ConfigTool_ConfigServiceCreateFileBackends() failed with %d",
                        err);
        return err;
    }

    err = ConfigTool_ConfigServiceInitBackends(configLib, hFs);
    if (err != OS_SUCCESS)
    {
        Debug_LOG_ERROR("ConfigTool_ConfigServiceInitBackends() failed with %d", err);
        return err;
    }

    return OS_SUCCESS;
}
