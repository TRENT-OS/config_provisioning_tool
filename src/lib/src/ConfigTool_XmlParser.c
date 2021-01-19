/*
 *  Parses the XML element nodes and writes the parameters and domains to the
 *  configuration library instance
 *
 *  Copyright (C) 2019-2020, Hensoldt Cyber GmbH
 *
 */

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "lib_debug/Debug.h"
#include "ConfigTool_XmlParser.h"


/* Defines -------------------------------------------------------------------*/
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

#define NUMELEMENTS ARRAY_SIZE(xmlElementLookupTable)
#define NKEYS       ARRAY_SIZE(lookupTable)

// below defines for elements of xml
#define ELEMENT_DOMAIN            "domain"
#define ELEMENT_PARAM_NAME        "param_name"
#define ELEMENT_TYPE              "type"
#define ELEMENT_VALUE             "value"
#define ELEMENT_ACCESS_POLICY     "access_policy"
#define ELEMENT_COMPONENT         "component"
#define ELEMENT_READ              "read"
#define ELEMENT_WRITE             "write"

// below defines for attributes of xml
#define ATTRIBUTE_NAME            "name"
#define ATTRIBUTE_ID              "id"

/* Private types/enums -------------------------------------------------------*/
typedef struct
{
    const char*  element;
    unsigned int val;
} ConfigTool_XmlParserElements_t;

typedef struct
{
    ConfigTool_ConfigServiceParamType_t type;
    char  paramName[OS_CONFIG_LIB_PARAMETER_NAME_LEN]; /**< name of params present (from XML) */
    void* value;    /**< value of params present (from XML) */
} ConfigTool_XmlParserParams_t;

typedef enum
{
    XML_ELEMENT_DOMAIN = 1,
    XML_ELEMENT_PARAM_NAME,
    XML_ELEMENT_TYPE,
    XML_ELEMENT_VALUE,
    XML_ELEMENT_ACCESS_POLICY,
    XML_ELEMENT_COMPONENT,
    XML_ELEMENT_READ,
    XML_ELEMENT_WRITE,
    XML_ELEMENT_BAD = -1
} ConfigTool_XmlParserElementValues_t;


/* Private variables ---------------------------------------------------------*/
static uint8_t parameterIndex, blobIndex, stringIndex = 0;
static ConfigTool_XmlParserParams_t xmlparams;
static bool isNewDomain = false;
static bool hasReadAccess = false;
static bool hasWriteAccess = false;

static ConfigTool_ConfigServiceTypes_t lookupTable[] =
{
    { "uint32",  UINT32 },
    { "uint64",  UINT64 },
    { "string", STRING },
    { "blob",   BLOB }
};

static ConfigTool_XmlParserElements_t xmlElementLookupTable[] =
{
    {ELEMENT_DOMAIN,        XML_ELEMENT_DOMAIN},
    {ELEMENT_PARAM_NAME,    XML_ELEMENT_PARAM_NAME},
    {ELEMENT_TYPE,          XML_ELEMENT_TYPE},
    {ELEMENT_VALUE,         XML_ELEMENT_VALUE},
    {ELEMENT_ACCESS_POLICY, XML_ELEMENT_ACCESS_POLICY},
    {ELEMENT_COMPONENT,     XML_ELEMENT_COMPONENT},
    {ELEMENT_READ,          XML_ELEMENT_READ},
    {ELEMENT_WRITE,         XML_ELEMENT_WRITE}
};


/* Private functions ---------------------------------------------------------*/
static
xmlNode* ConfigTool_GetNextValueElement(xmlNode* a_node)
{
    xmlNode* cur_node = NULL;

    for (cur_node = a_node; cur_node; cur_node = cur_node->next)
    {
        if (cur_node->type == XML_ELEMENT_NODE)
        {
            if (strncmp((const char*)cur_node->name, ELEMENT_VALUE,
                        sizeof(ELEMENT_VALUE)) == 0)
            {
                return cur_node;
            }
        }

        ConfigTool_GetNextValueElement(cur_node->children);
    }

    return NULL;
}

