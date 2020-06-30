/**
 * Copyright (C) 2020, Hensoldt Cyber GmbH
 *
 * @addtogroup Config Provisioning Tool
 * @{
 *
 * @file ConfigTool_FileNVM.h
 *
 * @brief NVM wrapper to make use of the native file I/O operations.
 *
 */

#pragma once

/* Includes ------------------------------------------------------------------*/
#include "LibMem/Nvm.h"


/* Exported macro ------------------------------------------------------------*/

#define FileNVM_TO_NVM(self) (&(self)->parent)


/* Exported types ------------------------------------------------------------*/

typedef struct FileNVM FileNVM;

struct FileNVM
{
    Nvm parent;
    const char* name;
    FILE* fp;
};


/* Exported functions ------------------------------------------------------- */
/**
 * @brief constructor
 *
 * @param self
 * @param name
 * @return true
 * @return false
 */
bool
FileNVM_ctor(
    FileNVM*    self,
    const char* name);

/**
 * @brief static implementation of virtual method NVM_write()
 *
 * @param nvm
 * @param addr
 * @param buffer
 * @param length
 * @return size_t
 */
size_t
FileNVM_write(
    Nvm* nvm, size_t addr,
    void const* buffer,
    size_t length);

/**
 * @brief static implementation of virtual method NVM_read()
 *
 * @param nvm
 * @param addr
 * @param buffer
 * @param length
 * @return size_t
 */
size_t
FileNVM_read(
    Nvm* nvm, size_t addr,
    void* buffer,
    size_t length);

/**
 * @brief static implementation of the erase method that is required
 * when working with flash
 *
 * @param nvm
 * @param addr
 * @param length
 * @return size_t
 */
size_t
FileNVM_erase(
    Nvm*   nvm,
    size_t addr,
    size_t length);

/**
 * @brief static implementation of virtual method NVM_getSize()
 *
 * @param nvm
 * @return size_t
 */
size_t
FileNVM_getSize(
    Nvm* nvm);

/**
 * @brief destructor
 *
 * @param nvm
 */
void
FileNVM_dtor(
    Nvm* nvm);

///@}
