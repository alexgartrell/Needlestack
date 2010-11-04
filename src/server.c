#include <server.h>

#include <stdio.h>
#include <stdlib.h>

#include <event.h>
#include <evhttp.h>

#include <filestack.h>
#include <dbg.h>

static void httpcallback(struct evhttp_request *req, void *data) {
    struct evbuffer *evbuffer;
    FileStack *fs = (FileStack *) data;
    int offset = 0;
    int size = 0;
    int rcode = 0;

    evbuffer = evbuffer_new();
    if(evbuffer == NULL) {
        log("evbuffer_new returned NULL");
        goto send404;
    }

    rcode = FileStack_lookup(fs, req->uri, &offset, &size);
    if(rcode != 0) goto send404;
    
    evbuffer_add(evbuffer, fs->filemem + offset, size);
    evhttp_send_reply(req, 200, "ok", evbuffer);

    evbuffer_free(evbuffer); evbuffer = NULL;

    return;

send404:
    if(evbuffer) evbuffer_free(evbuffer);
    evhttp_send_reply(req, 404, "not found", NULL);
}

int start_server(short port, FileStack *fs) {
    struct event_base *event_base = NULL;
    struct evhttp *evhttp = NULL;
    int rcode = 0;
    int exitcode = -1;

    check(fs != NULL, "Invalid arguments");

    event_base = event_base_new();
    check(event_base != NULL, "event_base_new failed");

    evhttp = evhttp_new(event_base);
    check(evhttp != NULL, "evhttp_new failed");

    rcode = evhttp_bind_socket(evhttp, NULL, port);
    check(rcode == 0, "Failed to bind socket on port %d", port);

    evhttp_set_gencb(evhttp, httpcallback, fs);
    
    rcode = event_base_loop(event_base, 0);
    check(rcode == 0, "event_loop invocation failed");

    exitcode = 0;

/* Intentional fall-through */
error:
    if(evhttp) evhttp_free(evhttp);
    if(event_base) event_base_free(event_base);

    return exitcode;
}