static
ConfigTool_ConfigServiceParamType_t
ConfigTool_XmlParserGetElementType(char* key)
{
    for (unsigned int i = 0; i < NKEYS; i++)
    {
        ConfigTool_ConfigServiceTypes_t* sym = &lookupTable[i];
        if (strcmp(sym->key, key) == 0)
        {
            return sym->parameterType;
        }
    }

    return BADTYPE;
}

static
OS_Error_t
ConfigTool_XmlParserSetAccessSetting(char* accessSetting, bool* accessRight)
{
    if (strcmp(accessSetting, "true") == 0)
    {
        *accessRight = true;
        return OS_SUCCESS;
    }

    if (strcmp(accessSetting, "false") == 0)
    {
        *accessRight = false;
        return OS_SUCCESS;
    }

    // if we get here the accessSetting must have been entered incorrect in the
    // config file
    Debug_LOG_ERROR("Invalid access setting received! An access right can only "
                    "be set to true or false");
    return OS_ERROR_GENERIC;
}

static
ConfigTool_XmlParserElementValues_t
ConfigTool_XmlParserGetXmlElement(char* key)
{
    for (unsigned int i = 0; i < NUMELEMENTS; i++)
    {
        ConfigTool_XmlParserElements_t* sym = &xmlElementLookupTable[i];
        if (strcmp(sym->element, key) == 0)
        {
            return sym->val;
        }
    }

    return XML_ELEMENT_BAD;
}

static void
ConfigTool_HandleBlobCount(
    xmlNode* cur_node,
    ConfigTool_ConfigServiceCounter_t* configCounter,
    const char* dirPath)
{
    xmlNode* nextNode = ConfigTool_GetNextValueElement(cur_node);
    char* node_content = (char*)xmlNodeGetContent(nextNode);

    char* filePath = malloc((strlen(dirPath) + 1) + (strlen(node_content) + 1));
    strcpy(filePath, dirPath);
    strcat(filePath, node_content);
    Debug_LOG_DEBUG("Generated file path %s", filePath);

    char* fileInBuf = ConfigTool_UtilCopyFileToBuf(filePath);
    if (!fileInBuf)
    {
        Debug_LOG_ERROR("Invalid blob passed!");
        return;
    }

    configCounter->blob_count += ConfigTool_UtilCalculateNumberOfBlocks(fileInBuf);

    free(node_content);
    free(filePath);
    free(fileInBuf);
}

static
OS_Error_t
ConfigTool_XmlParserWriteVariableLengthBlob(
    OS_ConfigServiceBackend_t* backend,
    uint32_t index,
    uint32_t numberOfBlocks,
    void const* buffer,
    size_t bufferLength)
{
    size_t blobBlockSize = OS_ConfigServiceBackend_getSizeOfRecords(backend);
    size_t blobCapacity = blobBlockSize * numberOfBlocks;

    if (bufferLength > blobCapacity)
    {
        Debug_LOG_DEBUG("Passed buffer length exceeds available blob capacity");
        return OS_ERROR_GENERIC;
    }

    // We anticipate a maximum size here which should be ok to place on the stack.
    char tmpBuf[OS_CONFIG_LIB_PARAMETER_MAX_BLOB_BLOCK_LENGTH] = {0};
    size_t bytesCopied = 0;

    while (bytesCopied < bufferLength)
    {
        size_t bytesToCopy;

        if ((bufferLength - bytesCopied) >= blobBlockSize)
        {
            bytesToCopy = blobBlockSize;
        }
        else
        {
            bytesToCopy = bufferLength - bytesCopied;
        }

        memcpy(tmpBuf, (char*)buffer + bytesCopied, bytesToCopy);

        OS_Error_t fetchResult = OS_ConfigServiceBackend_writeRecord(
                                     backend,
                                     index,
                                     tmpBuf,
                                     sizeof(tmpBuf));

        if (OS_SUCCESS != fetchResult)
        {
            Debug_LOG_DEBUG("OS_ConfigServiceBackend_writeRecord() failed \
                            with %d", fetchResult);
            return OS_ERROR_GENERIC;
        }

        bytesCopied += bytesToCopy;
        index++;
    }

    return OS_SUCCESS;
}

