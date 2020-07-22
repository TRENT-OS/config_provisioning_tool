/**
 * Configuration Provisioning Tool
 *
 * Copyright (C) 2019-2020, Hensoldt Cyber GmbH
 *
 */

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <libgen.h>
#include <unistd.h>

#include "LibDebug/Debug.h"
#include "ConfigTool.h"
#include "ConfigTool_XmlParser.h"
#include "ConfigTool_Backend.h"
#include "ConfigTool_ConfigService.h"
#include "ConfigTool_Util.h"


/* Private functions ---------------------------------------------------------*/
static
OS_Error_t ConfigTool_AssignFileSystemType(
    const char* fileSystemType,
    OS_FileSystem_Type_t* fsType)
{
    if (strncmp(fileSystemType, FATFS, sizeof(FATFS)) == 0)
    {
        Debug_LOG_DEBUG("Setting FileSystem Type to FAT");
        *fsType = OS_FileSystem_Type_FATFS;
        return OS_SUCCESS;
    }

    if (strncmp(fileSystemType, SPIFFS, sizeof(SPIFFS)) == 0)
    {
        Debug_LOG_DEBUG("Setting FileSystem Type to SPIFFS");
        *fsType = OS_FileSystem_Type_SPIFFS;
        return OS_SUCCESS;
    }

    printf("Requested FileSystem not supported!\n");
    return OS_ERROR_NOT_SUPPORTED;
}


static
OS_Error_t ConfigTool_CreateProvisionedImage(
    xmlDoc* doc,
    const char* filePath,
    const char* outFileName,
    OS_FileSystem_Type_t fsType)
{
    OS_FileSystem_Handle_t hFs;
    OS_ConfigServiceLib_t configLib;
    ConfigTool_ConfigCounter_t configCounter = {0};

    // Get the root element node
    xmlNode* rootElement = xmlDocGetRootElement(doc);
    if (rootElement == NULL)
    {
        Debug_LOG_ERROR("Failed to get the root element node of the XML file");
        return OS_ERROR_GENERIC;
    }

    // Get the directory path of the XML file
    const char* dirPath = dirname((char*)filePath);

    /* Count the number of domains and parameter of their respective types to
     * initialize the config service backend with
     */
    ConfigTool_XmlParserGetElementCount(rootElement, &configCounter, dirPath);

    Debug_LOG_DEBUG("Domain Count:%d, String Count:%d, Param Count:%d, Blob Count:%d",
                    configCounter.domain_count, configCounter.string_count,
                    configCounter.param_count, configCounter.blob_count);

    // Initialize file system
    Debug_LOG_DEBUG("Initializing FileSystem");
    OS_Error_t err = ConfigTool_BackendInit(&hFs, fsType);
    if (err != OS_SUCCESS)
    {
        Debug_LOG_ERROR("ConfigTool_Backend_init() failed with %d", err);
        return err;
    }

    // Initialize the configuration service library
    Debug_LOG_DEBUG("Initializing ConfigService");
    err = ConfigTool_ConfigServiceInit(&configLib, hFs, &configCounter);
    if (err != OS_SUCCESS)
    {
        Debug_LOG_ERROR("ConfigTool_ConfigServiceInit() failed with %d", err);
        return err;
    }

    /* Since we will reuse the configCounter to keep track of the elements that
     * are being written, we reset it to zero after initializing the
     * configuration lib
     */
    memset(&configCounter, 0, sizeof(ConfigTool_ConfigCounter_t));
    ConfigTool_XmlParserRun(&configLib, rootElement, &configCounter, filePath);

    // Deinitialize the Filesystem backend
    err = ConfigTool_BackendDeInit(hFs);
    if (err != OS_SUCCESS)
    {
        Debug_LOG_ERROR("ConfigTool_BackendDeInit() failed with %d", err);
        return err;
    }

    // Rename the generic output filename to the requested filename
    if (rename(HOSTSTORAGE_FILE_NAME, outFileName) != 0)
    {
        Debug_LOG_ERROR("Renaming the output file failed.");
        return OS_ERROR_GENERIC;
    }

    Debug_LOG_DEBUG("Provisioned configuration image successfully created as %s",
                    outFileName);

    return OS_SUCCESS;
}


/* ---------------------------------------------------------------------------*/
int main(int argc, char* argv[])
{
    const char* inFileName, *outFileName, *fileSystemType;

    int opt;
    while ((opt = getopt(argc, argv, "i:o:t:h")) != -1)
    {
        switch (opt)
        {
        case 'i':
            inFileName = optarg;
            break;
        case 'o':
            outFileName = optarg;
            break;
        case 't':
            fileSystemType = optarg;
            break;
        case '?':
            printf("unknown option: %c\n", optopt);
            break;
        case 'h':
            printf("Usage: -i [<path-to-xml_file>] -o [<output_nvm_file_name>] -t [filesystem_type]\n");
            return 0;
        }
    }

    if (argc < 6)
    {
        printf("Not enough arguments passed to run the tool!\n");
        printf("Usage: -i [<path-to-xml_file>] -o [<output_nvm_file_name>] -t [filesystem_type]\n");
        return -1;
    }

    OS_FileSystem_Type_t fsType = OS_FileSystem_Type_NONE;

    OS_Error_t err = ConfigTool_AssignFileSystemType(fileSystemType, &fsType);
    if (err != OS_SUCCESS)
    {
        Debug_LOG_ERROR("ConfigTool_AssignFileSystemType() failed with %d", err);
        return -1;
    }

    // Parse the file and get the DOM(document object model)
    xmlDoc* doc = xmlReadFile(inFileName, NULL, 0);
    if (doc == NULL)
    {
        Debug_LOG_ERROR("Could not parse input XML file %s", inFileName);
        return -1;
    }

    err = ConfigTool_CreateProvisionedImage(
              doc,
              inFileName,
              outFileName,
              fsType);
    if (err != OS_SUCCESS)
    {
        Debug_LOG_ERROR("ConfigTool_CreateProvisionedImage() failed with %d", err);
        xmlFreeDoc(doc);
        return -1;
    }

    // free the document
    xmlFreeDoc(doc);

    return 0;
}
