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
#include "OS_Error.h"
#include "OS_ConfigService.h"


/* Defines -------------------------------------------------------------------*/
#define PARAMETER_FILE "PARAM.BIN"
#define DOMAIN_FILE    "DOMAIN.BIN"
#define STRING_FILE    "STRING.BIN"
#define BLOB_FILE      "BLOB.BIN"


/* Exported types/enums ------------------------------------------------------*/
/**
 * @brief Holds the currently supported parameter types of the configuration library
 *
 */
typedef enum
{
    INT32 = 1,
    INT64,
    STRING,
    BLOB,
    BADTYPE = -1
} ConfigTool_ConfigServiceParamType_t;

/**
 * @brief Utilized to map the parsed parameter types to the supported parameter types
 *
 */
typedef struct
{
    const char*  key;
    ConfigTool_ConfigServiceParamType_t parameterType;
} ConfigTool_ConfigServiceTypes_t;

/**
 * @brief Counter to keep track of the parsed number of elements
 *
 */
typedef struct
{
    unsigned int param_count;    /**< number of params present  */
    unsigned int domain_count;   /**< number of domains present */
    unsigned int string_count;   /**< number of strings present */
    unsigned int blob_count;     /**< number of blobs present   */
} ConfigTool_ConfigServiceCounter_t;


/* Exported functions --------------------------------------------------------*/
/**
 * @brief Initializes the configuration service library and creates the
 * backend instances for the different parameter types (string, blob etc.) based
 * on the amount of elements required.
 *
 * @param configLib pointer to the configuration library instance
 * @param hFs filesystem handle
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
    OS_FileSystem_Handle_t hFs,
    ConfigTool_ConfigServiceCounter_t* configCounter);
