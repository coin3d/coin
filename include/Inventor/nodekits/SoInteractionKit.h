/**************************************************************************\
 *
 *  Copyright (C) 1998-2000 by Systems in Motion.  All rights reserved.
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
\**************************************************************************/

#ifndef __SOINTERACTIONKIT_H__
#define __SOINTERACTIONKIT_H__

#include <Inventor/nodekits/SoSubKit.h>
#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/lists/SbList.h>
#include <Inventor/lists/SoPathList.h>
#include <Inventor/fields/SoSFEnum.h>

class SoFieldSensor;
class SoSensor;
class SoSeparator;

class SoInteractionKit : public SoBaseKit
{
  typedef SoBaseKit inherited;

  SO_KIT_HEADER(SoInteractionKit);

  SO_KIT_CATALOG_ENTRY_HEADER(geomSeparator);
  SO_KIT_CATALOG_ENTRY_HEADER(topSeparator);

public:
  SoSFEnum renderCaching;
  SoSFEnum boundingBoxCaching;
  SoSFEnum renderCulling;
  SoSFEnum pickCulling;

public:
  SoInteractionKit(void);
  static void initClass(void);

  enum CacheEnabled { OFF, ON, AUTO };

  virtual SbBool setPartAsPath(const SbName &partname,
                               SoPath *path);
  virtual SbBool setPartAsDefault(const SbName &partname,
                                  SoNode *node,
                                  SbBool onlyifdefault = TRUE);
  virtual SbBool setPartAsDefault(const SbName &partname,
                                  const SbName &nodename,
                                  SbBool onlyifdefault = TRUE);
  SbBool isPathSurrogateInMySubgraph(const SoPath *path,
                                     SoPath *&pathToOwner,
                                     SbName  &surrogatename,
                                     SoPath *&surrogatepath,
                                     SbBool fillargs = TRUE);
  SbBool isPathSurrogateInMySubgraph(const SoPath *path);
  static void setSwitchValue(SoNode *node, const int newVal);

protected:
  virtual ~SoInteractionKit();
  virtual void copyContents(const SoFieldContainer *fromFC,
                            SbBool copyConnections);

  virtual SbBool setPart(const int partNum, SoNode *node);
  virtual SbBool readInstance(SoInput *in, unsigned short flags);
  static void readDefaultParts(const char *fileName,
                               const char defaultBuffer[],
                               int defBufSize);
  virtual SbBool setAnyPartAsDefault(const SbName &partname,
                                     SoNode *node,
                                     SbBool anypart = TRUE,
                                     SbBool onlyifdefault = TRUE);
  virtual SbBool setAnyPartAsDefault(const SbName &partname,
                                     const SbName &nodename,
                                     SbBool anypart = TRUE,
                                     SbBool onlyifdefault = TRUE);
  SbBool setAnySurrogatePath(const SbName &name,
                             SoPath *path,
                             SbBool leafcheck = FALSE,
                             SbBool publiccheck = FALSE);
  virtual SbBool setUpConnections(SbBool onoff, SbBool doitalways = FALSE);
  virtual void setDefaultOnNonWritingFields();

  SoFieldSensor *fieldSensor;
  static void fieldSensorCB(void *, SoSensor *);
  SoSeparator *oldTopSep;

  void connectSeparatorFields( SoSeparator *dest, SbBool onOff );

private:
  SoPathList surrogatePaths;
  SbList <SbName> surrogateNames;

  void addSurrogatePath(SoPath *path, const SbName &name);
  void removeSurrogatePath(const SbName &partname);
  void removeSurrogatePath(const int idx);
  int findSurrogateIndex(const SbName &partname) const;
  int findSurrogateInPath(const SoPath *path);

  static SbList <SoNode*> * defaultdraggerparts;
  static void clean(void);

  SoFieldSensor *topSeparatorSensor;
  static void sensorCB(void *, SoSensor*);
  SoSeparator *oldTopSeparator;
  void connectSeparator(SoSeparator *sep, const SbBool onOff);
};

#endif // !__SOINTERACTIONKIT_H__
