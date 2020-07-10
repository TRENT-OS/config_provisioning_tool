/**
 * Copyright (C) 2020, Hensoldt Cyber GmbH
 *
 * @addtogroup Config Provisioning Tool
 * @{
 *
 * @file ConfigTool_Backend.h
 *
 * @brief Contains relevant functions to initialize the FileSystem.
 *
 */

#pragma once

/* Includes ------------------------------------------------------------------*/
#include "ConfigTool.h"
#include "ConfigTool_Util.h"

#include "OS_FileSystem.h"
#include "LibHost/HostStorage.h"


/* Exported functions --------------------------------------------------------*/
/**
 * @brief Initializes the filesystem backend
 *
 * @param hFs filesystem handle
 * @return an error code
 * @retval OS_SUCCESS if the backend was initilazed successfully
 * @retval OS_ERROR_INVALID_PARAMETER if an invalid parameter was passed
 * @retval OS_ERROR_GENERIC if something went wrong during the init process
 */
OS_Error_t
ConfigTool_BackendInit(OS_FileSystem_Handle_t* hFs);

/**
 * @brief Deinitialize the filesystem
 *
 * @param hFs filesystem handle
 * @return an error code
 * @retval OS_SUCCESS if the backend was deinitilazed successfully
 * @retval OS_ERROR_INVALID_PARAMETER if an invalid parameter was passed
 * @retval OS_ERROR_GENERIC if something went wrong during the deinitialization
 */
OS_Error_t
ConfigTool_BackendDeInit(OS_FileSystem_Handle_t hFs);