static
OS_Error_t
ConfigTool_XmlParserAddIntParameter(
    OS_ConfigServiceLib_t* configLib,
    OS_ConfigServiceLibTypes_Parameter_t* parameter,
    OS_ConfigServiceLibTypes_ParameterType_t parameterType,
    unsigned int domainIndex,
    const char* parameterName,
    const void* parameterValue)
{
    parameter->domain.index = domainIndex;

    if (parameterType == OS_CONFIG_LIB_PARAMETER_TYPE_INTEGER32)
    {
        parameter->parameterType = parameterType;
        memcpy(&parameter->parameterValue, parameterValue, sizeof(uint32_t));
    }

    if (parameterType == OS_CONFIG_LIB_PARAMETER_TYPE_INTEGER64)
    {
        parameter->parameterType = parameterType;
        memcpy(&parameter->parameterValue, parameterValue, sizeof(uint64_t));
    }

    ConfigTool_UtilInitializeName(
        parameter->parameterName.name,
        OS_CONFIG_LIB_PARAMETER_NAME_LEN,
        parameterName);

    OS_Error_t err = OS_ConfigServiceBackend_writeRecord(
                         &configLib->parameterBackend,
                         parameterIndex,
                         parameter,
                         sizeof(OS_ConfigServiceLibTypes_Parameter_t));
    if (err != OS_SUCCESS)
    {
        Debug_LOG_ERROR("OS_ConfigServiceBackend_writeRecord() failed with: %d", err);
        return err;
    }

    parameterIndex++;

    return OS_SUCCESS;
}

static
OS_Error_t
ConfigTool_XmlParserAddStringParameter(
    OS_ConfigServiceLib_t* configLib,
    OS_ConfigServiceLibTypes_Parameter_t* parameter,
    unsigned int domainIndex,
    const char* parameterName,
    const void* parameterValue,
    size_t parameterSize)
{

    char str[OS_CONFIG_LIB_PARAMETER_MAX_STRING_LENGTH];
    memset(str, 0, sizeof(str));
    strncpy(str, (const char*)xmlparams.value, (sizeof(str) - 1));

    parameter->domain.index = domainIndex;
    parameter->parameterType = OS_CONFIG_LIB_PARAMETER_TYPE_STRING;

    ConfigTool_UtilInitializeName(
        parameter->parameterName.name,
        OS_CONFIG_LIB_PARAMETER_NAME_LEN,
        parameterName);

    parameter->parameterValue.valueString.index = stringIndex;
    parameter->parameterValue.valueString.size = strlen(str) + 1;

    OS_Error_t err = OS_ConfigServiceBackend_writeRecord(
                         &configLib->parameterBackend,
                         parameterIndex,
                         parameter,
                         sizeof(OS_ConfigServiceLibTypes_Parameter_t));;
    if (err != OS_SUCCESS)
    {
        Debug_LOG_ERROR("OS_ConfigServiceBackend_writeRecord() failed with: %d", err);
        return err;
    }

    err = OS_ConfigServiceBackend_writeRecord(
              &configLib->stringBackend,
              parameter->parameterValue.valueString.index,
              str,
              sizeof(str));
    if (err != OS_SUCCESS)
    {
        Debug_LOG_ERROR("OS_ConfigServiceBackend_writeRecord() failed with: %d", err);
        return err;
    }

    parameterIndex++;
    stringIndex++;

    return OS_SUCCESS;
}

/* The configuration library stores large blobs in chunks of blocks and therfore
 * it is first required to verify how many blocks need to be used to store the
 * complete blob
 */
