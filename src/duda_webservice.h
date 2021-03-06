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

#ifndef DUDA_DWEBSERVICE_H
#define DUDA_DWEBSERVICE_H

struct duda_setup {
    void (*event_signal_cb) (int fd, uint64_t val);
};

/* Web service information */
struct web_service {
    mk_ptr_t name;       /* web service name                 */
    mk_ptr_t fixed_name; /* service name given by vhost/file */
    mk_ptr_t docroot;    /* document root for static content */
    mk_ptr_t confdir;    /* configuration directory          */
    mk_ptr_t datadir;    /* data store                       */
    mk_ptr_t logdir;     /* directory to store logs          */

    int  enabled;
    int  is_root;
    int  url_force_redirect;
    int  bind_messages;

    void *handler;

    /* Routing/Map callbacks */
    char *map_root_name;
    void (*map_root_cb) (void *);

    struct mk_list *map_interfaces;
    struct mk_list *map_urls;

    /* global data */
    struct mk_list *global;

    /* pre-worker-loop functions */
    struct mk_list *pre_loop;

    /* workers list */
    struct mk_list *workers;

    /* loggers list */
    struct mk_list *loggers;

    /* packages loaded by the web service */
    struct mk_list *packages;

    /* generic setup things related to the web service and Duda stack */
    struct duda_setup *setup;

    /* node entry associated with services_list */
    struct mk_list _head;

    /* reference to the parent vhost_services entry */
    struct vhost_services *vh_parent;

    /* node entry associated with services_loaded */
    struct mk_list _head_loaded;

    /* Callbacks */
    void (*exit_cb) ();
};

#endif
