#ifndef COIN_SOPROFILINGREPORTGENERATOR_H
#define COIN_SOPROFILINGREPORTGENERATOR_H

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

#include <Inventor/SbBasic.h>
#include <Inventor/lists/SbList.h>

class SbProfilingData;
class SbProfilingReportSortCriteria;   // opaque internal
class SbProfilingReportPrintCriteria;  // opaque internal

class COIN_DLL_API SoProfilingReportGenerator {
public:
  static void init(void);

  enum Column {
    NAME,
    TYPE,
    COUNT,
    TIME_SECS,
    TIME_SECS_MAX,
    TIME_SECS_AVG,
    TIME_MSECS,
    TIME_MSECS_MAX,
    TIME_MSECS_AVG,
    TIME_PERCENT,
    TIME_PERCENT_MAX,
    TIME_PERCENT_AVG,
    MEM_BYTES,
    MEM_KILOBYTES,
    GFX_MEM_BYTES,
    GFX_MEM_KILOBYTES
  };

  enum SortOrder {
    TIME_ASC,
    TIME_DES,
    TIME_MAX_ASC,
    TIME_MAX_DES,
    TIME_AVG_ASC,
    TIME_AVG_DES,
    COUNT_ASC,
    COUNT_DES,
    ALPHANUMERIC_ASC,
    ALPHANUMERIC_DES,
    MEM_ASC,
    MEM_DES,
    GFX_MEM_ASC,
    GFX_MEM_DES
  };

  enum DataCategorization {
    TYPES,
    NAMES,
    NODES
  };

  enum CallbackResponse {
    CONTINUE,
    STOP
  };

  static SbProfilingReportSortCriteria * getReportSortCriteria(const SbList< SortOrder > & order);
  static SbProfilingReportSortCriteria * getDefaultReportSortCriteria(DataCategorization category);

  static SbProfilingReportPrintCriteria * getReportPrintCriteria(const SbList< Column > & order);
  static SbProfilingReportPrintCriteria * getDefaultReportPrintCriteria(DataCategorization category);
  static void freeCriteria(SbProfilingReportSortCriteria * criteria);
  static void freeCriteria(SbProfilingReportPrintCriteria * criteria);

  typedef CallbackResponse ReportCB(void * userdata, int entrynum, const char * text);

  static void generate(const SbProfilingData & data,
                       DataCategorization categorization,
                       SbProfilingReportSortCriteria * sort,
                       SbProfilingReportPrintCriteria * print,
                       int count,
                       SbBool addheader,
                       ReportCB * reportcallback,
                       void * userdata);

  static CallbackResponse stdoutCB(void * userdata, int entrynum, const char * text);
  static CallbackResponse stderrCB(void * userdata, int entrynum, const char * text);

}; // SoProfilingReportGenerator

#endif // !COIN_SOPROFILINGREPORTGENERATOR_H
