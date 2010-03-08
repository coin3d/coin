#ifndef COIN_EVAL_MINIMUM_H
#define COIN_EVAL_MINIMUM_H

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

#include <Inventor/scxml/ScXMLEvaluator.h>

// needed for the ScXMLEvaluator
ScXMLDataObj * scxml_minimum_parse(const char * buffer);

// needed for the flex/bison code
int scxml_minimum_parse(void);
int scxml_minimum_lex(void);
int scxml_minimum_error(const char *);
ScXMLDataObj * scxml_minimum_get_root_obj(void);
void scxml_minimum_clear_root_obj(void);

#endif // !COIN_EVAL_MINIMUM_H
