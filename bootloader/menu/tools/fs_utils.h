#ifndef _FS_UTILS_H_
#define _FS_UTILS_H_

#include "utils/types.h"
#include "libs/fatfs/ff.h"
#include "storage/sdmmc.h"
#include "storage/sdmmc_driver.h"
#include "utils/sprintf.h"

extern sdmmc_t g_sd_sdmmc;
extern sdmmc_storage_t g_sd_storage;
extern FATFS g_sd_fs;
extern bool sd_mounted;

bool delete_file(const char* filename);

void *sd_4_file_read2(const char *path);
char *read_file_string(const char *path);
int sd_save_2_file(void *buf, u32 size,const  char *filename);
bool sd_file_exists(const char* filename);
void copyfile(const char* source, const char* target);
void copyfileparam(char* param, char* source, char* target);
void copy_folder(char* sourse_folder, char* dest_folder);
bool sd_file_size(char *path);

void moverall(char *directory, char *destdir, char *filet, char *coment);
void f_move (char* file1, char* file2);
void copyarall(char* directory, char* destdir, char* filet, char* coment);
void deleteall(char* directory, char* filet, char* coment);
bool HasArchBit(const char *directory);
void Killflags(char *directory);

char *listfil(const char *directory, const char *pattern, bool includeHiddenFiles);
char *listfol(const char *directory, const char *pattern, bool includeHiddenFiles);

#endif
