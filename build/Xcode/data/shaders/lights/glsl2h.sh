#!/bin/sh
############################################################################
# glsl2h.sh
#
# This script generates a C header file from a GLSL file.  The header
# just contains the glsl file as a static char buffer.
#
# Authors:
#   Morten Eriksen <mortene@sim.no>
#   Lars J. Aas <larsa@sim.no>
#

UPCASEBASE=`basename $1 .glsl | tr '[a-z]' '[A-Z]'`

cat <<HERE_DOC_END
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

#ifndef COIN_INTERNAL
#error this is a private header file
#endif /* !COIN_INTERNAL */

#ifndef SO_${UPCASEBASE}_GLSL_H
#define SO_${UPCASEBASE}_GLSL_H

static const char ${UPCASEBASE}_shadersource[] =
HERE_DOC_END

cat $1 | sed -e \
's/\\/\\\\/g
s/"/\\"/g
3,$ s/^[ \t]*#.*//
s/^/  "/
s/$/\\n"/
$ s/$/;/'

# ATTN: the file did not just get corrupted ;-)

cat <<HERE_DOC_END

#endif /* ! SO_${UPCASEBASE}_GLSL_H */
HERE_DOC_END
