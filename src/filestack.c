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
    FILE *file = NULL;
    FILE *contentfile = NULL;
    unsigned int nread = 0;
    unsigned int nwritten = 0;
    char normalized_base[PATH_MAX + 1];
    int normalized_base_len = 0;
    char normalized_uri[PATH_MAX + 1];
    char buff[4096];
    int i = 0;
    int x = 0;

    check(output && base && files != NULL && len > 0, "Invalid arguments");
    
    check(realpath(base, normalized_base) != NULL, "Failed to normalize base");
    normalized_base[PATH_MAX] = '\0';
    normalized_base_len = strlen(normalized_base);

    file = fopen(output, "w");
    check(file != NULL, "Failed to open %s for writing", output);

    for(i = 0; i < len; i++) {
        check(realpath(files[i], normalized_uri) != NULL,
              "Failed to normalize file %s", files[i]);
        normalized_uri[PATH_MAX] = '\0';

        check(!(strncmp(normalized_base, normalized_uri, normalized_base_len)),
              "file %s is out of base %s\n", normalized_uri, normalized_base);
        
        x = strlen(normalized_uri + normalized_base_len);
        check(fwrite(&x, sizeof(x), 1, file) == 1, "failed to write");

        fprintf(file, "%s", normalized_uri + normalized_base_len);

        contentfile = fopen(files[i], "r");
        check(contentfile != NULL, "Unable to open contentfile");
        
        fseek(contentfile, 0, SEEK_END);
        x = ftell(contentfile);
        fseek(contentfile, 0, SEEK_SET);
        
        nwritten = fwrite(&x, sizeof(x), 1, file);
        check(nwritten == 1, "failed to write");


        while((nread = fread(buff, 1, sizeof(buff), contentfile)) > 0) {
            nwritten = fwrite(buff, 1, nread, file);
            check(nwritten == nread, "Failed to write to file");
        }

        fclose(contentfile);
    }
    
    fclose(file);

    return 0;
error:
    if(file) fclose(file);

    return -1;
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
    fs->filemem = mmap(NULL, fs->filemem_len, PROT_READ, MAP_FILE | MAP_SHARED, fs->fd, 0);
    check(fs->filemem != MAP_FAILED, "Failed to mmap %s", path);

    check(fs->filemem_len > sizeof(int), "File %s is malformed", path);

    unsigned int file_off = 0;
    while(file_off < fs->filemem_len) {
        char uri[MAX_URI_LENGTH + 1];
        unsigned int len;
        unsigned int off;
        unsigned int size;

        check(file_off + sizeof(int) <= fs->filemem_len, "Malformed file");
        memcpy(&len, fs->filemem + file_off, sizeof(int));
        file_off += sizeof(int);

        check(file_off + len < fs->filemem_len && len <= MAX_URI_LENGTH, 
              "Malformed file");
        memcpy(uri, fs->filemem + file_off, len);
        uri[len] = '\0';
        file_off += len;

        check(file_off + sizeof(int) <= fs->filemem_len, "Malformed file");
        memcpy(&size, fs->filemem + file_off, sizeof(int));
        file_off += sizeof(int);

        check(file_off + size <= fs->filemem_len, "Malformed file");
        off = file_off;
        file_off += size;

        // Now we have size, off, and uri
        info.offset = off;
        info.size = size;
        rcode = FileIndex_add(fs->index, uri, &info);
        check(rcode == 0, "Failed to construct index");
    }

    return fs;
error:
    FileStack_free(fs);
    if(file) fclose(file);
    return NULL;
}

int FileStack_lookup(FileStack *fs, char *uri, unsigned int *offset,
                     unsigned int *size) {
    FileInfo *info = NULL;

    check(fs && uri && offset && size, "Invalid arguments");
    info = FileIndex_lookup(fs->index, uri);

    if(info == NULL) return -1;

    *offset = info->offset;
    *size = info->size;
    return 0;

error:
    return -1;
}

void FileStack_free(FileStack *fs) {
    if(fs) {
        FileIndex_free(fs->index);
        if(fs->filemem) munmap(fs->filemem, fs->filemem_len);
        if(fs->fd > 0) close(fs->fd);
    }
}
