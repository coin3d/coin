#ifndef COIN_XML_PATH_H
#define COIN_XML_PATH_H

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

#include <stdarg.h>

#include <Inventor/C/XML/types.h>

/* ********************************************************************** */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

COIN_DLL_API cc_xml_path * cc_xml_path_new(void);
COIN_DLL_API void          cc_xml_path_delete_x(cc_xml_path * path);

COIN_DLL_API void          cc_xml_path_clear_x(cc_xml_path * path);
COIN_DLL_API void          cc_xml_path_set_x(cc_xml_path * path, ...); /* use 'v' in signature? */
COIN_DLL_API void          cc_xml_path_copy_x(cc_xml_path * path, cc_xml_path * path2);
COIN_DLL_API void          cc_xml_path_reverse_x(cc_xml_path * path);

COIN_DLL_API int           cc_xml_path_get_length(const cc_xml_path * path);
COIN_DLL_API const char *  cc_xml_path_get_type(const cc_xml_path * path, int idx);
COIN_DLL_API int           cc_xml_path_get_index(const cc_xml_path * path, int idx);
COIN_DLL_API int           cc_xml_path_match_p(const cc_xml_path * path, const cc_xml_elt * elt);

COIN_DLL_API void          cc_xml_path_append_x(cc_xml_path * path, const char * elt, int idx);
COIN_DLL_API void          cc_xml_path_append_path_x(cc_xml_path * path, cc_xml_path * path2);
COIN_DLL_API void          cc_xml_path_prepend_x(cc_xml_path * path, const char * elt, int idx);
COIN_DLL_API void          cc_xml_path_prepend_path_x(cc_xml_path * path, cc_xml_path * path2);
COIN_DLL_API void          cc_xml_path_truncate_x(cc_xml_path * path, int length);

COIN_DLL_API void          cc_xml_path_dump(const cc_xml_path * path);

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

/* ********************************************************************** */

#endif /* !COIN_XML_PATH_H */
