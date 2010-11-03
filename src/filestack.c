#include <filestack.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fcntl.h>
#include <limits.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#include <dbg.h>

int FileStack_create(char *output, char *base, char *files[], int len) {
    FileInfo *info = NULL;
    FILE *file = NULL;
    FILE *contentfile = NULL;
    int nwritten = 0;
    unsigned int nread = 0;
    struct stat statbuf;
    int rcode = 0;
    int offset = 0;
    char normalized_base[PATH_MAX + 1];
    int normalized_base_len = 0;
    char normalized_uri[PATH_MAX + 1];
    int i = 0;
    
    check(output && base && files != NULL && len > 0, "Invalid arguments");
    
    check(realpath(base, normalized_base) != NULL, "Failed to normalize base");
    normalized_base[PATH_MAX] = '\0';
    normalized_base_len = strlen(normalized_base);

    info = calloc(sizeof(info[0]), len);
    check(info != NULL, "Out of memory");

    offset = sizeof(int) + len * sizeof(FileInfo);
    for(i = 0; i < len; i++) {
        rcode = stat(files[i], &statbuf);
        check(rcode == 0, "Unable to stat file %s", files[i]);

        info[i].offset = offset;
        info[i].size = statbuf.st_size;

        check(realpath(files[i], normalized_uri) != NULL,
              "Failed to normalize file %s", files[i]);
        check(!(strncmp(normalized_base, normalized_uri, normalized_base_len)),
              "file %s is out of base %s\n", normalized_uri, normalized_base);
                
        strncpy(info[i].uri, normalized_uri + normalized_base_len,
                MAX_URI_LENGTH);
        info[i].uri[MAX_URI_LENGTH] = '\0';

        offset += info[i].size;
    }


    file = fopen(output, "w");
    check(file != NULL, "Failed to open %s for writing", output);

    nwritten = fwrite(&len, sizeof(int), 1, file);
    check(nwritten == 1, "Failed to write to file");

    nwritten = fwrite(info, sizeof(info[0]), len, file);
    check(nwritten == len, "Failed to write to file");

    for(i = 0; i < len; i++) {
        contentfile = fopen(files[i], "r");
        check(contentfile != NULL, "Unable to open contentfile");
        char buff[4096];
        while((nread = fread(buff, 1, sizeof(buff), contentfile)) > 0) {
            check(fwrite(buff, 1, nread, file) == nread,
                  "Failed to write to file");
        }
        fclose(contentfile);
    }
    
    fclose(file);
    free(info);

    return 0;
error:
    if(file) fclose(file);
    if(info) free(info);

    return -1;
}

FileStack *FileStack_load(char *path) {
    FileStack *fs = NULL;
    FILE *file = NULL;
    int i = 0;
    int rcode = 0;
    struct stat stbuf;
    unsigned int contentstart = 0;

    fs = calloc(sizeof(*fs), 1);
    check(fs, "Out of memory");

    fs->index = IndexTrie_create();
    check(fs->index != NULL, "Failed to allocate fs->index");

    fs->fd = open(path, O_RDONLY);
    check(fs->fd >= 0, "Failed to open file %s", path);

    rcode = fstat(fs->fd, &stbuf);
    check(rcode == 0, "Could not stat %s", path);
    
    fs->filemem_len = stbuf.st_size;
    fs->filemem = mmap(NULL, fs->filemem_len, PROT_READ, MAP_FILE | MAP_SHARED, fs->fd, 0);
    check(fs->filemem != MAP_FAILED, "Failed to mmap %s", path);

    check(fs->filemem_len > sizeof(int), "File %s is malformed", path);

    memcpy(&fs->info_len, fs->filemem, sizeof(int));
    check(fs->info_len > 0, "Invalid info_len -- Broken File?");

    // Woo for type non-safety
    fs->info = (FileInfo *) (fs->filemem + sizeof(int));

    check(fs->filemem_len > sizeof(int) + fs->info_len * sizeof(FileInfo),
          "File %s is malformed", path);

    contentstart = sizeof(int) + fs->info_len * sizeof(FileInfo);

    // Further integrity checking
    for(i = 0; i < fs->info_len; i++) {
        unsigned int off = fs->info[i].offset;
        unsigned int size = fs->info[i].size;

        check(contentstart <= off && off < fs->filemem_len, "Malformed file");
        check(contentstart <= off + size && off + size <= fs->filemem_len,
              "Malformed file");
    }

    for(i = 0; i < fs->info_len; i++) {
        printf("Adding %s\n", fs->info[i].uri);
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
    check(fs && uri && offset && size, "Invalid arguments");

    idx = IndexTrie_lookup(fs->index, uri);
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
    if(fs) {
        IndexTrie_free(fs->index);
        if(fs->filemem) munmap(fs->filemem, fs->filemem_len);
        if(fs->fd > 0) close(fs->fd);
    }
}
