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

FileStack *FileStack_load(char *path);
FileInfo *FileStack_lookup(FileStack *fs, char *uri);
void FileStack_free(FileStack *fs);

#endif
