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
#  http://www.sim.no/ sales@sim.no Voice: +47 22114160 Fax: +47 67172912
#
############################################################################

# This is a tmake projectfile which generates a makefile for building
# a debug version of the Coin library.
#
#        19990522 mortene.

TARGET	    =   CoinDebug
CONFIG      =	debug sharedlib

LIBS        =

# defines available:
# COIN_DEBUG = [0,1]   (compile with/without run-time debug code)

DEFINES     =	COIN_DEBUG=1

MOC_DIR     =   objects
OBJECTS_DIR =   objects
