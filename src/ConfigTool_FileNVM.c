/*
 *  Map the NVM callback funtions to file I/O wrappers
 *
 *  Copyright (C) 2019-2020, Hensoldt Cyber GmbH
 *
 */

/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "ConfigTool_FileNVM.h"


/* Private variables ---------------------------------------------------------*/
static const Nvm_Vtable FileNvm_vtable =
{
    .read       = FileNVM_read,
    .erase      = FileNVM_erase,
    .getSize    = FileNVM_getSize,
    .write      = FileNVM_write,
    .dtor       = FileNVM_dtor
};


/* Public functions ----------------------------------------------------------*/
bool FileNVM_ctor(
    FileNVM*    self,
    const char* name)
{
    Debug_ASSERT_SELF(self);
    Nvm* nvm = FileNVM_TO_NVM(self);

    nvm->vtable = &FileNvm_vtable;
    self->name = name;
    self->fp = fopen(name, "rb+");

    if (self->fp == NULL)
    {
        self->fp = fopen(name, "wb");
        fseek(self->fp, NVM_DISK_SIZE, SEEK_SET);
        fputc('\0', self->fp);
        fclose(self->fp);
    }
    else
    {
        // File already exists. Erase any previous data in the partition
        size_t erasedLength = FileNVM_erase(nvm, 0, NVM_DISK_SIZE);
        if (erasedLength != NVM_DISK_SIZE)
        {
            Debug_LOG_ERROR("Couldn't format entire file. Erased %zu of %d requested bytes",
                            erasedLength, NVM_DISK_SIZE);
            return false;
        }
        fclose(self->fp);
    }

    self->fp = fopen(name, "rb+");
    if (self->fp == NULL)
    {
        Debug_LOG_ERROR("%s: fopen failed!", __func__);
        return false;
    }

    return true;
}

size_t FileNVM_write(
    Nvm* nvm, size_t addr,
    void const* buffer,
    size_t length)
{
    FileNVM* self = (FileNVM*) nvm;
    Debug_ASSERT_SELF(self);
    Debug_ASSERT(buffer != NULL);
    size_t writtenTotal = 0;

    if (fseek(self->fp, addr, SEEK_SET))
    {
        Debug_LOG_ERROR("%s: fseek failed!", __func__);
        return -1;
    }
    writtenTotal = fwrite(buffer, sizeof(char), length, self->fp);

    return writtenTotal;
}

size_t FileNVM_read(
    Nvm* nvm, size_t addr,
    void* buffer,
    size_t length)
{
    FileNVM* self = (FileNVM*) nvm;
    Debug_ASSERT_SELF(self);
    Debug_ASSERT(buffer != NULL);
    size_t readTotal = 0;

    if (fseek(self->fp, addr, SEEK_SET))
    {
        Debug_LOG_ERROR("%s: fseek failed!", __func__);
        return -1;
    }
    readTotal = fread(buffer, sizeof(char), length, self->fp);

    return readTotal;
}

size_t FileNVM_erase(
    Nvm*   nvm,
    size_t addr,
    size_t length)
{
    FileNVM* self = (FileNVM*) nvm;
    Debug_ASSERT_SELF(self);
    size_t erasedTotal = 0;

    void* buffer = calloc(length, sizeof(char));
    if (NULL == buffer)
    {
        Debug_LOG_ERROR("Failed to allocate %zu bytes!", length);
        return -1;
    }

    if (fseek(self->fp, addr, SEEK_SET))
    {
        free(buffer);
        Debug_LOG_ERROR("%s: fseek failed!", __func__);
        return -1;
    }
    erasedTotal = fwrite(buffer, sizeof(char), length, self->fp);

    free(buffer);

    return erasedTotal;
}

size_t FileNVM_getSize(
    Nvm* nvm)
{
    return NVM_DISK_SIZE;
}

void FileNVM_dtor(
    Nvm* nvm)
{
    FileNVM* self = (FileNVM*) nvm;
    Debug_ASSERT_SELF(self);
    fclose(self->fp);
}

