#include <filestack.h>

#include <stdlib.h>

#include <dbg.h>

int FileStack_create(char *output, char *files[], int len) {
    FileStack *fs = NULL;
    FILE *file = NULL;
    int nwritten = 0;

    check(output && files != NULL && len > 0, "Invalid arguments");
    
    FileInfo *info = calloc(sizeof(*fi), len);
    check(info != NULL, "Out of memory");

    int offset = sizeof(int) + len * sizeof(FileInfo);
    for(i = 0; i < len; i++) {
        
    }
        


    file = fopen(output, "w");
    check(file != NULL, "Failed to open %s for writing", output);

    nwritten = fwrite(&len, sizeof(int), 1, file);
    check(nwritten == 1, "Failed to write to file");

    
    
    fclose(file);

    return 0;
error:
    if(file) fclose(file);
    FileStack_free(fs);

    return -1;
}

FileStack *FileStack_load(char *path) {
    FileStack *fs = NULL;
    FILE *file = NULL;
    int info_len = 0;
    int nread = 0;
    int i = 0;
    int rcode = 0;

    file = fopen(path, "r");
    check(file != NULL, "Failed to open file");

    nread = fread(&info_len, sizeof(int), 1, file);
    check(nread == 1, "Failed to read from file");
    check(info_len > 0, "Invalid info_len -- Broken File?");

    fs = calloc(sizeof(*fs) + (sizeof(fs->info[0]) * info_len), 1);
    check(fs != NULL, "Out of memory");

    fs->index = IndexTrie_create();
    check(fs->index != NULL, "Failed to allocate fs->index");

    fs->info_len = info_len;

    nread = fread(fs->info, sizeof(fs->info[1]), fs->info_len, file):
    check(nread == info_len, "Failed to read all file info from file");

    // Done with this now, so it's ok to close it up
    fclose(file);
    
    for(i = 0; i < fs->info_len; i++) {
        rcode = IndexTrie_add(fs->index, fs->info[i].uri, i);
        check(rcode == 0, "Failed to construct index");
    }

    return fs;
error:
    FileStack_free(fs);
    if(file) fclose(file);
    return NULL;
}

int FileStack_lookup(FileStack *fs, char *uri, int *offset, int *size) {
    int idx = -1;
    char normalized_uri[PATH_MAX];
    char *rptr = NULL;

    check(fs && uri && offset && size, "Invalid arguments");

    rptr = realpath(uri, normalized_uri);
    check(rptr != NULL, "realpath failed?");

    idx = IndexTrie_lookup(fs->index, normalized_uri);
    check(idx < fs->info_len, "index returned is out of range %d", idx);

    // No need to alarm anyone if we get a simple 404
    if(idx < 0) return -2;
    
    *offset = fs->info[idx].offset;
    *size = fs->info[idx].size;
    return 0;

error:
    return -1;
}

void FileStack_free(FileStack *fs) {

}
