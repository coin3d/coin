#ifndef COIN_XML_ATTRIBUTE_H
#define COIN_XML_ATTRIBUTE_H

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

#include <Inventor/C/XML/types.h>

/* ********************************************************************** */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

COIN_DLL_API cc_xml_attr * cc_xml_attr_new(void);
COIN_DLL_API cc_xml_attr * cc_xml_attr_new_from_data(const char * name, const char * value);
COIN_DLL_API cc_xml_attr * cc_xml_attr_clone(const cc_xml_attr * orig);
COIN_DLL_API void cc_xml_attr_delete_x(cc_xml_attr * attr);

COIN_DLL_API void cc_xml_attr_set_name_x(cc_xml_attr * attr, const char * name);
COIN_DLL_API const char * cc_xml_attr_get_name(const cc_xml_attr * attr);
COIN_DLL_API void cc_xml_attr_set_value_x(cc_xml_attr * attr, const char * value);
COIN_DLL_API const char * cc_xml_attr_get_value(const cc_xml_attr * attr);

/* TODO: type-specific/converting attribute accessors
COIN_DLL_API int cc_xml_attr_is_int32(const cc_xml_attr * attr);
COIN_DLL_API int32_t cc_xml_attr_get_int32(const cc_xml_attr * attr);
COIN_DLL_API void cc_xml_attr_set_int32_x(const cc_xml_attr * attr, int32_t intval);
*/

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

/* ********************************************************************** */

#endif /* !COIN_XML_ATTRIBUTE_H */
