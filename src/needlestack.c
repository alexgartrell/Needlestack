#include <stdio.h>
#include <stdlib.h>

#include <dbg.h>
#include <server.h>

#define DEFAULT_PORT 8080

int main(int argc, char *argv[]) {
    short port = DEFAULT_PORT;
    if(argc > 1) port = atoi(argv[1]);

    return start_server(port);
}
