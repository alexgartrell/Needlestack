#include <fileindex.h>

#include <stdio.h>
#include <stdlib.h>

#define MAX_URI_LENGTH 1024
#define MIN_FILEINFO_ENTRIES 10

typedef struct FileInfo {
    char uri[MAX_URI_LENGTH];
    int off;
    int len;
} FileInfo;

struct FileIndex {
    IndexTrie *index_trie;
    int num_entries;
    FileInfo entries[MIN_FILEINFO_ENTRIES];
};

FileIndex *FileIndex_create() {
    FileIndex *fi = NULL;

    fi = calloc(sizeof(*fi), 1);
    
    fi->num_entries = MIN_FILEINFO_ENTRIES;
}

FileIndex *FileIndex_load(const char *path) {
    return NULL;
}

void FileIndex_free(FileIndex *fi) {

}

int FileIndex_store(FileIndex *, const char *path);

int FileIndex_lookup(FileIndex *, const char *uri, int *off, int *len);
int FileIndex_add(FileIndex *, const char *uri, int off, int len);
