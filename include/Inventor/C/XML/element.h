#ifndef COIN_XML_ELEMENT_H
#define COIN_XML_ELEMENT_H

/**************************************************************************\
 * Copyright (c) Kongsberg Oil & Gas Technologies AS
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 
 * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 * 
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 * 
 * Neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
\**************************************************************************/

#include <stdio.h>

#include <Inventor/C/XML/types.h>

#ifdef __cplusplus
extern "C" {
#endif /* __clusplus */

/* ********************************************************************** */

/* Elements returned by new(), new_from_data(), and clone() are caller-owned.
   delete_x() recursively deletes attributes and children, so it must only be
   called for an element currently owned by the caller.
   new_from_data() takes ownership of every attribute in the NULL-terminated
   attrs array; the array itself remains caller-owned. */
COIN_DLL_API cc_xml_elt * cc_xml_elt_new(void);
COIN_DLL_API cc_xml_elt * cc_xml_elt_new_from_data(const char * type, cc_xml_attr ** attrs);
COIN_DLL_API cc_xml_elt * cc_xml_elt_clone(const cc_xml_elt * elt);
COIN_DLL_API void cc_xml_elt_delete_x(cc_xml_elt * elt);

COIN_DLL_API void cc_xml_elt_set_type_x(cc_xml_elt * elt, const char * type);
COIN_DLL_API const char * cc_xml_elt_get_type(const cc_xml_elt * elt);

COIN_DLL_API void cc_xml_elt_set_cdata_x(cc_xml_elt * elt, const char * data);
COIN_DLL_API const char * cc_xml_elt_get_cdata(const cc_xml_elt * elt);
COIN_DLL_API const char * cc_xml_elt_get_data(const cc_xml_elt * elt);

/* Attribute ownership:
   - set_attribute_x() and set_attributes_x() transfer ownership to elt.
   - get_attribute() and get_attributes() return borrowed pointers.
   - remove_all_attributes_x() deletes all attributes owned by elt.
   The attrs array passed to set_attributes_x() remains caller-owned. */
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

/* Child ownership:
   - add_child_x() and insert_child_x() transfer ownership on success.
   - child getters return borrowed pointers.
   - remove_child_x() releases ownership to the caller without deleting.
   - replace_child_x() releases oldchild and takes ownership of newchild on
     success; on failure ownership and tree structure are unchanged.
   - set_parent_x() moves ownership from the current parent to the new parent;
     passing NULL releases ownership to the caller. */
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
