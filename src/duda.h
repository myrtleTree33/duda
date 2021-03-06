/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/*  Duda I/O
 *  --------
 *  Copyright (C) 2012-2014, Eduardo Silva P. <edsiper@gmail.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include "MKPlugin.h"

#include "duda_webservice.h"
#include "duda_conf.h"
#include "duda_gc_map.h"
#include "duda_qs_map.h"

#ifndef DUDA_MAIN_H
#define DUDA_MAIN_H

#define MAP_WS_APP_NAME   0X00
#define MAP_WS_INTERFACE  0X10
#define MAP_WS_METHOD     0X20
#define MAP_WS_PARAM      0X30
#define MAP_WS_END        0X40

/* Max number of parameters allowed in Duda URI */
#define MAP_WS_MAX_PARAMS 8

/*
 * This struct represent the web service request, as well it contains detailed
 * information about the response type and buffers associated
 */
typedef struct duda_request {

    /* web service details */
    struct web_service *ws_root;

    mk_ptr_t appname;
    mk_ptr_t interface;
    mk_ptr_t method;
    mk_ptr_t params[MAP_WS_MAX_PARAMS];
    short int n_params;

    /* Monkey request data: plugin, client_session & session_request */
    int socket;
    struct plugin *plugin;
    struct client_session *cs;
    struct session_request *sr;

    /* Static map */

    /* Method structure */
    struct duda_method *_method;

    /* Callback functions */
    void (*end_callback) (struct duda_request *);

    /* Internal statuses */
    long _st_http_content_length;         /* Fixed content length size     */
    unsigned int _st_http_headers_sent;   /* HTTP headers sent ?           */
    unsigned int _st_http_headers_off;    /* should we send headers ?      */
    unsigned int _st_body_writes;         /* Number of body_writes invoked */

    /* Query string */
    struct duda_qs_map qs;

    /* Gargabe collector */
    struct duda_gc_map gc;

    /* Data queues */
    struct mk_list queue_out;

    /* Lists linked to (events)*/
    struct mk_list _head_events_write;

    /* Head to red-black tree list that holds all DRs */
    struct rb_node _rb_head;

} duda_request_t;


/* self identifier for the plugin context inside Monkey internals */
struct plugin *duda_plugin;

pthread_key_t duda_global_events_write;
pthread_key_t duda_global_dr_list;
pthread_mutex_t duda_mutex_thctx;

mk_ptr_t dd_iov_none;

void *duda_load_library(const char *path);
void *duda_load_symbol(void *handle, const char *symbol);
int duda_service_end(duda_request_t *dr);

duda_request_t *duda_dr_list_get(int socket);

#endif
