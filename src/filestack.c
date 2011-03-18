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

#define next_int(ptr, off, len, x) {                    \
        check(off + sizeof(int) <= len, "Too short");   \
        memcpy(&x, ptr + off, sizeof(int));             \
        off += sizeof(int);                             \
    }

#define next_ptr(ptr, off, len, p, size) {      \
        next_int(ptr, off, len, size);          \
        check(off + size <= len, "Too short");  \
        p = ptr + off;                          \
        off += size;                            \
    }

FileStack *FileStack_load(char *path) {
    FileStack *fs = NULL;
    FileInfo info;
    FILE *file = NULL;
    int rcode = 0;
    struct stat stbuf;

    fs = calloc(sizeof(*fs), 1);
    check(fs, "Out of memory");

    fs->index = FileIndex_create();
    check(fs->index != NULL, "Failed to allocate fs->index");

    fs->fd = open(path, O_RDONLY);
    check(fs->fd >= 0, "Failed to open file %s", path);

    rcode = fstat(fs->fd, &stbuf);
    check(rcode == 0, "Could not stat %s", path);
    
    fs->filemem_len = stbuf.st_size;
    fs->filemem = mmap(NULL, fs->filemem_len, PROT_READ, MAP_FILE | MAP_SHARED,
                       fs->fd, 0);
    check(fs->filemem != MAP_FAILED, "Failed to mmap %s", path);

    check(fs->filemem_len > sizeof(int), "File %s is malformed", path);

    unsigned int file_off = 0;
    while(file_off < fs->filemem_len) {
        char *key;
        unsigned int key_size, size;

        next_ptr(fs->filemem, file_off, fs->filemem_len, key, key_size);

        if(!strncmp("end_stack", key, key_size)) {
            break;
        }
        else if(!strncmp("end_file", key, key_size)) {
            printf("Adding\n");
            printf("\turi = %s\n\tdata = %p\n\tcontent_type = %s\n", info.uri, info.ptr, info.content_type);
            rcode = FileIndex_add(fs->index, info.uri, &info);
            check(rcode == 0, "Failed to construct index");
            memset(&info, 0, sizeof(info));
        }
        else if(!strncmp("uri", key, key_size)) {
            next_ptr(fs->filemem, file_off, fs->filemem_len, info.uri, size);
        }
        else if(!strncmp("data", key, key_size)) {
            next_ptr(fs->filemem, file_off, fs->filemem_len, info.ptr,
                     info.size);
        }
        else if(!strncmp("content-type", key, key_size)) {
            next_ptr(fs->filemem, file_off, fs->filemem_len, info.content_type,
                     size);
        }
    }

    return fs;
error:
    FileStack_free(fs);
    if(file) fclose(file);
    return NULL;
}

FileInfo *FileStack_lookup(FileStack *fs, char *uri) {
    check(fs && uri, "Invalid arguments");
    return FileIndex_lookup(fs->index, uri);

error:
    return NULL;
}

void FileStack_free(FileStack *fs) {
    if(fs) {
        FileIndex_free(fs->index);
        if(fs->filemem) munmap(fs->filemem, fs->filemem_len);
        if(fs->fd > 0) close(fs->fd);
    }
}
