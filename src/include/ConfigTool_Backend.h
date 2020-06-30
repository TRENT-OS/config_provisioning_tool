/**
 * Copyright (C) 2020, Hensoldt Cyber GmbH
 *
 * @addtogroup Config Provisioning Tool
 * @{
 *
 * @file ConfigTool_Backend.h
 *
 * @brief Contains relevant functions to initialize the Partition
 * Manager and FileSystem.
 *
 */

#pragma once

/* Includes ------------------------------------------------------------------*/
#include "ConfigTool.h"
#include "ConfigTool_Util.h"
#include "ConfigTool_FileNVM.h"


/* Exported functions --------------------------------------------------------*/
/**
 * @brief Initializes the filesystem and partition manager backend used to write
 * the disk image.
 *
 * @param phandle partition handle
 * @param outFileName requested name for the created output file
 * @return an error code
 * @retval OS_SUCCESS if the backend was initilazed successfully
 * @retval OS_ERROR_GENERIC if something went wrong during the init process
 */
OS_Error_t
ConfigTool_BackendInit(hPartition_t* phandle, const char* outFileName);