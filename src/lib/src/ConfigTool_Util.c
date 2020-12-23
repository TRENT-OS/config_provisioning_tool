/*
 *  Utility functions
 *
 *  Copyright (C) 2019-2020, Hensoldt Cyber GmbH
 *
 */

/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include <stdlib.h>

#include "lib_debug/Debug.h"
#include "ConfigTool_Util.h"



/* Public functions ----------------------------------------------------------*/
uint32_t
ConfigTool_UtilCalculateNumberOfBlocks(const char* blobValue)
{
    // strlen() does not include NULL terminator
    uint32_t blobSize = strlen(blobValue) + 1;

    uint32_t calcNumberOfBlocks;

    if (blobSize <= OS_CONFIG_LIB_PARAMETER_MAX_BLOB_BLOCK_LENGTH)
    {
        calcNumberOfBlocks = 1;
    }

    if (blobSize > OS_CONFIG_LIB_PARAMETER_MAX_BLOB_BLOCK_LENGTH)
    {
        calcNumberOfBlocks =
            (blobSize / OS_CONFIG_LIB_PARAMETER_MAX_BLOB_BLOCK_LENGTH);

        if ((blobSize % OS_CONFIG_LIB_PARAMETER_MAX_BLOB_BLOCK_LENGTH) != 0)
        {
            calcNumberOfBlocks++;
        }
    }

    return calcNumberOfBlocks;
}

void
ConfigTool_UtilInitializeDomain(
    OS_ConfigServiceLibTypes_Domain_t* domain,
    char const* name)
{
    ConfigTool_UtilInitializeName(
        domain->name.name,
        OS_CONFIG_LIB_DOMAIN_NAME_LEN,
        name);
    domain->enumerator.index = 0;
}

void ConfigTool_UtilInitializeName(
    char* buf,
    size_t bufSize,
    char const* name)
{
    memset(buf, 0, bufSize);
    strncpy(buf, name, bufSize - 1);
}

char* ConfigTool_UtilCopyFileToBuf(const char* filename)
{
    FILE* handler = fopen(filename, "r");
    if (handler == NULL)
    {
        Debug_LOG_ERROR("Failed to open file!");
        return NULL;
    }

    // Seek the last byte of the file
    fseek(handler, 0, SEEK_END);
    // Offset from the first to the last byte, or in other words, filesize
    uint64_t fileSize = ftell(handler);
    // Go back to the start of the file
    rewind(handler);

    // Allocate a string that can hold it all
    char* buf = (char*) malloc(sizeof(char) * (fileSize + 1) );

    // Read it all in one operation
    size_t readSize = fread(buf, sizeof(char), fileSize, handler);

    // Close the file.
    fclose(handler);

    // Verify if the all content was read
    if (readSize != fileSize)
    {
        /* Something went wrong, throw away the memory and set
         * the buf to NULL
         */
        free(buf);
        Debug_LOG_ERROR("Could not read the requested length from file! \
                        Requested: %lu, Read: %zu", fileSize, readSize);
        return NULL;
    }

    /* fread doesn't set it so put a \0 in the last position
     * and buf is now a string
     */
    buf[fileSize] = '\0';

    return buf;
}
