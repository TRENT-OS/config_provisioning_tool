/**
 * Copyright (C) 2020, Hensoldt Cyber GmbH
 *
 * @addtogroup Config Provisioning Tool
 * @{
 *
 * @file ConfigTool_HostFsFile.h
 *
 * @brief Wrapper for the host FileSystem file operations
 */

#pragma once

/* Includes ------------------------------------------------------------------*/
#include "OS_FileSystem.h"
#include "OS_FileSystem_int.h"


/* Public functions ----------------------------------------------------------*/
/**
 * @brief Opens a requested file by the passed file name
 *
 * @param self filesystem handle
 * @param hFile file handle
 * @param name file name
 * @param mode mode when opening a file
 * @param flags flags when opening a file
 * @retval OS_SUCCESS if the requested file was opened successfully
 * @retval OS_ERROR_GENERIC if the file could not be opened successfully
 * @retval OS_ERROR_INVALID_PARAMETER if the passed mode is not supported
 */
OS_Error_t
ConfigTool_HostFsFileOpen(
    OS_FileSystem_Handle_t          self,
    OS_FileSystemFile_Handle_t      hFile,
    const char*                     name,
    const OS_FileSystem_OpenMode_t  mode,
    const OS_FileSystem_OpenFlags_t flags);

/**
 * @brief Closes the file assigned to the passed file handle
 *
 * @param self filesystem handle
 * @param hFile file handle
 * @retval OS_SUCCESS if the requested file was closed successfully
 * @retval OS_ERROR_GENERIC if the file could not be closed successfully
 */
OS_Error_t
ConfigTool_HostFsFileClose(
    OS_FileSystem_Handle_t     self,
    OS_FileSystemFile_Handle_t hFile);

/**
 * @brief Reads the requested length from the file assigned to the file handle
 * into the passed buffer
 *
 * @param self filesystem handle
 * @param hFile file handle
 * @param offset offset to read from
 * @param len length to read
 * @param buffer buffer to write the read data to
 * @retval OS_SUCCESS if the read operation succeeded
 * @retval OS_ERROR_ABORTED if the read operation failed
 */
OS_Error_t
ConfigTool_HostFsFileRead(
    OS_FileSystem_Handle_t     self,
    OS_FileSystemFile_Handle_t hFile,
    const off_t                offset,
    const size_t               len,
    void*                      buffer);

/**
 * @brief Writes the requested length from the passed buffer into the file
 * assigned to the file handle
 *
 * @param self filesystem handle
 * @param hFile file handle
 * @param offset offset to write to
 * @param len length to write
 * @param buffer buffer containing the data to write to the file
 * @retval OS_SUCCESS if write operation succeeded
 * @retval OS_ERROR_ABORTED if the write operation failed
 */
OS_Error_t
ConfigTool_HostFsFileWrite(
    OS_FileSystem_Handle_t     self,
    OS_FileSystemFile_Handle_t hFile,
    const off_t                offset,
    const size_t               len,
    const void*                buffer);

/**
 * @brief Deletes the file identified by the passed file name
 *
 * @param self filesystem handle
 * @param name file name
 * @retval OS_SUCCESS if the requested file was deleted successfully
 * @retval OS_ERROR_GENERIC if the file could not be deleted successfully
 */
OS_Error_t
ConfigTool_HostFsFileDelete(
    OS_FileSystem_Handle_t self,
    const char*            name);

/**
 * @brief Returns the size of the file identified by the passed file name
 *
 * @param self filesystem handle
 * @param name file name
 * @param sz pointer to set the file size to
 * @retval OS_SUCCESS if the requested file was opened successfully
 * @retval OS_ERROR_GENERIC if the file could not be opened or closed successfully
 * @retval OS_ERROR_ABORTED if adjusting the offset failed
 */
OS_Error_t
ConfigTool_HostFsFileGetSize(
    OS_FileSystem_Handle_t self,
    const char*            name,
    off_t*                 sz);
