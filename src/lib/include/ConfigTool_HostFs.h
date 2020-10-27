/* Copyright (C) 2020, HENSOLDT Cyber GmbH */

/**
 * @file
 * @brief Wrapper for the host FileSystem.
 *
 * @ingroup ConfigTool_HostFs
 *
 */
#pragma once

/* Includes ------------------------------------------------------------------*/
#include "OS_FileSystem.h"
#include "OS_FileSystem_int.h"

/**
 * @defgroup ConfigTool_HostFs Host FileSystem
 * @ingroup  ConfigProvisioningTool
 */

/* Public functions ----------------------------------------------------------*/
/**
 * @brief Initializes the filesystem handle with the host filesystem file
 * operation functions.
 *
 * @retval OS_SUCCESS - if the filesystem handle was initilazed successfully
 * @retval OS_ERROR_INSUFFICIENT_SPACE - if the memory allocation for the
 * filesystem handle fails
 */
OS_Error_t
ConfigTool_HostFsInit(
    OS_FileSystem_Handle_t*       self, //!< [out] Pointer to a filesystem handle
    const OS_FileSystem_Config_t* cfg   //!< [in] Pointer to a filesystem config
);

/**
 * @brief Frees the passed filesystem handle.
 *
 * @retval OS_SUCCESS - if the filesystem handle was freed successfully
 * @retval OS_ERROR_INVALID_PARAMETER - if an empty handle was passed
 */
OS_Error_t
ConfigTool_HostFsFree(
    OS_FileSystem_Handle_t self //!< [in] Filesystem handle
);
