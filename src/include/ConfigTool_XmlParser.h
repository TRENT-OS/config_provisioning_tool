/**
 * Copyright (C) 2020, Hensoldt Cyber GmbH
 *
 * @addtogroup Config Provisioning Tool
 * @{
 *
 * @file ConfigTool_XmlParser.h
 *
 * @brief Collection of functions to parse an XML file into an image usable by the
 * Configuration Service library
 *
 */

#pragma once

/* Includes ------------------------------------------------------------------*/
#include "ConfigTool.h"
#include "ConfigTool_Util.h"


/* Exported types/defines/enums ----------------------------------------------*/
/**
 * @brief Utilized to map the parsed XML elements to the supported element types
 *
 */
typedef struct
{
    const char*  element;
    unsigned int val;
} ConfigTool_XmlParserElements_t;

/**
 * @brief Holds the relevant objects of an XML parameter elemnent to write it to
 * the configuration backend
 */
typedef struct
{
    ConfigTool_ParameterTypes_t type;
    char  paramName[OS_CONFIG_LIB_PARAMETER_NAME_LEN]; /**< name of params present (from XML) */
    void* value;    /**< value of params present (from XML) */
} ConfigTool_XmlParserParams_t;

/**
 * @brief Holds all currently supported XML element types used in the
 *
 */
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


/* Exported functions --------------------------------------------------------*/
/**
 * @brief Iterates over the XML nodes and writes the values of the domains and
 * parameter elements to the configuration library instance
 *
 * @param configLib pointer to an initiliazed configuration library instance
 * @param a_node pointer to the first XML node of the document
 * @param configCounter pointer to the ConfigCounter object
 */
void ConfigTool_XmlParserRun(
    OS_ConfigServiceLib_t* configLib,
    xmlNode* a_node,
    ConfigTool_ConfigCounter_t* configCounter,
    const char* dirPath);

/**
 * @brief Iterates over the XML nodes and counts the number of domains and
 * different parameter elements that need to be written to the config
 * provisioning image
 *
 * @param a_node pointer to the first XML node of the document
 * @param configCounter pointer to the ConfigCounter object
 */
void ConfigTool_XmlParserGetElementCount(
    xmlNode* a_node,
    ConfigTool_ConfigCounter_t* configCounter,
    const char* dirPath);



