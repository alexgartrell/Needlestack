#include <fileindex.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <dbg.h>

static Node *Node_create();
static void Node_free(Node *);

static Node *Node_create() {
    Node *n = malloc(sizeof(*n));
    check(n != NULL, "Out of memory");
    memset(n, 0, sizeof(*n));
    n->occupied = 0;

    return n;

error:
    Node_free(n);
    return NULL;
}

static void Node_free(Node *n) {
    if(n != NULL) {
        int i;
        for(i = 0; i < NUM_CHARS; i++)
            if(n->nodes[i])
                Node_free(n->nodes[i]);
        free(n);
    }
}

FileIndex *FileIndex_create() {
    FileIndex *fi = calloc(sizeof(*fi), 1);
    check(fi != NULL, "Out of memory");

    fi->trie = Node_create();
    check(fi->trie != NULL, "Node_create failed");
    
    return fi;

error:
    FileIndex_free(fi);
    return NULL;
}


void FileIndex_free(FileIndex *fi) {
    if(fi) {
        Node_free(fi->trie);
        free(fi);
    }
}

int FileIndex_add(FileIndex *fi, const char *name, FileInfo *info) {
    const char *p;
    Node *cur;

    check(fi != NULL && name != NULL, "Invalid Arguments");

    for(p = name, cur = fi->trie; *p; p++) {
        if(cur->nodes[(int) *p] == NULL) {
            cur->nodes[(int) *p] = Node_create();
            check(cur->nodes[(int) *p] != NULL, "Failed to create node");
        }
        cur = cur->nodes[(int) *p];
    }

    cur->occupied = 1;
    cur->info = *info;

    return 0;

error:
    return -1;
}

FileInfo *FileIndex_lookup(FileIndex *fi, const char *name) {
    const char *p;
    Node *cur;

    check(fi != NULL && name != NULL, "Invalid Arguments");
    
    for(p = name, cur = fi->trie; *p && cur; p++)
        cur = cur->nodes[(int) *p];

    if(cur == NULL || !cur->occupied) return NULL;

    return &cur->info;

error:
    return NULL;
}
