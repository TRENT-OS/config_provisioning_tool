/**
 * Module to initialize the filesystem backend
 *
 * Copyright (C) 2019-2024, HENSOLDT Cyber GmbH
 * 
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * For commercial licensing, contact: info.cyber@hensoldt.net
 */

/* Includes ------------------------------------------------------------------*/
#include "lib_debug/Debug.h"

#include "ConfigTool_Backend.h"


/* Private variables ---------------------------------------------------------*/
extern FakeDataport_t* hostStorage_port;
static OS_FileSystem_Config_t cfgFs =
{
    .size = OS_FileSystem_USE_STORAGE_MAX,
    .storage = IF_OS_STORAGE_ASSIGN(
        HostStorage,
        hostStorage_port),
};


/* Private functions ---------------------------------------------------------*/
static OS_Error_t
ConfigTool_BackendPrepareFileSystem(
    OS_FileSystem_Handle_t* hFs,
    const OS_FileSystem_Config_t* cfgFs)
{
    OS_Error_t err = OS_FileSystem_init(hFs, cfgFs);
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


/* Exported functions --------------------------------------------------------*/
OS_Error_t ConfigTool_BackendInit(
    OS_FileSystem_Handle_t* hFs,
    OS_FileSystem_Type_t fsType)
{
    OS_Error_t err;

    // Set the filesystem type specified by the user input
    cfgFs.type = fsType;

    switch (cfgFs.type)
    {
    case OS_FileSystem_Type_FATFS:
        __attribute__ ((fallthrough));
    case OS_FileSystem_Type_LITTLEFS:
        __attribute__ ((fallthrough));
    case OS_FileSystem_Type_SPIFFS:
        err = ConfigTool_BackendPrepareFileSystem(hFs, &cfgFs);
        if (err != OS_SUCCESS)
        {
            Debug_LOG_ERROR("ConfigTool_BackendPrepareFileSystem() "
                            "failed with %d.", err);
            return err;
        }
        break;
    case OS_FileSystem_Type_NONE:
        err = ConfigTool_HostFsInit(hFs, &cfgFs);
        if (err != OS_SUCCESS)
        {
            Debug_LOG_ERROR("ConfigTool_HostFsInit() failed with %d.", err);
            return err;
        }
        break;
    default:
        Debug_LOG_ERROR("Unsupported FileSystem type.");
        return OS_ERROR_NOT_SUPPORTED;
    }

    return OS_SUCCESS;
}

OS_Error_t ConfigTool_BackendDeInit(
    OS_FileSystem_Handle_t hFs)
{
    if (cfgFs.type == OS_FileSystem_Type_NONE)
    {
        const OS_Error_t err = ConfigTool_HostFsFree(hFs);
        if (err != OS_SUCCESS)
        {
            Debug_LOG_ERROR("ConfigTool_HostFsFree() failed with %d.", err);
            return err;
        }
    }

    if (cfgFs.type == OS_FileSystem_Type_FATFS
        || cfgFs.type == OS_FileSystem_Type_SPIFFS)
    {
        OS_Error_t err = OS_FileSystem_unmount(hFs);
        if (err != OS_SUCCESS)
        {
            Debug_LOG_ERROR("OS_FileSystem_unmount() failed with %d.", err);
            return err;
        }

        err = OS_FileSystem_free(hFs);
        if (err != OS_SUCCESS)
        {
            Debug_LOG_ERROR("OS_FileSystem_free() failed with %d.", err);
            return err;
        }
    }

    return OS_SUCCESS;
}
