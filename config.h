/**
 * Copyright (C) 2020, Hensoldt Cyber GmbH
 *
 * Configuration for the utilized libraries
 */


#pragma once

//-----------------------------------------------------------------------------
// Debug
//-----------------------------------------------------------------------------
#if !defined(NDEBUG)
#   define Debug_Config_STANDARD_ASSERT
#   define Debug_Config_ASSERT_SELF_PTR
#else
#   define Debug_Config_DISABLE_ASSERT
#   define Debug_Config_NO_ASSERT_SELF_PTR
#endif

#define Debug_Config_LOG_LEVEL                  Debug_LOG_LEVEL_INFO
#define Debug_Config_INCLUDE_LEVEL_IN_MSG
#define Debug_Config_LOG_WITH_FILE_LINE


//-----------------------------------------------------------------------------
// Memory
//-----------------------------------------------------------------------------
#define Memory_Config_USE_STDLIB_ALLOC


//-----------------------------------------------------------------------------
// FILESYSTEM / PARTITION MANAGER
//-----------------------------------------------------------------------------
// Set the max. size of the output image
#define NVM_DISK_SIZE       0x600000  //6 MiB of memory

// Partition ID by which a system component can mount the specific partition
#define PARTITION_ID        0

// FAT options are 12, 16 and 32
#define FORMAT_OPTION       FS_TYPE_FAT16

// Size of the partition needs to be chosen large enough to be able to hold the
// configuration parameter data
#define PARTITION_SIZE      0x300000 //3 MiB

// Max. partition per disk
#define PARTITION_COUNT                         1

// Max. file handle per partition
#define FILE_HANDLE_COUNT                       1

// FAT config
#define FILE_DIR_ENTRY_COUNT                    16      // only for (FAT12/FAT16)
#define FS_HEADER_SECTOR_COUNT                  1

#define CLUSTER_SIZE_FAT                        0x200   // size of cluster = 512 Byte
#define OFFSET_SECTORS_COUNT_FAT                3

typedef struct
{
    const char* partition_name;
    int partition_size;
    int block_size;
} Partition_config_t;

// Set the number of partitions
typedef struct
{
    Partition_config_t partition[1];
} Partition_cat_t;

// Set the size of the partitions
static const Partition_cat_t partition_conf =
{
    .partition[0].partition_name = "",
    .partition[0].partition_size = PARTITION_SIZE,
    .partition[0].block_size     = 512,
};

// internal defines
#define PM_CONF_ARRAY_SIZE(x)                   (sizeof(x)/sizeof(x[0]))

#define PARTITION_CONFIGURATION_AT(x)           partition_conf.partition[x]

#define GET_PROPERTY_PARTITION_NAME_AT(x)       PARTITION_CONFIGURATION_AT(x).partition_name
#define GET_PROPERTY_PARTITION_SIZE_AT(x)       PARTITION_CONFIGURATION_AT(x).partition_size
#define GET_PROPERTY_BLOCK_SIZE_AT(x)           PARTITION_CONFIGURATION_AT(x).block_size

// setup disk/partition
#define GET_PROPERTY_PARTITION_COUNT            PM_CONF_ARRAY_SIZE(partition_conf.partition)
#define GET_PROPERTY_PARTITION_NAME(x)          GET_PROPERTY_PARTITION_NAME_AT(x)
#define GET_PROPERTY_PARTITION_SIZE(x)          GET_PROPERTY_PARTITION_SIZE_AT(x)
#define GET_PROPERTY_BLOCK_SIZE(x)              GET_PROPERTY_BLOCK_SIZE_AT(x)