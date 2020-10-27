/* Copyright (C) 2020, HENSOLDT Cyber GmbH */

/**
 * @file
 * @brief Wrapper for the host FileSystem file operations.
 *
 * @ingroup ConfigTool_HostFs
 *
 */
#pragma once

/* Includes ------------------------------------------------------------------*/
#include "OS_FileSystem.h"
#include "OS_FileSystem_int.h"


/* Public functions ----------------------------------------------------------*/
/**
 * @brief Opens a requested file by the passed file name.
 *
 * @retval OS_SUCCESS - if the requested file was opened successfully
 * @retval OS_ERROR_GENERIC - if the file could not be opened successfully
 * @retval OS_ERROR_INVALID_PARAMETER - if the passed mode is not supported
 */
OS_Error_t
ConfigTool_HostFsFileOpen(
    OS_FileSystem_Handle_t          self,  //!< [in] Filesystem handle
    OS_FileSystemFile_Handle_t      hFile, //!< [in] File handle
    const char*                     name,  //!< [in] File name
    const OS_FileSystem_OpenMode_t  mode,  //!< [in] Mode when opening a file
    const OS_FileSystem_OpenFlags_t flags  //!< [in] Flags when opening a file
);

/**
 * @brief Closes the file assigned to the passed file handle.
 *
 * @retval OS_SUCCESS - if the requested file was closed successfully
 * @retval OS_ERROR_GENERIC - if the file could not be closed successfully
 */
OS_Error_t
ConfigTool_HostFsFileClose(
    OS_FileSystem_Handle_t     self, //!< [in] Filesystem handle
    OS_FileSystemFile_Handle_t hFile //!< [in] File handle
);

/**
 * @brief Reads the requested length from the file assigned to the file handle
 * into the passed buffer.
 *
 * @retval OS_SUCCESS - if the read operation succeeded
 * @retval OS_ERROR_ABORTED - if the read operation failed
 */
OS_Error_t
ConfigTool_HostFsFileRead(
    OS_FileSystem_Handle_t     self,   //!< [in] Filesystem handle
    OS_FileSystemFile_Handle_t hFile,  //!< [in] File handle
    const off_t                offset, //!< [in] Offset to read from
    const size_t               len,    //!< [in] Length to read
    void*                      buffer  //!< [out] Buffer to write the read data
);

/**
 * @brief Writes the requested length from the passed buffer into the file
 * assigned to the file handle.
 *
 * @retval OS_SUCCESS - if write operation succeeded
 * @retval OS_ERROR_ABORTED - if the write operation failed
 */
OS_Error_t
ConfigTool_HostFsFileWrite(
    OS_FileSystem_Handle_t     self,   //!< [in] Filesystem handle
    OS_FileSystemFile_Handle_t hFile,  //!< [in] File handle
    const off_t                offset, //!< [in] Offset to write to
    const size_t               len,    //!< [in] Length to write
    const void*                buffer  //!< [in] Buffer filled with data to write
);

/**
 * @brief Deletes the file identified by the passed file name.
 *
 * @retval OS_SUCCESS - if the requested file was deleted successfully
 * @retval OS_ERROR_GENERIC - if the file could not be deleted successfully
 */
OS_Error_t
ConfigTool_HostFsFileDelete(
    OS_FileSystem_Handle_t self, //!< [in] Filesystem handle
    const char*            name  //!< [in] File name
);

/**
 * @brief Returns the size of the file identified by the passed file name.
 *
 * @retval OS_SUCCESS - if the requested file was opened successfully
 * @retval OS_ERROR_GENERIC - if the file could not be opened or closed
 * successfully
 * @retval OS_ERROR_ABORTED - if adjusting the offset failed
 */
OS_Error_t
ConfigTool_HostFsFileGetSize(
    OS_FileSystem_Handle_t self, //!< [in] Filesystem handle
    const char*            name, //!< [in] File name
    off_t*                 sz    //!< [out] Pointer to set the file size to
);
