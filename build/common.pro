############################################################################
#
#  Copyright (C) 1998-1999 by Systems in Motion.  All rights reserved.
#
#  This file is part of the Coin library.
#
#  This file may be distributed under the terms of the Q Public License
#  as defined by Troll Tech AS of Norway and appearing in the file
#  LICENSE.QPL included in the packaging of this file.
#
#  If you want to use Coin in applications not covered by licenses
#  compatible with the QPL, you can contact SIM to aquire a
#  Professional Edition license for Coin.
#
#  Systems in Motion AS, Prof. Brochs gate 6, N-7030 Trondheim, NORWAY
#  http://www.sim.no/ sales@sim.no Voice: +47 73540378 Fax: +47 73943861
#
############################################################################

# Common build settings for the debug, profile and release makefiles.

TEMPLATE	=	lib
# FIXME: this should eventually be "warn_on". 990314 mortene. 
CONFIG		+=	warn_off

DEFINES		+=	__SOLIB_INTERNAL__

INCLUDEPATH	+=	../include
DEPENDPATH	+=	../include