static
OS_Error_t
ConfigTool_XmlParserAddBlobParameter(
    OS_ConfigServiceLib_t* configLib,
    OS_ConfigServiceLibTypes_Parameter_t* parameter,
    unsigned int domainIndex,
    const char* parameterName,
    const void* parameterValue,
    size_t parameterSize)
{
    parameter->domain.index = domainIndex;
    parameter->parameterType = OS_CONFIG_LIB_PARAMETER_TYPE_BLOB;

    ConfigTool_UtilInitializeName(
        parameter->parameterName.name,
        OS_CONFIG_LIB_PARAMETER_NAME_LEN,
        parameterName);

    uint32_t calcNumberOfBlocks = ConfigTool_UtilCalculateNumberOfBlocks(
                                      parameterValue);

    Debug_LOG_DEBUG("Calculated number of blocks required: %u\n",
                    calcNumberOfBlocks);

    parameter->parameterValue.valueBlob.index = blobIndex;
    parameter->parameterValue.valueBlob.numberOfBlocks = calcNumberOfBlocks;
    parameter->parameterValue.valueBlob.size = parameterSize;

    OS_Error_t err = OS_ConfigServiceBackend_writeRecord(
                         &configLib->parameterBackend,
                         parameterIndex,
                         parameter,
                         sizeof(OS_ConfigServiceLibTypes_Parameter_t));
    if (err != OS_SUCCESS)
    {
        Debug_LOG_ERROR("OS_ConfigServiceBackend_writeRecord() failed with: %d", err);
        return err;
    }

    err = ConfigTool_XmlParserWriteVariableLengthBlob(
              &configLib->blobBackend,
              parameter->parameterValue.valueBlob.index,
              parameter->parameterValue.valueBlob.numberOfBlocks,
              parameterValue,
              parameterSize);
    if (err != OS_SUCCESS)
    {
        Debug_LOG_ERROR("OS_ConfigServiceBackend_writeRecord() failed with: %d", err);
        return err;
    }

    blobIndex += calcNumberOfBlocks;
    parameterIndex++;

    return OS_SUCCESS;
}

static
OS_Error_t
ConfigTool_HandleBlobParameter(
    OS_ConfigServiceLib_t* configLib,
    OS_ConfigServiceLibTypes_Parameter_t* parameter,
    ConfigTool_ConfigServiceCounter_t* configCounter,
    const char* dirPath)
{
    char* filePath = malloc((strlen(dirPath) + 1) + (strlen(xmlparams.value) + 1));
    strcpy(filePath, dirPath);
    strcat(filePath, xmlparams.value);
    Debug_LOG_DEBUG("Generated file path %s", filePath);

    void* blobptr = ConfigTool_UtilCopyFileToBuf(filePath);
    if (!blobptr)
    {
        Debug_LOG_ERROR("ConfigTool_UtilCopyFileToBuf() failed");
        return OS_ERROR_GENERIC;
    }
    // strlen() does not include NULL terminator
    size_t blob_size = strlen(blobptr) + 1;

    Debug_LOG_DEBUG("Adding parameter %s to domain %u",
                    xmlparams.paramName, configCounter->domain_count);
    OS_Error_t err = ConfigTool_XmlParserAddBlobParameter(
                         configLib,
                         parameter,
                         configCounter->domain_count,
                         xmlparams.paramName,
                         blobptr,
                         blob_size);
    if (err != OS_SUCCESS)
    {
        Debug_LOG_ERROR("ConfigTool_XmlParserAddBlobParameter() failed with: %d", err);
        return err;
    }

    free(filePath);
    free(blobptr);

    return OS_SUCCESS;
}

static
OS_Error_t
ConfigTool_HandleInt32Parameter(
    OS_ConfigServiceLib_t* configLib,
    OS_ConfigServiceLibTypes_Parameter_t* parameter,
    ConfigTool_ConfigServiceCounter_t* configCounter)
{
    /* Convert string to integer. Parameters can be entered either in
     * decimal or hex format in the XML file, so base is set to 0
     */
    char* endPtr;
    uint32_t parameterValue = strtoul(xmlparams.value, &endPtr, 0);
    if (endPtr == NULL)
    {
        Debug_LOG_ERROR("strtoul() failed to convert to uint32_t value");
        return OS_ERROR_GENERIC;
    }

    Debug_LOG_DEBUG("Adding parameter %s to domain %u",
                    xmlparams.paramName, configCounter->domain_count);
    OS_Error_t err = ConfigTool_XmlParserAddIntParameter(
                         configLib,
                         parameter,
                         OS_CONFIG_LIB_PARAMETER_TYPE_INTEGER32,
                         configCounter->domain_count,
                         xmlparams.paramName,
                         &parameterValue);
    if (err != OS_SUCCESS)
    {
        Debug_LOG_ERROR("ConfigTool_XmlParserAddIntParameter() failed with: %d", err);
        return err;
    }

    return OS_SUCCESS;
}

