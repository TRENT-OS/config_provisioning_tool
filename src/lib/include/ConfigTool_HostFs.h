/**
 * Copyright (C) 2020, Hensoldt Cyber GmbH
 *
 * @addtogroup Config Provisioning Tool
 * @{
 *
 * @file ConfigTool_HostFs.h
 *
 * @brief Wrapper for the host FileSystem
 */

#pragma once

/* Includes ------------------------------------------------------------------*/
#include "OS_FileSystem.h"
#include "OS_FileSystem_int.h"


/* Public functions ----------------------------------------------------------*/
/**
 * @brief Initializes the filesystem handle with the host filesystem file
 * operation functions
 *
 * @param self pointer to a filesystem handle
 * @param cfg pointer to the filesystem configuration
 * @retval OS_SUCCESS if the filesystem handle was initilazed successfully
 * @retval OS_ERROR_INSUFFICIENT_SPACE if the memory allocation for the
 * filesystem handle fails
 */
OS_Error_t
ConfigTool_HostFsInit(
    OS_FileSystem_Handle_t*       self,
    const OS_FileSystem_Config_t* cfg);

/**
 * @brief Frees the passed filesystem handle
 *
 * @param self pointer to a filesystem handle
 * @retval OS_SUCCESS if the filesystem handle was freed successfully
 * @retval OS_ERROR_INVALID_PARAMETER if an empty handle was passed
 */
OS_Error_t
ConfigTool_HostFsFree(
    OS_FileSystem_Handle_t self);


