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
#include <libxml/parser.h>
#include <libxml/tree.h>
#include "ConfigTool_ConfigService.h"
#include "ConfigTool_Util.h"


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
    ConfigTool_ConfigServiceCounter_t* configCounter,
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
    ConfigTool_ConfigServiceCounter_t* configCounter,
    const char* dirPath);
