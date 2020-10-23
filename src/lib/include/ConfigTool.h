/**
 * Copyright (C) 2020, Hensoldt Cyber GmbH
 *
 * @addtogroup Config Provisioning Tool
 * @{
 *
 * @file ConfigTool.h
 *
 * @brief Contains core datatypes, defines and helper functions used by the
 * Config Tool.
 *
 */

#pragma once

/* Includes ------------------------------------------------------------------*/
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <string.h>

#include "OS_Error.h"
#include "OS_ConfigService.h"


/* Defines -------------------------------------------------------------------*/
#define USAGE_STRING                             \
    printf("Usage: cpt -i [<path-to-xml_file>] " \
           "-o [<output_nvm_file_name>] "        \
           "-t [<filesystem_type>]\n")

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

#define NUMELEMENTS ARRAY_SIZE(xmlElementLookupTable)
#define NKEYS       ARRAY_SIZE(lookupTable)

// below defines to parse the requested filesystem type
#define FAT                       "FAT"
#define SPIFFS                    "SPIFFS"

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
} ConfigTool_ParameterTypes_t;

/**
 * @brief Utilized to map the parsed parameter types to the supported parameter types
 *
 */
typedef struct
{
    const char*  key;
    ConfigTool_ParameterTypes_t parameterType;
} ConfigTool_Types_t;

/**
 * @brief Counter to keep track of the parsed number of elements
 *
 */
typedef struct
{
    unsigned int param_count;    /**< number of params present  (from XML) */
    unsigned int domain_count;   /**< number of domains present (from XML) */
    unsigned int string_count;   /**< number of strings present (from XML) */
    unsigned int blob_count;     /**< number of blobs present   (from XML) */
} ConfigTool_ConfigCounter_t;
