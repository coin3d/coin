/**************************************************************************\
 * 
 *  Copyright (C) 1998-1999 by Systems in Motion.  All rights reserved.
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

#ifndef _SO_GET_BOUNDING_BOX_ACTION_H_
#define _SO_GET_BOUNDING_BOX_ACTION_H_

#include <Inventor/confdep.h>
#if defined(COIN_EXCLUDE_SOGETBOUNDINGBOXACTION)
#error "This file is excluded from Coin through the configuration system!"
#endif // COIN_EXCLUDE_SOGETBOUNDINGBOXACTION

#include <Inventor/actions/SoAction.h>

#include <Inventor/SbViewportRegion.h>

#ifdef __SOLIB_INTERNAL__
#include <Inventor/SbBox3f.h>
#include <Inventor/SbXfBox3f.h>
#else // !__SOLIB_INTERNAL__
#include <Inventor/SbBox.h>
#endif // !__SOLIB_INTERNAL__

class SoGetBoundingBoxAction : public SoAction {
  typedef SoAction inherited;

//$ BEGIN TEMPLATE ActionHeader( SoGetBoundingBoxAction )
private:
  static SoType classTypeId;

public:
  virtual SoType getTypeId(void) const;
  static SoType getClassTypeId(void);

protected:
  virtual const SoEnabledElementsList & getEnabledElements(void) const;
  static SoEnabledElementsList * enabledElements;
  static SoActionMethodList * methods;

public:
  static void addMethod(const SoType type, SoActionMethod method);
  static void enableElement(const SoType type, const int stackIndex);

  static void initClass(void);
  static void cleanClass(void);

  virtual ~SoGetBoundingBoxAction();
//$ END TEMPLATE ActionHeader

public:
  
  enum ResetType {
    TRANSFORM, BBOX, ALL
  };

  SoGetBoundingBoxAction(const SbViewportRegion &viewportRegion);
  void setViewportRegion(const SbViewportRegion &newRegion);
  const SbViewportRegion &getViewportRegion() const;
  SbBox3f getBoundingBox() const;
  SbXfBox3f &getXfBoundingBox();
  const SbVec3f &getCenter() const;
  void setInCameraSpace(const SbBool flag);
  SbBool isInCameraSpace() const;
  void setResetPath(const SoPath *path, 
		    const SbBool resetBefore = TRUE, 
		    const ResetType what = ALL);
  const SoPath *getResetPath() const;
  SbBool isResetPath() const;
  SbBool isResetBefore() const;
  ResetType getWhatReset() const;


  void checkResetBefore(); // for OI compability
  void checkResetAfter();
  
  void extendBy(const SbBox3f &box);
  void extendBy(const SbXfBox3f &box);

  void setCenter(const SbVec3f &center,
		 const SbBool transformCenter);
  SbBool isCenterSet() const;
  void resetCenter();

protected:
  virtual void beginTraversal(SoNode * const node);

private:
  SbXfBox3f bbox;
  SbVec3f center;  
  SbViewportRegion vpRegion;
  ResetType resetType;
  SoPath *resetPath;
  unsigned int flags;
}; // class SoGetBoundingBoxAction

#endif // ! _SO_GET_BOUNDING_BOX_ACTION_H_
