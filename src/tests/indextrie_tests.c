#include <indextrie.h>

#include <test.h>

int sanity_test() {
    IndexTrie *it = IndexTrie_create();

    assert(IndexTrie_lookup(it, "foo") == -1);
    assert(IndexTrie_add(it, "foo", 4) == 0);
    assert(IndexTrie_lookup(it, "foo") == 4);
    assert(IndexTrie_lookup(it, "bar") == -1);
    assert(IndexTrie_add(it, "foobar", 5) == 0);
    assert(IndexTrie_lookup(it, "foobar") == 5);
    assert(IndexTrie_lookup(it, "foo") == 4);

    return 0;
}

TESTS_MAIN(sanity_test)
