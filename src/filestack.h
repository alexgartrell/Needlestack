#ifndef _FILESTACK_H
#define _FILESTACK_H

#include <indextrie.h>

typedef struct FileInfo {
    char uri[MAX_URI_LENGTH];
    int offset;
    int size;
} FileInfo;

typedef struct FileStack {
    indextrie *index;
    int fd;
    int info_len;
    FileInfo info[0];
} FileStack;

int FileStack_create(char *output, char *files[], int len);

FileStack *FileStack_load(char *path);
int FileStack_lookup(FileStack *fs, char *uri, int *offset, int *size);
void FileStack_free(FileStack *fs);

#endif
