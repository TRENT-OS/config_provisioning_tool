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
OS_Error_t ConfigTool_CreateProvisionedPartition(
    xmlDoc* doc,
    const char* filePath,
    const char* outFileName)
{
    hPartition_t phandle;
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

    // Initialize file system and partition manager
    Debug_LOG_DEBUG("Initializing FileSystem and PartitionManager");
    OS_Error_t err = ConfigTool_BackendInit(&phandle, outFileName);
    if (err != OS_SUCCESS)
    {
        Debug_LOG_ERROR("ConfigTool_Backend_init() failed with %d", err);
        return err;
    }

    // Initialize the configuration service library
    Debug_LOG_DEBUG("Initializing ConfigService");
    err = ConfigTool_ConfigServiceInit(&configLib, phandle, &configCounter);
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

    Debug_LOG_DEBUG("Provisioned configuration partition successfully created as %s",
                    outFileName);

    return OS_SUCCESS;
}


/* ---------------------------------------------------------------------------*/
int main(int argc, char* argv[])
{
    const char* inFileName, *outFileName;

    int opt;
    while ((opt = getopt(argc, argv, "i:o:h")) != -1)
    {
        switch (opt)
        {
        case 'i':
            inFileName = optarg;
            break;
        case 'o':
            outFileName = optarg;
            break;
        case '?':
            printf("unknown option: %c\n", optopt);
            break;
        case 'h':
            printf("Usage: -i [<path-to-xml_file>] -o [<output_nvm_file_name>]\n");
            return 0;
        }
    }

    if (argc < 5)
    {
        printf("Not enough arguments passed to run the tool!\n");
        printf("Usage: -i [<path-to-xml_file>] -o [<output_nvm_file_name>]\n");
        return -1;
    }

    // Parse the file and get the DOM(document object model)
    xmlDoc* doc = xmlReadFile(inFileName, NULL, 0);
    if (doc == NULL)
    {
        Debug_LOG_ERROR("Could not parse input XML file %s", inFileName);
        return -1;
    }

    OS_Error_t err = ConfigTool_CreateProvisionedPartition(
                         doc,
                         inFileName,
                         outFileName);
    if (err != OS_SUCCESS)
    {
        Debug_LOG_ERROR("ConfigTool_CreateProvisionedPartition() failed with %d", err);
        xmlFreeDoc(doc);
        return -1;
    }

    // free the document
    xmlFreeDoc(doc);

    return 0;
}