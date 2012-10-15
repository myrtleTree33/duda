/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/*  Duda I/O
 *  --------
 *  Copyright (C) 2012, Eduardo Silva P. <edsiper@gmail.com>
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

/*
 * @OBJ_NAME: map
 * @OBJ_DESC: The map object provides a set of methods to specify the URL's map
 * and callbacks associated.
 */

#include "duda_api.h"
#include "duda_map.h"

int duda_map_static_check(duda_request_t *dr)
{
    struct mk_list *head;
    struct session_request *sr = dr->sr;
    struct duda_map_static_cb *st;

    mk_list_foreach(head, dr->ws_root->map_urls) {
        st = mk_list_entry(head, struct duda_map_static_cb, _head);
        if (strncmp(sr->uri_processed.data + dr->ws_root->name.len + 1,
                    st->path, st->path_len) == 0) {
            st->callback(dr);
            return 0;
        }
    }

    return -1;
}

/*
 * @METHOD_NAME: static_add
 * @METHOD_DESC: It maps a static URL address to a specific callback function
 * @METHOD_PARAM: path    the URL path, e.g: "/something".
 * @METHOD_PARAM: cb_name the callback function name, e.g: "cb_something"
 * @METHOD_PROTO: int static_add(const char *path, const char *cb_name)
 * @METHOD_RETURN: Upon successful completion it returns 0, on error returns -1.
 */
int duda_map_static_add(const char *path,  const char *cb_name, struct mk_list *list)
{
    struct duda_map_static_cb *st;

    st = mk_api->mem_alloc(sizeof(struct duda_map_static_cb));
    if (!st) {
        return -1;
    }

    st->path     = mk_api->str_dup(path);
    st->path_len = strlen(path);
    st->cb_name  = mk_api->str_dup(cb_name);

    mk_list_add(&st->_head, list);

    return 0;
}

/*
 * @METHOD_NAME: interface_new
 * @METHOD_DESC: Creates a new interface map (non-static).
 * @METHOD_PARAM: uid the interface name
 * @METHOD_PROTO: duda_interface_t *interface_new(char *uid)
 * @METHOD_RETURN: Upon successful completion it returns the interface struct, on error returns NULL.
 */
duda_interface_t *duda_map_interface_new(char *uid)
{
    duda_interface_t *iface;

    iface = mk_api->mem_alloc(sizeof(duda_interface_t));
    if (!iface) {
        return NULL;
    }

    iface->uid     = uid;
    iface->uid_len = strlen(uid);
    mk_list_init(&iface->methods);

    return iface;
}

/*
 * @METHOD_NAME: interface_add_method
 * @METHOD_DESC: It adds a method to an interface map
 * @METHOD_PARAM: method the method to link
 * @METHOD_PARAM: iface  the root interface
 * @METHOD_PROTO: void interface_add_method(duda_method_t *method, duda_interface_t *iface)
 * @METHOD_RETURN: None
 */
void duda_map_interface_add_method(duda_method_t *method,
                                   duda_interface_t *iface)
{
    mk_list_add(&method->_head, &iface->methods);
}


/* Creates a new method */
duda_method_t *_duda_map_method_new(char *uid, char *cb_webservice,
                                    void (*cb_builtin)(duda_request_t *),
                                    int n_params)
{
    duda_method_t *method;

    method = mk_api->mem_alloc(sizeof(duda_method_t));
    method->uid     = uid;
    method->uid_len = strlen(uid);
    method->num_params = n_params;

    if (cb_webservice) {
        method->callback = cb_webservice;
        method->cb_webservice = NULL;
        method->cb_builtin = NULL;
    }
    else {
        method->callback = NULL;
        method->cb_webservice = NULL;
        method->cb_builtin = cb_builtin;
    }

    mk_list_init(&method->params);
    return method;
}

/*
 * @METHOD_NAME: method_new
 * @METHOD_DESC: It creates a new interface method
 * @METHOD_PARAM: uid the method name
 * @METHOD_PARAM: callback the callback function name
 * @METHOD_PARAM: n_params number of parameters allowed
 * @METHOD_PROTO: duda_method_t *method_new(char *uid, char *callback, int n_params)
 * @METHOD_RETURN: Returns the new method structure.
 */
duda_method_t *duda_map_method_new(char *uid, char *callback, int n_params)
{
    return _duda_map_method_new(uid, callback, NULL, n_params);
}

/* Creates a new method */
duda_method_t *duda_map_method_builtin_new(char *uid,
                                           void (*cb_builtin) (duda_request_t *),
                                           int n_params)
{
    return _duda_map_method_new(uid, NULL, cb_builtin, n_params);
}

/*
 * @METHOD_NAME: method_add_param
 * @METHOD_DESC: It adds a parameter to a method
 * @METHOD_PARAM: param the parameter struct
 * @METHOD_PARAM: method the method where the parameter will be linked
 * @METHOD_PROTO: void method_add_param(duda_param_t *param, duda_method_t *method)
 * @METHOD_RETURN: None
 */
void duda_map_method_add_param(duda_param_t *param, duda_method_t *method)
{
    mk_list_add(&param->_head, &method->params);
}

/*
 * @METHOD_NAME: param_new
 * @METHOD_DESC: It creates a new method parameter
 * @METHOD_PARAM: uid the parameter name
 * @METHOD_PARAM: size the maximum size allowed for the parameter value
 * @METHOD_PROTO: void param_new(char *uid, short int size)
 * @METHOD_RETURN: It returns the parameter struct
 */
duda_param_t *duda_map_param_new(char *uid, short int size)
{
    duda_param_t *param;

    param = mk_api->mem_alloc(sizeof(duda_param_t));
    param->name = uid;
    param->max_len = size;

    return param;
}

/*
 * @METHOD_NAME: add_interface
 * @METHOD_DESC: It register an interface tree with the system maps
 * @METHOD_PARAM: iface the root interface
 * @METHOD_PROTO: void add_interface(duda_interface_t *iface)
 * @METHOD_RETURN: None
 */
void duda_map_add_interface(duda_interface_t *iface, struct mk_list *list)
{
    mk_list_add(&iface->_head, list);
}

struct duda_api_map *duda_map_object()
{
    struct duda_api_map *obj;

    obj = mk_api->mem_alloc(sizeof(struct duda_api_map));
    obj->_static_add = duda_map_static_add;
    obj->_add_interface = duda_map_add_interface;
    obj->interface_new = duda_map_interface_new;
    obj->interface_add_method = duda_map_interface_add_method;
    obj->method_new = duda_map_method_new;
    obj->method_builtin_new = duda_map_method_builtin_new;
    obj->method_add_param = duda_map_method_add_param;
    obj->param_new = duda_map_param_new;

    return obj;
}