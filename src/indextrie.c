#include <indextrie.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <dbg.h>

#define NUM_CHARS (1 << (sizeof(char) * 8))

typedef struct Node {
    struct Node *nodes[NUM_CHARS];
    int index;
} Node;

struct IndexTrie {
    Node *trie;
};

static Node *Node_create();
static void Node_free(Node *);

static Node *Node_create() {
    Node *n = malloc(sizeof(*n));
    check(n != NULL, "Out of memory");
    memset(n, 0, sizeof(*n));
    n->index = -1;

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

IndexTrie *IndexTrie_create() {
    IndexTrie *it = calloc(sizeof(*it), 1);
    check(it != NULL, "Out of memory");

    it->trie = Node_create();
    check(it->trie != NULL, "Out of memory");
    
    return it;

error:
    IndexTrie_free(it);
    return NULL;
}


void IndexTrie_free(IndexTrie *it) {
    Node_free(it->trie);
    free(it);
}

int IndexTrie_add(IndexTrie *it, const char *name, int index) {
    const char *p;
    Node *cur;

    check(it != NULL && name != NULL && index >= 0, "Invalid Arguments");

    for(p = name, cur = it->trie; *p; p++) {
        if(cur->nodes[(int) *p] == NULL) {
            cur->nodes[(int) *p] = Node_create();
            check(cur->nodes[(int) *p] != NULL, "Failed to create node");
        }
        cur = cur->nodes[(int) *p];
    }

    cur->index = index;

    return 0;

error:
    return -1;
}

int IndexTrie_lookup(IndexTrie *it, const char *name) {
    const char *p;
    Node *cur;

    check(it != NULL && name != NULL, "Invalid Arguments");
    
    for(p = name, cur = it->trie; *p && cur; p++)
        cur = cur->nodes[(int) *p];

    return (cur == NULL) ? -1 : cur->index;

error:
    return -1;
}
