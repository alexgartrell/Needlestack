#ifndef _FILEINDEX_H
#define _FILEINDEX_H

#define NUM_CHARS (1 << (sizeof(char) * 8))

typedef struct FileInfo {
    void *ptr;
    unsigned int size;
} FileInfo;

typedef struct Node {
    struct Node *nodes[NUM_CHARS];
    int occupied;
    FileInfo info;
} Node;

typedef struct FileIndex {
    Node *trie;
} FileIndex;


FileIndex *FileIndex_create();
void FileIndex_free(FileIndex *fi);
int FileIndex_add(FileIndex *fi, const char *name, FileInfo *info);
FileInfo *FileIndex_lookup(FileIndex *fi, const char *name);


#endif
