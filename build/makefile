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

#****************************************************************
#
# Makefile for Coin
#
# Usage:
#	make			Makes Coin development version (.so)
#	make coin		Makes Coin development version (.so)
#	make release		Makes Coin release version (.a)
#       make docs               Make doxygen documentation
#
#  These are for debugging/development only:
#	make profile		Makes Coin libary (.a) for profiling (g++ only)
#       make base               Makes libCoinBase.so with all base classes.
#                               (useful for development only).
#
#****************************************************************

# This seriously fucks up make under Red Hat 5.2, and doesn't seem to
# be necessary as long as bash is in the PATH. 19990412 mortene.
# SHELL = `which bash`

# This'll fix a bug in the footer target code when (at least) using
# date v1.16 and bash v1.14.7(1) between 00.00.00 and 09.59.59. May
# not be valid with other versions on other OSes of the date command
# (please report to larsa if so).

# OBSOLETE
#TIME_START := $(shell date +"%H*3600+%M*60+%S")
#TIME_NOW    = $(shell date +"%H*3600+%M*60+%S")
# NEW CODE
TIME_START := $(shell date +"%k*3600+%M*60+%S")
TIME_NOW    = $(shell date +"%k*3600+%M*60+%S")

all: header coin footer

header:
	@echo -e "\033[34m\n" \
	"\t****************************************************************\n" \
	"\t*                      COMPILING                               *\n" \
	"\t****************************************************************\n" \
	"\033[0m";

footer:
	@( now=`echo "$(TIME_NOW)" | sed -e 's/\([^0-9]\)0/\1/g'`; \
	   start=`echo "$(TIME_START)" | sed -e 's/\([^0-9]\)0/\1/g'`; \
	   now=`echo $$[now]`; \
	   start=`echo $$[start]`; \
	   time=$$[$$now-$$start]; \
	   mins=$$[$$time/60]; \
	   secs=$$[$$time%60]; \
	   if [ $$mins -lt 10 ]; then mins="0"$$mins; fi; \
	   if [ $$secs -lt 10 ]; then secs="0"$$secs; fi; \
	echo -e "\033[34m\n" \
	"\t****************************************************************\n" \
	"\t*         COMPILATION TIME: $$mins minutes and $$secs seconds.         *\n" \
	"\t****************************************************************\n" \
	"\033[0m"; )

# Top-level targets.

config:
	$(MAKE) -C scripts/lxdialog all
	$(SHELL) scripts/Menuconfig scripts/Config.in

coin: ../include/Inventor/soconfig.h makefile.coin .coin

release: makefile.release .release

profile: makefile.profile .profile

base: header makefile.base .base footer

# Autogenerate the soconfig.h file if it has not been manually made.

../include/Inventor/soconfig.h:
	if ! [ -f ../include/Inventor/soconfig.h ]; then echo "#define COIN_EXCLUDE_NOTHING 1" > ../include/Inventor/soconfig.h; fi

# Rules.

.coin:
	@mkdir -p objects
	@$(MAKE) -f makefile.coin

makefile.coin: coin.pro common.pro sources.pro
	tmake -p sources.pro -p common.pro coin.pro -o makefile.coin

.release:
	@mkdir -p releaseobjects
	@$(MAKE) -f makefile.release

makefile.release: release.pro common.pro sources.pro
	tmake -p sources.pro -p common.pro release.pro -o makefile.release

.profile:
	@mkdir -p profileobjects
	@$(MAKE) -f makefile.profile

makefile.profile: profile.pro common.pro sources.pro
	tmake -p sources.pro -p common.pro profile.pro -o makefile.profile

.base:
	@mkdir -p base_objects
	@$(MAKE) -f makefile.base

makefile.base: base.pro
	tmake base.pro -o makefile.base

sources.pro: ../include/Inventor/soconfig.h ../include/Inventor/confdep.h scripts/sourceinclusion.cpp
	$(MAKE) -C scripts all
	scripts/sourceinclusion > sources.pro

# Doxygen-generated documentation.

docs: .docs

.docs:
	doxygen ../docs/coin.doxygen
