/*
 *  Wrapper for the host FileSystem
 *
 * Copyright (C) 2020-2024, HENSOLDT Cyber GmbH
 * 
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * For commercial licensing, contact: info.cyber@hensoldt.net
 */

/* Includes ------------------------------------------------------------------*/
#include "lib_debug/Debug.h"

#include "ConfigTool_HostFs.h"
#include "ConfigTool_HostFsFile.h"


/* Private variables ---------------------------------------------------------*/
static const OS_FileSystem_FileOps_t hostFsFile_ops =
{
    .open       = ConfigTool_HostFsFileOpen,
    .close      = ConfigTool_HostFsFileClose,
    .read       = ConfigTool_HostFsFileRead,
    .write      = ConfigTool_HostFsFileWrite,
    .delete     = ConfigTool_HostFsFileDelete,
    .getSize    = ConfigTool_HostFsFileGetSize,
};


/* Public functions ----------------------------------------------------------*/
OS_Error_t
ConfigTool_HostFsInit(
    OS_FileSystem_Handle_t*       self,
    const OS_FileSystem_Config_t* cfg)
{
    OS_FileSystem_Handle_t fs;

    if ((fs = calloc(1, sizeof(OS_FileSystem_t))) == NULL)
    {
        Debug_LOG_ERROR("Failed to allocate memory");
        return OS_ERROR_INSUFFICIENT_SPACE;
    }

    fs->fileOps = &hostFsFile_ops;

    *self = fs;

    return OS_SUCCESS;
}

OS_Error_t
ConfigTool_HostFsFree(
    OS_FileSystem_Handle_t self)
{
    if (NULL == self)
    {
        Debug_LOG_ERROR("Empty handle received");
        return OS_ERROR_INVALID_PARAMETER;
    }

    free(self);

    return OS_SUCCESS;
}