static
OS_Error_t
ConfigTool_HandleInt64Parameter(
    OS_ConfigServiceLib_t* configLib,
    OS_ConfigServiceLibTypes_Parameter_t* parameter,
    ConfigTool_ConfigServiceCounter_t* configCounter)
{
    /* Convert string to long long integer. Parameters can be entered either in
     * decimal or hex format in the XML file, so base is set to 0
     */
    char* endPtr;
    uint64_t parameterValue = strtoull((char*)xmlparams.value, &endPtr, 0);
    if (endPtr == NULL)
    {
        Debug_LOG_ERROR("strtoull() failed to convert to uint64_t value");
        return OS_ERROR_GENERIC;
    }

    Debug_LOG_DEBUG("Adding parameter %s to domain %u",
                    xmlparams.paramName, configCounter->domain_count);
    OS_Error_t err = ConfigTool_XmlParserAddIntParameter(
                         configLib,
                         parameter,
                         OS_CONFIG_LIB_PARAMETER_TYPE_INTEGER64,
                         configCounter->domain_count,
                         xmlparams.paramName,
                         &parameterValue);
    if (err != OS_SUCCESS)
    {
        Debug_LOG_ERROR("ConfigTool_XmlParserAddIntParameter() failed with: %d", err);
        return err;
    }

    return OS_SUCCESS;
}

static
OS_Error_t
ConfigTool_HandleStringParameter(
    OS_ConfigServiceLib_t* configLib,
    OS_ConfigServiceLibTypes_Parameter_t* parameter,
    ConfigTool_ConfigServiceCounter_t* configCounter)
{
    size_t parameterLength = strlen((const char*)xmlparams.value);

    Debug_LOG_DEBUG("Adding parameter %s to domain %u",
                    xmlparams.paramName, configCounter->domain_count);
    OS_Error_t err = ConfigTool_XmlParserAddStringParameter(
                         configLib,
                         parameter,
                         configCounter->domain_count,
                         xmlparams.paramName,
                         xmlparams.value,
                         parameterLength);
    if (err != OS_SUCCESS)
    {
        Debug_LOG_ERROR("ConfigTool_XmlParserAddStringParameter() failed with: %d",
                        err);
        return err;
    }

    return OS_SUCCESS;
}

static
OS_Error_t
ConfigTool_XmlParserWriteDomainValue(
    OS_ConfigServiceLib_t* configLib,
    ConfigTool_ConfigServiceCounter_t* configCounter,
    const char* domainName)
{
    Debug_LOG_DEBUG("Domain: %s", domainName);

    // Increment the domain count after the parameters are written to it
    if (isNewDomain)
    {
        configCounter->domain_count++;
        isNewDomain = false;
    }

    OS_ConfigServiceLibTypes_Domain_t domain;
    ConfigTool_UtilInitializeDomain(&domain, domainName);
    OS_Error_t err = OS_ConfigServiceBackend_writeRecord(
                         &configLib->domainBackend,
                         configCounter->domain_count,
                         &domain,
                         sizeof(domain));
    if (err != OS_SUCCESS)
    {
        Debug_LOG_ERROR("OS_ConfigServiceBackend_writeRecord() failed with: %d", err);
        return err;
    }

    // Set flag so domain will be incremented after writing the parameters
    isNewDomain = true;

    return OS_SUCCESS;
}

