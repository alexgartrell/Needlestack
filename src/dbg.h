#ifndef _DBG_H
#define _DBG_H

#define log(M, ...) fprintf(stderr, M "\n", ##__VA_ARGS__)

#define check(A, M, ...) {                                              \
        if(!(A)) {                                                      \
            fprintf(stderr, M "\n", ##__VA_ARGS__);                     \
            fprintf(stderr, "Location %s:%d\n", __FILE__, __LINE__);    \
            perror("perror");                                           \
            goto error;                                                 \
        }                                                               \
    }


#endif
