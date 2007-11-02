#ifndef COIN_XML_ELEMENT_H
#define COIN_XML_ELEMENT_H

/**************************************************************************\
 *
 *  This file is part of the SIM Scenery library.
 *  Copyright (C) 2000-2007 by Systems in Motion.  All rights reserved.
 *
 *  This software is proprietary to and embodies the confidential 
 *  technology of Systems in Motion.  Possession, use, or copying of this
 *  software and media is authorized only pursuant to a valid written
 *  license from Systems in Motion or an authorized sublicensor. 
 *
 *  For more information, contact SIM <http://www.sim.no/> by means of:
 *  Mail: Systems in Motion AS, Bygdøy allé 5, N-0257 Oslo, Norway;
 *  Email: <sales@sim.no>; Voice: +47 23 27 25 10; Fax: +47 23 27 25 11.
 *
\**************************************************************************/

#include <Inventor/C/XML/types.h>

#ifdef __cplusplus
extern "C" {
#endif /* __clusplus */

/* ********************************************************************** */

cc_xml_elt *      cc_xml_elt_new(void);
void           cc_xml_elt_delete_x(cc_xml_elt * elt);

const char *   cc_xml_elt_get_type(const cc_xml_elt * elt);
const char *   cc_xml_elt_get_data(const cc_xml_elt * elt);
const char *   cc_xml_elt_get_cdata(const cc_xml_elt * elt);

int            cc_xml_elt_get_num_children(const cc_xml_elt * elt);
int            cc_xml_elt_get_num_children_of_type(const cc_xml_elt * elt, const char * type);
cc_xml_elt *      cc_xml_elt_get_child(const cc_xml_elt * elt, int child);
int            cc_xml_elt_get_child_index(const cc_xml_elt * elt, const cc_xml_elt * child);
int            cc_xml_elt_get_child_type_index(const cc_xml_elt * elt, const cc_xml_elt * child);
cc_xml_elt *      cc_xml_elt_get_parent(const cc_xml_elt * elt);
cc_xml_path *     cc_xml_elt_get_path(const cc_xml_elt * elt);

cc_xml_elt *      cc_xml_elt_get_child_of_type(const cc_xml_elt * elt, const char * type, int idx);
cc_xml_elt *      cc_xml_elt_get_child_of_type_x(cc_xml_elt * elt, const char * type, int idx);

void           cc_xml_elt_set_type_x(cc_xml_elt * elt, const char * type);
void           cc_xml_elt_set_cdata_x(cc_xml_elt * elt, const char * data);
void           cc_xml_elt_set_parent_x(cc_xml_elt * elt, cc_xml_elt * parent);
void           cc_xml_elt_add_child_x(cc_xml_elt * elt, cc_xml_elt * child);
void           cc_xml_elt_remove_child_x(cc_xml_elt * elt, cc_xml_elt * child);
void           cc_xml_elt_insert_child_x(cc_xml_elt * elt, cc_xml_elt * child, int idx);
int            cc_xml_elt_replace_child_x(cc_xml_elt * elt, cc_xml_elt * oldchild, cc_xml_elt * newchild);

int            cc_xml_elt_get_boolean(const cc_xml_elt * elt, int * value);
int            cc_xml_elt_get_integer(const cc_xml_elt * elt, int * value);
int            cc_xml_elt_get_uint64(const cc_xml_elt * elt, uint64_t * value);
int            cc_xml_elt_get_int64(const cc_xml_elt * elt, int64_t * value);
int            cc_xml_elt_get_uint32(const cc_xml_elt * elt, uint32_t * value);
int            cc_xml_elt_get_int32(const cc_xml_elt * elt, int32_t * value);
int            cc_xml_elt_get_float(const cc_xml_elt * elt, float * value);
int            cc_xml_elt_get_double(const cc_xml_elt * elt, double * value);

void           cc_xml_elt_set_boolean_x(cc_xml_elt * elt, int value);
void           cc_xml_elt_set_integer_x(cc_xml_elt * elt, int value);
void           cc_xml_elt_set_uint64_x(cc_xml_elt * elt, uint64_t value);
void           cc_xml_elt_set_int64_x(cc_xml_elt * elt, int64_t value);
void           cc_xml_elt_set_uint32_x(cc_xml_elt * elt, uint32_t value);
void           cc_xml_elt_set_int32_x(cc_xml_elt * elt, int32_t value);
void           cc_xml_elt_set_float_x(cc_xml_elt * elt, float value);
void           cc_xml_elt_set_double_x(cc_xml_elt * elt, double value);

void           cc_xml_elt_strip_whitespace_x(cc_xml_elt * elt);

cc_xml_elt *      cc_xml_elt_get_traversal_next(const cc_xml_elt * root, cc_xml_elt * here);

const cc_xml_elt * cc_xml_elt_find(const cc_xml_elt * root, const cc_xml_path * path);
const cc_xml_elt * cc_xml_elt_find_next(const cc_xml_elt * root, cc_xml_elt * from, cc_xml_path * path);
cc_xml_elt *      cc_xml_elt_create_x(cc_xml_elt * from, cc_xml_path * path);

cc_xml_elt *      cc_xml_elt_clone(const cc_xml_elt * elt);

#ifdef COIN_INTERNAL
void           cc_xml_elt_dump_to_file(const cc_xml_elt * elt, int indent, FILE * fp);
#endif // COIN_INTERNAL

/* ********************************************************************** */

#ifdef __cplusplus
} /* extern "C" */
#endif /* __clusplus */

#endif /* !COIN_XML_ELEMENT_H */
