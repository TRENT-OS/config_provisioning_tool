/*
 *  Wrapper for the host FileSystem file operations
 *
 *  Copyright (C) 2020, HENSOLDT Cyber GmbH
 *
 */

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>

#include "lib_debug/Debug.h"
#include "ConfigTool_HostFsFile.h"


/* Private variables ---------------------------------------------------------*/
static FILE* fileHandle[MAX_FILE_HANDLES];


/* Public functions ----------------------------------------------------------*/
OS_Error_t
ConfigTool_HostFsFileOpen(
    OS_FileSystem_Handle_t          self,
    OS_FileSystemFile_Handle_t      hFile,
    const char*                     name,
    const OS_FileSystem_OpenMode_t  mode,
    const OS_FileSystem_OpenFlags_t flags)
{
    FILE* fp;
    char oMode[4];

    switch (mode)
    {
    case OS_FileSystem_OpenMode_RDONLY:
        strcpy(oMode, "rb");
        break;
    case OS_FileSystem_OpenMode_WRONLY:
        strcpy(oMode, "rb+");
        break;
    case OS_FileSystem_OpenMode_RDWR:
        if (flags & OS_FileSystem_OpenFlags_CREATE)
        {
            strcpy(oMode, "wb+");
            break;
        }
        strcpy(oMode, "rb+");
        break;
    default:
        Debug_LOG_ERROR("Unsupported file open mode");
        return OS_ERROR_INVALID_PARAMETER;
    }

    if ((fp = fopen(name, oMode)) == NULL)
    {
        Debug_LOG_ERROR("fopen() failed");
        return OS_ERROR_GENERIC;
    }

    fileHandle[hFile] = fp;

    return OS_SUCCESS;
}

OS_Error_t
ConfigTool_HostFsFileClose(
    OS_FileSystem_Handle_t     self,
    OS_FileSystemFile_Handle_t hFile)
{
    FILE* fp = fileHandle[hFile];
    int rc;

    if ((rc = fclose(fp)) < 0)
    {
        Debug_LOG_ERROR("fclose() failed with %d", rc);
        return OS_ERROR_GENERIC;
    }

    return OS_SUCCESS;
}

OS_Error_t
ConfigTool_HostFsFileRead(
    OS_FileSystem_Handle_t     self,
    OS_FileSystemFile_Handle_t hFile,
    const off_t                offset,
    const size_t               len,
    void*                      buffer)
{
    FILE* fp = fileHandle[hFile];
    size_t sz;
    int rc;

    if ((rc = fseek(fp, offset, SEEK_SET)) != 0)
    {
        Debug_LOG_ERROR("fseek() failed with %d", rc);
        fclose(fp);
        return OS_ERROR_ABORTED;
    }

    sz = fread(buffer, 1, len, fp);
    if (sz != len)
    {
        Debug_LOG_ERROR("fread() read %zu bytes instead of %zu bytes",
                        sz, len);
        fclose(fp);
        return OS_ERROR_ABORTED;
    }

    return OS_SUCCESS;
}

OS_Error_t
ConfigTool_HostFsFileWrite(
    OS_FileSystem_Handle_t     self,
    OS_FileSystemFile_Handle_t hFile,
    const off_t                offset,
    const size_t               len,
    const void*                buffer)
{
    FILE* fp = fileHandle[hFile];
    size_t sz;
    int rc;

    if ((rc = fseek(fp, offset, SEEK_SET)) != 0)
    {
        Debug_LOG_ERROR("fseek() failed with %d", rc);
        fclose(fp);
        return OS_ERROR_ABORTED;
    }

    sz = fwrite(buffer, 1, len, fp);
    if (sz != len)
    {
        Debug_LOG_ERROR("fwrite() wrote %zu bytes instead of %zu bytes",
                        sz, len);
        return OS_ERROR_ABORTED;
    }

    return OS_SUCCESS;
}

OS_Error_t
ConfigTool_HostFsFileDelete(
    OS_FileSystem_Handle_t self,
    const char*            name)
{
    int rc;

    if ((rc = remove(name)) < 0)
    {
        Debug_LOG_ERROR("remove() failed with %d", rc);
        return OS_ERROR_GENERIC;
    }

    return OS_SUCCESS;
}

OS_Error_t
ConfigTool_HostFsFileGetSize(
    OS_FileSystem_Handle_t self,
    const char*            name,
    off_t*                 sz)
{
    FILE* fp;
    int rc;

    if ((fp = fopen(name, "rb")) == NULL)
    {
        Debug_LOG_ERROR("fopen() failed");
        return OS_ERROR_GENERIC;
    }

    if ((rc = fseek(fp, 0L, SEEK_END)) != 0)
    {
        Debug_LOG_ERROR("fseek() failed with %d", rc);
        fclose(fp);
        return OS_ERROR_ABORTED;
    }

    if ((*sz = ftell(fp)) < 0)
    {
        Debug_LOG_ERROR("ftell() failed with %lu", *sz);
        fclose(fp);
        return OS_ERROR_ABORTED;
    }

    // Reset the offset again
    if ((rc = fseek(fp, 0L, SEEK_SET)) != 0)
    {
        Debug_LOG_ERROR("fseek() failed with %d", rc);
        fclose(fp);
        return OS_ERROR_ABORTED;
    }

    if ((rc = fclose(fp)) < 0)
    {
        Debug_LOG_ERROR("fclose() failed with %d", rc);
        return OS_ERROR_GENERIC;
    }

    return OS_SUCCESS;
}
