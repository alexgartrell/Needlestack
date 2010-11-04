#ifndef _FILESTACK_H
#define _FILESTACK_H

#include <fileindex.h>

#define MAX_URI_LENGTH 256

typedef struct FileStack {
    FileIndex *index;
    int fd;
    void *filemem;
    unsigned int filemem_len;
} FileStack;

int FileStack_create(char *output, char *base, char *files[], int len);

FileStack *FileStack_load(char *path);
int FileStack_lookup(FileStack *fs, char *uri, void **ptr, 
                     unsigned int *size);
void FileStack_free(FileStack *fs);

#endif
