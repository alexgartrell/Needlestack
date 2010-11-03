#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <getopt.h>

#include <dbg.h>
#include <server.h>
#include <filestack.h>

#define DEFAULT_PORT 8080

int main(int argc, char *argv[]) {
    int rcode = 0;
    short port = DEFAULT_PORT;

    char *file = NULL;
    char *base = ".";
    int create = 0;

    FileStack *fs = NULL;
    
    int ch = 0;

    struct option options[] = {
        {"create", no_argument, NULL, 'c'},
        {"file", required_argument, NULL, 'f'},
        {"base", required_argument, NULL, 'b'},
        {"port", required_argument, NULL, 'p'},
        {NULL, 0, NULL, 0}
    };

    while((ch = getopt_long(argc, argv, "cb:f:p:", options, NULL)) != -1) {
        switch(ch) {
        case 'c':
            create = 1;
            break;
        case 'f':
            file = optarg;
            break;
        case 'b':
            base = optarg;
            break;
        case 'p':
            port = atoi(optarg);
            break;
        default:
            goto usage;
        }
    }

    if(create) {
        if(!file || !base) goto usage;

        rcode = FileStack_create(file, base, argv + optind, argc - optind);
    }
    else {
        if(!file) goto usage;
        fs = FileStack_load(file);
        
        rcode = start_server(port, fs);
    }

    return rcode;

usage:
    printf("Read the code\n");
    return 1;
}
