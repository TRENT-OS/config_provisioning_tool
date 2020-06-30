/**
 * Module to initialize the filesystem and partition manager
 *
 * Copyright (C) 2019-2020, Hensoldt Cyber GmbH
 *
 */

/* Includes ------------------------------------------------------------------*/
#include "LibDebug/Debug.h"

#include "ConfigTool_Backend.h"

/* Defines -------------------------------------------------------------------*/
#define PARTITION_ID        0
#define FORMAT_OPTION       FS_TYPE_FAT16

/* Exported functions --------------------------------------------------------*/
OS_Error_t ConfigTool_BackendInit(
    hPartition_t* phandle,
    const char* outFileName)
{
    OS_PartitionManagerDataTypes_PartitionData_t pm_partition_data;

    // Create objects for Config NVM with the file I/0 callback functions
    static FileNVM fileNvm;

    if (!FileNVM_ctor(&fileNvm, outFileName))
    {
        Debug_LOG_ERROR("Failed to initialize FileNVM!");
        return OS_ERROR_GENERIC;
    }

    OS_Error_t err = OS_PartitionManager_init(&fileNvm);
    if (err != OS_SUCCESS)
    {
        Debug_LOG_ERROR("OS_PartitionManager_init: %d", err);
        return OS_ERROR_GENERIC;
    }

    // Create partitions with info received from the partition manager
    err = OS_PartitionManager_getInfoPartition(PARTITION_ID, &pm_partition_data);

    Debug_LOG_DEBUG("Partition Name:%s, Partition ID:%d, Partition Size:%ld, "
                    "Block Size:%d",
                    pm_partition_data.partition_name,
                    pm_partition_data.partition_id,
                    pm_partition_data.partition_size,
                    pm_partition_data.block_size);
    if (err != OS_SUCCESS)
    {
        Debug_LOG_ERROR("Fail to get partition information from partition manager: %d!",
                        err);
        return OS_ERROR_GENERIC;
    }

    err = OS_Filesystem_init(pm_partition_data.partition_id, 0);
    if (err != OS_SUCCESS)
    {
        Debug_LOG_ERROR("Fail to initialize filesystem: %d!", err);
        return OS_ERROR_GENERIC;
    }

    if ( (*phandle = OS_Filesystem_open(pm_partition_data.partition_id)) < 0)
    {
        Debug_LOG_ERROR("Fail to open partition: %d!", pm_partition_data.partition_id);
        return OS_ERROR_GENERIC;
    }

    if (OS_Filesystem_create(
            *phandle,
            FORMAT_OPTION,
            pm_partition_data.partition_size,
            0,  // default value: size of sector:   512
            0,  // default value: size of cluster:  512
            0,  // default value: reserved sectors count: FAT12/FAT16 = 1; FAT32 = 3
            0,  // default value: count file/dir entries: FAT12/FAT16 = 16; FAT32 = 0
            0,  // default value: count header sectors: 512
            FS_PARTITION_OVERWRITE_CREATE)
        != OS_SUCCESS)
    {
        Debug_LOG_ERROR("Fail to create filesystem on partition: %d!",
                        pm_partition_data.partition_id);
        return OS_ERROR_GENERIC;
    }

    if (OS_Filesystem_mount(*phandle) != OS_SUCCESS)
    {
        return OS_ERROR_GENERIC;
    }

    return OS_SUCCESS;
}
