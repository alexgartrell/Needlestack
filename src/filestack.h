#ifndef _FILESTACK_H
#define _FILESTACK_H

#include <indextrie.h>

#define MAX_URI_LENGTH 256

typedef struct FileInfo {
    char uri[MAX_URI_LENGTH + 1];
    unsigned int offset;
    unsigned int size;
} FileInfo;

typedef struct FileStack {
    IndexTrie *index;
    int fd;
    void *filemem;
    unsigned int filemem_len;
    int info_len;
    FileInfo *info;
} FileStack;

int FileStack_create(char *output, char *base, char *files[], int len);

FileStack *FileStack_load(char *path);
int FileStack_lookup(FileStack *fs, char *uri, int *offset, int *size);
void FileStack_free(FileStack *fs);

#endif
