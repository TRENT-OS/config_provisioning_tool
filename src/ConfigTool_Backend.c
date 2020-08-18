/**
 * Module to initialize the filesystem backend
 *
 * Copyright (C) 2019-2020, Hensoldt Cyber GmbH
 *
 */

/* Includes ------------------------------------------------------------------*/
#include "LibDebug/Debug.h"

#include "ConfigTool_Backend.h"


/* Private variables ---------------------------------------------------------*/
extern FakeDataport_t* hostStorage_dp;
static OS_FileSystem_Config_t cfgFs =
{
    .size = OS_FileSystem_STORAGE_MAX,
    .storage = IF_OS_STORAGE_ASSIGN(
        HostStorage,
        hostStorage_dp),
};


/* Exported functions --------------------------------------------------------*/
OS_Error_t ConfigTool_BackendInit(
    OS_FileSystem_Handle_t* hFs,
    OS_FileSystem_Type_t fsType)
{
    // Set the filesystem type specified by the user input
    cfgFs.type = fsType;

    OS_Error_t err = OS_FileSystem_init(hFs, &cfgFs);
    if (err != OS_SUCCESS)
    {
        Debug_LOG_ERROR("OS_FileSystem_init() failed with %d.", err);
        return err;
    }

    err = OS_FileSystem_format(*hFs);
    if (err != OS_SUCCESS)
    {
        Debug_LOG_ERROR("OS_FileSystem_format() failed with %d.", err);
        return err;
    }

    err = OS_FileSystem_mount(*hFs);
    if (err != OS_SUCCESS)
    {
        Debug_LOG_ERROR("OS_FileSystem_mount() failed with %d.", err);
        return err;
    }

    return OS_SUCCESS;
}

OS_Error_t ConfigTool_BackendDeInit(
    OS_FileSystem_Handle_t hFs)
{
    OS_Error_t err = OS_FileSystem_unmount(hFs);
    if (err != OS_SUCCESS)
    {
        Debug_LOG_ERROR("OS_FileSystem_mount() failed with %d.", err);
        return err;
    }

    err = OS_FileSystem_free(hFs);
    if (err != OS_SUCCESS)
    {
        Debug_LOG_ERROR("OS_FileSystem_mount() failed with %d.", err);
        return err;
    }

    return OS_SUCCESS;
}
