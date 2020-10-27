/* Copyright (C) 2020, HENSOLDT Cyber GmbH */

/**
 * @file
 * @brief Module with various helper functions.
 *
 * @ingroup ConfigProvisioningTool
 */
#pragma once

/* Includes ------------------------------------------------------------------*/
#include "OS_ConfigService.h"


/* Exported functions --------------------------------------------------------*/
/**
 * @brief Helper function that copies the contents of the passed file to a
 * buffer that is dynamically allocated.
 *
 * @return char* pointer to the allocated buffer containing the file content
 */
char*
ConfigTool_UtilCopyFileToBuf(
    const char* filename //!< [in] Path to the file to copy to the buffer
);

/**
 * @brief Helper function that initializes a domain with the given name.
 */
void
ConfigTool_UtilInitializeDomain(
    OS_ConfigServiceLibTypes_Domain_t* domain, /*!< [out] Domain object to write
                                                          the name to */
    char const* name                           /*!< [in] Name to write to the
                                                         domain object */
);

/**
 * @brief Helper function that initializes the passed buffer and copies the
 * passed name to it.
 */
void
ConfigTool_UtilInitializeName(
    char* buf,       //!< [out] Buffer to copy the name to
    size_t bufSize,  //!< [in] Size of the buffer
    char const* name //!< [in] Name to copy into the buffer
);

/**
 * @brief Calculates the number blocks required to store the blob parameters.
 *
 * @return uint32_t calculated number of blocks required to store the blob value
 */
uint32_t
ConfigTool_UtilCalculateNumberOfBlocks(
    const char* blobValue //!< [in] Pointer to the blob value
);
