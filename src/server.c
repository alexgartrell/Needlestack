#include <server.h>

#include <stdio.h>
#include <stdlib.h>

#include <event.h>
#include <evhttp.h>

#include <filestack.h>
#include <dbg.h>

#define DEFAULT_INDEX "index.html"

static void httpcallback(struct evhttp_request *req, void *data) {
    struct evbuffer *evbuffer;
    FileStack *fs = (FileStack *) data;
    FileInfo *info;
    char slen[20];
    char uri_buff[1024];

    evbuffer = evbuffer_new();
    if(evbuffer == NULL) {
        log("evbuffer_new returned NULL");
        goto send404;
    }

    info = FileStack_lookup(fs, req->uri);
    if(info == NULL) {
        if(req->uri[strlen(req->uri) - 1] == '/')
            snprintf(uri_buff, sizeof(uri_buff), "%s" DEFAULT_INDEX, req->uri);
        else
            snprintf(uri_buff, sizeof(uri_buff), "%s/" DEFAULT_INDEX,
                     req->uri);
    }
    if(info == NULL) goto send404;
    
    snprintf(slen, sizeof(slen), "%d", info->size);
    evhttp_add_header(req->output_headers, "Content-Length", slen);
    evhttp_add_header(req->output_headers, "Content-Type", info->content_type);

    evbuffer_add(evbuffer, info->ptr, info->size);
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
