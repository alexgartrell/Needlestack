#ifndef _DBG_H
#define _DBG_H

#define check(A, M, ...) {                          \
        if(!(A)) {                                  \
            fprintf(stderr, M "\n", ##__VA_ARGS__); \
            goto error;                             \
        }                                           \
    }


#endif
