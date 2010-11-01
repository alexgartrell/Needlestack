#ifndef _FILEINDEX_H_
#define _FILEINDEX_H_

typedef struct FileIndex FileIndex;

FileIndex *FileIndex_create();
FileIndex *FileIndex_load(const char *path);
void FileIndex_free(FileIndex *);
int FileIndex_store(FileIndex *, const char *path);

int FileIndex_lookup(FileIndex *, const char *uri, int *off, int *len);
int FileIndex_add(FileIndex *, const char *uri, int off, int len);

#endif
