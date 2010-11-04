#include <fileindex.h>

#include <test.h>

int sanity_test() {
    FileIndex *fi = FileIndex_create();
    FileInfo info[] = {{4, 5}, {6, 7}};
    FileInfo *out;

    assert(FileIndex_lookup(fi, "foo") == NULL);
    assert(FileIndex_add(fi, "foo", &info[0]) == 0);

    assert((out = FileIndex_lookup(fi, "foo")) != NULL)
    assert(out->offset == info[0].offset && out->size == info[0].size);

    assert((out = FileIndex_lookup(fi, "bar")) == NULL);

    assert(FileIndex_add(fi, "foobar", &info[1]) == 0);
    assert((out = FileIndex_lookup(fi, "foobar")) != NULL);
    assert(out->offset == info[1].offset && out->size == info[1].size);

    return 0;
}

TESTS_MAIN(sanity_test)
