#!/bin/sh
############################################################################
# iv2h.sh
#
# This script generates a C header file from an Inventor file.  The header
# just contains the inventor file as a static char buffer.
#
# Authors:
#   Morten Eriksen <mortene@sim.no>
#   Lars J. Aas <larsa@sim.no>
#

UPCASEBASE=`basename $1 .iv | tr '[a-z]' '[A-Z]'`

cat <<HERE_DOC_END
/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2002 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Systems in Motion about acquiring
 *  a Coin Professional Edition License.
 *
 *  See <URL:http://www.coin3d.org> for  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

#ifndef SO_${UPCASEBASE}_IV_H
#define SO_${UPCASEBASE}_IV_H

static const char ${UPCASEBASE}_draggergeometry[] =
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

#endif /* ! SO_${UPCASEBASE}_IV_H */
HERE_DOC_END
