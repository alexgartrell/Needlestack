#include <fileindex.h>

#include <test.h>

int sanity_test() {
    FileIndex *fi = FileIndex_create();
    char a, b, c, d, e, f;
    FileInfo info[] = {{&a, &b, &c, 5}, {&d, &e, &f, 7}};
    FileInfo *out;

    assert(FileIndex_lookup(fi, "foo") == NULL);
    assert(FileIndex_add(fi, "foo", &info[0]) == 0);

    assert((out = FileIndex_lookup(fi, "foo")) != NULL)
    assert(out->ptr == info[0].ptr && out->size == info[0].size);

    assert((out = FileIndex_lookup(fi, "bar")) == NULL);

    assert(FileIndex_add(fi, "foobar", &info[1]) == 0);
    assert((out = FileIndex_lookup(fi, "foobar")) != NULL);
    assert(out->ptr == info[1].ptr && out->size == info[1].size);

    return 0;
}

TESTS_MAIN(sanity_test)
