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

UPCASEBASE=`basename $1 .iv | tr a-z A-Z`

cat <<HERE_DOC_END
/**************************************************************************
 *
 *  Copyright (C) 2000 by Systems in Motion.  All rights reserved.
 *
 *  This file is part of the Coin library.
 *
 *  This file may be distributed under the terms of the Q Public License
 *  as defined by Troll Tech AS of Norway and appearing in the file
 *  LICENSE.QPL included in the packaging of this file.
 *
 *  If you want to use Coin in applications not covered by licenses
 *  compatible with the QPL, you can contact SIM to aquire a
 *  Professional Edition license for Coin.
 *
 *  Systems in Motion AS, Prof. Brochs gate 6, N-7030 Trondheim, NORWAY
 *  http://www.sim.no/ sales@sim.no Voice: +47 22114160 Fax: +47 67172912
 *
 **************************************************************************/

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