static
OS_Error_t
ConfigTool_XmlParserWriteParamValue(
    OS_ConfigServiceLib_t* configLib,
    ConfigTool_ConfigServiceCounter_t* configCounter,
    const char* filePath)
{
    OS_Error_t err;
    OS_ConfigServiceLibTypes_Parameter_t parameter = {0};

    // Use _SetAll as access rights per component are currently not supported
    if (hasReadAccess)
    {
        OS_ConfigServiceAccessRights_SetAll(&parameter.readAccess);
        // Reset the read access state
        hasReadAccess = false;
    }

    if (hasWriteAccess)
    {
        OS_ConfigServiceAccessRights_SetAll(&parameter.writeAccess);
        // Reset the write access state
        hasWriteAccess = false;
    }

    Debug_LOG_DEBUG("Parameter: %s", xmlparams.paramName);

    switch (xmlparams.type)
    {
    case UINT32:
        Debug_LOG_DEBUG("Param Type uint32");
        err = ConfigTool_HandleInt32Parameter(configLib, &parameter, configCounter);
        if (err != OS_SUCCESS)
        {
            Debug_LOG_ERROR("ConfigTool_HandleInt32Parameter() failed with: %d", err);
            return err;
        }
        break;

    case UINT64:
        Debug_LOG_DEBUG("Param Type uint64");
        err = ConfigTool_HandleInt64Parameter(configLib, &parameter, configCounter);
        if (err != OS_SUCCESS)
        {
            Debug_LOG_ERROR("ConfigTool_HandleInt64Parameter() failed with: %d", err);
            return err;
        }
        break;

    case STRING:
        Debug_LOG_DEBUG("Param Type String, Value:%s", (char*)xmlparams.value);
        err = ConfigTool_HandleStringParameter(configLib, &parameter, configCounter);
        if (err != OS_SUCCESS)
        {
            Debug_LOG_ERROR("ConfigTool_HandleStringParameter() failed with: %d", err);
            return err;
        }
        break;

    case BLOB:
        Debug_LOG_DEBUG("Param Type Blob, Value:%s", (char*)xmlparams.value);
        err = ConfigTool_HandleBlobParameter(
                  configLib,
                  &parameter,
                  configCounter,
                  filePath);
        if (err != OS_SUCCESS)
        {
            Debug_LOG_ERROR("ConfigTool_HandleBlobParameter() failed with: %d", err);
            return err;
        }
        break;

    default:
        Debug_LOG_ERROR("Unsupported parameter type!");
        err = OS_ERROR_GENERIC;
        break;
    }

    return err;
} // end of ConfigTool_XmlParserWriteParamValue()


/* Exported functions --------------------------------------------------------*/
/* Parse through the XML elements found in the nodes and count them according to
 * their supported type. This aggregation is later used to create the
 * configuration backend
 */
void ConfigTool_XmlParserGetElementCount(
    xmlNode* a_node,
    ConfigTool_ConfigServiceCounter_t* configCounter,
    const char* dirPath)
{
    for (xmlNode* cur_node = a_node; cur_node; cur_node = cur_node->next)
    {
        if (cur_node->type == XML_ELEMENT_NODE)
        {
            Debug_LOG_DEBUG("Received Element name: %s", cur_node->name);
            char* cur_node_name = (char*)cur_node->name;

            if (strncmp(cur_node_name, ELEMENT_DOMAIN, sizeof(ELEMENT_DOMAIN)) == 0)
            {
                Debug_LOG_DEBUG("Found a domain");
                configCounter->domain_count++;
            }

            if (strncmp(cur_node_name, ELEMENT_TYPE, sizeof(ELEMENT_TYPE)) == 0)
            {
                char* node_content = (char*)xmlNodeGetContent(cur_node);
                switch (ConfigTool_XmlParserGetElementType(node_content))
                {
                case STRING:
                    Debug_LOG_DEBUG("Found a String parameter");
                    configCounter->param_count++;
                    configCounter->string_count++;
                    break;

                case BLOB:
                    Debug_LOG_DEBUG("Found a Blob parameter");
                    configCounter->param_count++;
                    ConfigTool_HandleBlobCount(cur_node, configCounter, dirPath);
                    break;

                case UINT32:
                case UINT64:
                    Debug_LOG_DEBUG("Found an Int parameter");
                    configCounter->param_count++;
                    break;

                case BADTYPE:
                    Debug_LOG_ERROR("No valid type found in XML, exiting...");
                    exit(1);

                default:
                    Debug_LOG_ERROR("Unsupported parameter type!");
                    break;
                }
                free(node_content);
            }
        }
        ConfigTool_XmlParserGetElementCount(cur_node->children, configCounter, dirPath);
    }
}

