#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <getopt.h>

#include <dbg.h>
#include <server.h>
#include <filestack.h>

#define DEFAULT_PORT 8080

static char *USAGE = 
    "\t%s -f <stackfile.stack> [-p <port>]\n"
    "\n"
    "arguments: \n"
    "\t -f <stackfile>\tstack file to use\n"
    "\t -p <port>\tport to listen on\n";

int main(int argc, char *argv[]) {
    short port = DEFAULT_PORT;

    char *file = NULL;

    FileStack *fs = NULL;
    
    int ch = 0;

    struct option options[] = {
        {"file", required_argument, NULL, 'f'},
        {"port", required_argument, NULL, 'p'},
        {NULL, 0, NULL, 0}
    };

    while((ch = getopt_long(argc, argv, "cb:f:p:", options, NULL)) != -1) {
        switch(ch) {
        case 'f':
            file = optarg;
            break;
        case 'p':
            port = atoi(optarg);
            break;
        default:
            goto usage;
        }
    }

    if(!file) goto usage;
    fs = FileStack_load(file);
        
    return start_server(port, fs);

usage:
    printf("needlestack usage:\n");
    printf(USAGE, argv[0]);
    return 1;
}
