#ifndef _INDEXTRIE_H
#define _INDEXTRIE_H

typedef struct IndexTrie IndexTrie;

IndexTrie *IndexTrie_create();
void IndexTrie_free(IndexTrie *);
int IndexTrie_add(IndexTrie *, const char *name, int index);
int IndexTrie_lookup(IndexTrie *, const char *name);

#endif
