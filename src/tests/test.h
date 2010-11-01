#ifndef _TEST_H
#define _TEST_H

#include <stdio.h>

#define assert(A) {                                                 \
        if(!(A)) {                                                  \
            fprintf(stderr, "Assertion failed (%s:%d): %s\n",       \
                    __FILE__, __LINE__, #A);                        \
            return 1;                                               \
        }                                                           \
    }

typedef int (*test_t)();

#define TESTS_MAIN(...)                                         \
    int main(int argc, char *argv[]) {                          \
        test_t tests[] = {__VA_ARGS__};                         \
        int len = sizeof(tests) / sizeof((tests)[0]);           \
        int i;                                                  \
        int failed = 0;                                         \
        for(i = 0; i < len; i++) {                              \
            failed = failed || tests[i]();                      \
        }                                                       \
        if(failed)                                              \
            printf("Tests failed in " __FILE__ "\n");           \
        else                                                    \
            printf("All tests passed in " __FILE__ "\n");       \
        return failed;                                          \
    }

#endif
