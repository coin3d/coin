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

# Compile a release version of the Coin library, with no extra code
# for debugging purposes etc.

TARGET	    =   Coin
CONFIG      =	release staticlib

LIBS        =	-L$(SIMAGELIB)/build -lsimage

# defines available:
# COIN_DEBUG = [0,1]   (compile with/without run-time debug code)

DEFINES     =	COIN_DEBUG=0

MOC_DIR     =   releaseobjects
OBJECTS_DIR =   releaseobjects
