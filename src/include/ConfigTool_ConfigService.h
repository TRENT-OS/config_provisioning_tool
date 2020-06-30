/**
 * Copyright (C) 2020, Hensoldt Cyber GmbH
 *
 * @addtogroup Config Provisioning Tool
 * @{
 *
 * @file ConfigTool_ConfigService.h
 *
 * @brief Contains relevant functions to setup and initialize the Configurations
 * Service library.
 *
 */

#pragma once

/* Includes ------------------------------------------------------------------*/
#include "ConfigTool.h"


/* Defines -------------------------------------------------------------------*/
#define PARAMETER_FILE "PARAM.BIN"
#define DOMAIN_FILE    "DOMAIN.BIN"
#define STRING_FILE    "STRING.BIN"
#define BLOB_FILE      "BLOB.BIN"


/* Exported functions --------------------------------------------------------*/
/**
 * @brief Initializes the configuration service library and creates the
 * backend instances for the different parameter types (string, blob etc.) based
 * on the amount of elements required.
 *
 * @param configLib pointer to the configuration library instance
 * @param phandle pointer to the partition handle
 * @param configCounter pointer to the ConfigCounter object containing the
 * amount of elements that need to be written to the backends
 * @retval OS_SUCCESS if the configuration backend was initilazed successfully
 * @retval OS_ERROR_GENERIC if something went wrong during the init or writing
 * process
 * @retval OS_ERROR_INVALID_PARAMETER if an invalid parameter was written to the
 * backend
 */
OS_Error_t
ConfigTool_ConfigServiceInit(
    OS_ConfigServiceLib_t* configLib,
    hPartition_t phandle,
    ConfigTool_ConfigCounter_t* configCounter);