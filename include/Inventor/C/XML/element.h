#ifndef COIN_XML_ELEMENT_H
#define COIN_XML_ELEMENT_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) by Kongsberg Oil & Gas Technologies.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg Oil & Gas Technologies
 *  about acquiring a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg Oil & Gas Technologies, Bygdoy Alle 5, 0257 Oslo, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

#include <stdio.h>

#include <Inventor/C/XML/types.h>

#ifdef __cplusplus
extern "C" {
#endif /* __clusplus */

/* ********************************************************************** */

COIN_DLL_API cc_xml_elt * cc_xml_elt_new(void);
COIN_DLL_API cc_xml_elt * cc_xml_elt_new_from_data(const char * type, cc_xml_attr ** attrs);
COIN_DLL_API cc_xml_elt * cc_xml_elt_clone(const cc_xml_elt * elt);
COIN_DLL_API void cc_xml_elt_delete_x(cc_xml_elt * elt);

COIN_DLL_API void cc_xml_elt_set_type_x(cc_xml_elt * elt, const char * type);
COIN_DLL_API const char * cc_xml_elt_get_type(const cc_xml_elt * elt);

COIN_DLL_API void cc_xml_elt_set_cdata_x(cc_xml_elt * elt, const char * data);
COIN_DLL_API const char * cc_xml_elt_get_cdata(const cc_xml_elt * elt);
COIN_DLL_API const char * cc_xml_elt_get_data(const cc_xml_elt * elt);

COIN_DLL_API void cc_xml_elt_remove_all_attributes_x(cc_xml_elt * elt);
COIN_DLL_API void cc_xml_elt_set_attribute_x(cc_xml_elt * elt, cc_xml_attr * attr);
COIN_DLL_API void cc_xml_elt_set_attributes_x(cc_xml_elt * elt, cc_xml_attr ** attrs);

COIN_DLL_API cc_xml_attr * cc_xml_elt_get_attribute(const cc_xml_elt * elt, const char * attrname);
COIN_DLL_API int cc_xml_elt_get_num_attributes(const cc_xml_elt * elt);
COIN_DLL_API const cc_xml_attr ** cc_xml_elt_get_attributes(const cc_xml_elt * elt);

COIN_DLL_API int             cc_xml_elt_get_num_children(const cc_xml_elt * elt);
COIN_DLL_API int             cc_xml_elt_get_num_children_of_type(const cc_xml_elt * elt, const char * type);
COIN_DLL_API cc_xml_elt *    cc_xml_elt_get_child(const cc_xml_elt * elt, int child);
COIN_DLL_API int             cc_xml_elt_get_child_index(const cc_xml_elt * elt, const cc_xml_elt * child);
COIN_DLL_API int             cc_xml_elt_get_child_type_index(const cc_xml_elt * elt, const cc_xml_elt * child);
COIN_DLL_API cc_xml_elt *    cc_xml_elt_get_parent(const cc_xml_elt * elt);
COIN_DLL_API cc_xml_path *   cc_xml_elt_get_path(const cc_xml_elt * elt);

COIN_DLL_API cc_xml_elt *    cc_xml_elt_get_child_of_type(const cc_xml_elt * elt, const char * type, int idx);
COIN_DLL_API cc_xml_elt *    cc_xml_elt_get_child_of_type_x(cc_xml_elt * elt, const char * type, int idx);

COIN_DLL_API void           cc_xml_elt_set_parent_x(cc_xml_elt * elt, cc_xml_elt * parent);
COIN_DLL_API void           cc_xml_elt_add_child_x(cc_xml_elt * elt, cc_xml_elt * child);
COIN_DLL_API void           cc_xml_elt_remove_child_x(cc_xml_elt * elt, cc_xml_elt * child);
COIN_DLL_API void           cc_xml_elt_insert_child_x(cc_xml_elt * elt, cc_xml_elt * child, int idx);
COIN_DLL_API int            cc_xml_elt_replace_child_x(cc_xml_elt * elt, cc_xml_elt * oldchild, cc_xml_elt * newchild);

COIN_DLL_API int            cc_xml_elt_get_boolean(const cc_xml_elt * elt, int * value);
COIN_DLL_API int            cc_xml_elt_get_integer(const cc_xml_elt * elt, int * value);
COIN_DLL_API int            cc_xml_elt_get_uint64(const cc_xml_elt * elt, uint64_t * value);
COIN_DLL_API int            cc_xml_elt_get_int64(const cc_xml_elt * elt, int64_t * value);
COIN_DLL_API int            cc_xml_elt_get_uint32(const cc_xml_elt * elt, uint32_t * value);
COIN_DLL_API int            cc_xml_elt_get_int32(const cc_xml_elt * elt, int32_t * value);
COIN_DLL_API int            cc_xml_elt_get_float(const cc_xml_elt * elt, float * value);
COIN_DLL_API int            cc_xml_elt_get_double(const cc_xml_elt * elt, double * value);

COIN_DLL_API void           cc_xml_elt_set_boolean_x(cc_xml_elt * elt, int value);
COIN_DLL_API void           cc_xml_elt_set_integer_x(cc_xml_elt * elt, int value);
COIN_DLL_API void           cc_xml_elt_set_uint64_x(cc_xml_elt * elt, uint64_t value);
COIN_DLL_API void           cc_xml_elt_set_int64_x(cc_xml_elt * elt, int64_t value);
COIN_DLL_API void           cc_xml_elt_set_uint32_x(cc_xml_elt * elt, uint32_t value);
COIN_DLL_API void           cc_xml_elt_set_int32_x(cc_xml_elt * elt, int32_t value);
COIN_DLL_API void           cc_xml_elt_set_float_x(cc_xml_elt * elt, float value);
COIN_DLL_API void           cc_xml_elt_set_double_x(cc_xml_elt * elt, double value);

COIN_DLL_API void           cc_xml_elt_strip_whitespace_x(cc_xml_elt * elt);

COIN_DLL_API cc_xml_elt *   cc_xml_elt_get_traversal_next(const cc_xml_elt * root, cc_xml_elt * here);

COIN_DLL_API const cc_xml_elt * cc_xml_elt_find(const cc_xml_elt * root, const cc_xml_path * path);
COIN_DLL_API const cc_xml_elt * cc_xml_elt_find_next(const cc_xml_elt * root, cc_xml_elt * from, cc_xml_path * path);
COIN_DLL_API cc_xml_elt * cc_xml_elt_create_x(cc_xml_elt * from, cc_xml_path * path);

/* ********************************************************************** */

#ifdef __cplusplus
} /* extern "C" */
#endif /* __clusplus */

#endif /* !COIN_XML_ELEMENT_H */
