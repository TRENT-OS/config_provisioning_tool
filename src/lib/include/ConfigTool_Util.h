/**
 * Copyright (C) 2020, Hensoldt Cyber GmbH
 *
 * @addtogroup Config Provisioning Tool
 * @{
 *
 * @file ConfigTool_Util.h
 *
 * @brief Module with various helper functions
 */

#pragma once

/* Includes ------------------------------------------------------------------*/
#include "ConfigTool.h"


/* Exported functions --------------------------------------------------------*/
/**
 * @brief Helper function that copies the contents of the passed file to a
 * buffer that is dynamically allocated.
 *
 * @param filename path to the file that should be copied into a buffer
 * @return char* pointer to the allocated buffer containing the file content
 */
char*
ConfigTool_UtilCopyFileToBuf(const char* filename);

/**
 * @brief Helper function that initializes a domain with the given name.
 *
 * @param domain domain object to write the name to
 * @param name name to write to the domain object
 */
void
ConfigTool_UtilInitializeDomain(
    OS_ConfigServiceLibTypes_Domain_t* domain,
    char const* name);

/**
 * @brief Helper function that initializes the passed buffer and copies the
 * passed name to it.
 *
 * @param buf buffer to copy the name to
 * @param bufSize size of the buffer
 * @param name name to copy into the buffer
 */
void
ConfigTool_UtilInitializeName(
    char* buf,
    size_t bufSize,
    char const* name);

/**
 * @brief Calculates the number blocks required to store the blob parameters
 *
 * @param blobValue pointer to the blob value
 * @return uint32_t calculated number of blocks required to store the blob value
 */
uint32_t
ConfigTool_UtilCalculateNumberOfBlocks(const char* blobValue);