/* Parse the XML parameters. XML params are in the form of a tree structure due to
 * DOM and hence can be parsed recursively.
 */
void ConfigTool_XmlParserRun(
    OS_ConfigServiceLib_t* configLib,
    xmlNode* a_node,
    ConfigTool_ConfigServiceCounter_t* configCounter,
    const char* dirPath)
{
    OS_Error_t err;

    for (xmlNode* cur_node = a_node; cur_node; cur_node = cur_node->next)
    {
        if (cur_node->type == XML_ELEMENT_NODE)
        {
            Debug_LOG_DEBUG("Element name: %s", cur_node->name);
            char* cur_node_name = (char*)cur_node->name;

            Debug_LOG_DEBUG("Element content:%s", xmlNodeGetContent(cur_node));
            char* node_content = (char*)xmlNodeGetContent(cur_node);

            switch (ConfigTool_XmlParserGetXmlElement(cur_node_name))
            {
            case XML_ELEMENT_ACCESS_POLICY:
            case XML_ELEMENT_COMPONENT:
                ; // skip as per component access policy is not supported yet
                break;

            case XML_ELEMENT_READ:
                err = ConfigTool_XmlParserSetAccessSetting(node_content, &hasReadAccess);
                if (err != OS_SUCCESS)
                {
                    Debug_LOG_ERROR("ConfigTool_XmlParserSetAccessSetting() failed with %d", err);
                    free(node_content);
                    exit(1);
                }
                break;

            case XML_ELEMENT_WRITE:
                err = ConfigTool_XmlParserSetAccessSetting(node_content, &hasWriteAccess);
                if (err != OS_SUCCESS)
                {
                    Debug_LOG_ERROR("ConfigTool_XmlParserSetAccessSetting() failed with %d", err);
                    free(node_content);
                    exit(1);
                }
                break;

            case XML_ELEMENT_DOMAIN:
                Debug_LOG_DEBUG("Found a domain with name: %s", (char*)xmlNodeGetContent(
                                    cur_node->properties->children));
                char* domainName = (char*)xmlNodeGetContent(
                                       cur_node->properties->children);
                err = ConfigTool_XmlParserWriteDomainValue(
                          configLib,
                          configCounter,
                          domainName);
                if (err != OS_SUCCESS)
                {
                    Debug_LOG_ERROR("ConfigTool_XmlParserWriteDomainValue() failed with %d", err);
                    free(domainName);
                    free(node_content);
                    exit(1);
                }
                free(domainName);
                break;

            case XML_ELEMENT_PARAM_NAME:
                strncpy(xmlparams.paramName,
                        node_content,
                        // strlen() does not include NULL terminator
                        strlen(node_content) + 1);
                break;

            case XML_ELEMENT_TYPE:
                xmlparams.type = ConfigTool_XmlParserGetElementType(node_content);
                if (xmlparams.type < 0)
                {
                    Debug_LOG_ERROR("Unsupported parameter type!");
                    free(node_content);
                    exit(1);
                }
                break;

            case XML_ELEMENT_VALUE:
                xmlparams.value = (void*)node_content;
                err = ConfigTool_XmlParserWriteParamValue(configLib, configCounter, dirPath);
                if (err != OS_SUCCESS)
                {
                    Debug_LOG_ERROR("ConfigTool_XmlParserWriteParamValue() failed with %d", err);
                    free(node_content);
                    exit(1);
                }
                break;

            default:
                Debug_LOG_DEBUG("No known element found. Continuing...");
                break;
            }
            free(node_content);
        }
        // recursive function to parse and write params

        ConfigTool_XmlParserRun(configLib, cur_node->children, configCounter, dirPath);
    }
}
