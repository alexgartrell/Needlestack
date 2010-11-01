#include <stdio.h>
#include <stdlib.h>

#include <event.h>
#include <evhttp.h>

#include <dbg.h>

static void httpcallback(struct evhttp_request *req, void *data) {
    printf("remote_host = %s\nuri = %s\n", req->remote_host, req->uri);
    evhttp_send_reply(req, 404, "not found", NULL);
}

int start_server(short port) {
    struct event_base *event_base = NULL;
    struct evhttp *evhttp = NULL;
    int rcode = 0;
    int exitcode = -1;

    event_base = event_base_new();
    check(event_base != NULL, "event_base_new failed");

    evhttp = evhttp_new(event_base);
    check(evhttp != NULL, "evhttp_new failed");

    rcode = evhttp_bind_socket(evhttp, NULL, port);
    check(rcode == 0, "Failed to bind socket on port %d", port);

    evhttp_set_gencb(evhttp, httpcallback, NULL);
    
    rcode = event_base_loop(event_base, 0);
    check(rcode == 0, "event_loop invocation failed");

    exitcode = 0;

/* Intentional fall-through */
error:
    if(evhttp) evhttp_free(evhttp);
    if(event_base) event_base_free(event_base);

    return exitcode;
}
