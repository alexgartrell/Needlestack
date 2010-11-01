#include <fileindex.h>

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
