/*
 * Copyright (C) 2020-2024, HENSOLDT Cyber GmbH
 * 
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * For commercial licensing, contact: info.cyber@hensoldt.net
 */

/**
 * @file
 * @brief Contains relevant functions to initialize the FileSystem.
 *
 * @ingroup ConfigProvisioningTool
 */
#pragma once

/* Includes ------------------------------------------------------------------*/
#include "ConfigTool_Util.h"
#include "ConfigTool_HostFs.h"
#include "ConfigTool_HostFsFile.h"

#include "OS_FileSystem.h"
#include "lib_host/HostStorage.h"


/* Exported functions --------------------------------------------------------*/
/**
 * @brief Initializes the filesystem backend.
 *
 * @return an error code
 * @retval OS_SUCCESS - if the backend was initilazed successfully
 * @retval OS_ERROR_INVALID_PARAMETER - if an invalid parameter was passed
 * @retval OS_ERROR_GENERIC - if something went wrong during the init process
 */
OS_Error_t
ConfigTool_BackendInit(
    OS_FileSystem_Handle_t* hFs, //!< [in] Pointer to a filesystem handle
    OS_FileSystem_Type_t fsType  //!< [in] Filesystem type
);

/**
 * @brief Deinitialize the filesystem.
 *
 * @return an error code
 * @retval OS_SUCCESS - if the backend was deinitilazed successfully
 * @retval OS_ERROR_INVALID_PARAMETER - if an invalid parameter was passed
 * @retval OS_ERROR_GENERIC - if something went wrong during the deinitialization
 */
OS_Error_t
ConfigTool_BackendDeInit(
    OS_FileSystem_Handle_t hFs //!< [in] Filesystem handle
);
